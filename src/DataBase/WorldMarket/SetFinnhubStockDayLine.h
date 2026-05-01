// SetFinnhubStockDayLine.h : CSetChinaMarketDayLineInfo 的声明

#pragma once

#include"VirtualSetHistoryCandle.h"

class CSetFinnhubStockDayLine final : public CVirtualSetHistoryCandle {
public:
	CSetFinnhubStockDayLine(const CString& strSchema = "WorldMarket", const CString& strTable = "finnhub_stock_dayline", CDatabase* pDatabase = nullptr)
		: CVirtualSetHistoryCandle(strSchema, strTable, pDatabase) {}
	~CSetFinnhubStockDayLine() override = default;
};
