#include"pch.h"

#include"ProductNeteaseRT.h"

#include"JsonParse.h"

CProductNeteaseRT::CProductNeteaseRT() {
	m_lCurrentStockPosition = 0;
	m_strInquiryFunction = _T("http://api.money.126.net/data/feed/");
}

CString CProductNeteaseRT::CreateMessage() {
	return m_strInquiryFunction; // 新浪实时数据的申请字符串由CNeteaseRTWebInquiry类完成，本Product无需动作。
}

void CProductNeteaseRT::ParseAndStoreWebData(CWebDataPtr pWebData) {
	const shared_ptr<vector<CWebRTDataPtr>> pvWebRTData = ParseNeteaseRT(pWebData);

	for (const auto& pRTData : *pvWebRTData) {
		gl_qNeteaseRT.PushData(pRTData); // 将此实时数据指针存入实时数据队列
	}
}

shared_ptr<vector<CWebRTDataPtr>> CProductNeteaseRT::ParseNeteaseRT(const CWebDataPtr& pWebData) {
	//return ParseNeteaseRTDataWithNlohmannJSon(pWebData);
	return ParseNeteaseRTDataWithSimdjson(pWebData); // 使用simdjson解析
}
