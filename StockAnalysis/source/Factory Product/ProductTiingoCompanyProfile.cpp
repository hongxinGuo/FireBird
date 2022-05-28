#include "pch.h"
#include "ProductTiingoCompanyProfile.h"

IMPLEMENT_DYNCREATE(CProductTiingoCompanyProfile, CProductWebSourceData)

CProductTiingoCompanyProfile::CProductTiingoCompanyProfile() {
	m_strClassName = _T("Tiingo company profile");
	ASSERT(0); // ипн╢й╣ож
}

CString CProductTiingoCompanyProfile::CreatMessage(void) {
	return _T("");
}

bool CProductTiingoCompanyProfile::ParseAndStoreWebData(CWebDataPtr pData) {
	return false;
}