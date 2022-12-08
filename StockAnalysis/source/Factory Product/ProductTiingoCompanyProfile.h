/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Tiingo此项功能为非免费项目，目前尚未使用。
//
////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"VirtualProductWebData.h"

class CProductTiingoCompanyProfile : public CVirtualProductWebData {
public:
	DECLARE_DYNCREATE(CProductTiingoCompanyProfile)
		CProductTiingoCompanyProfile();
	~CProductTiingoCompanyProfile() {}

	virtual CString CreateMessage(void) override final;
	virtual bool ParseAndStoreWebData(CWebDataPtr pData) override final;
};

typedef shared_ptr<CProductTiingoCompanyProfile> CTiingoCompanyProfilePtr;
