#pragma once

#include"ProductFinnhub.h"

class CMarketStatus;

class CProductFinnhubMarketStatus final : public CProductFinnhub {
public:
	CProductFinnhubMarketStatus();
	~CProductFinnhubMarketStatus() override = default;

	void InquireData(const std::stop_token& st) override; // default do nothing
	void WebStatusCheck(cpr::Response& r) override;
	void UpdateSystemStatus() override;

	shared_ptr<vector<string>> CreateMessage() override;
	shared_ptr<vector<CMarketStatus>> Parse(const string& text);
};

using CProductFinnhubMarketStatusPtr = shared_ptr<CProductFinnhubMarketStatus>;
