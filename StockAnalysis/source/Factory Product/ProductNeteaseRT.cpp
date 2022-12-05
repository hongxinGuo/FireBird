#include"pch.h"

#include"ProductNeteaseRT.h"
#include"ChinaMarket.h"

#include"JsonParse.h"
#include"ChinaMarket.h"

IMPLEMENT_DYNCREATE(CProductNeteaseRT, CVirtualProductWebData)

CProductNeteaseRT::CProductNeteaseRT() {
	m_lCurrentStockPosition = 0;
	m_strInquiry = _T("http://api.money.126.net/data/feed/");
}

CString CProductNeteaseRT::CreatMessage(void) {
	static long slCurrentStockPosition = 0;

	return m_strInquiry; // 新浪实时数据的申请字符串由CNeteaseRTWebInquiry类完成，本Product无需动作。
}

bool CProductNeteaseRT::ParseAndStoreWebData(CWebDataPtr pWebData) {
	shared_ptr<vector<CWebRTDataPtr>> pvWebRTData;

	pvWebRTData = ParseNeteaseRTDataWithNlohmannJSon(pWebData);
	for (auto& pRTData : *pvWebRTData) {
		gl_pChinaMarket->PushNeteaseRT(pRTData);// 将此实时数据指针存入实时数据队列
	}
	return true;
}

bool CProductNeteaseRT::ParseNeteaseRT(vector<CWebRTDataPtr>&, CWebDataPtr pWebData) {
	return false;
}