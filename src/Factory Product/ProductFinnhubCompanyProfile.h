#pragma once

#include"ProductFinnhub.h"

class CProductFinnhubCompanyProfile final : public CProductFinnhub {
public:
	CProductFinnhubCompanyProfile();
	~CProductFinnhubCompanyProfile() override = default;

	CString CreateMessage(void) final;
	bool ParseAndStoreWebData(CWebDataPtr pWebData) final;
	bool ParseFinnhubStockProfile(CWebDataPtr pWebData, CWorldStockPtr pStock);
};

using CFinnhubCompanyProfilePtr = shared_ptr<CProductFinnhubCompanyProfile>;