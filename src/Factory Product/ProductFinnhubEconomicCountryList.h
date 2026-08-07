#pragma once

#include"ProductFinnhub.h"

class CCountry;

class CProductFinnhubEconomicCountryList final : public CProductFinnhub {
public:
	CProductFinnhubEconomicCountryList();
	~CProductFinnhubEconomicCountryList() override = default;

	shared_ptr<vector<string>> CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	shared_ptr<vector<CCountry>> ParseFinnhubCountryList(const CWebDataPtr& pWebData);

	void UpdateSystemStatus() override;
};

using CFinnhubEconomicCountryListPtr = shared_ptr<CProductFinnhubEconomicCountryList>;
