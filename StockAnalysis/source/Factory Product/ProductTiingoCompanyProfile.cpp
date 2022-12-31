#include "pch.h"
#include "ProductTiingoCompanyProfile.h"

CProductTiingoCompanyProfile::CProductTiingoCompanyProfile() {
	m_strClassName = _T("Tiingo company profile");
	ASSERT(0); // ��δʵ��
}

CString CProductTiingoCompanyProfile::CreateMessage(void) {
	m_strTotalInquiryMessage = _T("");
	return m_strTotalInquiryMessage;
}

bool CProductTiingoCompanyProfile::ParseAndStoreWebData(CWebDataPtr pWebData) {
	return true;
}
