#include "pch.h"

#include "TengxunDayLineWebInquiry.h"

#include"ChinaMarket.h"

#include"TengxunDayLineDataSource.h"

CTengxunDayLineWebInquiry::CTengxunDayLineWebInquiry() : CVirtualWebInquiry() {
	m_strInquiryFunction = _T("https://web.ifzq.gtimg.cn/appstock/app/fqkline/get?param=");
	m_strSuffix = _T("");
	m_strInquiryToken = _T("");
	m_strConnectionName = _T("TengxunDayLine");

	ConfigureSession();
}

//
//
// 腾讯日线数据为json制式，目前在productTengxunDayLine中解析。
//
//
bool CTengxunDayLineWebInquiry::ParseData(CWebDataPtr pWebData) {
	return false;
}

/////////////////////////////////////////////////////////////////////////////////
//
// 查询字符串的格式为：
//
//
////////////////////////////////////////////////////////////////////////////////
bool CTengxunDayLineWebInquiry::PrepareNextInquiringString(void) {
	// 腾讯日线的申请信息由TengxunDayLineDataSource负责完成。
	CreateTotalInquiringString(_T(""));

	return true;
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
