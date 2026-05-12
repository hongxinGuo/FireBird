// CSetChinaMarketDayLineInfo.h : CSetChinaMarketDayLineInfo 的声明

#pragma once

#include"VirtualSetHistoryCandle.h"

class CSetWeekLineInfo final : public CVirtualSetHistoryCandle {
public:
	CSetWeekLineInfo(const CString& strSchema = "stock_market", const CString& strTable = "china_stock_weekline", CDatabase* pDatabase = nullptr)
		: CVirtualSetHistoryCandle(strSchema, strTable, pDatabase) {}
	~CSetWeekLineInfo() override = default;
};
