#pragma once

#include"ProductTiingo.h"
#include"TiingoDayLine.h"

class CProductTiingoStockDayLine final : public CProductTiingo {
public:
	CProductTiingoStockDayLine();
	~CProductTiingoStockDayLine() override = default;

	string CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	CTiingoDayLinesPtr ParseTiingoStockDayLine(const CWebDataPtr& pWebData);

	string GetDayLineInquiryParam(const string& strSymbol, long lStartDate, long lCurrentDate);
};

using CTiingoStockDayLinePtr = shared_ptr<CProductTiingoStockDayLine>;
