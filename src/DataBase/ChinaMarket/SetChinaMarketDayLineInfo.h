// CSetChinaMarketDayLineInfo.h : CSetChinaMarketDayLineInfo 的声明

#pragma once

#include"VirtualSetHistoryCandle.h"

class CSetChinaMarketDayLineInfo : public CVirtualSetHistoryCandle {
public:
	CSetChinaMarketDayLineInfo(const CString& strSchema = "ChinaMarket", const CString& strTable = "dayline", CDatabase* pDatabase = nullptr)
		: CVirtualSetHistoryCandle(strSchema, strTable, pDatabase) {}
};
