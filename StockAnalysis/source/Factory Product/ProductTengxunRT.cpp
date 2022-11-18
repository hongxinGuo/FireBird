#include"pch.h"

#include"ProductTengxunRT.h"

#include"WebInquirer.h"

IMPLEMENT_DYNCREATE(CProductTengxunRT, CVirtualProductWebData)

CProductTengxunRT::CProductTengxunRT() {
	m_lCurrentStockPosition = 0;
	m_strInquiry = _T("http://qt.gtimg.cn/q=");
}

CString CProductTengxunRT::CreatMessage(void) {
	static long slCurrentStockPosition = 0;

	return m_strInquiry; // 新浪实时数据的申请字符串由CTengxunRTWebInquiry类完成，本Product无需动作。
}

bool CProductTengxunRT::ParseAndStoreWebData(CWebDataPtr pWebData) {
	gl_WebInquirer.PushTengxunRTData(pWebData);
	return true;
}

bool CProductTengxunRT::ParseTengxunRT(vector<CWebRTDataPtr>&, CWebDataPtr pWebData) {
	return false;
}