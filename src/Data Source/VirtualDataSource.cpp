/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Note 需要拆分为多次申请才能完成一次数据采集的product，使用并行工作线程同时申请，接收到所有数据后一起传递给product来处理。
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "pch.h"

#include "VirtualDataSource.h"
#include"VirtualWebProduct.h"
#include "InquireEngine.h"
#include "log.h"
#include "SystemConfiguration.h"
#include "SystemMessage.h"

#include"Thread.h"
#include "WebData.h"

using std::atomic;
using std::binary_semaphore;
using std::make_shared;

atomic<int64_t> CVirtualDataSource::sm_lTotalByteRead = 0;
atomic<int64_t> CVirtualDataSource::sm_lTotalByteReadPerSecond = 0;

CVirtualDataSource::CVirtualDataSource() {
	SetDefaultSessionOption();
}
void CVirtualDataSource::ReportFinishedMsg(const std::string& msg) {
	gl_systemMessage.PushInformationMessage(msg);
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
void CVirtualDataSource::Run(const local_seconds& lMarketTime) {
	ABSL_DCHECK(!IsInquiring());
	gl_runtime.thread_executor()->post([this, lMarketTime] { //Note 此处必须使用thread_executor
			GenerateInquiryMessage(lMarketTime);
			if (HaveInquiry()) {
				SetInquiring(true);
				if (m_bUsingNewInterface) {
					InquireData2();
				}
				else {
					InquireData();
				}
			}
		});
}

namespace {
	binary_semaphore s_InquiryWebData{ 1 };
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//
// 数据的申请和处理皆位于此处。
// 由于待申请的数据可能有多个，故而生成多个线程来申请，并且等待所有的线程完成。
// 需要多个申请的数据源有：腾讯日线数据。
//
//Note 只能使用thread_pool_executor或者background_executor，不能使用thread_executor。
//Note 20250227, 现在似乎可以使用thread_executor了，原因不明。
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////
void CVirtualDataSource::InquireData() {
	ABSL_DCHECK(gl_systemConfiguration.IsWorkingMode()); // 不允许测试
	ABSL_DCHECK(IsInquiring());
	auto start = time_point_cast<milliseconds>(steady_clock::now());
	vector<result<CWebDataPtr>> vResults;
	while (HaveInquiry()) { // 一次申请可以有多个数据
		GetCurrentProduct();
		CreateCurrentInquireString();
		ABSL_DCHECK(!m_pInquiryStrings->empty());
		for (size_t index = 0; index < m_pInquiryStrings->size(); index++) {
			auto inquiryString = m_pInquiryStrings->at(index);
			if (m_bConcurrentForbid) {
				Sleep(1000);
				s_InquiryWebData.acquire();
				ABSL_DLOG(INFO) << std::format("%s %d times\n", m_pCurrentProduct->GetInquiringSymbol(), ++index);
			}
			CInquireEnginePtr pEngine = make_shared<CInquireEngine>(m_internetOption, inquiryString, GetHeaders());
			auto result = gl_runtime.thread_executor()->submit([this, pEngine] {
				auto pWebData = pEngine->GetWebData();
				SetWebErrorCode(pEngine->GetErrorCode());
				SetHTTPStatusCode(pEngine->GetHTTPStatusCode());
				if (!pEngine->IsWebError()) this->UpdateStatus(pWebData);
				if (m_bConcurrentForbid) {
					s_InquiryWebData.release();
				}
				return pWebData;
			});
			vResults.emplace_back(std::move(result));
		}
	}
	const shared_ptr<vector<CWebDataPtr>> pvWebData = make_shared<vector<CWebDataPtr>>();
	for (auto& pWebData : vResults) {
		auto p = pWebData.get(); // 在这里等待所有的线程执行完毕
		if (p != nullptr) { // 抛弃无效数据，空数据的话要保存
			p->SetStockCode(m_pCurrentProduct->GetInquiringSymbol());
			sm_lTotalByteRead += p->GetBufferLength();
			pvWebData->push_back(p);
		}
	}
	if (!pvWebData->empty() && !IsWebError() && !gl_systemConfiguration.IsExitingSystem()) {
		CheckWebData(pvWebData->at(0)); // 返回的数据是错误信息？检查错误，判断申请资格，更新禁止目录
		m_pCurrentProduct->CalculateTotalDataLength(pvWebData);
		m_pCurrentProduct->ParseAndStoreWebData(pvWebData);
		m_pCurrentProduct->UpdateSystemStatus();
	}
	auto end = time_point_cast<milliseconds>(steady_clock::now());
	SetCurrentInquiryTime((end - start).count());
	ABSL_DCHECK(!HaveInquiry());
	ABSL_DCHECK(IsInquiring());  //至此尚未重置此标识
	SetInquiring(false); // 此标识的重置需要位于位于最后一步
}

void CVirtualDataSource::InquireData2() {
	ABSL_DCHECK(gl_systemConfiguration.IsWorkingMode()); // 不允许测试
	ABSL_DCHECK(IsInquiring());
	auto start = time_point_cast<milliseconds>(steady_clock::now());
	while (HaveInquiry()) { // 一次申请可以有多个数据
		GetCurrentProduct();
		m_pCurrentProduct->InquireData(m_strHeaders, m_strParam, m_strSuffix, m_strInquiryToken);
		m_pCurrentProduct->UpdateSystemStatus();
	}
	auto end = time_point_cast<milliseconds>(steady_clock::now());
	SetCurrentInquiryTime((end - start).count());
	ABSL_DCHECK(!HaveInquiry());
	ABSL_DCHECK(IsInquiring());  //至此尚未重置此标识
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
	ABSL_DCHECK(m_pCurrentProduct != nullptr);
	auto pInquiryStrings = m_pCurrentProduct->CreateMessage();
	CreateTotalInquiringString(pInquiryStrings);
}

void CVirtualDataSource::CreateTotalInquiringString(shared_ptr<vector<string>> pInquiryStrings) {
	for (size_t i = 0; i < pInquiryStrings->size(); i++) {
		pInquiryStrings->at(i) += m_strParam + m_strSuffix + m_strInquiryToken;
	}
	m_pInquiryStrings = pInquiryStrings;
}

void CVirtualDataSource::ReportErrorNotHandled(const string& sError) {
	gl_dailyLogger->warn("error not processed: {}", sError);
	gl_SoftwareDevelopingLogger->error("error not processed: {}", sError);
	string s = "error not processed:";
	s += sError;
	gl_systemMessage.PushInnerSystemInformationMessage(s);
}
