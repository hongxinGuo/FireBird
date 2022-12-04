#include "pch.h"
#include "ProductTiingoCompanyProfile.h"

IMPLEMENT_DYNCREATE(CProductTiingoCompanyProfile, CVirtualProductWebData)

CProductTiingoCompanyProfile::CProductTiingoCompanyProfile() {
	m_strClassName = _T("Tiingo company profile");
	ASSERT(0); // ипн╢й╣ож
}

CString CProductTiingoCompanyProfile::CreatMessage(void) {
	m_strTotalInquiryMessage = _T("");
	return m_strTotalInquiryMessage;
}

bool CProductTiingoCompanyProfile::ParseAndStoreWebData(CWebDataPtr pData, CVirtualDataSource* pDataSource) {
	return false;
}