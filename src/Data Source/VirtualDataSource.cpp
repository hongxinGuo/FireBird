/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Note ��Ҫ���Ϊ�������������һ�����ݲɼ���product��ʹ�ò��й����߳�ͬʱ���룬���յ��������ݺ�һ�𴫵ݸ�product������
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "pch.h"

#include "VirtualDataSource.h"

#include"Thread.h"

#include"HighPerformanceCounter.h"

atomic_long CVirtualDataSource::sm_lTotalByteRead = 0;
atomic_long CVirtualDataSource::sm_lTotalByteReadPerSecond = 0;

CVirtualDataSource::CVirtualDataSource() {
	SetDefaultSessionOption();
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
	if (!IsInquiring()) {
		gl_runtime.thread_executor()->post([p, lMarketTime] {
			p->RunWorkingThread(lMarketTime);
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

	if (!IsInquiring()) {
		ASSERT(!HaveInquiry());
		if (GenerateInquiryMessage(lMarketTime)) {
			ASSERT(IsInquiring());
		}
	}

	if (IsInquiring()) {
		vector<result<CWebDataPtr>> vResults;
		while (HaveInquiry()) { // һ����������ж������
			ASSERT(gl_systemConfiguration.IsWorkingMode()); // ���������
			ASSERT(IsInquiring());
			GetCurrentProduct();
			GenerateCurrentInquiryMessage();
			CDataInquireEnginePtr pEngine = make_shared<CInquireEngine>(m_internetOption, GetInquiringString(), GetHeaders());
			auto result = gl_runtime.thread_pool_executor()->submit([this, pEngine] {
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
		ASSERT(IsInquiring()); // ִ�е���ʱ���в����������´ε����ݡ�
		if (!gl_systemConfiguration.IsExitingSystem() && !pvWebData->empty()) {
			m_eErrorMessageData = IsAErrorMessageData(pvWebData->at(0)); // ���ص������Ǵ�����Ϣ���������ж������ʸ񣬸��½�ֹĿ¼
			m_pCurrentProduct->ParseAndStoreWebData(pvWebData);
			m_pCurrentProduct->UpdateDataSourceStatus(this->GetShared()); // ���ﴫ�ݵ���ʵ��DataSource����ָ��
		}
		ASSERT(!HaveInquiry()); // û���ִ������
		ASSERT(IsInquiring()); // ִ�е���ʱ���в����������´ε����ݡ�
		SetInquiring(false); // �˱�ʶ��������Ҫλ��λ�����һ��
	}
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
