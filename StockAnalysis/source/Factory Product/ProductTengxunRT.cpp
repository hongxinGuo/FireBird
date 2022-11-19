#include"pch.h"

#include"ProductTengxunRT.h"

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

bool CProductTengxunRT::ParseAndStoreWebData(CWebDataPtr pWebData) {
	ParseTengxunRTData(pWebData);
	return true;
}

bool CProductTengxunRT::ParseTengxunRT(vector<CWebRTDataPtr>&, CWebDataPtr pWebData) {
	return false;
}