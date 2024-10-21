#include "pch.h"
#include "ProductTiingoCompanyProfile.h"

CProductTiingoCompanyProfile::CProductTiingoCompanyProfile() {
	ASSERT(0); // not implemented
}

CString CProductTiingoCompanyProfile::CreateMessage() {
	m_strInquiringSymbol = _T("All");
	m_strInquiry = _T("");
	return m_strInquiry;
}

void CProductTiingoCompanyProfile::ParseAndStoreWebData(CWebDataPtr pWebData) {}
