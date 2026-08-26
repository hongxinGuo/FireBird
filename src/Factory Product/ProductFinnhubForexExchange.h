#pragma once

#include"ProductFinnhub.h"

using std::string;
using std::shared_ptr;
using std::vector;

class CProductFinnhubForexExchange final : public CProductFinnhub {
public:
	CProductFinnhubForexExchange();
	~CProductFinnhubForexExchange() override = default;

	void InquireData(const std::stop_token& st, const string& strHeaders, const string& strParams, const string& strSuffix, const string& strInquiryToken) override; // default do nothing
	void WebStatusCheck(cpr::Response& r) override;

	shared_ptr<vector<string>> CreateMessage() override;
	shared_ptr<vector<string>> Parse(const string& text);

	void UpdateSystemStatus() override;
};

using CFinnhubForexExchangePtr = shared_ptr<CProductFinnhubForexExchange>;
