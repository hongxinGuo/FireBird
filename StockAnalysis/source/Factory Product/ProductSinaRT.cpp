#include"pch.h"

#include"ProductSinaRT.h"

#include"WebInquirer.h"

IMPLEMENT_DYNCREATE(CProductSinaRT, CVirtualProductWebData)

CProductSinaRT::CProductSinaRT() {
	m_lCurrentStockPosition = 0;
	m_strInquiry = _T("https://hq.sinajs.cn/list=");
}

CString CProductSinaRT::CreatMessage(void) {
	static long slCurrentStockPosition = 0;

	return m_strInquiry; // 新浪实时数据的申请字符串由CSinaRTWebInquiry类完成，本Product无需动作。
}

bool CProductSinaRT::ParseAndStoreWebData(CWebDataPtr pWebData) {
	gl_WebInquirer.PushSinaRTData(pWebData); // 目前只是将数据存储于暂存队列中。今后将数据的解析处理过程移至此处。
	return true;
}

bool CProductSinaRT::ParseSinaRT(vector<CWebRTDataPtr>&, CWebDataPtr pWebData) {
	return false;
}