#include"pch.h"

#include"ProductSinaRT.h"
#include"ChinaMarket.h"
#include "ChinaStockCodeConverter.h"

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
	gl_systemMessage.SetStockCodeForInquiringRTData(XferSinaToStandard(strSinaStockCode));
	return m_strInquiryFunction + strStocks;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// 使用工作线程模式改写后，实际执行时间却变长了。估计时线程切换时间太长的缘故。不采用工作线程模式。
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CProductSinaRT::ParseAndStoreWebData(CWebDataPtr pWebData) {
	ParseSinaRTData(pWebData);
	//ParseSinaRTDataUsingWorkingThread(pWebData); // 使用工作线程并行解析
}

bool CProductSinaRT::ParseSinaRT(vector<CWebRTDataPtr>&, CWebDataPtr pWebData) {
	return false;
}
