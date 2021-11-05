#include"pch.h"
#include"globedef.h"
#include"Thread.h"
#include"ChinaMarket.h"

#include "NeteaseDayLineWebInquiry.h"
#include"WebInquirer.h"

using namespace std;
#include<thread>

CNeteaseDayLineWebInquiry::CNeteaseDayLineWebInquiry() : CVirtualWebInquiry() {
	m_strWebDataInquirePrefix = _T("http://quotes.money.163.com/service/chddata.html?code=");
	m_strWebDataInquireSuffix = _T("&fields=TCLOSE;HIGH;LOW;TOPEN;LCLOSE;CHG;TURNOVER;VOTURNOVER;VATURNOVER;TCAP;MCAP");
	m_strConnectionName = _T("NeteaseDayLine");
}

CNeteaseDayLineWebInquiry::~CNeteaseDayLineWebInquiry() {
}

/////////////////////////////////////////////////////////////////////////////////
//
// ��ѯ�ַ����ĸ�ʽΪ��
//
//
////////////////////////////////////////////////////////////////////////////////
bool CNeteaseDayLineWebInquiry::PrepareNextInquiringStr(void) {
	CString strMiddle = _T("");
	char buffer2[200];
	CString strStockCode;

	// ׼�������������������ʽ
	if (gl_pChinaMarket->CreateNeteaseDayLineInquiringStr(strMiddle, gl_pChinaMarket->GetTotalStock())) {
		strStockCode = XferNeteaseToStandred(strMiddle);
		SetDownLoadingStockCode(strStockCode);
		gl_pChinaMarket->SetStockCodeForInquiringNeteaseDayLine(strStockCode);
		strMiddle += _T("&start=19900101&end=");
		sprintf_s(buffer2, _T("%8d"), gl_pChinaMarket->GetMarketDate());
		strMiddle += buffer2;
		CreateTotalInquiringString(strMiddle);
		gl_pChinaMarket->CheckValidOfNeteaseDayLineInquiringStr(strMiddle);

		return true;
	}
	return false;
}

void CNeteaseDayLineWebInquiry::UpdateStatusWhenSecceed(CWebDataPtr pData) {
	pData->SetStockCode(GetDownLoadingStockCode());
}

void CNeteaseDayLineWebInquiry::ClearUpIfReadingWebDataFailed(void) {
	CString strErrorMessage;
	strErrorMessage = GetDownLoadingStockCode();
	strErrorMessage += _T(" �������߶�ȡ�̳߳���");
	gl_systemMessage.PushErrorMessage(strErrorMessage);
}

void CNeteaseDayLineWebInquiry::StoreWebData(CWebDataPtr pWebDataBeStored) {
	gl_WebInquirer.PushNeteaseDayLineData(pWebDataBeStored);
}

/// <summary>
/// �����strStockCodeΪ��׼��ʽ��600000.SS��000001.SZ��
/// </summary>
/// <param name="strStockCode"></param>
void CNeteaseDayLineWebInquiry::SetDownLoadingStockCode(CString strStockCode) {
	m_strDownLoadingStockCode = strStockCode;
}