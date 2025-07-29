#include"pch.h"

#include"NeteaseDayLineDataSource.h"
#include"ProductNeteaseDayLine.h"

#include"ChinaMarket.h"
#include "ChinaStockCodeConverter.h"
#include "WebData.h"

CNeteaseDayLineDataSource::CNeteaseDayLineDataSource() {
	ASSERT(gl_systemConfiguration.IsInitialized());
	m_strInquiryFunction = _T("http://quotes.money.163.com/service/chddata.html?code=");
	m_strSuffix = _T("&fields=TCLOSE;HIGH;LOW;TOPEN;LCLOSE;CHG;TURNOVER;VOTURNOVER;VATURNOVER;TCAP;MCAP");

	CNeteaseDayLineDataSource::ConfigureInternetOption();

	CNeteaseDayLineDataSource::Reset();
}

bool CNeteaseDayLineDataSource::Reset() {
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
bool CNeteaseDayLineDataSource::GenerateInquiryMessage(const long lCurrentTime) {
	if (gl_systemConfiguration.IsWebBusy()) return false; // �����������ʱ�������������������ݡ�
	if (gl_pChinaMarket->IsSystemReady() && gl_dataContainerChinaStock.IsUpdateDayLine() && gl_pChinaMarket->IsDummyTime()
		&& (gl_pChinaMarket->GetMarketTime() > 114500)) {
		if (!IsInquiring()) {
			CreateProduct();
			return true;
		}
	}
	return false;
}

void CNeteaseDayLineDataSource::CreateProduct() {
	// ׼�������������������ʽ
	const string strMessage = _T("http://quotes.money.163.com/service/chddata.html?code=");;
	const string strSuffix = _T("&fields=TCLOSE;HIGH;LOW;TOPEN;LCLOSE;CHG;TURNOVER;VOTURNOVER;VATURNOVER;TCAP;MCAP");
	string strParam = gl_dataContainerChinaStock.CreateNeteaseDayLineInquiringStr();
	if (strParam.length() > 0) {
		char buffer2[200];
		const string strStockCode = XferNeteaseToStandard(strParam);
		SetDownLoadingStockCode(strStockCode);
		gl_systemMessage.SetStockCodeForInquiryDayLine(strStockCode);
		strParam += _T("");
		string s = fmt::format("&start=19900101&end={:8Ld}", gl_pChinaMarket->GetMarketDate());
		strParam += s;
		m_strParam = strParam;
		gl_pChinaMarket->CheckValidOfNeteaseDayLineInquiringStr(strParam);
		const CProductNeteaseDayLinePtr product = make_shared<CProductNeteaseDayLine>();
		product->SetMarket(gl_pChinaMarket);
		product->SetInquiryFunction(strMessage + strParam + strSuffix);
		StoreInquiry(product);
		SetInquiring(true);
	}
}

/////////////////////////////////////////////////////////////////////////////////
//
// ��ѯ�ַ����ĸ�ʽΪ��
//
//
////////////////////////////////////////////////////////////////////////////////
void CNeteaseDayLineDataSource::CreateCurrentInquireString() {
	m_strInquiry = m_pCurrentProduct->CreateMessage();
}

void CNeteaseDayLineDataSource::ConfigureInternetOption() {
	m_internetOption.option_connect_timeout = 5000;
	m_internetOption.option_receive_timeout = 5000;
	m_internetOption.option_data_receive_timeout = 5000;
	m_internetOption.option_send_timeout = 500;
	m_internetOption.option_connect_retries = 1;
}

void CNeteaseDayLineDataSource::UpdateStatus(CWebDataPtr pData) {
	pData->SetStockCode(GetDownLoadingStockCode());
}

/// <summary>
/// �����strStockCodeΪ��׼��ʽ��600000.SS��000001.SZ��
/// </summary>
/// <param name="strStockCode"></param>
void CNeteaseDayLineDataSource::SetDownLoadingStockCode(const string& strStockCode) {
	m_strDownLoadingStockCode = strStockCode;
}
