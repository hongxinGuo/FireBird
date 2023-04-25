#pragma once

#include"ProductFinnhub.h"

class CProductFinnhubCompanyProfile final : public CProductFinnhub {
public:
	CProductFinnhubCompanyProfile();
	~CProductFinnhubCompanyProfile() override = default;

	CString CreateMessage() override;
	bool ParseAndStoreWebData(CWebDataPtr pWebData) override;
	bool ParseFinnhubStockProfile(CWebDataPtr pWebData, CWorldStockPtr pStock);
};

using CFinnhubCompanyProfilePtr = shared_ptr<CProductFinnhubCompanyProfile>;
