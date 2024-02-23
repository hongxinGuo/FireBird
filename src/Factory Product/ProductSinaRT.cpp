#include"pch.h"

#include"ProductSinaRT.h"
#include"ChinaMarket.h"
#include "ChinaStockCodeConverter.h"
#include "HighPerformanceCounter.h"

#include"JsonParse.h"
#include "SinaRTDataSource.h"

CProductSinaRT::CProductSinaRT() {
	m_lCurrentStockPosition = 0;
	m_strInquiryFunction = _T("https://hq.sinajs.cn/list=");
}

/////////////////////////////////////////////////////////////////////////////////////
//
// 申请下一批次股票实时数据
// 如果处于寻找今日活跃股票期间（9:10--9:29, 11:31--12:59),则使用全局股票池
// 开市时使用今日活跃股票池
//
/////////////////////////////////////////////////////////////////////////////////////
CString CProductSinaRT::CreateMessage() {
	const CString strStocks = GetMarket()->GetSinaStockInquiringStr(gl_pSinaRTDataSource->GetInquiringNumber(), GetMarket()->IsCheckingActiveStock());
	const string_view strSinaStockCode = string_view(strStocks, 8); // 只提取第一个股票代码。新浪代码格式为：sh000001，共八个字符。
	gl_systemMessage.SetStockCodeForInquiringRTData(XferSinaToStandard(strSinaStockCode).c_str());
	return m_strInquiryFunction + strStocks;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 使用thread pool + coroutine协程并行解析，速度比单线程模式要快15%。
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CProductSinaRT::ParseAndStoreWebData(CWebDataPtr pWebData) {
	ParseSinaRTData(pWebData);
	//ParseSinaRTDataUsingWorkingThread(pWebData); // 使用thread pool + coroutine协程并行解析，速度比单线程模式快40%。

	/*
	CWebRTDataPtr p;
	CHighPerformanceCounter counter1, counter2;
	ParseSinaRTDataUsingWorkingThread(pWebData); // 使用thread pool + coroutine协程并行解析，速度比单线程模式快40%。
	int total2 = 0;
	while (gl_qChinaMarketRTData.try_dequeue(p)) total2++;
	int i = total2;
	ParseSinaRTData(pWebData); // 使用thread pool + coroutine协程并行解析，速度比单线程模式快40%。
	int total = 0;
	while (gl_qChinaMarketRTData.try_dequeue(p)) total++;
	i = total;
	*/
}
