#pragma once

#include"ProductFinnhub.h"

class CProductFinnhubCompanyProfileConcise final : public CProductFinnhub {
public:
	CProductFinnhubCompanyProfileConcise();
	~CProductFinnhubCompanyProfileConcise() override = default;

	CString CreateMessage() override;
	bool ParseAndStoreWebData(CWebDataPtr pWebData) override;
	bool ParseFinnhubStockProfileConcise(const CWebDataPtr& pWebData, const CWorldStockPtr& pStock);
};

using CFinnhubCompanyProfileConcisePtr = shared_ptr<CProductFinnhubCompanyProfileConcise>;
