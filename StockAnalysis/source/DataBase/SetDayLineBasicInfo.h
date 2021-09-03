// CSetDayLineBasicInfo.h : CSetDayLineBasicInfo µÄÉùÃ÷

#pragma once

#include"VirtualSetHistoryCandle.h"

class CSetDayLineBasicInfo : public CVirtualSetHistoryCandle
{
public:
	CSetDayLineBasicInfo(CString strSchema = _T("ChinaMarket"), CString strTable = _T("dayline"), CDatabase* pDatabase = nullptr)
		: CVirtualSetHistoryCandle(strSchema, strTable, pDatabase) {
	}
	DECLARE_DYNAMIC(CSetDayLineBasicInfo)
};
