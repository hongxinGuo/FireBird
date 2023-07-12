#include"pch.h"

#include"ProductSinaRT.h"
#include"ChinaMarket.h"
#include "ChinaStockCodeConverter.h"

#include"JsonParse.h"

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
	const CString strStocks = gl_pChinaMarket->GetSinaStockInquiringStr(gl_pSinaRTDataSource->GetInquiringNumber(), gl_pChinaMarket->IsCheckingActiveStock());
	const string_view strSinaStockCode = string_view(strStocks, 8); // 只提取第一个股票代码。新浪代码格式为：sh000001，共八个字符。
	gl_systemMessage.SetStockCodeForInquiringRTData(XferSinaToStandard(strSinaStockCode.data()));
	return m_strInquiryFunction + strStocks;
}

bool CProductSinaRT::ParseAndStoreWebData(CWebDataPtr pWebData) {
	const shared_ptr<vector<CWebRTDataPtr>> pvWebRTData = ParseSinaRTData(pWebData);

	gl_ProcessChinaMarketRTData.acquire();
	for (const auto& pRTData : *pvWebRTData) {
		gl_pChinaMarket->PushSinaRT(pRTData);// 将此实时数据指针存入实时数据队列
	}
	gl_ProcessChinaMarketRTData.release();

	return true;
}

bool CProductSinaRT::ParseSinaRT(vector<CWebRTDataPtr>&, CWebDataPtr pWebData) {
	return false;
}
