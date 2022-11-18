#include"pch.h"

#include"ProductNeteaseRT.h"

#include"WebInquirer.h"

IMPLEMENT_DYNCREATE(CProductNeteaseRT, CVirtualProductWebData)

CProductNeteaseRT::CProductNeteaseRT() {
	m_lCurrentStockPosition = 0;
	m_strInquiry = _T("http://api.money.126.net/data/feed/");
}

CString CProductNeteaseRT::CreatMessage(void) {
	static long slCurrentStockPosition = 0;

	return m_strInquiry; // 新浪实时数据的申请字符串由CNeteaseRTWebInquiry类完成，本Product无需动作。
}

bool CProductNeteaseRT::ParseAndStoreWebData(CWebDataPtr pWebData) {
	gl_WebInquirer.PushNeteaseRTData(pWebData);
	return true;
}

bool CProductNeteaseRT::ParseNeteaseRT(vector<CWebRTDataPtr>&, CWebDataPtr pWebData) {
	return false;
}