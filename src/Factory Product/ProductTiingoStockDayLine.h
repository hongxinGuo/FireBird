#pragma once

import FireBird.Product.Tiingo.Base;
import FireBird.HistoryCandle.TiingoDayLine;

class CProductTiingoStockDayLine final : public CProductTiingo {
public:
	CProductTiingoStockDayLine();
	~CProductTiingoStockDayLine() override = default;

	CString CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	CTiingoDayLinesPtr ParseTiingoStockDayLine(const CWebDataPtr& pWebData);
};

using CTiingoStockDayLinePtr = shared_ptr<CProductTiingoStockDayLine>;
