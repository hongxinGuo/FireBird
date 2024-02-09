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
// todo 解析实时数据是本系统中最耗时的任务，考虑采用工作线程的模式改写之，这样可以利用多个CPU。可以四个解析任务并行执行。
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CProductSinaRT::ParseAndStoreWebData(CWebDataPtr pWebData) {
	const shared_ptr<vector<CWebRTDataPtr>> pvWebRTData = ParseSinaRTData(pWebData);

	for (const auto& pRTData : *pvWebRTData) {
		gl_qChinaMarketRTData.enqueue(pRTData);// 将此实时数据指针存入实时数据队列
	}

	//ParseSinaRTData2(pWebData); // 使用工作线程并行解析
}

bool CProductSinaRT::ParseSinaRT(vector<CWebRTDataPtr>&, CWebDataPtr pWebData) {
	return false;
}
