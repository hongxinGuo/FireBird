#include"pch.h"
module;
module FireBird.WebProduct.NeteaseRT;

import FireBird.System.Data;
import FireBird.Accessory.JsonParse;

CProductNeteaseRT::CProductNeteaseRT() {
	m_lCurrentStockPosition = 0;
	m_strInquiryFunction = _T("http://api.money.126.net/data/feed/");
}

CString CProductNeteaseRT::CreateMessage() {
	return m_strInquiryFunction; // ����ʵʱ���ݵ������ַ�����CNeteaseRTWebInquiry����ɣ���Product���趯����
}

void CProductNeteaseRT::ParseAndStoreWebData(CWebDataPtr pWebData) {
	const shared_ptr<vector<CWebRTDataPtr>> pvWebRTData = ParseNeteaseRT(pWebData);

	for (const auto& pRTData : *pvWebRTData) {
		gl_qChinaMarketRTData.enqueue(pRTData); // ����ʵʱ����ָ�����ʵʱ���ݶ���
	}
}

shared_ptr<vector<CWebRTDataPtr>> CProductNeteaseRT::ParseNeteaseRT(const CWebDataPtr& pWebData) {
	return ParseNeteaseRTDataWithSimdjson(pWebData); // ʹ��simdjson����
}
