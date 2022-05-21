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
// 网易实时数据缺少关键性的成交金额一项，故而无法作为基本数据，只能作为补充用。
//
//////////////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"
#include"globedef.h"
#include"SystemMessage.h"
#include"ThreadStatus.h"
#include"ChinaMarket.h"

#include"Thread.h"
#include "NeteaseRTWebInquiry.h"

#include"WebInquirer.h"

using namespace std;
#include<thread>

CNeteaseRTWebInquiry::CNeteaseRTWebInquiry() : CVirtualWebInquiry() {
	m_strWebDataInquirePrefix = _T("http://api.money.126.net/data/feed/");
	m_strWebDataInquireSuffix = _T("");
	m_strConnectionName = _T("NeteaseRT");
	m_fReportStatus = false;
	m_lInquiringNumber = 900; // 网易实时数据查询默认值
}

CNeteaseRTWebInquiry::~CNeteaseRTWebInquiry() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 目前neteaseRTData使用nlohmann json库函数解析。由于json::parse()函数的运行时间较长，导致使用debug模式编译后的系统解析时间太长，
// 故而决定使用独立线程来解析接收到的数据，并存入数据队列中。
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
bool CNeteaseRTWebInquiry::ParseAndStoreData(CWebDataPtr pWebData) {
	thread thread1(ThreadParseAndStoreNeteaseRTData, this, pWebData);
	thread1.detach();
	return true;
}

UINT ThreadParseAndStoreNeteaseRTData(CNeteaseRTWebInquiry * pInquiry, CWebDataPtr pWebData) {
	pInquiry->ParseData(pWebData);
	if (pWebData->IsParsed()) {
		pInquiry->StoreWebData(pWebData);
	}
	return 1000;
}

bool CNeteaseRTWebInquiry::ParseData(CWebDataPtr pWebData) {
	pWebData->SetParsed(pWebData->CreateJSon(pWebData->GetJSon(), 21, 2));
	pWebData->SetJSonContentType(true);
	return pWebData->IsParsed();
}

bool CNeteaseRTWebInquiry::ReportStatus(long lNumberOfData) const {
	TRACE("读入%d个网易实时数据\n", lNumberOfData);
	return true;
}

bool CNeteaseRTWebInquiry::PrepareNextInquiringStr(void) {
	CString strMiddle = _T("");
	CString strNeteaseStockCode;
	// 申请下一批次股票实时数据
	strMiddle = GetNextInquiringMiddleStr(m_lInquiringNumber, gl_pChinaMarket->IsCheckingActiveStock()); // 目前还是使用全部股票池
	strNeteaseStockCode = strMiddle.Left(7); //只提取第一个股票代码.网易代码格式为：0600000，100001，共七个字符
	gl_systemMessage.SetStockCodeForInquiringRTData(XferNeteaseToStandred(strNeteaseStockCode));
	CreateTotalInquiringString(strMiddle);

	return true;
}

CString CNeteaseRTWebInquiry::GetNextInquiringMiddleStr(long lTotalNumber, bool fUsingTotalStockSet) {
	return gl_pChinaMarket->GetNeteaseStockInquiringMiddleStr(lTotalNumber, fUsingTotalStockSet);
}

void CNeteaseRTWebInquiry::StoreWebData(CWebDataPtr pWebDataBeStored) {
	gl_WebInquirer.PushNeteaseRTData(pWebDataBeStored);
}