// SetCurrentWeekLine.h : CSetCurrentWeekLine 的声明

#pragma once

#include"VirtualSetHistoryCandleBasic.h"

class CSetCurrentWeekLine : public CVirtualSetHistoryCandleBasic {
public:
	CSetCurrentWeekLine(const CString& strSchema = "ChinaMarket", const CString& strTable = "CurrentWeekLine", CDatabase* pDatabase = nullptr)
		: CVirtualSetHistoryCandleBasic(strSchema, strTable, pDatabase) {}
};
