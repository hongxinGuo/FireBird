#pragma once

#include"ProductTiingo.h"
#include "TiingoStockDailyMeta.h"

class CProductTiingoStockDailyMeta final : public CProductTiingo {
public:
	CProductTiingoStockDailyMeta();
	~CProductTiingoStockDailyMeta() override = default;

	CString CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	CTiingoStockDailyMetaPtr ParseTiingoStockDailyMeta(const CWebDataPtr& pWebData);
};

using CProductTiingoStockDailyMetaPtr = shared_ptr<CProductTiingoStockDailyMeta>;
