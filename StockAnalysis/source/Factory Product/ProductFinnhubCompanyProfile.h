#pragma once

#include"ProductFinnhub.h"

class CProductFinnhubCompanyProfile final : public CProductFinnhub {
public:
	DECLARE_DYNCREATE(CProductFinnhubCompanyProfile)
	CProductFinnhubCompanyProfile();
	~CProductFinnhubCompanyProfile() override = default;

	virtual CString CreateMessage(void) override final;
	virtual bool ParseAndStoreWebData(CWebDataPtr pWebData) override final;
	bool ParseFinnhubStockProfile(CWebDataPtr pWebData, CWorldStockPtr pStock);
};

typedef shared_ptr<CProductFinnhubCompanyProfile> CFinnhubCompanyProfilePtr;
