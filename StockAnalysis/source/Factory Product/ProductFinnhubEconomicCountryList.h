#pragma once

#include"ProductWebData.h"
#include"Country.h"

class CProductFinnhubEconomicCountryList : public CProductWebSourceData {
public:
	DECLARE_DYNCREATE(CProductFinnhubEconomicCountryList)
	CProductFinnhubEconomicCountryList();
	~CProductFinnhubEconomicCountryList() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ParseAndStoreWebData(CWebDataPtr pWebData) override final;
	CCountryVectorPtr ParseFinnhubCountryList(CWebDataPtr pWebData);
};

typedef shared_ptr<CProductFinnhubEconomicCountryList> CFinnhubEconomicCountryListPtr;
