#include"pch.h"

#include"ProductSinaRT.h"
#include"SinaRTDataSource.h"
#include"WebRTDataContainer.h"

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

bool CProductSinaRT::ParseAndStoreWebData(CWebDataPtr pWebData, CVirtualDataSource* pDataSource) {
	vector<CWebRTDataPtr> vWebRTData;

	ASSERT(pDataSource != nullptr);
	ASSERT(pDataSource->IsKindOf(RUNTIME_CLASS(CSinaRTDataSource)));
	ParseSinaRTData(pWebData, vWebRTData);
	for (auto& pRTData : vWebRTData) {
		gl_WebRTDataContainer.PushSinaData(pRTData); // ����ʵʱ����ָ�����ʵʱ���ݶ���
	}

	return true;
}

bool CProductSinaRT::ParseSinaRT(vector<CWebRTDataPtr>&, CWebDataPtr pWebData) {
	return false;
}