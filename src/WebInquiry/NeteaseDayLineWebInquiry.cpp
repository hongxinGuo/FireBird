#include"pch.h"

#include"ChinaStockCodeConverter.h"
#include"ChinaMarket.h"

#include "NeteaseDayLineWebInquiry.h"
#include"NeteaseDayLineDataSource.h"

CNeteaseDayLineWebInquiry::CNeteaseDayLineWebInquiry() : CVirtualWebInquiry() {
	m_strInquiryFunction = _T("http://quotes.money.163.com/service/chddata.html?code=");
	m_strSuffix = _T("&fields=TCLOSE;HIGH;LOW;TOPEN;LCLOSE;CHG;TURNOVER;VOTURNOVER;VATURNOVER;TCAP;MCAP");
	m_strInquiryToken = _T("");
	m_strConnectionName = _T("NeteaseDayLine");

	ConfigureSession();
}

/////////////////////////////////////////////////////////////////////////////////
//
// ��ѯ�ַ����ĸ�ʽΪ��
//
//
////////////////////////////////////////////////////////////////////////////////
bool CNeteaseDayLineWebInquiry::PrepareNextInquiringString(void) {
	// ׼�������������������ʽ
	CString strMiddle = gl_pChinaMarket->CreateNeteaseDayLineInquiringStr();
	if (strMiddle.GetLength() > 0) {
		char buffer2[200];
		const CString strStockCode = XferNeteaseToStandard(strMiddle);
		SetDownLoadingStockCode(strStockCode);
		gl_systemMessage.SetStockCodeForInquiringNeteaseDayLine(strStockCode);
		strMiddle += _T("&start=19900101&end=");
		sprintf_s(buffer2, _T("%8d"), gl_pChinaMarket->GetMarketDate());
		strMiddle += buffer2;
		CreateTotalInquiringString(strMiddle);
		gl_pChinaMarket->CheckValidOfNeteaseDayLineInquiringStr(strMiddle);

		return true;
	}
	return false;
}

void CNeteaseDayLineWebInquiry::CreateTotalInquiringString(CString strMiddle) {
	m_strInquiry = m_strInquiryFunction + strMiddle + m_strSuffix + m_strInquiryToken;
}

void CNeteaseDayLineWebInquiry::ConfigureSession(void) {
	ASSERT(m_pSession != nullptr);
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 60000); // ���������Netease�������ݽ���ʱ�䲻����1�롣
	m_pSession->SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, 60000); // ���ý��ճ�ʱʱ��Ϊ5��
	m_pSession->SetOption(INTERNET_OPTION_SEND_TIMEOUT, 500); // ���÷��ͳ�ʱʱ��Ϊ500����
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1); // 1������
}

void CNeteaseDayLineWebInquiry::UpdateStatusAfterSucceed(CWebDataPtr pData) {
	pData->SetStockCode(GetDownLoadingStockCode());
}

/// <summary>
/// �����strStockCodeΪ��׼��ʽ��600000.SS��000001.SZ��
/// </summary>
/// <param name="strStockCode"></param>
void CNeteaseDayLineWebInquiry::SetDownLoadingStockCode(CString strStockCode) {
	m_strDownLoadingStockCode = strStockCode;
}
