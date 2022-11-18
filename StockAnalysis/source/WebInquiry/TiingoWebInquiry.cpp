#include"pch.h"

#include"Thread.h"

#include "TiingoWebInquiry.h"
#include"WorldMarket.h"
#include"WebInquirer.h"

using namespace std;
#include<thread>

CTiingoWebInquiry::CTiingoWebInquiry() : CVirtualWebInquiry() {
	m_pDataSource = nullptr;
	m_strInquiryFunction = _T(""); // Tiingo�и������ݣ�����ǰ׺���������뺯��ÿ�����ã���ͬ��ǰ׺���벻ͬ�����ݡ�
	m_strInquiryToken = _T("");
	m_strConnectionName = _T("Tiingo");
	m_lInquiringNumber = 1; // Tiingoʵʱ���ݲ�ѯ����Ĭ��ֵ
}

CTiingoWebInquiry::~CTiingoWebInquiry() {
}

bool CTiingoWebInquiry::PrepareNextInquiringString(void) {
	// ����Tiingo�ṩ�������ݣ���ÿ�����ݷֱ������ȡ���ᵼ�³���̫�����ȡ�����ʶ��ڴ˷��ࡣ
	CreateTotalInquiringString(_T(""));

	return true;
}

bool CTiingoWebInquiry::ReportStatus(long lNumberOfData) const {
	TRACE("����%d��Tiingo����\n", lNumberOfData);
	return true;
}

void CTiingoWebInquiry::ConfigerateSession(void) {
	ASSERT(m_pSession != nullptr);
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 120000); // �������ӳ�ʱʱ��Ϊ120��
	m_pSession->SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, 120000); // ���ý��ճ�ʱʱ��Ϊ120��
	m_pSession->SetOption(INTERNET_OPTION_SEND_TIMEOUT, 2000); // ���÷��ͳ�ʱʱ��Ϊ2��
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1); // 2������
}

/////////////////////////////////////////////////////////////////////////////////////
//
// �������յ������ݣ�Ĭ�����ݸ�ʽΪJSon, ʹ��boost perproty tree����
//
////////////////////////////////////////////////////////////////////////////////////
bool CTiingoWebInquiry::ParseData(CWebDataPtr pWebData) {
	return pWebData->ParseWithPropertyTree(0, 0);
}

void CTiingoWebInquiry::ClearUpIfReadingWebDataFailed(void) {
	while (m_pDataSource->GetReceivedDataSize() > 0) m_pDataSource->GetReceivedData();
	m_pDataSource->SetInquiring(false); // �������̳߳��ֹ���ʱ����Ҫ���Tiingo���������־��
	gl_systemMessage.PushErrorMessage(_T("Tiingo�����̳߳���"));
}

void CTiingoWebInquiry::UpdateStatusAfterReadingWebData(void) {
	m_pDataSource->SetDataReceived(true); // �������������ݺ����״̬��
}

void CTiingoWebInquiry::StoreWebData(CWebDataPtr pWebDataBeStored) {
	m_pDataSource->StoreReceivedData(pWebDataBeStored);
}