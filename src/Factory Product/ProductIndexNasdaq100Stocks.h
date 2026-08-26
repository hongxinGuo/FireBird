#pragma once

#include"VirtualWorldMarketWebProduct.h"

using std::vector;
using std::shared_ptr;

class CProductIndexNasdaq100Stocks : public CVirtualWorldMarketWebProduct {
public:
	CProductIndexNasdaq100Stocks();
	// 不允许赋值。
	CProductIndexNasdaq100Stocks(const CProductIndexNasdaq100Stocks&) = delete;
	CProductIndexNasdaq100Stocks& operator=(const CProductIndexNasdaq100Stocks&) = delete;
	CProductIndexNasdaq100Stocks(const CProductIndexNasdaq100Stocks&&) noexcept = delete;
	CProductIndexNasdaq100Stocks& operator=(const CProductIndexNasdaq100Stocks&&) noexcept = delete;
	~CProductIndexNasdaq100Stocks() override = default;

	void InquireData(const std::stop_token& st, const string& strHeaders, const string& strParams, const string& strSuffix, const string& strInquiryToken) override; // default do nothing
	void WebStatusCheck(cpr::Response& r) override;
	void UpdateSystemStatus() override;

	shared_ptr<vector<string>> CreateMessage() override;
	vector<string> Parse(const string& text);
};

using CProductIndexNasdaq100StocksPtr = shared_ptr<CProductIndexNasdaq100Stocks>;
