#include"pch.h"

#include"ProductNeteaseRT.h"
#include"ChinaMarket.h"

#include"JsonParse.h"

CProductNeteaseRT::CProductNeteaseRT() {
	m_lCurrentStockPosition = 0;
	m_strInquiryFunction = _T("http://api.money.126.net/data/feed/");
}

CString CProductNeteaseRT::CreateMessage(void) {
	return m_strInquiryFunction; // ����ʵʱ���ݵ������ַ�����CNeteaseRTWebInquiry����ɣ���Product���趯����
}

bool CProductNeteaseRT::ParseAndStoreWebData(CWebDataPtr pWebData) {
	const shared_ptr<vector<CWebRTDataPtr>> pvWebRTData = ParseNeteaseRT(pWebData);

	for (const auto& pRTData : *pvWebRTData) {
		gl_pChinaMarket->PushNeteaseRT(pRTData); // ����ʵʱ����ָ�����ʵʱ���ݶ���
	}
	return true;
}

shared_ptr<vector<CWebRTDataPtr>> CProductNeteaseRT::ParseNeteaseRT(CWebDataPtr pWebData) {
	return ParseNeteaseRTDataWithNlohmannJSon(pWebData);
}
