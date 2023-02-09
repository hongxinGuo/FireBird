#include"pch.h"

#include"ProductNeteaseRT.h"
#include"ChinaMarket.h"

#include"JsonParse.h"

CProductNeteaseRT::CProductNeteaseRT() {
	m_lCurrentStockPosition = 0;
	m_strInquiryFunction = _T("http://api.money.126.net/data/feed/");
}

CString CProductNeteaseRT::CreateMessage(void) {
	return m_strInquiryFunction; // 新浪实时数据的申请字符串由CNeteaseRTWebInquiry类完成，本Product无需动作。
}

bool CProductNeteaseRT::ParseAndStoreWebData(CWebDataPtr pWebData) {
	const shared_ptr<vector<CWebRTDataPtr>> pvWebRTData = ParseNeteaseRT(pWebData);

	for (const auto& pRTData : *pvWebRTData) {
		gl_pChinaMarket->PushNeteaseRT(pRTData); // 将此实时数据指针存入实时数据队列
	}
	return true;
}

shared_ptr<vector<CWebRTDataPtr>> CProductNeteaseRT::ParseNeteaseRT(CWebDataPtr pWebData) {
	return ParseNeteaseRTDataWithNlohmannJSon(pWebData);
}
