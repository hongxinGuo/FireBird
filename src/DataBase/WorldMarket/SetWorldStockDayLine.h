// SetWorldStockDayLine.h : CSetDayLineBasicInfo µÄÉùÃ÷

#pragma once

#include"VirtualSetHistoryCandleBasic.h"

class CSetWorldStockDayLine : public CVirtualSetHistoryCandleBasic {
public:
	CSetWorldStockDayLine(CString strSchema = _T("WorldMarket"), CString strTable = _T("stock_dayline"), CDatabase* pDatabase = nullptr)
		: CVirtualSetHistoryCandleBasic(strSchema, strTable, pDatabase) {
	}
};
