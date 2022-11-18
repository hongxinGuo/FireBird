#include"pch.h"

#include"StockCodeConverter.h"
#include"Thread.h"
#include"ChinaMarket.h"

#include"SinaRTDataSource.h"

#include "SinaRTWebInquiry.h"
#include"WebInquirer.h"

using namespace std;
#include<thread>

/// <summary>
/// 新浪实时数据服务器要求提供报头验证数据。
///
/// </summary>
CSinaRTWebInquiry::CSinaRTWebInquiry() : CVirtualWebInquiry() {
	// 2022年1月20日后，新浪实时数据服务器需要添加报头验证数据，格式为： Referer:https://finance.sina.com.cn
	// User-Agent部分只用于说明格式,即报头皆以\r\n（CRLF)结束
	m_strHeaders = _T("User-Agent:FireBird\r\nReferer:https://finance.sina.com.cn\r\n");

	m_strInquiryFunction = _T("https://hq.sinajs.cn/list="); // 新浪实时数据服务器已使用https格式
	m_strInquiryToken = _T("");
	m_strConnectionName = _T("SinaRT");
	m_lInquiringNumber = 850; // 新浪实时数据查询数量默认值

	ConfigerateSession();
}

CSinaRTWebInquiry::~CSinaRTWebInquiry() {
}

bool CSinaRTWebInquiry::PrepareNextInquiringString(void) {
	CString strMiddle = _T("");
	CString strSinaStockCode;

	// 申请下一批次股票实时数据
	// 如果处于寻找今日活跃股票期间（9:10--9:29, 11:31--12:59),则使用全局股票池
	// 开市时使用今日活跃股票池
	strMiddle = GetNextInquiringMiddleString(m_lInquiringNumber, gl_pChinaMarket->IsCheckingActiveStock());
	strSinaStockCode = strMiddle.Left(8); // 只提取第一个股票代码。新浪代码格式为：sh000001，共八个字符。
	gl_systemMessage.SetStockCodeForInquiringRTData(XferSinaToStandred(strSinaStockCode));
	CreateTotalInquiringString(strMiddle);

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 新浪网络实时数据提取函数。
// 目前只提取前12000个股票的实时数据。
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CSinaRTWebInquiry::GetNextInquiringMiddleString(long lTotalNumber, bool fUsingTotalStockSet) {
	return gl_pChinaMarket->GetSinaStockInquiringStr(lTotalNumber, fUsingTotalStockSet);
}

/// <summary>
/// 网易实时数据接收时，不时会出现解析失败的情况。感觉原因出在网易数据服务器发送错误，但还要继续观察。
/// 调整Session的选项可能会改善接收情况。
/// </summary>
/// <param name=""></param>
void CSinaRTWebInquiry::ConfigerateSession(void) {
	ASSERT(m_pSession != nullptr);
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 150); // 正常情况下sina实时数据接收时间不超过50毫秒。
	m_pSession->SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, 150); // 设置接收超时时间为150毫秒
	m_pSession->SetOption(INTERNET_OPTION_SEND_TIMEOUT, 50); // 设置发送超时时间为50毫秒
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1); // 1次重试
}

bool CSinaRTWebInquiry::ReportStatus(long lNumberOfData) const {
	TRACE("读入%d个新浪实时数据\n", lNumberOfData);
	return true;
}

void CSinaRTWebInquiry::ClearUpIfReadingWebDataFailed(void) {
	while (gl_pSinaRTDataSource->GetReceivedDataSize() > 0) gl_pSinaRTDataSource->GetReceivedData();
	gl_pSinaRTDataSource->SetInquiring(false); // 当工作线程出现故障时，需要清除Quandl数据申请标志。
}

/// <summary>
/// Finnhub.io在顺利读取后，会报告当前系统状态。如果报告的状态码为429时，说明读取频率超速，系统拒绝回答。
/// </summary>
/// <param name=""></param>
void CSinaRTWebInquiry::UpdateStatusAfterReadingWebData(void) {
	gl_pSinaRTDataSource->SetDataReceived(true); // 接收完网络数据后，清除状态。
}

void CSinaRTWebInquiry::StoreWebData(CWebDataPtr pWebDataBeStored) {
	gl_WebInquirer.PushSinaRTData(pWebDataBeStored);
	gl_pSinaRTDataSource->StoreReceivedData(pWebDataBeStored);
}