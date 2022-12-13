/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Tiingo此项功能为非免费项目，目前尚未使用。
//
////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"VirtualWebProduct.h"

class CProductTiingoCompanyProfile : public CVirtualWebProduct {
public:
	DECLARE_DYNCREATE(CProductTiingoCompanyProfile)
		CProductTiingoCompanyProfile();
	~CProductTiingoCompanyProfile() {}

	virtual CString CreateMessage(void) override final;
	virtual bool ParseAndStoreWebData(CWebDataPtr pData) override final;
};

typedef shared_ptr<CProductTiingoCompanyProfile> CTiingoCompanyProfilePtr;
