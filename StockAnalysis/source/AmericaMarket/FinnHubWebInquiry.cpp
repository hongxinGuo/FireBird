#include"pch.h"

#include"Thread.h"

#include "FinnhubWebInquiry.h"
#include"WorldMarket.h"
#include"WebInquirer.h"

using namespace std;
#include<thread>

CFinnhubWebInquiry::CFinnhubWebInquiry() : CVirtualWebInquiry() {
	m_strWebDataInquirePrefix = _T(""); // finnhub有各种数据，故其前缀由数据申请函数每次设置，不同的前缀申请不同的数据。
	m_strWebDataInquireSuffix = _T("");
	m_strConnectionName = _T("Finnhub");
	m_lInquiringNumber = 1; // Finnhub实时数据查询数量默认值

	ConfigerateSession();
}

CFinnhubWebInquiry::~CFinnhubWebInquiry() {
}

bool CFinnhubWebInquiry::PrepareNextInquiringStr(void) {
	CWebRTDataPtr pRTData = nullptr;

	CString strMiddle = _T("");

	// 申请下一批次股票实时数据。 此网络数据提取器使用FinnhubMarket
	// 由于Finnhub提供各种数据，而每个数据分别设计提取器会导致出现太多的提取器，故而在此分类。

	// 1 准备前缀字符串
	// 2. 准备中间字符串
	// 3. 准备后缀字符串
	//
	CreateTotalInquiringString(m_strWebDataInquireMiddle);

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CFinnhubWebInquiry::GetNextInquiringMiddleStr(long lTotalNumber, bool fSkipUnactiveStock) {
	// do nothing
	return _T("");
}

bool CFinnhubWebInquiry::ReportStatus(long lNumberOfData) const {
	TRACE("读入%d个Finnhub数据\n", lNumberOfData);
	return true;
}

void CFinnhubWebInquiry::ConfigerateSession(void) {
	ASSERT(m_pSession != nullptr);
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 15000); // 设置连接超时时间为15秒。 正常情况下Finnhub.io数据接收时间不超过5秒。
	m_pSession->SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, 15000); // 设置接收超时时间为15秒
	m_pSession->SetOption(INTERNET_OPTION_SEND_TIMEOUT, 500); // 设置发送超时时间为1秒
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_RETRIES, 0); // 0次重试
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
	gl_pWorldMarket->SetFinnhubInquiring(false); // 当工作线程出现故障时，需要清除Quandl数据申请标志。
}

void CFinnhubWebInquiry::UpdateAfterReadingWebData(void) {
	gl_pWorldMarket->SetFinnhubDataReceived(true); // 接收完网络数据后，清除状态。
}

void CFinnhubWebInquiry::StoreWebData(CWebDataPtr pWebDataReceived) {
	gl_pDataSourceFinnhub->StoreReceivedData(pWebDataReceived);
}