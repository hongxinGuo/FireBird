#pragma once

#include"WorldMarket.h"
#include"ProductFinnhub.h"

class CProductFinnhubMarketHoliday final : public CProductFinnhub {
public:
	CProductFinnhubMarketHoliday();
	~CProductFinnhubMarketHoliday() override = default;

	CString CreateMessage() override;
	bool ParseAndStoreWebData(CWebDataPtr pWebData) override;
	static bool IsNeedAddExchangeCode(const CString& strStockSymbol, const CString& strExchangeCode);
	CWorldStockVectorPtr ParseFinnhubMarketHoliday(const CWebDataPtr& pWebData);
};

using CProductFinnhubMarketHolidayPtr = shared_ptr<CProductFinnhubMarketHoliday>;
