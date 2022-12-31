#pragma once

#include"VirtualWebProduct.h"
#include"DayLine.h"

class CProductTiingoStockDayLine final : public CVirtualWebProduct {
public:
	CProductTiingoStockDayLine();
	~CProductTiingoStockDayLine() override = default;

	CString CreateMessage(void) final;
	bool ParseAndStoreWebData(CWebDataPtr pWebData) override;
	CDayLineVectorPtr ParseTiingoStockDayLine(CWebDataPtr pWebData);
};

using CTiingoStockDayLinePtr = shared_ptr<CProductTiingoStockDayLine>;
