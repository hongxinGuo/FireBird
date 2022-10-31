#include"pch.h"

#include"VirtualProductWebData.h"

IMPLEMENT_DYNCREATE(CVirtualProductWebData, CObject)

CVirtualProductWebData::CVirtualProductWebData() : CObject() {
	m_pMarket = nullptr;
	m_lIndex = -1; // ����ΪԽ��

	m_strClassName = _T("VirtualProductData");
	m_iReceivedDataStatus = 0;

	m_strInquiringStr = _T("");
	m_strTotalInquiryMessage = _T("");
	m_strInquiringExchange = _T("ALL"); // Ĭ�����������еĽ��������ݡ�
}

bool CVirtualProductWebData::IsUSMarket(void) {
	if (m_strInquiringExchange.Compare(_T("US")) == 0) return true;
	else return false;
}