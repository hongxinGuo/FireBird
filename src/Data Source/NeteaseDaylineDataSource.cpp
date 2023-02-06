#include"pch.h"

#include"NeteaseDayLineDataSource.h"
#include"ProductNeteaseDayLine.h"

#include"ChinaMarket.h"
#include "ChinaStockCodeConverter.h"

CNeteaseDayLineDataSource::CNeteaseDayLineDataSource() {
	m_strInquiryFunction = _T("http://quotes.money.163.com/service/chddata.html?code=");
	m_strSuffix = _T("&fields=TCLOSE;HIGH;LOW;TOPEN;LCLOSE;CHG;TURNOVER;VOTURNOVER;VATURNOVER;TCAP;MCAP");
	m_strInquiryToken = _T("");

	CNeteaseDayLineDataSource::ConfigureSession();

	CNeteaseDayLineDataSource::Reset();
}

bool CNeteaseDayLineDataSource::Reset(void) {
	return true;
}

bool CNeteaseDayLineDataSource::UpdateStatus(void) {
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
// ץȡ��������.��ʼ��11:45:01
//
// ���ڿ��ܻ�ץȡȫ��5000�������������ݣ�����ʱ�䳬��10���ӣ��ʶ�9:15:00��һ������ϵͳʱ��ȥ���£�����9:25:00�ڶ�������ϵͳ��ſ�ʼ��
// Ϊ�˷�ֹ������ϵͳ������ͻ��ʵ��ִ��ʱ���Ӻ���11:45:01,�Ҳ�������ʵʱ���ݵĹ���ʱ��
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CNeteaseDayLineDataSource::Inquire(const long lCurrentTime) {
	if (gl_systemStatus.IsWebBusy()) return false; // �����������ʱ�������������������ݡ�
	if (gl_pChinaMarket->IsSystemReady() && gl_pChinaMarket->IsDayLineNeedUpdate() && gl_pChinaMarket->IsDummyTime()
		&& (gl_pChinaMarket->GetMarketTime() > 114500)) {
		if (!IsInquiring()) {
			const auto product = make_shared<CProductNeteaseDayLine>();
			StoreInquiry(product);
			SetInquiring(true);
			return true;
		}
	}
	return true;
}

void CNeteaseDayLineDataSource::CreateProduct() {
	// ׼�������������������ʽ
	const CString strMessage;
	CString strParam = gl_pChinaMarket->CreateNeteaseDayLineInquiringStr();
	char buffer2[200];
	const CString strStockCode = XferNeteaseToStandard(strParam);
	SetDownLoadingStockCode(strStockCode);
	gl_systemMessage.SetStockCodeForInquiringNeteaseDayLine(strStockCode);
	strParam += _T("&start=19900101&end=");
	sprintf_s(buffer2, _T("%8d"), gl_pChinaMarket->GetMarketDate());
	strParam += buffer2;
	m_strParam = strParam;
	gl_pChinaMarket->CheckValidOfNeteaseDayLineInquiringStr(strParam);
	m_pProductCurrentNeteaseDayLine = make_shared<CProductNeteaseDayLine>();
	m_pProductCurrentNeteaseDayLine->SetInquiry(strMessage);
}

/////////////////////////////////////////////////////////////////////////////////
//
// ��ѯ�ַ����ĸ�ʽΪ��
//
//
////////////////////////////////////////////////////////////////////////////////
bool CNeteaseDayLineDataSource::PrepareNextInquiringString(void) {
	// ׼�������������������ʽ
	CString strParam = gl_pChinaMarket->CreateNeteaseDayLineInquiringStr();
	if (strParam.GetLength() > 0) {
		char buffer2[200];
		const CString strStockCode = XferNeteaseToStandard(strParam);
		SetDownLoadingStockCode(strStockCode);
		gl_systemMessage.SetStockCodeForInquiringNeteaseDayLine(strStockCode);
		strParam += _T("&start=19900101&end=");
		sprintf_s(buffer2, _T("%8d"), gl_pChinaMarket->GetMarketDate());
		strParam += buffer2;
		m_strParam = strParam;
		CreateTotalInquiringString();
		gl_pChinaMarket->CheckValidOfNeteaseDayLineInquiringStr(strParam);

		return true;
	}
	return false;
}

void CNeteaseDayLineDataSource::ConfigureSession(void) {
	ASSERT(m_pSession != nullptr);
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 60000); // ���������Netease�������ݽ���ʱ�䲻����1�롣
	m_pSession->SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, 60000); // ���ý��ճ�ʱʱ��Ϊ5��
	m_pSession->SetOption(INTERNET_OPTION_SEND_TIMEOUT, 500); // ���÷��ͳ�ʱʱ��Ϊ500����
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1); // 1������
}

void CNeteaseDayLineDataSource::UpdateStatusAfterSucceed(CWebDataPtr pData) {
	pData->SetStockCode(GetDownLoadingStockCode());
}

/// <summary>
/// �����strStockCodeΪ��׼��ʽ��600000.SS��000001.SZ��
/// </summary>
/// <param name="strStockCode"></param>
void CNeteaseDayLineDataSource::SetDownLoadingStockCode(CString strStockCode) {
	m_strDownLoadingStockCode = strStockCode;
}
