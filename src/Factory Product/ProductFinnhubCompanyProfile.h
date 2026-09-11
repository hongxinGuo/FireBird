#pragma once

#include"ProductFinnhub.h"

class CFinnhubStock;

class CProductFinnhubCompanyProfile final : public CProductFinnhub {
public:
	CProductFinnhubCompanyProfile();
	~CProductFinnhubCompanyProfile() override = default;

	void InquireData(const std::stop_token& st) override;
	void WebStatusCheck(cpr::Response& r) override;
	void UpdateSystemStatus() override;

	shared_ptr<vector<string>> CreateMessage() override;
	bool Parse(const string& text, const shared_ptr<CFinnhubStock>& pStock) const;
};

using CFinnhubCompanyProfilePtr = shared_ptr<CProductFinnhubCompanyProfile>;
