#include "pch.h"
#include "TiingoCompanyProfile.h"

CTiingoCompanyProfile::CTiingoCompanyProfile() {
	m_strClassName = _T("Tiingo company profile");
	ASSERT(0); // ипн╢й╣ож
}

CString CTiingoCompanyProfile::CreatMessage(void) {
	return _T("");
}

bool CTiingoCompanyProfile::ProcessWebData(CWebDataPtr pData) {
	return false;
}