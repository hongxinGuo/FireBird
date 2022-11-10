#include "pch.h"

#include "VirtualDataSource.h"
#include"Thread.h"

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
		if (IsDataReceived()) { //�Ѿ�����������������Finnhub�����Ѿ����յ��ˣ�
			GetInquiry();

			m_pWebInquiry->SetInquiryFunction(m_pCurrentProduct->CreatMessage()); // ���ù����ַ���

			SetDataReceived(false); // ���ô˱�ʶ��Ҫ�������������̣߳�GetWebData��֮ǰ���������߳��еĶ������׳���
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
	vector<CWebDataPtr> vWebData;
	vector<CVirtualProductWebDataPtr> vProductWebData;

	if (m_pCurrentProduct == nullptr) return false;

	if (IsDataReceived()) { // ����������ݽ������
		if (m_qReceivedData.Size() > 0) {  // ����ǰ��������
			ASSERT(IsInquiring());
			pWebData = m_qReceivedData.PopData();
			m_pCurrentProduct->CheckNoRightToAccess(pWebData);
			if (m_pCurrentProduct->IsNoRightToAccess()) { // ���ϵͳ������Ȩ��ѯ��������
				// Ŀǰ�������ϵͳ��Ϣ�б���
				gl_systemMessage.PushInnerSystemInformationMessage(_T("No right to access: ") + m_pCurrentProduct->GetInquiry() + _T(",  Exchange = ") + m_pCurrentProduct->GetInquiringExchange());
				m_pCurrentProduct->AddInaccessibleExchangeIfNeeded(); // ����Ƿ���Ȩ��ѯ
			}
			// ��Щ�������ݱȽϴ󣬴�����Ҫ��ʱ�䳬�����������г��Ĺ�Ʊ������5M������ʱ��Ϊ��������
			// �ʶ���Ҫ���ǽ�������������̻߳���
			m_pCurrentProduct->ParseAndStoreWebData(pWebData);
			SetInquiring(false);
			// �������߳�ʵ�ַ�ʽ
			// vWebData[0] = pWebData;
			// vProductWebData[0] = m_pCurrentProduct;
			// thread thread1(ThreadWebSourceParseAndStoreWebData, this, vProductWebData, vWebData);
			// thread1.detach();
			return true;
		}
	}
	return false;
}

UINT ThreadWebSourceParseAndStoreWebData(CVirtualDataSource* pDataSource, vector<CVirtualProductWebDataPtr> vProductWebData, vector<CWebDataPtr> vWebData) {
	CVirtualProductWebDataPtr pProductWebData = vProductWebData.at(0);
	CWebDataPtr pWebData = vWebData.at(0);

	pProductWebData->ParseAndStoreWebData(pWebData);
	pDataSource->SetInquiring(false);

	return 203;
}