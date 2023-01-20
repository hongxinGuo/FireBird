#include"pch.h"

#include"ProductSinaRT.h"
#include"ChinaMarket.h"

#include"JsonParse.h"

CProductSinaRT::CProductSinaRT() {
	m_lCurrentStockPosition = 0;
	m_strInquiry = _T("https://hq.sinajs.cn/list=");
}

CString CProductSinaRT::CreateMessage(void) {
	return m_strInquiry; // 新浪实时数据的申请字符串由CSinaRTWebInquiry类完成，本Product无需动作。
}

bool CProductSinaRT::ParseAndStoreWebData(CWebDataPtr pWebData) {
	const shared_ptr<vector<CWebRTDataPtr>> pvWebRTData = ParseSinaRTData(pWebData);
	for (const auto& pRTData : *pvWebRTData) {
		gl_pChinaMarket->PushSinaRT(pRTData);// 将此实时数据指针存入实时数据队列
	}

	return true;
}

bool CProductSinaRT::ParseSinaRT(vector<CWebRTDataPtr>&, CWebDataPtr pWebData) {
	return false;
}
