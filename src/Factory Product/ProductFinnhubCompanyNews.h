/////////////////////////////////////////////////////////////////////////////////////////
//
// finnhub company news. available in North America only.
//
//
////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"ProductFinnhub.h"

#include"FinnhubCompanyNews.h"

class CProductFinnhubCompanyNews final : public CProductFinnhub {
public:
	CProductFinnhubCompanyNews();
	~CProductFinnhubCompanyNews() override = default;

	void InquireData(const std::stop_token& st) override; // default do nothing
	void WebStatusCheck(cpr::Response& r) override;
	void UpdateSystemStatus() override;

	shared_ptr<vector<string>> CreateMessage() override;
	CCompanyNewssPtr Parse(const string& text);
};

using CProductFinnhubCompanyNewsPtr = shared_ptr<CProductFinnhubCompanyNews>;
