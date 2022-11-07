#include"pch.h"

#include"Thread.h"

#include "FinnhubWebInquiry.h"
#include"WorldMarket.h"
#include"WebInquirer.h"

using namespace std;
#include<thread>

CFinnhubWebInquiry::CFinnhubWebInquiry() : CVirtualWebInquiry() {
	m_strInquiryFunction = _T(""); // finnhub�и������ݣ�����ǰ׺���������뺯��ÿ�����ã���ͬ��ǰ׺���벻ͬ�����ݡ�
	m_strInquiryToken = _T("");
	m_strConnectionName = _T("Finnhub");
	m_lInquiringNumber = 1; // Finnhubʵʱ���ݲ�ѯ����Ĭ��ֵ

	ConfigerateSession();
}

CFinnhubWebInquiry::~CFinnhubWebInquiry() {
}

bool CFinnhubWebInquiry::PrepareNextInquiringStr(void) {
	// ����Finnhub�ṩ�������ݣ���ÿ�����ݷֱ������ȡ���ᵼ�³���̫�����ȡ�����ʶ��ڴ˷��ࡣ
	CreateTotalInquiringString(_T(""));

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CFinnhubWebInquiry::GetNextInquiringMiddleStr(long lTotalNumber, bool fSkipUnactiveStock) {
	// do nothing
	return _T("");
}

bool CFinnhubWebInquiry::ReportStatus(long lNumberOfData) const {
	TRACE("����%d��Finnhub����\n", lNumberOfData);
	return true;
}

void CFinnhubWebInquiry::ConfigerateSession(void) {
	ASSERT(m_pSession != nullptr);
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 15000); // �������ӳ�ʱʱ��Ϊ15�롣 ���������Finnhub.io���ݽ���ʱ�䲻����5�롣
	m_pSession->SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, 15000); // ���ý��ճ�ʱʱ��Ϊ15��
	m_pSession->SetOption(INTERNET_OPTION_SEND_TIMEOUT, 1000); // ���÷��ͳ�ʱʱ��Ϊ1��
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1); // 1������
}

/////////////////////////////////////////////////////////////////////////////////////
//
// �������յ������ݣ�Ĭ�����ݸ�ʽΪJSon, ʹ��boost perproty tree����
//
////////////////////////////////////////////////////////////////////////////////////
bool CFinnhubWebInquiry::ParseData(CWebDataPtr pWebData) {
	return pWebData->ParseWithPropertyTree(0, 0);
}

void CFinnhubWebInquiry::ClearUpIfReadingWebDataFailed(void) {
	while (gl_pDataSourceFinnhub->GetReceivedDataSize() > 0) gl_pDataSourceFinnhub->GetReceivedData();
	gl_pWorldMarket->SetFinnhubInquiring(false); // �������̳߳��ֹ���ʱ����Ҫ���Quandl���������־��
}

void CFinnhubWebInquiry::UpdateStatusAfterReadingWebData(void) {
	gl_pWorldMarket->SetFinnhubDataReceived(true); // �������������ݺ����״̬��
}

void CFinnhubWebInquiry::StoreWebData(CWebDataPtr pWebDataReceived) {
	gl_pDataSourceFinnhub->StoreReceivedData(pWebDataReceived);
}