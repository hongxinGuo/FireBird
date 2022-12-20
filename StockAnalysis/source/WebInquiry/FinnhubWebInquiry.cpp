#include"pch.h"

#include"Thread.h"

#include "FinnhubWebInquiry.h"
#include"FinnhubDataSource.h"

CFinnhubWebInquiry::CFinnhubWebInquiry() : CVirtualWebInquiry() {
	m_strInquiryFunction = _T(""); // finnhub有各种数据，故其前缀由数据申请函数每次设置，不同的前缀申请不同的数据。
	m_strInquiryToken = _T("");
	m_strConnectionName = _T("Finnhub");
	m_lInquiringNumber = 1; // Finnhub实时数据查询数量默认值

	ConfigureSession();
}

CFinnhubWebInquiry::~CFinnhubWebInquiry() {
}

bool CFinnhubWebInquiry::PrepareNextInquiringString(void) {
	// 由于Finnhub提供各种数据，而每个数据分别设计提取器会导致出现太多的提取器，故而不在此处解析申请字符串，只是将解析后的字符串组装起来。
	// 具体的解析由各Product具体执行。
	CreateTotalInquiringString(_T(""));

	return true;
}

bool CFinnhubWebInquiry::ReportStatus(long lNumberOfData) const {
	TRACE("读入%d个Finnhub数据\n", lNumberOfData);
	return true;
}

void CFinnhubWebInquiry::ConfigureSession(void) {
	ASSERT(m_pSession != nullptr);
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 15000); // 设置连接超时时间为15秒。 正常情况下Finnhub.io数据接收时间不超过5秒。
	m_pSession->SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, 15000); // 设置接收超时时间为15秒
	m_pSession->SetOption(INTERNET_OPTION_SEND_TIMEOUT, 1000); // 设置发送超时时间为1秒
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1); // 1次重试
}

/////////////////////////////////////////////////////////////////////////////////////
//
//
// 使用nlohmann json解析。
//
////////////////////////////////////////////////////////////////////////////////////
bool CFinnhubWebInquiry::ParseData(CWebDataPtr pWebData) {
	return pWebData->ParseUsingNlohmannJson(0, 0);
}
