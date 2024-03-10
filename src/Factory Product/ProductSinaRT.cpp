#include"pch.h"

#include"ProductSinaRT.h"
#include"ChinaMarket.h"
#include "ChinaStockCodeConverter.h"
#include "HighPerformanceCounter.h"

#include <simdjson.h>

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
// 使用thread pool + coroutine协程并行解析，速度比单线程模式要快一倍以上。
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CProductSinaRT::ParseAndStoreWebData(CWebDataPtr pWebData) {
	//ParseSinaRTData(pWebData);
	ParseSinaRTDataUsingWorkingThread(pWebData); // 使用thread pool + coroutine协程并行解析，速度比单线程模式快一倍以上。

	/*
	CWebRTDataPtr p;
	CHighPerformanceCounter counter1, counter2;
	counter1.start();
	ParseSinaRTDataUsingWorkingThread(pWebData); // 使用thread pool + coroutine协程并行解析，速度比单线程模式快40%。
	counter1.stop();
	int total2 = 0;
	while (gl_qChinaMarketRTData.try_dequeue(p)) total2++;
	counter2.start();
	ParseSinaRTData(pWebData); // 使用thread pool + coroutine协程并行解析，速度比单线程模式快40%。
	counter2.stop();
	int total = 0;
	while (gl_qChinaMarketRTData.try_dequeue(p)) total++;
	ASSERT(total2 == total); // Note 由于计算实时数据得线程并行执行，这里偶尔会不相等而导致判断错误。忽略之。
	double f = static_cast<double>(counter1.GetElapseTick()) / counter2.GetElapseTick();
	double f2 = f;
	*/
}
