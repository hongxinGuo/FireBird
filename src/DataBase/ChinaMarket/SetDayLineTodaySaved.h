// CSetDayLineTodaySaved.h : CSetDayLineTodaySaved 的声明

#pragma once

#include"VirtualSetHistoryCandle.h"

class CSetDayLineTodaySaved : public CVirtualSetHistoryCandle {
public:
	CSetDayLineTodaySaved(const CString& strSchema = "ChinaMarket", const CString& strTable = "today", CDatabase* pDatabase = nullptr)
		: CVirtualSetHistoryCandle(strSchema, strTable, pDatabase) {}

	~CSetDayLineTodaySaved() override = default;
};
