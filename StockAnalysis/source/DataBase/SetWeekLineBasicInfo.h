// CSetDayLineBasicInfo.h : CSetDayLineBasicInfo µÄÉùÃ÷

#pragma once

#include"VirtualSetHistoryCandle.h"

class CSetWeekLineBasicInfo : public CVirtualSetHistoryCandle
{
public:
	CSetWeekLineBasicInfo(CString strSchema = _T("ChinaMarket"), CString strTable = _T("weekline"), CDatabase* pDatabase = nullptr)
		: CVirtualSetHistoryCandle(strSchema, strTable, pDatabase) {
	}
	DECLARE_DYNAMIC(CSetWeekLineBasicInfo)
};
