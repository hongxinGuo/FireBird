#pragma once

#include"ProductFinnhub.h"

class CFinnhubStock;

class CProductFinnhubCompanyProfile final : public CProductFinnhub {
public:
	CProductFinnhubCompanyProfile();
	~CProductFinnhubCompanyProfile() override = default;

	string CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	bool ParseFinnhubStockProfile(CWebDataPtr pWebData, shared_ptr<CFinnhubStock> pStock) const;
};

using CFinnhubCompanyProfilePtr = shared_ptr<CProductFinnhubCompanyProfile>;
