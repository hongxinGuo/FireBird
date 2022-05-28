/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Tiingo此项功能为非免费项目，目前尚未使用。
//
////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"ProductWebData.h"

class CProductTiingoCompanyProfile : public CProductWebSourceData {
public:
	DECLARE_DYNCREATE(CProductTiingoCompanyProfile)
	CProductTiingoCompanyProfile();
	~CProductTiingoCompanyProfile() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ParseAndStoreWebData(CWebDataPtr pData) override final;
};

typedef shared_ptr<CProductTiingoCompanyProfile> CTiingoCompanyProfilePtr;
