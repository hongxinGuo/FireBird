#include"pch.h"

#include"ProductSinaRT.h"
#include"ChinaMarket.h"

#include"JsonParse.h"
#include"ChinaMarket.h"

IMPLEMENT_DYNCREATE(CProductSinaRT, CVirtualProductWebData)

CProductSinaRT::CProductSinaRT() {
	m_lCurrentStockPosition = 0;
	m_strInquiry = _T("https://hq.sinajs.cn/list=");
}

CString CProductSinaRT::CreatMessage(void) {
	static long slCurrentStockPosition = 0;

	return m_strInquiry; // ����ʵʱ���ݵ������ַ�����CSinaRTWebInquiry����ɣ���Product���趯����
}

bool CProductSinaRT::ParseAndStoreWebData(CWebDataPtr pWebData) {
	shared_ptr<vector<CWebRTDataPtr>> pvWebRTData = nullptr;

	pvWebRTData = ParseSinaRTData(pWebData);
	for (auto& pRTData : *pvWebRTData) {
		gl_pChinaMarket->PushSinaRT(pRTData);// ����ʵʱ����ָ�����ʵʱ���ݶ���
	}

	return true;
}

bool CProductSinaRT::ParseSinaRT(vector<CWebRTDataPtr>&, CWebDataPtr pWebData) {
	return false;
}