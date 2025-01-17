#pragma once

#include"ProductTiingo.h"
#include"TiingoDayLine.h"

class CProductTiingoStockDayLine final : public CProductTiingo {
public:
	CProductTiingoStockDayLine();
	~CProductTiingoStockDayLine() override = default;

	CString CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	CTiingoDayLinesPtr ParseTiingoStockDayLine(const CWebDataPtr& pWebData);

	CString GetDayLineInquiryParam(const CString& strSymbol, long lStartDate, long lCurrentDate);
};

using CTiingoStockDayLinePtr = shared_ptr<CProductTiingoStockDayLine>;
