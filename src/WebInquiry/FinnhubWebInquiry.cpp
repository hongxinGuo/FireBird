#include"pch.h"

#include"Thread.h"

#include "FinnhubWebInquiry.h"
#include"FinnhubDataSource.h"

CFinnhubWebInquiry::CFinnhubWebInquiry() : CVirtualWebInquiry() {
	m_strInquiryFunction = _T(""); // finnhub�и������ݣ�����ǰ׺���������뺯��ÿ�����ã���ͬ��ǰ׺���벻ͬ�����ݡ�
	m_strInquiryToken = _T("");
	m_strConnectionName = _T("Finnhub");
	m_lInquiringNumber = 1; // Finnhubʵʱ���ݲ�ѯ����Ĭ��ֵ

	ConfigureSession();
}

CFinnhubWebInquiry::~CFinnhubWebInquiry() {}

bool CFinnhubWebInquiry::PrepareNextInquiringString(void) {
	// ����Finnhub�ṩ�������ݣ���ÿ�����ݷֱ������ȡ���ᵼ�³���̫�����ȡ�����ʶ����ڴ˴����������ַ�����ֻ�ǽ���������ַ�����װ������
	// ����Ľ����ɸ�Product����ִ�С�
	CreateTotalInquiringString(_T(""));

	return true;
}

bool CFinnhubWebInquiry::ReportStatus(long lNumberOfData) const {
	TRACE("����%d��Finnhub����\n", lNumberOfData);
	return true;
}

void CFinnhubWebInquiry::ConfigureSession(void) {
	ASSERT(m_pSession != nullptr);
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 15000); // �������ӳ�ʱʱ��Ϊ15�롣 ���������Finnhub.io���ݽ���ʱ�䲻����5�롣
	m_pSession->SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, 15000); // ���ý��ճ�ʱʱ��Ϊ15��
	m_pSession->SetOption(INTERNET_OPTION_SEND_TIMEOUT, 1000); // ���÷��ͳ�ʱʱ��Ϊ1��
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1); // 1������
}

/////////////////////////////////////////////////////////////////////////////////////
//
//
// ʹ��nlohmann json������
//
////////////////////////////////////////////////////////////////////////////////////
bool CFinnhubWebInquiry::ParseData(CWebDataPtr pWebData) {
	return pWebData->CreateNlohmannJson(0, 0);
}