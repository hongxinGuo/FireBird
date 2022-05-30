#include"pch.h"
#include"globedef.h"
#include"SystemMessage.h"
#include"Thread.h"
#include"ChinaMarket.h"

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

	m_strWebDataInquirePrefix = _T("https://hq.sinajs.cn/list="); // 新浪实时数据服务器已使用https格式
	m_strWebDataInquireSuffix = _T("");
	m_strConnectionName = _T("SinaRT");
	m_lInquiringNumber = 850; // 新浪实时数据查询数量默认值
}

CSinaRTWebInquiry::~CSinaRTWebInquiry() {
}

bool CSinaRTWebInquiry::PrepareNextInquiringStr(void) {
	CString strMiddle = _T("");
	CString strSinaStockCode;

	// 申请下一批次股票实时数据
	// 如果处于寻找今日活跃股票期间（9:10--9:29, 11:31--12:59),则使用全局股票池
	// 开市时使用今日活跃股票池
	strMiddle = GetNextInquiringMiddleStr(m_lInquiringNumber, gl_pChinaMarket->IsCheckingActiveStock());
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
CString CSinaRTWebInquiry::GetNextInquiringMiddleStr(long lTotalNumber, bool fUsingTotalStockSet) {
	return gl_pChinaMarket->GetSinaStockInquiringStr(lTotalNumber, fUsingTotalStockSet);
}

void CSinaRTWebInquiry::InitializeSession(void) {
	if (m_pSession != nullptr) delete m_pSession;
	m_pSession = new CInternetSession{ _T("StockAnalysis") };
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 200); // 正常情况下sina实时数据接收时间不超过50毫秒。
	m_pSession->SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, 200); // 设置接收超时时间为200毫秒
	m_pSession->SetOption(INTERNET_OPTION_SEND_TIMEOUT, 100); // 设置发送超时时间为2秒
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1); // 1次重试
}

bool CSinaRTWebInquiry::ReportStatus(long lNumberOfData) const {
	TRACE("读入%d个新浪实时数据\n", lNumberOfData);
	return true;
}

void CSinaRTWebInquiry::StoreWebData(CWebDataPtr pWebDataBeStored) {
	gl_WebInquirer.PushSinaRTData(pWebDataBeStored);
}