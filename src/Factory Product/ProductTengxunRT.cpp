#include"pch.h"

#include"ProductTengxunRT.h"
#include"JsonParse.h"
#include "TengxunRTDataSource.h"

CProductTengxunRT::CProductTengxunRT() {
	m_lCurrentStockPosition = 0;
	m_strInquiryFunction = _T("http://qt.gtimg.cn/q=");
}

CString CProductTengxunRT::CreateMessage() {
	// 申请下一批次股票实时数据。
	// 申请腾讯实时数据时，如果遇到不存在的股票代码，服务器会返回v_pv_none_match="1";，导致系统故障，
	// 故而现在只使用有效股票代码。
	const CString strStocks = gl_dataContainerChinaStock.GetNextTengxunStockInquiringMiddleStr(gl_pTengxunRTDataSource->GetInquiringNumber()); // 使用活跃股票池
	return m_strInquiryFunction + strStocks;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// todo 解析实时数据是本系统中最耗时的任务，考虑采用工作线程的模式改写之，这样可以利用多个CPU。可以四个解析任务并行执行。
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CProductTengxunRT::ParseAndStoreWebData(CWebDataPtr pWebData) {
	const shared_ptr<vector<CWebRTDataPtr>> pvWebRTData = ParseTengxunRTData(pWebData);

	gl_ProcessChinaMarketRTData.acquire();
	for (const auto& pRTData : *pvWebRTData) {
		gl_qTengxunRT.PushData(pRTData);// 将此实时数据指针存入实时数据队列
	}
	gl_ProcessChinaMarketRTData.release();
	return true;
}

bool CProductTengxunRT::ParseTengxunRT(vector<CWebRTDataPtr>&, CWebDataPtr pWebData) {
	return false;
}
