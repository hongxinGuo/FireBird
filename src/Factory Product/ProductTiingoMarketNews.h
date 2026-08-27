#pragma once

#include"ProductTiingo.h"
#include "TiingoMarketNews.h"

class CTiingoMarketNews;

class CProductTiingoMarketNews final : public CProductTiingo {
public:
	CProductTiingoMarketNews();
	CProductTiingoMarketNews(const CProductTiingoMarketNews&) = delete;
	CProductTiingoMarketNews& operator=(const CProductTiingoMarketNews&) = delete;
	CProductTiingoMarketNews(const CProductTiingoMarketNews&&) noexcept = delete;
	CProductTiingoMarketNews& operator=(const CProductTiingoMarketNews&&) noexcept = delete;
	~CProductTiingoMarketNews() override = default;

	void InquireData(const std::stop_token& st, const string& strHeaders, const string& strParams, const string& strSuffix, const string& strInquiryToken) override; // default do nothing
	void WebStatusCheck(cpr::Response& r) override;
	void UpdateSystemStatus() override;

	shared_ptr<vector<string>> CreateMessage() override;
	CTiingoMarketNewssPtr Parse(const string& text);

};

using CProductTiingoMarketNewsPtr = shared_ptr<CProductTiingoMarketNews>;
