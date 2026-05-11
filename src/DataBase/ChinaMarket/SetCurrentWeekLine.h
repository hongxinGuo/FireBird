// SetCurrentWeekLine.h : CSetCurrentWeekLine 的声明

#pragma once

#include"VirtualSetHistoryCandle.h"

class CSetCurrentWeekLine : public CVirtualSetHistoryCandle {
public:
	CSetCurrentWeekLine(const CString& strSchema = "ChinaMarket", const CString& strTable = "china_current_weekline", CDatabase* pDatabase = nullptr)
		: CVirtualSetHistoryCandle(strSchema, strTable, pDatabase) {}
};
