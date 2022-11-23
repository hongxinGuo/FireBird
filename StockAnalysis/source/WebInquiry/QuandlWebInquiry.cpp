#include"pch.h"

#include"WorldMarket.h"

#include "QuandlWebInquiry.h"
#include"QuandlDataSource.h"

using namespace std;
#include<thread>

//  api_key=zBMXMyoTyiy_N3pMb3ex
//m_strInquiryToken = _T("&api_key=zBMXMyoTyiy_N3pMb3ex"); // ��Կ�������
// ������ǵڶ���,����dell240��������
//m_strInquiryToken = _T("&api_key=zBMXMyoTyiy_N3pMb3ex"); // ��Կ�������
CQuandlWebInquiry::CQuandlWebInquiry() : CVirtualWebInquiry() {
	m_strInquiryFunction = _T(""); // Quandl�и������ݣ�����ǰ׺���������뺯��ÿ�����ã���ͬ��ǰ׺���벻ͬ�����ݡ�

	// finnhub������һ���˻�ͬʱ�������������ϣ��ʶ�ʹ�������˻��������ܷ�ܿ��ٶ����ơ�
	// �п���һ����ַֻ����һ���˻����У������������˻��ķ���Ҳ�������á�
	if (m_strInquiryToken.GetLength() < 5) {
#ifdef DEBUG
		m_strInquiryToken = _T("&api_key=zBMXMyoTyiy_N3pMb3ex"); // ���԰�ʹ��ymail�˻�
#else
		m_strInquiryToken = _T("&api_key=zBMXMyoTyiy_N3pMb3ex"); // ���а�ʹ��hotmail�˻�
#endif // DEBUG
	}
	m_strConnectionName = _T("Quandl");
	m_lInquiringNumber = 1; // Quandlʵʱ���ݲ�ѯ����Ĭ��ֵ

	ConfigerateSession();
}

CQuandlWebInquiry::~CQuandlWebInquiry() {
}

bool CQuandlWebInquiry::PrepareNextInquiringString(void) {
	// ����Quandl�ṩ�������ݣ���ÿ�����ݷֱ������ȡ���ᵼ�³���̫�����ȡ�����ʶ��ڴ˷��ࡣ
	CreateTotalInquiringString(_T(""));

	return true;
}

bool CQuandlWebInquiry::ReportStatus(long lNumberOfData) const {
	TRACE("����%d������ʵʱ����\n", lNumberOfData);
	return true;
}

void CQuandlWebInquiry::ConfigerateSession(void) {
	ASSERT(m_pSession != nullptr);
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 120000); // �������ӳ�ʱʱ��Ϊ120��
	m_pSession->SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, 120000); // ���ý��ճ�ʱʱ��Ϊ120��
	m_pSession->SetOption(INTERNET_OPTION_SEND_TIMEOUT, 2000); // ���÷��ͳ�ʱʱ��Ϊ2��
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_RETRIES, 2); // 2������
}

/////////////////////////////////////////////////////////////////////////////////////
//
// �������յ������ݣ�Ĭ�����ݸ�ʽΪJSon, ʹ��boost perproty tree����
//
////////////////////////////////////////////////////////////////////////////////////
bool CQuandlWebInquiry::ParseData(CWebDataPtr pWebData) {
	return pWebData->ParseUsingPropertyTree(0, 0);
}

void CQuandlWebInquiry::ClearUpIfReadingWebDataFailed(void) {
	while (gl_pDataSourceQuandl->GetReceivedDataSize() > 0) gl_pDataSourceQuandl->GetReceivedData();
	gl_pDataSourceQuandl->SetInquiring(false); // �������̳߳��ֹ���ʱ����Ҫ������������־��
	gl_systemMessage.PushErrorMessage(_T("Quandl�����̳߳���"));
}

void CQuandlWebInquiry::UpdateStatusAfterReadingWebData(void) {
	gl_pDataSourceQuandl->SetDataReceived(true); // �������������ݺ����״̬��
}

void CQuandlWebInquiry::StoreWebData(CWebDataPtr pWebDataBeStored) {
	gl_pDataSourceQuandl->StoreReceivedData(pWebDataBeStored);
}