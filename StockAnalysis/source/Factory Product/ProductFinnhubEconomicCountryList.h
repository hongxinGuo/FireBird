#pragma once

#include"ProductFinnhub.h"
#include"Country.h"

class CProductFinnhubEconomicCountryList final : public CProductFinnhub {
public:
	DECLARE_DYNCREATE(CProductFinnhubEconomicCountryList)
	CProductFinnhubEconomicCountryList();
	~CProductFinnhubEconomicCountryList() override = default;

	virtual CString CreateMessage(void) override final;
	virtual bool ParseAndStoreWebData(CWebDataPtr pWebData) override final;
	CCountryVectorPtr ParseFinnhubCountryList(CWebDataPtr pWebData);
};

typedef shared_ptr<CProductFinnhubEconomicCountryList> CFinnhubEconomicCountryListPtr;
