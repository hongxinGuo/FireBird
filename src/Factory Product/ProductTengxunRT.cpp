#include"pch.h"

#include"ProductTengxunRT.h"
#include"ChinaMarket.h"

#include"JsonParse.h"

CProductTengxunRT::CProductTengxunRT() {
	m_lCurrentStockPosition = 0;
	m_strInquiry = _T("http://qt.gtimg.cn/q=");
}

CString CProductTengxunRT::CreateMessage(void) {
	return m_strInquiry; // 新浪实时数据的申请字符串由CTengxunRTWebInquiry类完成，本Product无需动作。
}

bool CProductTengxunRT::ParseAndStoreWebData(CWebDataPtr pWebData) {
	shared_ptr<vector<CWebRTDataPtr>> pvWebRTData = ParseTengxunRTData(pWebData);
	for (const auto& pRTData : *pvWebRTData) {
		gl_pChinaMarket->PushTengxunRT(pRTData);// 将此实时数据指针存入实时数据队列
	}

	return true;
}

bool CProductTengxunRT::ParseTengxunRT(vector<CWebRTDataPtr>&, CWebDataPtr pWebData) {
	return false;
}
