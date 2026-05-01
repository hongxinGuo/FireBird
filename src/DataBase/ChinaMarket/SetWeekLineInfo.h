// CSetChinaMarketDayLineInfo.h : CSetChinaMarketDayLineInfo 的声明

#pragma once

#include"VirtualSetHistoryCandle.h"

class CSetWeekLineInfo final : public CVirtualSetHistoryCandle {
public:
	CSetWeekLineInfo(const CString& strSchema = "ChinaMarket", const CString& strTable = "weekline", CDatabase* pDatabase = nullptr)
		: CVirtualSetHistoryCandle(strSchema, strTable, pDatabase) {}
	~CSetWeekLineInfo() override = default;
};
