#pragma once

#include"ProductFinnhub.h"

class CProductFinnhubCompanyProfileConcise final : public CProductFinnhub {
public:
	CProductFinnhubCompanyProfileConcise();
	~CProductFinnhubCompanyProfileConcise() override = default;

	void InquireData(const std::stop_token& st, const string& strHeaders, const string& strParams, const string& strSuffix, const string& strInquiryToken) override; // default do nothing
	void WebStatusCheck(cpr::Response& r) override;

	shared_ptr<vector<string>> CreateMessage() override;
	bool Parse(const string& text, const CFinnhubStockPtr& pStock) const;

	void UpdateSystemStatus() override;
};

using CFinnhubCompanyProfileConcisePtr = shared_ptr<CProductFinnhubCompanyProfileConcise>;
