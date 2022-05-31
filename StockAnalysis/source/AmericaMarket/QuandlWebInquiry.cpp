#include"pch.h"
#include"globedef.h"
#include"Thread.h"
#include"ThreadStatus.h"
#include"SystemMessage.h"

#include"WorldMarket.h"

#include "QuandlWebInquiry.h"
#include"WebInquirer.h"

using namespace std;
#include<thread>

//  api_key=zBMXMyoTyiy_N3pMb3ex
//m_strWebDataInquireSuffix = _T("&api_key=zBMXMyoTyiy_N3pMb3ex"); // 密钥放在最后
// 下面的是第二个,用于dell240工作机。
//m_strWebDataInquireSuffix = _T("&api_key=zBMXMyoTyiy_N3pMb3ex"); // 密钥放在最后
CQuandlWebInquiry::CQuandlWebInquiry() : CVirtualWebInquiry() {
	m_strWebDataInquirePrefix = _T(""); // Quandl有各种数据，故其前缀由数据申请函数每次设置，不同的前缀申请不同的数据。

	// finnhub不允许一个账户同时用于两个机器上，故而使用两个账户，看看能否避开速度限制。
	// 有可能一个网址只允许一个账户运行，则这种两个账户的方法也不起作用。
	if (m_strWebDataInquireSuffix.GetLength() < 5) {
#ifdef DEBUG
		m_strWebDataInquireSuffix = _T("&api_key=zBMXMyoTyiy_N3pMb3ex"); // 调试版使用ymail账户
#else
		m_strWebDataInquireSuffix = _T("&api_key=zBMXMyoTyiy_N3pMb3ex"); // 发行版使用hotmail账户
#endif // DEBUG
	}
	m_strConnectionName = _T("Quandl");
	m_lInquiringNumber = 1; // Quandl实时数据查询数量默认值

	ConfigerateSession();
}

CQuandlWebInquiry::~CQuandlWebInquiry() {
}

bool CQuandlWebInquiry::PrepareNextInquiringStr(void) {
	CWebRTDataPtr pRTData = nullptr;

	CString strMiddle = _T("");

	// 申请下一批次股票实时数据。 此网络数据提取器使用QuandlMarket
	// 由于Quandl提供各种数据，而每个数据分别设计提取器会导致出现太多的提取器，故而在此分类。

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
CString CQuandlWebInquiry::GetNextInquiringMiddleStr(long lTotalNumber, bool fSkipUnactiveStock) {
	// do nothing
	return _T("");
}

bool CQuandlWebInquiry::ReportStatus(long lNumberOfData) const {
	TRACE("读入%d个新浪实时数据\n", lNumberOfData);
	return true;
}

void CQuandlWebInquiry::ConfigerateSession(void) {
	ASSERT(m_pSession != nullptr);
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 120000); // 设置连接超时时间为120秒
	m_pSession->SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, 120000); // 设置接收超时时间为120秒
	m_pSession->SetOption(INTERNET_OPTION_SEND_TIMEOUT, 2000); // 设置发送超时时间为2秒
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1); // 2次重试
}

void CQuandlWebInquiry::ClearUpIfReadingWebDataFailed(void) {
	while (gl_WebInquirer.QuandlDataSize() > 0) gl_WebInquirer.PopQuandlData();
	gl_pWorldMarket->SetQuandlInquiring(false); // 当工作线程出现故障时，需要清除Quandl数据申请标志。
	gl_systemMessage.PushErrorMessage(_T("Quandl工作线程出错"));
}

void CQuandlWebInquiry::UpdateAfterReadingWebData(void) {
	gl_pWorldMarket->SetQuandlDataReceived(true); // 接收完网络数据后，清除状态。
}

void CQuandlWebInquiry::StoreWebData(CWebDataPtr pWebDataBeStored) {
	gl_WebInquirer.PushQuandlData(pWebDataBeStored);
}