/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Note 需要拆分为多次申请才能完成一次数据采集的product，使用并行工作线程同时申请，接收到所有数据后一起传递给product来处理。
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "pch.h"

#include "VirtualDataSource.h"
#include"VirtualWebProduct.h"
#include "log.h"
#include "SystemConfiguration.h"
#include "SystemMessage.h"

#include"Thread.h"

using std::atomic;
using std::binary_semaphore;
using std::make_shared;

atomic<int64_t> CVirtualDataSource::sm_lTotalByteRead = 0;
atomic<int64_t> CVirtualDataSource::sm_lTotalByteReadPerSecond = 0;

CVirtualDataSource::CVirtualDataSource() {
	SetDefaultSessionOption();
}
CVirtualDataSource::~CVirtualDataSource() {
	if (m_runThread.joinable()) { // Close thread.
		m_runThread.request_stop();
		m_runThread.join();
	}
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
	if (!IsInquiring()) {
		// 如果已有运行完的线程，先请求停止并等待其结束（避免并发的 Run 导致竞态）
		if (m_runThread.joinable()) {
			m_runThread.request_stop();
			m_runThread.join();
		}

		// 启动新的 jthread，支持协作式停止（std::stop_token）
		m_runThread = std::jthread([this, lMarketTime](const std::stop_token& st) {
			// 如果被立即请求停止，则直接返回
			if (st.stop_requested()) return;

			GenerateInquiryMessage(lMarketTime);
			if (HaveInquiry()) {
				SetInquiring(true);
				Inquire(st);
			}
		});
	}
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
void CVirtualDataSource::Inquire(const std::stop_token& st) {
	ABSL_DCHECK(IsInquiring());
	while (HaveInquiry()) { // 一次申请可以有多个数据
		if (st.stop_requested()) break;
		GetCurrentProduct();
		m_pCurrentProduct->InquireData(st, m_strHeaders, m_strParam, m_strSuffix, m_token);
		m_pCurrentProduct->UpdateSystemStatus();
	}
	SetHTTPStatusCode(m_pCurrentProduct->GetStatusCode());
	SetCurrentInquiryTime(m_pCurrentProduct->GetElapsedTime() * 1000);
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
		pInquiryStrings->at(i) += m_strParam + m_strSuffix + m_token;
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

void CVirtualDataSource::StopThread() {
	if (m_runThread.joinable()) {
		m_runThread.request_stop();
		m_runThread.join();
	}
}
