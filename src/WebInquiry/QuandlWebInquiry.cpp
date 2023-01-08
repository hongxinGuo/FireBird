#include"pch.h"

#include"WorldMarket.h"

#include "QuandlWebInquiry.h"
#include"QuandlDataSource.h"

CQuandlWebInquiry::CQuandlWebInquiry() : CVirtualWebInquiry() {
	m_strInquiryFunction = _T(""); // Quandl有各种数据，故其前缀由数据申请函数每次设置，不同的前缀申请不同的数据。
	m_strInquiryToken = _T(""); // 调试版使用ymail账户
	m_strConnectionName = _T("Quandl");
	m_lInquiringNumber = 1; // Quandl实时数据查询数量默认值

	ConfigureSession();
}

bool CQuandlWebInquiry::PrepareNextInquiringString(void) {
	// 由于Quandl提供各种数据，而每个数据分别设计提取器会导致出现太多的提取器，故而在此分类。
	CreateTotalInquiringString(_T(""));

	return true;
}

void CQuandlWebInquiry::CreateTotalInquiringString(CString strMiddle) {
	m_strInquiry = m_strInquiryFunction + strMiddle + m_strSuffix + _T("&api_key=") + m_strInquiryToken;
}

bool CQuandlWebInquiry::ReportStatus(long lNumberOfData) const {
	TRACE("读入%d个新浪实时数据\n", lNumberOfData);
	return true;
}

void CQuandlWebInquiry::ConfigureSession(void) {
	ASSERT(m_pSession != nullptr);
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 120000); // 设置连接超时时间为120秒
	m_pSession->SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, 120000); // 设置接收超时时间为120秒
	m_pSession->SetOption(INTERNET_OPTION_SEND_TIMEOUT, 2000); // 设置发送超时时间为2秒
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1); // 2次重试
}

/////////////////////////////////////////////////////////////////////////////////////
//
//
// 使用nlohmann json解析。
//
////////////////////////////////////////////////////////////////////////////////////
bool CQuandlWebInquiry::ParseData(CWebDataPtr pWebData) {
	return pWebData->CreateNlohmannJson(0, 0);
}
