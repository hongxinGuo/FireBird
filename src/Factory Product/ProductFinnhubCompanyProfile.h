#pragma once

#include"ProductFinnhub.h"

class CProductFinnhubCompanyProfile final : public CProductFinnhub {
public:
	CProductFinnhubCompanyProfile();
	~CProductFinnhubCompanyProfile() override = default;

	void InquireData(const std::stop_token& st) override; // default do nothing
	void WebStatusCheck(cpr::Response& r) override;
	void UpdateSystemStatus() override;

	shared_ptr<vector<string>> CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	bool ParseFinnhubStockProfile(CWebDataPtr pWebData, CFinnhubStockPtr pStock) const;
};

using CFinnhubCompanyProfilePtr = shared_ptr<CProductFinnhubCompanyProfile>;
