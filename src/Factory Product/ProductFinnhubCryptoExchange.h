#pragma once

#include"ProductFinnhub.h"

class CWebData;

class CProductFinnhubCryptoExchange final : public CProductFinnhub {
public:
	CProductFinnhubCryptoExchange();
	~CProductFinnhubCryptoExchange() override = default;

	void InquireData(const std::stop_token& st, const string& strHeaders, const string& strParams, const string& strSuffix, const string& strInquiryToken) override; // default do nothing
	void WebStatusCheck(cpr::Response& r) override;
	shared_ptr<vector<string>> CreateMessage() override;
	void UpdateSystemStatus() override;
	shared_ptr<vector<string>> Parse(const string& text);
};

using CFinnhubCryptoExchangePtr = shared_ptr<CProductFinnhubCryptoExchange>;
