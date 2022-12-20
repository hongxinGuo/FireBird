#pragma once

#include"ProductFinnhub.h"
#include"Country.h"

class CProductFinnhubEconomicCountryList final : public CProductFinnhub {
public:
	DECLARE_DYNCREATE(CProductFinnhubEconomicCountryList)
	CProductFinnhubEconomicCountryList();
	~CProductFinnhubEconomicCountryList() override = default;

	CString CreateMessage(void) final;
	bool ParseAndStoreWebData(CWebDataPtr pWebData) final;
	CCountryVectorPtr ParseFinnhubCountryList(CWebDataPtr pWebData);
};

using CFinnhubEconomicCountryListPtr = shared_ptr<CProductFinnhubEconomicCountryList>;
