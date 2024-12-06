#pragma once

import FireBird.Set.VirtualHistoryCandleExtend;

class CSetWeekLineExtendInfo : public CVirtualSetHistoryCandleExtend {
public:
	CSetWeekLineExtendInfo(const CString& strSchema = _T("ChinaMarket"), const CString& strTable = _T("WeekLineinfo"), CDatabase* pDatabase = nullptr)
		: CVirtualSetHistoryCandleExtend(strSchema, strTable, pDatabase) {}
};
