#pragma once

#include"ProductTiingo.h"
#include"DayLine.h"

class CProductTiingoStockDayLine final : public CProductTiingo {
public:
	CProductTiingoStockDayLine();
	~CProductTiingoStockDayLine() override = default;

	CString CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	CDayLinesPtr ParseTiingoStockDayLine(const CWebDataPtr& pWebData);
};

using CTiingoStockDayLinePtr = shared_ptr<CProductTiingoStockDayLine>;
