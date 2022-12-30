/////////////////////////////////////////////////////////////////////////////////////////////////
//
// 从网络文件file中读取网易制式实时数据，返回值是所读数据是否出现格式错误。
// 开始处为第一个{，结束处为倒数第二个}。如果尚有数据需处理，则被处理的字符为','；如果没有数据了，则被处理的字符为' '。
//
// 要获取最新行情，访问数据接口：http://api.money.126.net/data/feed/0601872
//
// _ntes_quote_callback({"0601872":{"code": "0601872", "percent": 0.038251, "high": 5.72, "askvol3": 311970, "askvol2": 257996,
//                      "askvol5": 399200, "askvol4": 201000, "price": 5.7, "open": 5.53, "bid5": 5.65, "bid4": 5.66, "bid3": 5.67,
//                       "bid2": 5.68, "bid1": 5.69, "low": 5.51, "updown": 0.21, "type": "SH", "symbol": "601872", "status": 0,
//                       "ask4": 5.73, "bidvol3": 234700, "bidvol2": 166300, "bidvol1": 641291, "update": "2019/11/04 15:59:54",
//                       "bidvol5": 134500, "bidvol4": 96600, "yestclose": 5.49, "askvol1": 396789, "ask5": 5.74, "volume": 78750304,
//                       "ask1": 5.7, "name": "\u62db\u5546\u8f6e\u8239", "ask3": 5.72, "ask2": 5.71, "arrow": "\u2191",
//                        "time": "2019/11/04 15:59:52", "turnover": 443978974} });
//
// 网易实时数据缺少关键性的成交金额一项，故而无法作为基本数据，只能作为补充用。(turnover就是成交金额，不缺了）。
//
// 网易实时数据服务器的发送频率为每六秒更新一次，比新浪实时数据的更新速度（每三秒一次）要慢一倍，故而做为第一备选。
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include"ChinaStockCodeConverter.h"
#include"ChinaMarket.h"

#include "NeteaseRTWebInquiry.h"
#include"NeteaseRTDataSource.h"

CNeteaseRTWebInquiry::CNeteaseRTWebInquiry() : CVirtualWebInquiry() {
	m_strInquiryFunction = _T("http://api.money.126.net/data/feed/");
	m_strInquiryToken = _T("");
	m_strConnectionName = _T("NeteaseRT");
	m_fReportStatus = false;
	m_lInquiringNumber = 900; // 网易实时数据查询默认值

	ConfigureSession();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 目前neteaseRTData使用nlohmann json库函数解析，其解析速度大致是property tree的两倍。
// 解析900个网易数据，DEBUG模式下大致时间要300-400毫秒；Release模式下也就50毫秒。
//
// 在此解析数据的话，系统容易锁死，原因大概与网络读取数据有关，但确切原因不明，故而决定将解析功能移至DataSource的工作线程中。
// 且在使用全局优化来编译JsonParse.cpp文件后，其速度在DEBUG模式下也能够达到要求，故而无需特别处理了。
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
bool CNeteaseRTWebInquiry::ParseData(CWebDataPtr pWebData) {
	return false;
}

bool CNeteaseRTWebInquiry::ReportStatus(long lNumberOfData) const {
	TRACE("读入%d个网易实时数据\n", lNumberOfData);
	return true;
}

bool CNeteaseRTWebInquiry::PrepareNextInquiringString(void) {
	// 申请下一批次股票实时数据
	const CString strMiddle = GetNextInquiringMiddleString(m_lInquiringNumber, gl_pChinaMarket->IsCheckingActiveStock()); // 目前还是使用全部股票池
	const CString strNeteaseStockCode = strMiddle.Left(7); //只提取第一个股票代码.网易代码格式为：0600000，100001，共七个字符
	gl_systemMessage.SetStockCodeForInquiringRTData(XferNeteaseToStandard(strNeteaseStockCode));
	CreateTotalInquiringString(strMiddle);

	return true;
}

CString CNeteaseRTWebInquiry::GetNextInquiringMiddleString(long lTotalNumber, bool fUsingTotalStockSet) {
	return gl_pChinaMarket->GetNeteaseStockInquiringMiddleStr(lTotalNumber, fUsingTotalStockSet);
}

/// <summary>
/// 网易实时数据的接收，不时出现错误数据。目前无法确定是服务器本身的错误还是接收中出现的错误。如果服务器本身就发送错了，就无法修正了。
/// 修改session各参数，试试能否解决问题。
/// </summary>
/// <param name=""></param>
void CNeteaseRTWebInquiry::ConfigureSession(void) {
	ASSERT(m_pSession != nullptr);
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 1000); // 设置连接超时时间为4000毫秒。 正常情况下网易实时数据接收时间不超过200毫秒。
	m_pSession->SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, 1000); // 设置接收超时时间为4000毫秒
	m_pSession->SetOption(INTERNET_OPTION_SEND_TIMEOUT, 200); // 设置发送超时时间为500毫秒
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_RETRIES, 2); // 重试一次。
}
