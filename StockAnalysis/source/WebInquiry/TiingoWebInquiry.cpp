#include"pch.h"

#include "TiingoWebInquiry.h"
#include"TiingoDataSource.h"

using namespace std;
#include<thread>

CTiingoWebInquiry::CTiingoWebInquiry() : CVirtualWebInquiry() {
	m_strInquiryFunction = _T(""); // Tiingo有各种数据，故其前缀由数据申请函数每次设置，不同的前缀申请不同的数据。
	m_strInquiryToken = _T("");
	m_strConnectionName = _T("Tiingo");
	m_lInquiringNumber = 1; // Tiingo实时数据查询数量默认值
}

CTiingoWebInquiry::~CTiingoWebInquiry() {
}

bool CTiingoWebInquiry::PrepareNextInquiringString(void) {
	// 由于Tiingo提供各种数据，而每个数据分别设计提取器会导致出现太多的提取器，故而在此分类。
	CreateTotalInquiringString(_T(""));

	return true;
}

bool CTiingoWebInquiry::ReportStatus(long lNumberOfData) const {
	TRACE("读入%d个Tiingo数据\n", lNumberOfData);
	return true;
}

void CTiingoWebInquiry::ConfigerateSession(void) {
	ASSERT(m_pSession != nullptr);
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 120000); // 设置连接超时时间为120秒
	m_pSession->SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, 120000); // 设置接收超时时间为120秒
	m_pSession->SetOption(INTERNET_OPTION_SEND_TIMEOUT, 2000); // 设置发送超时时间为2秒
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1); // 2次重试
}

/////////////////////////////////////////////////////////////////////////////////////
//
// 解析接收到的数据，默认数据格式为JSon, 使用boost perproty tree解析
//
////////////////////////////////////////////////////////////////////////////////////
bool CTiingoWebInquiry::ParseData(CWebDataPtr pWebData) {
	return pWebData->ParseUsingPropertyTree(0, 0);
}