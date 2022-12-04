#pragma once

#include"ProductFinnhub.h"

class CProductFinnhubCompanyProfile : public CProductFinnhub {
public:
	DECLARE_DYNCREATE(CProductFinnhubCompanyProfile)
		CProductFinnhubCompanyProfile();
	~CProductFinnhubCompanyProfile() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ParseAndStoreWebData(CWebDataPtr pWebData, CVirtualDataSource* pDataSource = nullptr) override final;
	bool ParseFinnhubStockProfile(CWebDataPtr pWebData, CWorldStockPtr pStock);
};

typedef shared_ptr<CProductFinnhubCompanyProfile> CFinnhubCompanyProfilePtr;