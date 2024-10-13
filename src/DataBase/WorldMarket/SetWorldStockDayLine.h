// SetWorldStockDayLine.h : CSetDayLineBasicInfo µÄÉùÃ÷

#pragma once

#include"VirtualSetHistoryCandleBasic.h"

class CSetWorldStockDayLine : public CVirtualSetHistoryCandleBasic {
public:
	CSetWorldStockDayLine(const CString& strSchema = _T("WorldMarket"), const CString& strTable = _T("finnhub_stock_dayline"), CDatabase* pDatabase = nullptr)
		: CVirtualSetHistoryCandleBasic(strSchema, strTable, pDatabase) {}
};
