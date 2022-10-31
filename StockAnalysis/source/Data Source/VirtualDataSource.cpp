#include "pch.h"

#include "VirtualDataSource.h"

CVirtualDataSource::CVirtualDataSource(void) {
	m_pWebInquiry = nullptr;
	m_pCurrentProduct = nullptr;

	Reset();
}

bool CVirtualDataSource::Reset(void) {
	m_fInquiring = false;
	m_fDataReceived = true;

	return true;
}

void CVirtualDataSource::Run(long lCurrentTime) {
	Inquiry(lCurrentTime);
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
	bool fDone = false;

	if (m_qProduct.size() > 0) { // ������ȴ���
		ASSERT(IsInquiring());
		if (IsDataReceived()) { //�Ѿ�����������������Finnhub�����Ѿ����յ��ˣ�
			m_pCurrentProduct = m_qProduct.front();
			m_qProduct.pop();

			m_pWebInquiry->SetInquiryingStringPrefix(m_pCurrentProduct->CreatMessage()); // ����ǰ׺

			SetDataReceived(false); // ���ô˱�ʶ��Ҫ�������������̣߳�GetWebData��֮ǰ���������߳��еĶ������׳���
			m_pWebInquiry->GetWebData();
			fDone = true;
		}
	}

	return fDone;
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
	bool fDone = false;

	if (m_pCurrentProduct == nullptr) return false;

	if (IsDataReceived()) { // ����������ݽ������
		if (m_qReceivedData.Size() > 0) {  // ����ǰ��������
			ASSERT(IsInquiring());
			pWebData = m_qReceivedData.PopData();

			m_pCurrentProduct->ParseAndStoreWebData(pWebData);
			if (m_pCurrentProduct->NoRightToAccess()) { // ���ϵͳ������Ȩ��ѯ��������
				// Ŀǰ�������ϵͳ��Ϣ�б���
				gl_systemMessage.PushInnerSystemInformationMessage(_T("No right to access ") + m_pCurrentProduct->GetInquiringStr());
				if (!m_pCurrentProduct->IsUSMarket()) { // ֻ�е����������г�ʱ���ż�����ݵĿɶ�ȡ�ԡ������г���Ĭ��Ϊ�ɶ�ȡ��
					m_pCurrentProduct->AddInaccessibleExchangeIfNeeded(); // ����Ƿ���Ȩ��ѯ
				}
			}
			m_pWebInquiry->SetInquiryingStringMiddle(_T("")); // ��Щ��������û���õ��м��ַ��Σ���������֮ǰ���м��ַ��Σ�����еĻ���������������ַ����Ĵ���
			SetInquiring(false);
			fDone = true;
		}
	}

	return fDone;
}