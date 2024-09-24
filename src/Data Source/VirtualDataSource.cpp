/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Note ��Ҫ���Ϊ�������������һ�����ݲɼ���product��ʹ�ò��й����߳�ͬʱ���룬���յ��������ݺ�һ�𴫵ݸ�product������
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
	m_fEnable = true; // Ĭ��Ϊ����ִ��

	SetDefaultSessionOption();
	m_strHeaders = _T("");

	m_fWebError = false;
	m_strInquiry = _T("");
	m_strInquiryFunction = _T("");
	m_strSuffix = _T("");
	m_strInquiryToken = _T("");

	m_lInquiringNumber = 500; // ÿ�β�ѯ����Ĭ��ֵΪ500
	m_tCurrentInquiryTime = 0;
	m_pCurrentProduct = nullptr;

	m_fInquiring = false;
	m_bIsWorkingThreadRunning = false;

	m_llLastTimeTickCount = 0;
}

///////////////////////////////////////////////////////////////////////////////////
///<summary>
/// DataSource�Ķ��㺯����
///
/// Note ���ú�������ʹ��thread_pool_executor����background_executor��ֻ��ʹ��thread_executor������thread_pool_executor�������̵߳ķ���ֵ�޷���ȡ��ԭ����顣
///
/// Note ����ʹ�ö�����thread_executor�������У�����ʹ��thread_pool_executor����background_executor��
//  �����������ʹ�õ�thread_pool_executor����֮������ͻ�����²���ͬ�����⡣
///
///</summary>
///
/// lMarketTime����ǰ�г�ʱ��
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
// ���ݵ�����ʹ����λ�ڴ˴���
// ���ڴ���������ݿ����ж�����ʶ����ɶ���߳������룬���ҵȴ����е��߳���ɡ�
// ��Ҫ������������Դ�У���Ѷ�������ݡ�
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
		while (HaveInquiry()) { // һ����������ж������
			ASSERT(gl_systemConfiguration.IsWorkingMode()); // ���������
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
				SetHTTPStatusCode(pEngine->GetHTTPStatusCode());
				SetWebErrorCode(pEngine->GetErrorCode());
				return pWebData;
			});
			vResults.emplace_back(std::move(result));
		}
		const shared_ptr<vector<CWebDataPtr>> pvWebData = make_shared<vector<CWebDataPtr>>();
		for (auto& pWebData : vResults) {
			auto p = pWebData.get(); // ������ȴ����е��߳�ִ�����
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
			m_eErrorMessageData = IsAErrorMessageData(pvWebData->at(0)); // ���ص������Ǵ�����Ϣ��
			m_pCurrentProduct->ParseAndStoreWebData(pvWebData);
			m_pCurrentProduct->UpdateDataSourceStatus(this->GetShared()); // ���ﴫ�ݵ���ʵ��DataSource����ָ��
		}
		ASSERT(IsInquiring()); // ִ�е���ʱ���в����������´ε����ݡ�
		ASSERT(!HaveInquiry()); // û���ִ������
		SetInquiring(false); // �˱�ʶ��������Ҫλ��λ�����һ��
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

void CVirtualDataSource::ReportErrorNotHandled(const string& sError) {
	gl_dailyLogger->warn("error not processed: {}", sError);
	gl_SoftwareDevelopingLogger->warn("error not processed: {}", sError);
	CString s = _T("error not processed:");
	s += sError.c_str();
	gl_systemMessage.PushInnerSystemInformationMessage(s);
}
