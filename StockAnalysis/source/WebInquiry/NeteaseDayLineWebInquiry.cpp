#include"pch.h"

#include"StockCodeConverter.h"
#include"ChinaMarket.h"

#include "NeteaseDayLineWebInquiry.h"
#include"NeteaseDayLineDataSource.h"


#include<thread>

CNeteaseDayLineWebInquiry::CNeteaseDayLineWebInquiry() : CVirtualWebInquiry() {
	m_strInquiryFunction = _T("http://quotes.money.163.com/service/chddata.html?code=");
	m_strInquiryToken = _T("&fields=TCLOSE;HIGH;LOW;TOPEN;LCLOSE;CHG;TURNOVER;VOTURNOVER;VATURNOVER;TCAP;MCAP");
	m_strConnectionName = _T("NeteaseDayLine");

	ConfigerateSession();
}

CNeteaseDayLineWebInquiry::~CNeteaseDayLineWebInquiry() {
}

/////////////////////////////////////////////////////////////////////////////////
//
// 查询字符串的格式为：
//
//
////////////////////////////////////////////////////////////////////////////////
bool CNeteaseDayLineWebInquiry::PrepareNextInquiringString(void) {
	CString strMiddle = _T("");
	char buffer2[200];
	CString strStockCode;

	// 准备网易日线数据申请格式
	if (gl_pChinaMarket->CreateNeteaseDayLineInquiringStr(strMiddle)) {
		strStockCode = XferNeteaseToStandard(strMiddle);
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

void CNeteaseDayLineWebInquiry::ConfigerateSession(void)
{
	ASSERT(m_pSession != nullptr);
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 60000); // 正常情况下Netease日线数据接收时间不超过1秒。
	m_pSession->SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, 60000); // 设置接收超时时间为5秒
	m_pSession->SetOption(INTERNET_OPTION_SEND_TIMEOUT, 500); // 设置发送超时时间为500毫秒
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1); // 1次重试
}

void CNeteaseDayLineWebInquiry::UpdateStatusAfterSucceed(CWebDataPtr pData) {
	pData->SetStockCode(GetDownLoadingStockCode());
}

/// <summary>
/// 这里的strStockCode为标准制式：600000.SS，000001.SZ，
/// </summary>
/// <param name="strStockCode"></param>
void CNeteaseDayLineWebInquiry::SetDownLoadingStockCode(CString strStockCode) {
	m_strDownLoadingStockCode = strStockCode;
}