#pragma once

#include"ProductTiingo.h"
#include "TiingoMarketNews.h"

class CProductTiingoMarketNews final : public CProductTiingo {
public:
	CProductTiingoMarketNews();
	CProductTiingoMarketNews(const CProductTiingoMarketNews&) = delete;
	CProductTiingoMarketNews& operator=(const CProductTiingoMarketNews&) = delete;
	CProductTiingoMarketNews(const CProductTiingoMarketNews&&) noexcept = delete;
	CProductTiingoMarketNews& operator=(const CProductTiingoMarketNews&&) noexcept = delete;
	~CProductTiingoMarketNews() override = default;

	string CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	CTiingoMarketNewssPtr ParseTiingoMarketNews(const CWebDataPtr& pWebData);

	void UpdateSystemStatus() override;
};

using CProductTiingoMarketNewsPtr = shared_ptr<CProductTiingoMarketNews>;
