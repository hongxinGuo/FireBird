#include "pch.h"

#include "TengxunDayLineWebInquiry.h"

#include"ChinaStockCodeConverter.h"
#include"ChinaMarket.h"

#include"TengxunDayLineDataSource.h"
#include "TimeConvert.h"

CTengxunDayLineWebInquiry::CTengxunDayLineWebInquiry() : CVirtualWebInquiry() {
	m_strInquiryFunction = _T("https://web.ifzq.gtimg.cn/appstock/app/fqkline/get?param=");
	m_strSuffix = _T("");
	m_strInquiryToken = _T("");
	m_strConnectionName = _T("TengxunDayLine");

	ConfigureSession();
}

/////////////////////////////////////////////////////////////////////////////////
//
// 查询字符串的格式为：
//
//
////////////////////////////////////////////////////////////////////////////////
bool CTengxunDayLineWebInquiry::PrepareNextInquiringString(void) {
	// 准备网易日线数据申请格式
	CString strMiddle = gl_pChinaMarket->CreateTengxunDayLineInquiringStr();
	if (strMiddle.GetLength() > 0) {
		char buffer[20];
		const CString strStockCode = XferTengxunToStandard(strMiddle);
		SetDownLoadingStockCode(strStockCode);
		gl_systemMessage.SetStockCodeForInquiringNeteaseDayLine(strStockCode);
		strMiddle += _T(",day,,,");
		long lDiffer = TimeSpawn(gl_pChinaMarket->GetMarketDate(), gl_pChinaMarket->GetStock(strStockCode)->GetDayLineEndDate());
		lDiffer++; // 由于腾讯日线不提供前收盘数据，故需往前多提取一日的数据
		if (lDiffer > 2000) lDiffer = 2000; // 目前腾讯日线只能提供2000个数据。
		sprintf_s(buffer, _T("%d"), lDiffer);
		strMiddle += buffer;
		strMiddle += _T(",,");
		CreateTotalInquiringString(strMiddle);

		return true;
	}
	return false;
}

void CTengxunDayLineWebInquiry::CreateTotalInquiringString(CString strMiddle) {
	m_strInquiry = m_strInquiryFunction + strMiddle + m_strSuffix + m_strInquiryToken;
}

void CTengxunDayLineWebInquiry::ConfigureSession(void) {
	ASSERT(m_pSession != nullptr);
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 60000); // 正常情况下Tengxun日线数据接收时间不超过1秒。
	m_pSession->SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, 60000); // 设置接收超时时间为5秒
	m_pSession->SetOption(INTERNET_OPTION_SEND_TIMEOUT, 500); // 设置发送超时时间为500毫秒
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1); // 1次重试
}

void CTengxunDayLineWebInquiry::UpdateStatusAfterSucceed(CWebDataPtr pData) {
	pData->SetStockCode(GetDownLoadingStockCode());
}

/// <summary>
/// 这里的strStockCode为标准制式：600000.SS，000001.SZ，
/// </summary>
/// <param name="strStockCode"></param>
void CTengxunDayLineWebInquiry::SetDownLoadingStockCode(CString strStockCode) {
	m_strDownLoadingStockCode = strStockCode;
}
