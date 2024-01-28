#pragma once

#include "MarketHoliday.h"
#include"ProductFinnhub.h"

class CProductFinnhubMarketHoliday final : public CProductFinnhub {
public:
	CProductFinnhubMarketHoliday();
	~CProductFinnhubMarketHoliday() override = default;

	CString CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	CMarketHolidayVectorPtr ParseFinnhubMarketHoliday(const CWebDataPtr& pWebData);
};

using CProductFinnhubMarketHolidayPtr = shared_ptr<CProductFinnhubMarketHoliday>;
