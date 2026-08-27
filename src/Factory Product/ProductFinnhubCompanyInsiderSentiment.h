#pragma once

#include"ProductFinnhub.h"

#include"InsiderSentiment.h"

using std::shared_ptr;
using std::vector;

class CProductFinnhubCompanyInsiderSentiment final : public CProductFinnhub {
public:
	CProductFinnhubCompanyInsiderSentiment();
	~CProductFinnhubCompanyInsiderSentiment() override = default;

	void InquireData(const std::stop_token& st) override; // default do nothing
	void WebStatusCheck(cpr::Response& r) override;
	void UpdateSystemStatus() override;
	CInsiderSentimentsPtr Parse(const string& text);

	shared_ptr<vector<string>> CreateMessage() override;
};

using CProductFinnhubCompanyInsiderSentimentPtr = shared_ptr<CProductFinnhubCompanyInsiderSentiment>;
