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
// 使用工作线程模式改写后，实际执行时间却变长了。估计时线程切换时间太长的缘故。不采用工作线程模式。
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CProductTengxunRT::ParseAndStoreWebData(CWebDataPtr pWebData) {
	ParseTengxunRTData(pWebData);
	//ParseTengxunRTDataUsingWorkingThread(pWebData);
}

bool CProductTengxunRT::ParseTengxunRT(vector<CWebRTDataPtr>&, CWebDataPtr pWebData) {
	return false;
}
