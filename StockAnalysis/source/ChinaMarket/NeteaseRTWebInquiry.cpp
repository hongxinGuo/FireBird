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
#ifdef _DEBUG
	m_lInquiringNumber = 900; // 网易实时数据查询默认值
#else
	m_lInquiringNumber = 900; // 网易实时数据查询默认值
#endif
}

CNeteaseRTWebInquiry::~CNeteaseRTWebInquiry() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 目前neteaseRTData使用nlohmann json库函数解析，其解析速度大致是property tree的两倍。
// 解析900个网易数据，DEBUG模式下大致时间要300-400毫秒；Release模式下也就50毫秒。
//
// 在此解析数据的话，系统容易锁死，原因大概与网络读取数据有关，但确切原因不明，故而决定将解析功能移至ChinaMarket的后台工作线程中。
// 且在使用全局优化来编译JsonParse.cpp文件后，其速度在DEBUG模式下也能够达到要求，故而无需特别处理了。
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
bool CNeteaseRTWebInquiry::ParseData(CWebDataPtr pWebData) {
	ASSERT(pWebData->GetDataBuffer().at(pWebData->GetBufferLength() - 1) == ';');
	ASSERT(pWebData->GetDataBuffer().at(pWebData->GetBufferLength() - 2) == ')');
	ASSERT(pWebData->GetDataBuffer().at(pWebData->GetBufferLength() - 3) == '}');

	/*
	LARGE_INTEGER liBegin{ 0,0 }, liEnd{ 0,0 }, d1, d2;
	bool fBegin = false, fEnd = false;
	long long  differ1 = 0, differ2 = 0, differ3;

	fBegin = QueryPerformanceCounter(&liBegin);
	for (int i = 0; i < 100; i++) {
		pWebData->CreatePropertyTree(21, 2);
	}
	fEnd = QueryPerformanceCounter(&liEnd);
	differ1 = liEnd.QuadPart - liBegin.QuadPart;

	fBegin = QueryPerformanceCounter(&liBegin);
	for (int i = 0; i < 100; i++) {
		pWebData->CreateNlohmannJSon(21, 2);
	}
	fEnd = QueryPerformanceCounter(&liEnd);
	differ2 = liEnd.QuadPart - liBegin.QuadPart;
	*/

	return false;
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