/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Note 需要拆分为多次申请才能完成一次数据采集的product，使用并行工作线程同时申请，接收到所有数据后一起传递给product来处理。
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "pch.h"

#include "VirtualDataSource.h"
#include "InquireEngine.h"

#include"Thread.h"
#include "WebData.h"

atomic<int64_t> CVirtualDataSource::sm_lTotalByteRead = 0;
atomic<int64_t> CVirtualDataSource::sm_lTotalByteReadPerSecond = 0;

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
	CVirtualDataSourcePtr p = this->GetShared(); //Note 这里传递至线程时要使用智能指针，以防止原智能指针被解析。
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
// Note 20250227, 现在似乎可以使用thread_executor了，原因不明。
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////
void CVirtualDataSource::InquireData() {
	ASSERT(gl_systemConfiguration.IsWorkingMode()); // 不允许测试
	ASSERT(IsInquiring());

	auto start = chrono::time_point_cast<chrono::milliseconds>(chrono::steady_clock::now());

	vector<result<CWebDataPtr>> vResults;
	while (HaveInquiry()) { // 一次申请可以有多个数据
		GetCurrentProduct();
		CreateCurrentInquireString();
		CInquireEnginePtr pEngine = make_shared<CInquireEngine>(m_internetOption, GetInquiringString(), GetHeaders());
		auto result = gl_runtime.thread_executor()->submit([this, pEngine] {
			auto pWebData = pEngine->GetWebData();
			SetWebErrorCode(pEngine->GetErrorCode());
			SetHTTPStatusCode(pEngine->GetHTTPStatusCode());
			if (!pEngine->IsWebError()) this->UpdateStatus(pWebData);
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
	if (!gl_systemConfiguration.IsExitingSystem() && !pvWebData->empty() && !IsWebError()) {
		m_eErrorMessageData = IsAErrorMessageData(pvWebData->at(0)); // 返回的数据是错误信息？检查错误，判断申请资格，更新禁止目录
		m_pCurrentProduct->CalculateTotalDataLength(pvWebData);
		m_pCurrentProduct->ParseAndStoreWebData(pvWebData);
		m_pCurrentProduct->UpdateDataSourceStatus(this->GetShared()); // 这里传递的是实际DataSource的智能指针
	}
	auto end = chrono::time_point_cast<chrono::milliseconds>(chrono::steady_clock::now());
	SetCurrentInquiryTime((end - start).count());
	ASSERT(!HaveInquiry()); // 没有现存的申请
	ASSERT(IsInquiring()); // 执行到此时，尚不允许申请下次的数据。
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
	string s = _T("error not processed:");
	s += sError;
	gl_systemMessage.PushInnerSystemInformationMessage(s);
}
