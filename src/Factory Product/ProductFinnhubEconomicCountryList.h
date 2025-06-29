#pragma once

#include"ProductFinnhub.h"
#include"Country.h"

class CProductFinnhubEconomicCountryList final : public CProductFinnhub {
public:
	CProductFinnhubEconomicCountryList();
	~CProductFinnhubEconomicCountryList() override = default;

	string CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	CCountriesPtr ParseFinnhubCountryList(const CWebDataPtr& pWebData);

	void UpdateDataSourceStatus(CVirtualDataSourcePtr pDataSource) override;
};

using CFinnhubEconomicCountryListPtr = shared_ptr<CProductFinnhubEconomicCountryList>;
