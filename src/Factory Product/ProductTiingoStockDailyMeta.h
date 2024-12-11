#pragma once

#include"ProductTiingo.h"
import FireBird.Type.TiingoStockDailyMeta;

class CProductTiingoStockDailyMeta final : public CProductTiingo {
public:
	CProductTiingoStockDailyMeta();
	~CProductTiingoStockDailyMeta() override = default;

	CString CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	CTiingoStockDailyMetaPtr ParseTiingoStockDailyMeta(const CWebDataPtr& pWebData);
};

using CProductTiingoStockDailyMetaPtr = shared_ptr<CProductTiingoStockDailyMeta>;
