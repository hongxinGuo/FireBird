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

using std::thread;

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
/// <summary>
/// DataSource�Ķ��㺯����
/// ��������ϢΪ��ʱ�����ɵ�ǰ��ѯ�ַ�����
/// ������������Ϣ��û���������еĲ�ѯ�߳�ʱ�����ɲ�ѯ�̡߳�
///
/// Note ���ú�������ʹ��thread_pool_executor����background_executor��ֻ��ʹ��thread_executor������thread_pool_executor�������̵߳ķ���ֵ�޷���ȡ��ԭ����顣
///
/// </summary>
///
/// ����ʹ�ö�����thread_executor�������У�����ʹ��thread_pool_executor����background_executor��
//  �����������ʹ�õ�thread_pool_executor����֮������ͻ�����²���ͬ�����⡣
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

void CVirtualDataSource::RunWorkingThread(const long lMarketTime) {
	SetWorkingThreadRunning(true);
	if (!IsInquiring()) {
		ASSERT(!HaveInquiry());
		GenerateInquiryMessage(lMarketTime);
	}

	if (IsInquiring()) {
		vector<result<CWebDataPtr>> vResults;
		while (HaveInquiry()) {
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
				SetCurrentInquiryTime(counter.GetElapsedMillisecond());
				return pWebData;
			});
			vResults.emplace_back(std::move(result));
		}
		const shared_ptr<vector<CWebDataPtr>> pvWebData = make_shared<vector<CWebDataPtr>>();
		for (auto& pWebData : vResults) {
			auto p = pWebData.get(); // Note ������ȴ����е��߳�ִ�����
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
