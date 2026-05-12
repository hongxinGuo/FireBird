// SetTiingoStockDayLine.h : CSetChinaMarketDayLineInfo 的声明

#pragma once

#include"VirtualSetHistoryCandle.h"

class CSetTiingoStockDayLine final : public CVirtualSetHistoryCandle {
public:
	CSetTiingoStockDayLine(const CString& strSchema = "stock_market", const CString& strTable = "tiingo_stock_dayline", CDatabase* pDatabase = nullptr)
		: CVirtualSetHistoryCandle(strSchema, strTable, pDatabase) {}
	~CSetTiingoStockDayLine() override = default;
};
