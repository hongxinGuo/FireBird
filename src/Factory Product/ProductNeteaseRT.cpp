#include"pch.h"

#include"ProductNeteaseRT.h"

#include"JsonParse.h"

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
		gl_qNeteaseRT.PushData(pRTData); // ����ʵʱ����ָ�����ʵʱ���ݶ���
	}
}

shared_ptr<vector<CWebRTDataPtr>> CProductNeteaseRT::ParseNeteaseRT(const CWebDataPtr& pWebData) {
	//return ParseNeteaseRTDataWithNlohmannJSon(pWebData);
	return ParseNeteaseRTDataWithSimdjson(pWebData); // ʹ��simdjson����
}
