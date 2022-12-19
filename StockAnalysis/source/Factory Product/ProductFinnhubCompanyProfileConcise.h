#pragma once

#include"ProductFinnhub.h"

class CProductFinnhubCompanyProfileConcise final : public CProductFinnhub {
public:
	DECLARE_DYNCREATE(CProductFinnhubCompanyProfileConcise)
	CProductFinnhubCompanyProfileConcise();
	~CProductFinnhubCompanyProfileConcise() override = default;

	CString CreateMessage(void) final;
	bool ParseAndStoreWebData(CWebDataPtr pWebData) final;
	bool ParseFinnhubStockProfileConcise(CWebDataPtr pWebData, CWorldStockPtr pStock);
	bool ParseFinnhubStockProfileConcise2(CWebDataPtr pWebData, CWorldStockPtr pStock);
};

using CFinnhubCompanyProfileConcisePtr = shared_ptr<CProductFinnhubCompanyProfileConcise>;
