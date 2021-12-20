#pragma once

#include"WebSourceDataProduct.h"

class CFinnhubCompanyProfileConcise : public CWebSourceDataProduct {
public:
	DECLARE_DYNCREATE(CFinnhubCompanyProfileConcise)
	CFinnhubCompanyProfileConcise();
	~CFinnhubCompanyProfileConcise() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ProcessWebData(CWebDataPtr pWebData) override final;
	bool ParseFinnhubStockProfileConcise(CWebDataPtr pWebData, CWorldStockPtr pStock);
};

typedef shared_ptr<CFinnhubCompanyProfileConcise> CFinnhubCompanyProfileConcisePtr;