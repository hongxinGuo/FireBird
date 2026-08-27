#pragma once
#pragma once

#include "FinnhubCompanySymbolChange.h"
#include"ProductFinnhub.h"

class CCompanySymbolChange;

class CProductFinnhubCompanySymbolChange final : public CProductFinnhub {
public:
	CProductFinnhubCompanySymbolChange();
	~CProductFinnhubCompanySymbolChange() override = default;

	void InquireData(const std::stop_token& st) override; // default do nothing
	void WebStatusCheck(cpr::Response& r) override;

	shared_ptr<vector<string>> CreateMessage() override;
	CCompanySymbolChangesPtr Parse(const string& text);

	void UpdateSystemStatus() override;

protected:
	bool m_bVoidJson{ false };
};

using CFinnhubCompanySymbolChangePtr = shared_ptr<CProductFinnhubCompanySymbolChange>;
