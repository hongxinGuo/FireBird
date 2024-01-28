/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//Tiingo此项为收费业务
//
////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"ProductTiingo.h"

class CProductTiingoCompanyProfile final : public CProductTiingo {
public:
	CProductTiingoCompanyProfile();
	~CProductTiingoCompanyProfile() override = default;

	CString CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
};

using CTiingoCompanyProfilePtr = shared_ptr<CProductTiingoCompanyProfile>;
