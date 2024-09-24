#pragma once

#include"ProductTiingo.h"
#include "TiingoMarketNews.h"

class CProductTiingoMarketNews final : public CProductTiingo {
public:
	CProductTiingoMarketNews();
	~CProductTiingoMarketNews() override = default;

	CString CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	CVectorTiingoMarketNewsPtr ParseTiingoMarketNews(const CWebDataPtr& pWebData);

	void UpdateDataSourceStatus(CVirtualDataSourcePtr pDataSource) override;
};

using CProductTiingoMarketNewsPtr = shared_ptr<CProductTiingoMarketNews>;
