#include "pch.h"
#include "ProductTiingoCompanyProfile.h"

CProductTiingoCompanyProfile::CProductTiingoCompanyProfile() {
	m_strClassName = _T("Tiingo company profile");
	ASSERT(0); // ��δʵ��
}

CString CProductTiingoCompanyProfile::CreateMessage() {
	m_strInquiry = _T("");
	return m_strInquiry;
}

bool CProductTiingoCompanyProfile::ParseAndStoreWebData(CWebDataPtr pWebData) {
	return true;
}
