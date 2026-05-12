// CSetForexDayLine.h : CSetForexDayLine 的声明

#pragma once

#include"VirtualSetHistoryCandle.h"

class CSetForexDayLine final : public CVirtualSetHistoryCandle {
public:
	CSetForexDayLine(const CString& strSchema = "stock_market", const CString& strTable = "finnhub_forex_dayline", CDatabase* pDatabase = nullptr)
		: CVirtualSetHistoryCandle(strSchema, strTable, pDatabase) {}
	~CSetForexDayLine() override = default;
};
