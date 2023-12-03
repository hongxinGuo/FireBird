#pragma once

#include"ProductTiingo.h"
#include"DayLine.h"

class CProductTiingoStockDayLine final : public CProductTiingo {
public:
	CProductTiingoStockDayLine();
	~CProductTiingoStockDayLine() override = default;

	CString CreateMessage() override;
	bool ParseAndStoreWebData(CWebDataPtr pWebData) override;
	CDayLineVectorPtr ParseTiingoStockDayLine(const CWebDataPtr& pWebData) const;
};

using CTiingoStockDayLinePtr = shared_ptr<CProductTiingoStockDayLine>;
