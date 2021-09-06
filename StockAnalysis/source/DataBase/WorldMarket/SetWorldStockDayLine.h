// SetWorldStockDayLine.h : CSetDayLineBasicInfo µÄÉùÃ÷

#pragma once

#include"VirtualSetHistoryCandle.h"

class CSetWorldStockDayLine : public CVirtualSetHistoryCandle
{
public:
	CSetWorldStockDayLine(CString strSchema = _T("WorldMarket"), CString strTable = _T("stock_dayline"), CDatabase* pDatabase = nullptr)
		: CVirtualSetHistoryCandle(strSchema, strTable, pDatabase) {
	}
	DECLARE_DYNAMIC(CSetWorldStockDayLine)
};
