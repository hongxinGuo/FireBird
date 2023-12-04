#include"pch.h"

#include"ProductTengxunRT.h"
#include"ChinaMarket.h"

#include"JsonParse.h"
#include "TengxunRTDataSource.h"
#include "TimeConvert.h"

CProductTengxunRT::CProductTengxunRT() {
	m_lCurrentStockPosition = 0;
	m_strInquiryFunction = _T("http://qt.gtimg.cn/q=");
}

CString CProductTengxunRT::CreateMessage() {
	// ������һ���ι�Ʊʵʱ���ݡ�
	// ������Ѷʵʱ����ʱ��������������ڵĹ�Ʊ���룬�������᷵��v_pv_none_match="1";������ϵͳ���ϣ�
	// �ʶ�����ֻʹ����Ч��Ʊ���롣
	const CString strStocks = GetMarket()->GetNextTengxunStockInquiringMiddleStr(gl_pTengxunRTDataSource->GetInquiringNumber()); // ʹ�û�Ծ��Ʊ��
	return m_strInquiryFunction + strStocks;
}

bool CProductTengxunRT::ParseAndStoreWebData(CWebDataPtr pWebData) {
	const shared_ptr<vector<CWebRTDataPtr>> pvWebRTData = ParseTengxunRTData(pWebData);

	gl_ProcessChinaMarketRTData.acquire();
	for (const auto& pRTData : *pvWebRTData) {
		GetMarket()->PushTengxunRT(pRTData);// ����ʵʱ����ָ�����ʵʱ���ݶ���
	}
	gl_ProcessChinaMarketRTData.release();
	return true;
}

bool CProductTengxunRT::ParseTengxunRT(vector<CWebRTDataPtr>&, CWebDataPtr pWebData) {
	return false;
}
