// CSetForexDayLine.h : CSetForexDayLine µÄÉùÃ÷

#pragma once

#include"VirtualSetHistoryCandleBasic.h"

class CSetForexDayLine final : public CVirtualSetHistoryCandleBasic {
public:
	CSetForexDayLine(const CString& strSchema = "WorldMarket", const CString& strTable = "finnhub_forex_dayline", CDatabase* pDatabase = nullptr)
		: CVirtualSetHistoryCandleBasic(strSchema, strTable, pDatabase) {}
};
