#include "pch.h"

#include "DataSource.h"
#include"SystemConfigeration.h"

CDataSource::CDataSource(void) {
	m_pWebInquiry = nullptr;
	m_pCurrentProduct = nullptr;

	Reset();
}

void CDataSource::Reset(void) {
	m_fInquiring = false;
	m_fDataReceived = true;
}

void CDataSource::Run(long lCurrentTime) {
	ProcessWebDataReceived();
	ProcessInquiringMessage();
}

////////////////////////////////////////////////////////////////////////////////////////////
//
// finnhub��ȡ������������ͽ����ֻ�ִ�з�ʽ���ʶ����ٵ������β����һ���ֻء�
//
/////////////////////////////////////////////////////////////////////////////////////////////
bool CDataSource::ProcessInquiringMessage(void) {
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
bool CDataSource::ProcessWebDataReceived(void) {
	CWebDataPtr pWebData = nullptr;
	bool fDone = false;

	if (m_pCurrentProduct == nullptr) return false;

	if (IsDataReceived()) { // ����������ݽ������
		if (m_qReveivedData.Size() > 0) {  // ����ǰ��������
			ASSERT(IsInquiring());
			pWebData = m_qReveivedData.PopData();

			m_pCurrentProduct->ParseAndStoreWebData(pWebData);

			m_pWebInquiry->SetInquiryingStringMiddle(_T("")); // ��Щ��������û���õ��м��ַ��Σ���������֮ǰ���м��ַ��Σ�����еĻ���������������ַ����Ĵ���
			SetInquiring(false);
			fDone = true;
			m_pCurrentProduct = nullptr;
		}
	}

	return fDone;
}