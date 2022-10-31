#include"pch.h"

#include"VirtualProductWebData.h"

IMPLEMENT_DYNCREATE(CVirtualProductWebData, CObject)

CVirtualProductWebData::CVirtualProductWebData() : CObject() {
	m_pMarket = nullptr;
	m_lIndex = -1; // 设置为越界

	m_strClassName = _T("VirtualProductData");
	m_iReceivedDataStatus = 0;

	m_strInquiringStr = _T("");
	m_strTotalInquiryMessage = _T("");
	m_strInquiringExchange = _T("ALL"); // 默认是申请所有的交易所数据。
}

bool CVirtualProductWebData::IsUSMarket(void) {
	if (m_strInquiringExchange.Compare(_T("US")) == 0) return true;
	else return false;
}