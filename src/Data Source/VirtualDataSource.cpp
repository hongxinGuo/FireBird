/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Note 需要拆分为多次申请才能完成一次数据采集的product，使用并行工作线程同时申请，接收到所有数据后一起传递给product来处理。
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "pch.h"
module;
#include"globedef.h"
#include"NlohmannJsonHeader.h"
#include"ConCurrenCppHeader.h"
module FireBird.DataSource.Virtual;

import FireBird.Type.WebData;
import FireBird.System.Message;
import FireBird.System.Configuration;

using std::vector;

atomic_int64_t CVirtualDataSource::sm_lTotalByteRead = 0;
atomic_int64_t CVirtualDataSource::sm_lTotalByteReadPerSecond = 0;

CVirtualDataSource::CVirtualDataSource() {
	SetDefaultSessionOption();
}

///////////////////////////////////////////////////////////////////////////////////
///<summary>
/// DataSource的顶层函数。
///
/// Note 调用函数不能使用thread_pool_executor或者background_executor，只能使用thread_executor，否则thread_pool_executor所生成线程的返回值无法读取，原因待查。
///
/// Note 必须使用独立的thread_executor任务序列，不能使用thread_pool_executor或者background_executor，否则解析工作使用的thread_pool_executor会与之产生冲突，导致产生同步问题。原因不明。
///
///</summary>
///
/// lMarketTime：当前市场时间
///
////////////////////////////////////////////////////////////////////////////////////
void CVirtualDataSource::Run(long lMarketTime) {
	CVirtualDataSourcePtr p = this->GetShared();
	if (!IsInquiring()) {
		gl_runtime.thread_executor()->post([p, lMarketTime] { //Note 此处必须使用thread_executor
				p->GenerateInquiryMessage(lMarketTime);
				if (p->IsInquiring()) {
					p->InquireData();
				}
			});
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//
// 数据的申请和处理皆位于此处。
// 由于待申请的数据可能有多个，故而生成多个线程来申请，并且等待所有的线程完成。
// 需要多个申请的数据源有：腾讯日线数据。
//
//Note 只能使用thread_pool_executor或者background_executor，不能使用thread_executor。
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////
void CVirtualDataSource::InquireData() {
	CString str = typeid(*this).name();
	ASSERT(gl_systemConfiguration.IsWorkingMode()); // 不允许测试
	ASSERT(IsInquiring());

	static time_t s_LastInquiryTime = 0;

	vector<result<CWebDataPtr>> vResults;
	while (HaveInquiry()) { // 一次申请可以有多个数据
		GetCurrentProduct();
		CreateCurrentInquireString();
		CInquireEnginePtr pEngine = make_shared<CInquireEngine>(m_internetOption, GetInquiringString(), GetHeaders());
		auto background = gl_runtime.background_executor();
		auto result = gl_runtime.background_executor()->submit([this, pEngine] { //Note 只能使用thread_pool_executor或者background_executor
				auto start = chrono::time_point_cast<chrono::milliseconds>(chrono::steady_clock::now());
				auto pWebData = pEngine->GetWebData();
				if (!pEngine->IsWebError()) this->UpdateStatus(pWebData);
				auto end = chrono::time_point_cast<chrono::milliseconds>(chrono::steady_clock::now());
				time_t ttCurrentInquiryTime = (end - start).count();
				if (s_LastInquiryTime > m_iMaxNormalInquireTime && ttCurrentInquiryTime > m_iMaxNormalInquireTime) {
					SetWebBusy(true); //
				}
				else {
					SetWebBusy(false);
				}
				s_LastInquiryTime = ttCurrentInquiryTime;
				SetCurrentInquiryTime(ttCurrentInquiryTime);
				SetHTTPStatusCode(pEngine->GetHTTPStatusCode());
				SetWebErrorCode(pEngine->GetErrorCode());
				return pWebData;
			});
		vResults.emplace_back(std::move(result));
	}
	const shared_ptr<vector<CWebDataPtr>> pvWebData = make_shared<vector<CWebDataPtr>>();
	for (auto& pWebData : vResults) {
		auto p = pWebData.get(); // 在这里等待所有的线程执行完毕
		if (p != nullptr) {
			sm_lTotalByteRead += p->GetBufferLength();
			pvWebData->push_back(p);
		}
	}
	if (vResults.size() == pvWebData->size()) { // no web error?
		m_fWebError = false;
	}
	else { // web error
		m_fWebError = true;
		CString strMessage2 = str + m_strInquiry.Left(70);
		gl_warnLogger->warn("virtualDataSource thread number not match {} -- {}  {}", vResults.size(), pvWebData->size(), strMessage2.GetBuffer());
	}
	if (!gl_systemConfiguration.IsExitingSystem() && !pvWebData->empty()) {
		m_eErrorMessageData = IsAErrorMessageData(pvWebData->at(0)); // 返回的数据是错误信息？检查错误，判断申请资格，更新禁止目录
		m_pCurrentProduct->ParseAndStoreWebData(pvWebData); //todo 根据m_eErrorMessageData的状态决定是否解析
		m_pCurrentProduct->UpdateDataSourceStatus(this->GetShared()); // 这里传递的是实际DataSource的智能指针
	}
	ASSERT(!HaveInquiry()); // 没有现存的申请
	//ASSERT(IsInquiring()); // 执行到此时，尚不允许申请下次的数据。
	if (!IsInquiring()) {
		CString strMessage = str + m_strInquiry.Left(70);
		gl_systemMessage.PushInnerSystemInformationMessage(strMessage);
		gl_warnLogger->warn("CVirtualWebData.InquireData() reentry: {}", strMessage.GetBuffer());
	}
	SetInquiring(false); // 此标识的重置需要位于位于最后一步
}

void CVirtualDataSource::SetDefaultSessionOption() {
	m_internetOption.option_connect_timeout = 120000;
	m_internetOption.option_receive_timeout = 120000;
	m_internetOption.option_data_receive_timeout = 120000;
	m_internetOption.option_send_timeout = 2000;
	m_internetOption.option_connect_retries = 1;
}

void CVirtualDataSource::CreateCurrentInquireString() {
	ASSERT(m_pCurrentProduct != nullptr);
	m_strInquiryFunction = m_pCurrentProduct->CreateMessage();
	CreateTotalInquiringString();
}

void CVirtualDataSource::CreateTotalInquiringString() {
	m_strInquiry = m_strInquiryFunction + m_strParam + m_strSuffix + m_strInquiryToken;
}

void CVirtualDataSource::ReportErrorNotHandled(const string& sError) {
	gl_dailyLogger->warn("error not processed: {}", sError);
	gl_SoftwareDevelopingLogger->warn("error not processed: {}", sError);
	CString s = _T("error not processed:");
	s += sError.c_str();
	gl_systemMessage.PushInnerSystemInformationMessage(s);
}
