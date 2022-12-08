#pragma once

#include"ProductFinnhub.h"

class CProductFinnhubCompanyProfile : public CProductFinnhub {
public:
	DECLARE_DYNCREATE(CProductFinnhubCompanyProfile)
		CProductFinnhubCompanyProfile();
	~CProductFinnhubCompanyProfile() {}

	virtual CString CreateMessage(void) override final;
	virtual bool ParseAndStoreWebData(CWebDataPtr pWebData) override final;
	bool ParseFinnhubStockProfile(CWebDataPtr pWebData, CWorldStockPtr pStock);
};

typedef shared_ptr<CProductFinnhubCompanyProfile> CFinnhubCompanyProfilePtr;