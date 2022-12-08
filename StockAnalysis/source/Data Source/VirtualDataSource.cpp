#include "pch.h"

#include "VirtualDataSource.h"
#include"Thread.h"
#include"ThreadStatus.h"

CVirtualDataSource::CVirtualDataSource(void) {
	m_pWebInquiry = nullptr;
	m_pCurrentProduct = nullptr;
	m_fEnable = true; // Ĭ��Ϊ����ִ��

	Reset();
}

CVirtualDataSource::~CVirtualDataSource(void) {
}

bool CVirtualDataSource::Reset(void) {
	m_fInquiring = false;
	m_fWebInquiryFinished = true;

	return true;
}

void CVirtualDataSource::Run(long lCurrentTime) {
	if (!m_fEnable) return; // ������ִ�еĻ���ֱ�ӷ���
	Inquire(lCurrentTime);
	if (ProcessWebDataReceived()) { // �ȴ�����յ�����������
		UpdateStatus();
	}
	ProcessInquiringMessage(); // Ȼ����������һ����������
}

////////////////////////////////////////////////////////////////////////////////////////////
//
// DataSource��ȡ������������ͽ����ֻ�ִ�з�ʽ���ʶ����ٵ������β����һ���ֻء�
//
/////////////////////////////////////////////////////////////////////////////////////////////
bool CVirtualDataSource::ProcessInquiringMessage(void) {
	if (HaveInquiry()) { // ������ȴ���
		if (IsWebInquiryFinishedAndClearFlag()) { //�Ѿ����������������������Ѿ����յ��ˣ����ô˱�ʶ��Ҫ�������������̣߳�GetWebData��֮ǰ���������߳��еĶ������׳���
			GetInquiry();
			SetCurrentInquiryFunction(m_pCurrentProduct->CreateMessage()); // ���ù����ַ���
			StartThreadGetWebData();
			return true;
		}
	}
	return false;
}

//////////////////////////////////////////////
//
// �������߳̽��յ���������Ϣ��
//
// ֻ����ͬʱ����һ����Ϣ������Ϣ��������������֮ǰ������������һ����Ϣ�������ܹ���֤ͬһ�ԡ�������Finnhub��վ���ٶ����ƣ�
// ÿ����ֻ����60�����룬�ʶ�û�б�Ҫǿ�������ٶȡ�
//
// Ŀǰ�ѽ��й��г���SinaRT��NeteaseRT��TengxunRT��NeteaseDayLine������ʹ�ô�����Դʵ���ˡ�����SinaRT��NeteaseRTҪ���ʵʱ�ԱȽ�ǿ��
// �ʶ�����������Դ�����ȷ��غ������ݵ�ģʽ��--20221120
//
//////////////////////////////////////////////
bool CVirtualDataSource::ProcessWebDataReceived(void) {
	CWebDataPtr pWebData = nullptr;

	if (HaveReceiviedData()) {  // ����ǰ��������
		ASSERT(m_pCurrentProduct != nullptr);
		pWebData = GetReceivedData();
		if (pWebData->IsParsed()) {
			m_pCurrentProduct->CheckNoRightToAccess(pWebData);
			if (m_pCurrentProduct->IsNoRightToAccess()) { // ���ϵͳ������Ȩ��ѯ��������
				// Ŀǰ�������ϵͳ��Ϣ�б���
				gl_systemMessage.PushInnerSystemInformationMessage(_T("No right to access: ") + m_pCurrentProduct->GetInquiry() + _T(",  Exchange = ") + m_pCurrentProduct->GetInquiringExchange());
				m_pCurrentProduct->AddInaccessibleExchangeIfNeeded(); // ����Ƿ���Ȩ��ѯ
			}
		}
		ASSERT(IsInquiring()); // ִ�е���ʱ���в����������´ε����ݡ�
		SetInquiring(false); // �˱�ʶ��������Ҫλ��λ�����һ��
		// ��Щ�������ݱȽϴ󣬴�����Ҫ��ʱ�䳬�����������г��Ĺ�Ʊ������5M������ʱ��Ϊ���������� �ʶ���Ҫ��ProductWebData�ĺ���ParseAndStoreWebData()�̻߳���
		// ���̱߳���λ�ڱ�����������������SetInquiry(false)���������´����룬�ʶ��ܷ�ֹ�����������⡣
		thread thread1(ThreadWebSourceParseAndStoreWebData, this, m_pCurrentProduct, pWebData);
		thread1.detach();
		return true;
	}
	return false;
}

// ���ź������ڽ���WebSource�е����ݡ�
// ��ParseAndStoreData�߳����������3�����������ܱ�֤�㹻�ļ����ٶȣ�Ҳ���ᷢ��ϵͳ����������Ϊ4��ʱ�����ܹ۲쵽ϵͳ������
counting_semaphore<3> gl_WebSourceParseAndStoreData{ 3 };

UINT ThreadWebSourceParseAndStoreWebData(not_null<CVirtualDataSource*> pDataSource, not_null<CVirtualProductWebDataPtr> pProductWebData, not_null<CWebDataPtr> pWebData) {
	gl_WebSourceParseAndStoreData.acquire();
	gl_ThreadStatus.IncreaseBackGroundWorkingThread();
	pDataSource->ParseAndStoreData(pProductWebData, pWebData);
	gl_ThreadStatus.DecreaseBackGroundWorkingThread();
	gl_WebSourceParseAndStoreData.release();

	return 203;
}

/// <summary>
///
/// </summary>
/// <param name="pProductWebData"></param>
/// <param name="pWebData"></param>
void CVirtualDataSource::ParseAndStoreData(CVirtualProductWebDataPtr pProductWebData, CWebDataPtr pWebData) {
	pProductWebData->ParseAndStoreWebData(pWebData);
}