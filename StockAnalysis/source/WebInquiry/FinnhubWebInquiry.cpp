#include"pch.h"

#include"Thread.h"

#include "FinnhubWebInquiry.h"
#include"WorldMarket.h"
#include"WebInquirer.h"

using namespace std;
#include<thread>

CFinnhubWebInquiry::CFinnhubWebInquiry() : CVirtualWebInquiry() {
	m_strInquiryFunction = _T(""); // finnhub有各种数据，故其前缀由数据申请函数每次设置，不同的前缀申请不同的数据。
	m_strInquiryToken = _T("");
	m_strConnectionName = _T("Finnhub");
	m_lInquiringNumber = 1; // Finnhub实时数据查询数量默认值

	ConfigerateSession();
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

void CFinnhubWebInquiry::ConfigerateSession(void) {
	ASSERT(m_pSession != nullptr);
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 15000); // 设置连接超时时间为15秒。 正常情况下Finnhub.io数据接收时间不超过5秒。
	m_pSession->SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, 15000); // 设置接收超时时间为15秒
	m_pSession->SetOption(INTERNET_OPTION_SEND_TIMEOUT, 1000); // 设置发送超时时间为1秒
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1); // 1次重试
}

/////////////////////////////////////////////////////////////////////////////////////
//
// 解析接收到的数据，默认数据格式为JSon, 使用boost perproty tree解析
//
////////////////////////////////////////////////////////////////////////////////////
bool CFinnhubWebInquiry::ParseData(CWebDataPtr pWebData) {
	return pWebData->ParseWithPropertyTree(0, 0);
}

void CFinnhubWebInquiry::ClearUpIfReadingWebDataFailed(void) {
	while (gl_pDataSourceFinnhub->GetReceivedDataSize() > 0) gl_pDataSourceFinnhub->GetReceivedData();
	gl_pDataSourceFinnhub->SetInquiring(false); // 当工作线程出现故障时，需要清除Finnhub数据申请标志。
}

/// <summary>
/// Finnhub.io在顺利读取后，会报告当前系统状态。如果报告的状态码为429时，说明读取频率超速，系统拒绝回答。
/// </summary>
/// <param name=""></param>
void CFinnhubWebInquiry::UpdateStatusAfterReadingWebData(void) {
	gl_pDataSourceFinnhub->SetDataReceived(true);// 接收完网络数据后，清除状态。
}

void CFinnhubWebInquiry::StoreWebData(CWebDataPtr pWebDataReceived) {
	gl_pDataSourceFinnhub->StoreReceivedData(pWebDataReceived);
}