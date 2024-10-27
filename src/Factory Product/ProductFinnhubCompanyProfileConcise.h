#pragma once

#include"ProductFinnhub.h"

class CProductFinnhubCompanyProfileConcise final : public CProductFinnhub {
public:
	CProductFinnhubCompanyProfileConcise();
	~CProductFinnhubCompanyProfileConcise() override = default;

	CString CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	bool ParseFinnhubStockProfileConcise(const CWebDataPtr& pWebData, const CFinnhubStockPtr& pStock) const;
};

using CFinnhubCompanyProfileConcisePtr = shared_ptr<CProductFinnhubCompanyProfileConcise>;
