#include"pch.h"

#include"ProductTengxunRT.h"
#include"ChinaMarket.h"

#include"JsonParse.h"

IMPLEMENT_DYNCREATE(CProductTengxunRT, CVirtualWebProduct)

CProductTengxunRT::CProductTengxunRT() {
	m_lCurrentStockPosition = 0;
	m_strInquiry = _T("http://qt.gtimg.cn/q=");
}

CString CProductTengxunRT::CreateMessage(void) {
	static long slCurrentStockPosition = 0;

	return m_strInquiry; // ����ʵʱ���ݵ������ַ�����CTengxunRTWebInquiry����ɣ���Product���趯����
}

bool CProductTengxunRT::ParseAndStoreWebData(CWebDataPtr pWebData) {
	shared_ptr<vector<CWebRTDataPtr>> pvWebRTData = nullptr;

	pvWebRTData = ParseTengxunRTData(pWebData);
	for (auto& pRTData : *pvWebRTData) {
		gl_pChinaMarket->PushTengxunRT(pRTData);// ����ʵʱ����ָ�����ʵʱ���ݶ���
	}

	return true;
}

bool CProductTengxunRT::ParseTengxunRT(vector<CWebRTDataPtr>&, CWebDataPtr pWebData) {
	return false;
}