// CSetDayLineBasicInfo.h : CSetDayLineBasicInfo ������

#pragma once

#include"VirtualSetHistoryCandleBasic.h"

class CSetWeekLineBasicInfo : public CVirtualSetHistoryCandleBasic
{
public:
	CSetWeekLineBasicInfo(CString strSchema = _T("ChinaMarket"), CString strTable = _T("weekline"), CDatabase* pDatabase = nullptr)
		: CVirtualSetHistoryCandleBasic(strSchema, strTable, pDatabase) {
	}
	DECLARE_DYNAMIC(CSetWeekLineBasicInfo)
};
