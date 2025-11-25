#pragma once

#include"VirtualSetHistoryCandleExtend.h"

class CSetWeekLineExtendInfo : public CVirtualSetHistoryCandleExtend {
public:
	CSetWeekLineExtendInfo(const CString& strSchema = "ChinaMarket", const CString& strTable = "WeekLineinfo", CDatabase* pDatabase = nullptr)
		: CVirtualSetHistoryCandleExtend(strSchema, strTable, pDatabase) {}
};
