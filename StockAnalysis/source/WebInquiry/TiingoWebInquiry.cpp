#include"pch.h"

#include "TiingoWebInquiry.h"
#include"TiingoDataSource.h"

CTiingoWebInquiry::CTiingoWebInquiry() : CVirtualWebInquiry() {
	m_strInquiryFunction = _T(""); // Tiingo�и������ݣ�����ǰ׺���������뺯��ÿ�����ã���ͬ��ǰ׺���벻ͬ�����ݡ�
	m_strInquiryToken = _T("");
	m_strConnectionName = _T("Tiingo");
	m_lInquiringNumber = 1; // Tiingoʵʱ���ݲ�ѯ����Ĭ��ֵ
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

void CTiingoWebInquiry::ConfigureSession(void) {
	ASSERT(m_pSession != nullptr);
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 120000); // �������ӳ�ʱʱ��Ϊ120��
	m_pSession->SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, 120000); // ���ý��ճ�ʱʱ��Ϊ120��
	m_pSession->SetOption(INTERNET_OPTION_SEND_TIMEOUT, 500); // ���÷��ͳ�ʱʱ��Ϊ2��
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_RETRIES, 2); // 2������
}

/////////////////////////////////////////////////////////////////////////////////////
//
//
// ʹ��nlohmann json������
//
////////////////////////////////////////////////////////////////////////////////////
bool CTiingoWebInquiry::ParseData(CWebDataPtr pWebData) {
	return pWebData->CreateNlohmannJson(0, 0);
}