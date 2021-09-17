// CSetDayLineBasicInfo.h : CSetDayLineBasicInfo µÄÉùÃ÷

#pragma once

#include"VirtualSetHistoryCandleBasic.h"

class CSetDayLineBasicInfo : public CVirtualSetHistoryCandleBasic
{
public:
	CSetDayLineBasicInfo(CString strSchema = _T("ChinaMarket"), CString strTable = _T("dayline"), CDatabase* pDatabase = nullptr)
		: CVirtualSetHistoryCandleBasic(strSchema, strTable, pDatabase) {
	}
	DECLARE_DYNAMIC(CSetDayLineBasicInfo)
};
