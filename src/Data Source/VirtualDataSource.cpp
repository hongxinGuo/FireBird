/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Note 需要拆分为多次申请才能完成一次数据采集的product，使用并行工作线程同时申请，接收到所有数据后一起传递给product来处理。
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "pch.h"

#include "VirtualDataSource.h"

#include"Thread.h"
#include"ThreadStatus.h"

#include"HighPerformanceCounter.h"

atomic_long CVirtualDataSource::sm_lTotalByteRead = 0;
atomic_long CVirtualDataSource::sm_lTotalByteReadPerSecond = 0;

CVirtualDataSource::CVirtualDataSource() {
	m_fEnable = true; // 默认为允许执行

	SetDefaultSessionOption();
	m_strHeaders = _T("");

	m_fWebError = false;
	m_strInquiry = _T("");
	m_strInquiryFunction = _T("");
	m_strSuffix = _T("");
	m_strInquiryToken = _T("");

	m_lInquiringNumber = 500; // 每次查询数量默认值为500
	m_tCurrentInquiryTime = 0;
	m_pCurrentProduct = nullptr;

	m_fInquiring = false;
	m_bIsWorkingThreadRunning = false;

	m_llLastTimeTickCount = 0;
}

///////////////////////////////////////////////////////////////////////////////////
///<summary>
/// DataSource的顶层函数。
///
/// Note 调用函数不能使用thread_pool_executor或者background_executor，只能使用thread_executor，否则thread_pool_executor所生成线程的返回值无法读取，原因待查。
///
/// Note 必须使用独立的thread_executor任务序列，不能使用thread_pool_executor或者background_executor，
//  否则解析工作使用的thread_pool_executor会与之产生冲突，导致产生同步问题。
///
///</summary>
///
/// lMarketTime：当前市场时间
///
////////////////////////////////////////////////////////////////////////////////////
void CVirtualDataSource::Run(long lMarketTime) {
	CVirtualDataSourcePtr p = this->GetShared();
	if (!IsInquiring() && !IsWorkingThreadRunning()) {
		gl_runtime.thread_executor()->post([p, lMarketTime] {
			gl_ThreadStatus.IncreaseWebInquiringThread();
			p->RunWorkingThread(lMarketTime);
			gl_ThreadStatus.DecreaseWebInquiringThread();
		});
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//
// 数据的申请和处理皆位于此处。
// 由于待申请的数据可能有多个，故而生成多个线程来申请，并且等待所有的线程完成。
//
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////
void CVirtualDataSource::RunWorkingThread(const long lMarketTime) {
	static time_t s_LastInquiryTime = 0;

	SetWorkingThreadRunning(true);
	if (!IsInquiring()) {
		ASSERT(!HaveInquiry());
		GenerateInquiryMessage(lMarketTime);
	}

	if (IsInquiring()) {
		vector<result<CWebDataPtr>> vResults;
		while (HaveInquiry()) { // 一次申请可以有多个数据
			ASSERT(gl_systemConfiguration.IsWorkingMode()); // 不允许测试
			ASSERT(IsInquiring());
			GetCurrentProduct();
			GenerateCurrentInquiryMessage();
			CDataInquireEnginePtr pEngine = make_shared<CInquireEngine>(m_internetOption, GetInquiringString(), GetHeaders());
			auto result = gl_runtime.background_executor()->submit([this, pEngine] {
				CHighPerformanceCounter counter;
				counter.start();
				auto pWebData = pEngine->GetWebData();
				if (!pEngine->IsWebError()) this->UpdateStatus(pWebData);
				else
					ASSERT(pWebData == nullptr);
				counter.stop();
				time_t ttCurrentInquiryTime = counter.GetElapsedMillisecond();
				if (s_LastInquiryTime > m_iMaxNormalInquireTime && ttCurrentInquiryTime > m_iMaxNormalInquireTime) SetWebBusy(true); // 
				else SetWebBusy(false);
				s_LastInquiryTime = ttCurrentInquiryTime;
				SetCurrentInquiryTime(ttCurrentInquiryTime);
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
		}
		if (!gl_systemConfiguration.IsExitingSystem() && !pvWebData->empty()) {
			CheckInaccessible(pvWebData->at(0));
			m_pCurrentProduct->ParseAndStoreWebData(pvWebData);
			m_pCurrentProduct->UpdateDataSourceStatus(this->GetShared()); // 这里传递的是实际DataSource智能指针
		}
		ASSERT(IsInquiring()); // 执行到此时，尚不允许申请下次的数据。
		ASSERT(!HaveInquiry()); // 没有现存的申请
		SetInquiring(false); // 此标识的重置需要位于位于最后一步
	}
	SetWorkingThreadRunning(false);
}

void CVirtualDataSource::SetDefaultSessionOption() {
	m_internetOption.option_connect_timeout = 120000;
	m_internetOption.option_receive_timeout = 120000;
	m_internetOption.option_data_receive_timeout = 120000;
	m_internetOption.option_send_timeout = 2000;
	m_internetOption.option_connect_retries = 1;
}

void CVirtualDataSource::GenerateCurrentInquiryMessage() {
	ASSERT(m_pCurrentProduct != nullptr);
	m_strInquiryFunction = m_pCurrentProduct->CreateMessage();
	CreateTotalInquiringString();
}

void CVirtualDataSource::CreateTotalInquiringString() {
	m_strInquiry = m_strInquiryFunction + m_strParam + m_strSuffix + m_strInquiryToken;
}
