// SetFinnhubStockDayLine.h : CSetDayLineBasicInfo µÄÉùÃ÷

#pragma once

import FireBird.Set.VirtualHistoryCandleBasic;

class CSetFinnhubStockDayLine final : public CVirtualSetHistoryCandleBasic {
public:
	CSetFinnhubStockDayLine(const CString& strSchema = _T("WorldMarket"), const CString& strTable = _T("finnhub_stock_dayline"), CDatabase* pDatabase = nullptr)
		: CVirtualSetHistoryCandleBasic(strSchema, strTable, pDatabase) {}
};
