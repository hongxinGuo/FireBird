#pragma once

#include"ProductFinnhub.h"

class CFinnhubStock;

class CProductFinnhubCompanyProfileConcise final : public CProductFinnhub {
public:
	CProductFinnhubCompanyProfileConcise();
	~CProductFinnhubCompanyProfileConcise() override = default;

	string CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	bool ParseFinnhubStockProfileConcise(const CWebDataPtr& pWebData, const shared_ptr<CFinnhubStock>& pStock) const;
};

using CFinnhubCompanyProfileConcisePtr = shared_ptr<CProductFinnhubCompanyProfileConcise>;
