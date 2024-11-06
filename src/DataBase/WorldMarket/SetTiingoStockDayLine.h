// SetTiingoStockDayLine.h : CSetDayLineBasicInfo µÄÉùÃ÷

#pragma once

#include"VirtualSetHistoryCandleBasic.h"

class CSetTiingoStockDayLine final : public CVirtualSetHistoryCandleBasic {
public:
	CSetTiingoStockDayLine(const CString& strSchema = _T("WorldMarket"), const CString& strTable = _T("tiingo_stock_dayline"), CDatabase* pDatabase = nullptr)
		: CVirtualSetHistoryCandleBasic(strSchema, strTable, pDatabase) {}
};
