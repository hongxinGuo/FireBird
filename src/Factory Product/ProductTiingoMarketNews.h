#pragma once

#include"ProductTiingo.h"

class CTiingoMarketNews;

class CProductTiingoMarketNews final : public CProductTiingo {
public:
	CProductTiingoMarketNews();
	CProductTiingoMarketNews(const CProductTiingoMarketNews&) = delete;
	CProductTiingoMarketNews& operator=(const CProductTiingoMarketNews&) = delete;
	CProductTiingoMarketNews(const CProductTiingoMarketNews&&) noexcept = delete;
	CProductTiingoMarketNews& operator=(const CProductTiingoMarketNews&&) noexcept = delete;
	~CProductTiingoMarketNews() override = default;

	shared_ptr<vector<string>> CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	shared_ptr<vector<CTiingoMarketNews>> ParseTiingoMarketNews(const CWebDataPtr& pWebData);

	void UpdateSystemStatus() override;
};

using CProductTiingoMarketNewsPtr = shared_ptr<CProductTiingoMarketNews>;
