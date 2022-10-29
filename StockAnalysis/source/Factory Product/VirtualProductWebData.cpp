#include"pch.h"

#include"VirtualProductWebData.h"

IMPLEMENT_DYNCREATE(CVirtualProductWebData, CObject)

CVirtualProductWebData::CVirtualProductWebData() : CObject() {
	m_pMarket = nullptr;
	m_lIndex = -1; // 设置为越界

	m_strClassName = _T("VirtualProductData");
	m_iReceivedDataStatus = 0;
	m_iNumberOfNoRightToAccess = 0;

	m_strInquiringStr = _T("");
	m_strTotalInquiryMessage = _T("");
	m_strInquiringExchange = _T("");
}