#pragma once

#include"ProductFinnhub.h"

class CCountry;

class CProductFinnhubEconomicCountryList final : public CProductFinnhub {
public:
	CProductFinnhubEconomicCountryList();
	~CProductFinnhubEconomicCountryList() override = default;

	void InquireData(const std::stop_token& st) override; // default do nothing
	void WebStatusCheck(cpr::Response& r) override;
	void UpdateSystemStatus() override;

	shared_ptr<vector<string>> CreateMessage() override;
	shared_ptr<vector<CCountry>> Parse(const std::string& text);
};

using CFinnhubEconomicCountryListPtr = shared_ptr<CProductFinnhubEconomicCountryList>;
