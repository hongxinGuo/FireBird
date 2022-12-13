#include"pch.h"

#include"ProductNeteaseRT.h"
#include"ChinaMarket.h"

#include"JsonParse.h"

IMPLEMENT_DYNCREATE(CProductNeteaseRT, CVirtualWebProduct)

CProductNeteaseRT::CProductNeteaseRT() {
	m_lCurrentStockPosition = 0;
	m_strInquiry = _T("http://api.money.126.net/data/feed/");
}

CString CProductNeteaseRT::CreateMessage(void) {
	static long slCurrentStockPosition = 0;

	return m_strInquiry; // 新浪实时数据的申请字符串由CNeteaseRTWebInquiry类完成，本Product无需动作。
}

bool CProductNeteaseRT::ParseAndStoreWebData(CWebDataPtr pWebData) {
	const auto pvWebRTData = ParseNeteaseRTDataWithNlohmannJSon(pWebData);

	for (const auto& pRTData : *pvWebRTData) {
		gl_pChinaMarket->PushNeteaseRT(pRTData); // 将此实时数据指针存入实时数据队列
	}
	return true;
}

bool CProductNeteaseRT::ParseNeteaseRT(vector<CWebRTDataPtr>&, CWebDataPtr pWebData) {
	return false;
}
