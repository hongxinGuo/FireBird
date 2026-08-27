#pragma once

#include"ProductFinnhub.h"

#include"SECFiling.h"

class CProductFinnhubSECFilings final : public CProductFinnhub {
public:
	CProductFinnhubSECFilings();
	~CProductFinnhubSECFilings() override = default;

	void InquireData(const std::stop_token& st) override; // default do nothing
	void WebStatusCheck(cpr::Response& r) override;
	void UpdateSystemStatus() override;

	shared_ptr<vector<string>> CreateMessage() override;
	CSECFilingsPtr Parse(const string& text);
};

using CFinnhubSECFilings = shared_ptr<CProductFinnhubSECFilings>;
