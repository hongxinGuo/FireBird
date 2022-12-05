#include"pch.h"

#include"ProductTengxunRT.h"
#include"TengxunRTDataSource.h"

#include"JsonParse.h"

IMPLEMENT_DYNCREATE(CProductTengxunRT, CVirtualProductWebData)

CProductTengxunRT::CProductTengxunRT() {
	m_lCurrentStockPosition = 0;
	m_strInquiry = _T("http://qt.gtimg.cn/q=");
}

CString CProductTengxunRT::CreatMessage(void) {
	static long slCurrentStockPosition = 0;

	return m_strInquiry; // 新浪实时数据的申请字符串由CTengxunRTWebInquiry类完成，本Product无需动作。
}

bool CProductTengxunRT::ParseAndStoreWebData(CWebDataPtr pWebData, CVirtualDataSource* pDataSource) {
	shared_ptr<vector<CWebRTDataPtr>> pvWebRTData = nullptr;

	ASSERT(pDataSource != nullptr);
	ASSERT(pDataSource->IsKindOf(RUNTIME_CLASS(CTengxunRTDataSource)));
	pvWebRTData = ParseTengxunRTData(pWebData);
	for (auto& pRTData : *pvWebRTData) {
		static_cast<CTengxunRTDataSource*>(pDataSource)->PushData(pRTData);// 将此实时数据指针存入实时数据队列
	}

	return true;
}

bool CProductTengxunRT::ParseTengxunRT(vector<CWebRTDataPtr>&, CWebDataPtr pWebData) {
	return false;
}