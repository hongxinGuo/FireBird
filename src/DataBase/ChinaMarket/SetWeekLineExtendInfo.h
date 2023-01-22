#pragma once

#include"VirtualSetHistoryCandleExtend.h"

class CSetWeekLineExtendInfo : public CVirtualSetHistoryCandleExtend {
public:
	CSetWeekLineExtendInfo(CString strSchema = _T("ChinaMarket"), CString strTable = _T("WeekLineinfo"), CDatabase* pDatabase = nullptr)
		: CVirtualSetHistoryCandleExtend(strSchema, strTable, pDatabase) {
	}
};
