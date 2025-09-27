#pragma once

#include"ProductTiingo.h"
#include "TiingoMarketNews.h"

class CProductTiingoMarketNews final : public CProductTiingo {
public:
	CProductTiingoMarketNews();
	~CProductTiingoMarketNews() override = default;

	string CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	CVectorTiingoMarketNewsPtr ParseTiingoMarketNews(const CWebDataPtr& pWebData);

	void UpdateSystemStatus(CVirtualDataSourcePtr pDataSource) override;
};

using CProductTiingoMarketNewsPtr = shared_ptr<CProductTiingoMarketNews>;
