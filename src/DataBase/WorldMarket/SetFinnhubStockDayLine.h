// SetFinnhubStockDayLine.h : CSetChinaMarketDayLneBasicInfo 的声明

#pragma once

#include"VirtualSetHistoryCandleBasic.h"

class CSetFinnhubStockDayLine final : public CVirtualSetHistoryCandleBasic {
public:
	CSetFinnhubStockDayLine(const CString& strSchema = "WorldMarket", const CString& strTable = "finnhub_stock_dayline", CDatabase* pDatabase = nullptr)
		: CVirtualSetHistoryCandleBasic(strSchema, strTable, pDatabase) {}
	~CSetFinnhubStockDayLine() override = default;
};
