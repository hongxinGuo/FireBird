#pragma once

#include"ProductWebData.h"

class CProductFinnhubCompanyProfile : public CProductWebSourceData {
public:
	DECLARE_DYNCREATE(CProductFinnhubCompanyProfile)
	CProductFinnhubCompanyProfile();
	~CProductFinnhubCompanyProfile() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ProcessWebData(CWebDataPtr pWebData) override final;
	bool ParseFinnhubStockProfile(CWebDataPtr pWebData, CWorldStockPtr pStock);
};

typedef shared_ptr<CProductFinnhubCompanyProfile> CFinnhubCompanyProfilePtr;