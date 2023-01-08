#include"pch.h"

#include"WorldMarket.h"

#include "QuandlWebInquiry.h"
#include"QuandlDataSource.h"

CQuandlWebInquiry::CQuandlWebInquiry() : CVirtualWebInquiry() {
	m_strInquiryFunction = _T(""); // Quandl�и������ݣ�����ǰ׺���������뺯��ÿ�����ã���ͬ��ǰ׺���벻ͬ�����ݡ�
	m_strInquiryToken = _T(""); // ���԰�ʹ��ymail�˻�
	m_strConnectionName = _T("Quandl");
	m_lInquiringNumber = 1; // Quandlʵʱ���ݲ�ѯ����Ĭ��ֵ

	ConfigureSession();
}

bool CQuandlWebInquiry::PrepareNextInquiringString(void) {
	// ����Quandl�ṩ�������ݣ���ÿ�����ݷֱ������ȡ���ᵼ�³���̫�����ȡ�����ʶ��ڴ˷��ࡣ
	CreateTotalInquiringString(_T(""));

	return true;
}

void CQuandlWebInquiry::CreateTotalInquiringString(CString strMiddle) {
	m_strInquiry = m_strInquiryFunction + strMiddle + m_strSuffix + _T("&api_key=") + m_strInquiryToken;
}

bool CQuandlWebInquiry::ReportStatus(long lNumberOfData) const {
	TRACE("����%d������ʵʱ����\n", lNumberOfData);
	return true;
}

void CQuandlWebInquiry::ConfigureSession(void) {
	ASSERT(m_pSession != nullptr);
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 120000); // �������ӳ�ʱʱ��Ϊ120��
	m_pSession->SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, 120000); // ���ý��ճ�ʱʱ��Ϊ120��
	m_pSession->SetOption(INTERNET_OPTION_SEND_TIMEOUT, 2000); // ���÷��ͳ�ʱʱ��Ϊ2��
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1); // 2������
}

/////////////////////////////////////////////////////////////////////////////////////
//
//
// ʹ��nlohmann json������
//
////////////////////////////////////////////////////////////////////////////////////
bool CQuandlWebInquiry::ParseData(CWebDataPtr pWebData) {
	return pWebData->CreateNlohmannJson(0, 0);
}
