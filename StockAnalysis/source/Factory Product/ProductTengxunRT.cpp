#include"pch.h"

#include"ProductTengxunRT.h"
#include"TengxunRTDataSource.h"
#include"WebRTDataContainer.h"

#include"JsonParse.h"

IMPLEMENT_DYNCREATE(CProductTengxunRT, CVirtualProductWebData)

CProductTengxunRT::CProductTengxunRT() {
	m_lCurrentStockPosition = 0;
	m_strInquiry = _T("http://qt.gtimg.cn/q=");
}

CString CProductTengxunRT::CreatMessage(void) {
	static long slCurrentStockPosition = 0;

	return m_strInquiry; // ����ʵʱ���ݵ������ַ�����CTengxunRTWebInquiry����ɣ���Product���趯����
}

bool CProductTengxunRT::ParseAndStoreWebData(CWebDataPtr pWebData, CVirtualDataSource* pDataSource) {
	vector<CWebRTDataPtr> vWebRTData;

	ASSERT(pDataSource != nullptr);
	ASSERT(pDataSource->IsKindOf(RUNTIME_CLASS(CTengxunRTDataSource)));
	ParseTengxunRTData(pWebData, vWebRTData);
	for (auto& pRTData : vWebRTData) {
		gl_WebRTDataContainer.PushTengxunData(pRTData); // ����ʵʱ����ָ�����ʵʱ���ݶ���
	}

	return true;
}

bool CProductTengxunRT::ParseTengxunRT(vector<CWebRTDataPtr>&, CWebDataPtr pWebData) {
	return false;
}