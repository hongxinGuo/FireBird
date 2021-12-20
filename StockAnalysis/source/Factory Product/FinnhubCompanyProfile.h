#pragma once

#include"WebSourceDataProduct.h"

class CFinnhubCompanyProfile : public CWebSourceDataProduct {
public:
	DECLARE_DYNCREATE(CFinnhubCompanyProfile)
	CFinnhubCompanyProfile();
	~CFinnhubCompanyProfile() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ProcessWebData(CWebDataPtr pWebData) override final;
	bool ParseFinnhubStockProfile(CWebDataPtr pWebData, CWorldStockPtr pStock);
};

typedef shared_ptr<CFinnhubCompanyProfile> CFinnhubCompanyProfilePtr;