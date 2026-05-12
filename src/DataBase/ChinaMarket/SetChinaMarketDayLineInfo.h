// CSetChinaMarketDayLineInfo.h : CSetChinaMarketDayLineInfo 的声明

#pragma once

#include"VirtualSetHistoryCandle.h"

class CSetChinaMarketDayLineInfo : public CVirtualSetHistoryCandle {
public:
	CSetChinaMarketDayLineInfo(const CString& strSchema = "stock_market", const CString& strTable = "china_stock_dayline", CDatabase* pDatabase = nullptr)
		: CVirtualSetHistoryCandle(strSchema, strTable, pDatabase) {}
};
