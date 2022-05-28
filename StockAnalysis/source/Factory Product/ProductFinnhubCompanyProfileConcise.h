#pragma once

#include"ProductWebData.h"

class CProductFinnhubCompanyProfileConcise : public CProductWebSourceData {
public:
	DECLARE_DYNCREATE(CProductFinnhubCompanyProfileConcise)
	CProductFinnhubCompanyProfileConcise();
	~CProductFinnhubCompanyProfileConcise() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ParseAndStoreWebData(CWebDataPtr pWebData) override final;
	bool ParseFinnhubStockProfileConcise(CWebDataPtr pWebData, CWorldStockPtr pStock);
};

typedef shared_ptr<CProductFinnhubCompanyProfileConcise> CFinnhubCompanyProfileConcisePtr;