/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//Tiingo此项为收费业务
//
////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"VirtualWebProduct.h"

class CProductTiingoCompanyProfile final : public CVirtualWebProduct {
public:
	CProductTiingoCompanyProfile();
	~CProductTiingoCompanyProfile() override = default;

	CString CreateMessage() override;
	bool ParseAndStoreWebData(CWebDataPtr pWebData) override;
};

using CTiingoCompanyProfilePtr = shared_ptr<CProductTiingoCompanyProfile>;
