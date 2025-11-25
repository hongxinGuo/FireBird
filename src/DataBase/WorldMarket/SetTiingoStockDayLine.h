// SetTiingoStockDayLine.h : CSetChinaMarketDayLneBasicInfo 的声明

#pragma once

#include"VirtualSetHistoryCandleBasic.h"

class CSetTiingoStockDayLine final : public CVirtualSetHistoryCandleBasic {
public:
	CSetTiingoStockDayLine(const CString& strSchema = "WorldMarket", const CString& strTable = "tiingo_stock_dayline", CDatabase* pDatabase = nullptr)
		: CVirtualSetHistoryCandleBasic(strSchema, strTable, pDatabase) {}
};
