#include "pch.h"

#include "VirtualDataSource.h"
#include"Thread.h"

CVirtualDataSource::CVirtualDataSource(void) {
	m_pWebInquiry = nullptr;
	m_pCurrentProduct = nullptr;

	Reset();
}

CVirtualDataSource::~CVirtualDataSource(void) {
	m_pWebInquiry = nullptr;
}

bool CVirtualDataSource::Reset(void) {
	m_fInquiring = false;
	m_fDataReceived = true;
	m_fPermitToConcurrentProceed = false; // Ĭ�ϲ������д�����յ������ݣ������̱߳����ڴ��������ݺ󣬷�������Inquiry��ʶ��

	return true;
}

void CVirtualDataSource::Run(long lCurrentTime) {
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
	if (m_qProduct.size() > 0) { // ������ȴ���
		ASSERT(IsInquiring());
		if (IsDataReceivedAndClearFlag()) { //�Ѿ�����������������Finnhub�����Ѿ����յ��ˣ����ô˱�ʶ��Ҫ�������������̣߳�GetWebData��֮ǰ���������߳��еĶ������׳���
			GetInquiry();
			m_pWebInquiry->SetInquiryFunction(m_pCurrentProduct->CreatMessage()); // ���ù����ַ���
			m_pWebInquiry->GetWebData();
			return true;
		}
	}
	return false;
}

//////////////////////////////////////////////
//
// �������߳̽��յ���������Ϣ��
//
// Ŀǰֻ����ͬʱ����һ����Ϣ������Ϣ��������������֮ǰ������������һ����Ϣ�������ܹ���֤ͬһ�ԡ�������Finnhub��վ���ٶ����ƣ�
// ÿ����ֻ����60�����룬�ʶ�û�б�Ҫǿ�������ٶȡ�
//
//////////////////////////////////////////////
bool CVirtualDataSource::ProcessWebDataReceived(void) {
	CWebDataPtr pWebData = nullptr;

	if (m_pCurrentProduct == nullptr) return false;

	if (IsDataReceived()) { // ����������ݽ������
		if (m_qReceivedData.Size() > 0) {  // ����ǰ��������
			ASSERT(IsInquiring());
			pWebData = m_qReceivedData.PopData();
			if (pWebData->IsParsed()) {
				m_pCurrentProduct->CheckNoRightToAccess(pWebData);
				if (m_pCurrentProduct->IsNoRightToAccess()) { // ���ϵͳ������Ȩ��ѯ��������
					// Ŀǰ�������ϵͳ��Ϣ�б���
					gl_systemMessage.PushInnerSystemInformationMessage(_T("No right to access: ") + m_pCurrentProduct->GetInquiry() + _T(",  Exchange = ") + m_pCurrentProduct->GetInquiringExchange());
					m_pCurrentProduct->AddInaccessibleExchangeIfNeeded(); // ����Ƿ���Ȩ��ѯ
				}
			}
			// ��Щ�������ݱȽϴ󣬴�����Ҫ��ʱ�䳬�����������г��Ĺ�Ʊ������5M������ʱ��Ϊ��������
			// �ʶ���Ҫ��ProductWebData�ĺ���ParseAndStoreWebData()�̻߳���
			thread thread1(ThreadWebSourceParseAndStoreWebData, this, m_pCurrentProduct, pWebData);
			thread1.detach();
			return true;
		}
	}
	return false;
}

UINT ThreadWebSourceParseAndStoreWebData(not_null<CVirtualDataSource*> pDataSource, not_null<CVirtualProductWebDataPtr> pProductWebData, not_null<CWebDataPtr> pWebData) {
	if (pDataSource->IsPermitToConcurrentProceed()) { // ��������д������ݵĻ�
		pDataSource->SetInquiring(false); // ����ϵͳ���������µ����ݣ�Ȼ���ٴ�����յ�������
		pProductWebData->ParseAndStoreWebData(pWebData);
	}
	else { // ����������д������ݵĻ���
		pProductWebData->ParseAndStoreWebData(pWebData); // �ڴ��������ݺ󣬷�����ϵͳ�����µ����ݡ�
		pDataSource->SetInquiring(false); // �˱�ʶ��������Ҫλ�ڸù����߳��С� ��λ�����һ�����Ա�֤�ٴ���������ʱ�Ѵ���������һ�ν��յ������ݡ�
	}
	return 203;
}