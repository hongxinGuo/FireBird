#pragma once

import FireBird.Product.Tiingo.Base;
import FireBird.HistoryCandle.DayLine;

class CProductTiingoForexDayLine final : public CProductTiingo {
public:
	CProductTiingoForexDayLine();
	~CProductTiingoForexDayLine() override = default;

	CString CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	CDayLinesPtr ParseTiingoForexDayLine(const CWebDataPtr& pWebData);
};

using CTiingoForexDayLinePtr = shared_ptr<CProductTiingoForexDayLine>;
