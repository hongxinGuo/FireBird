// CSetChinaMarketDayLneBasicInfo.h : CSetChinaMarketDayLneBasicInfo µÄÉùÃ÷

#pragma once

#include"VirtualSetHistoryCandleBasic.h"

class CSetWeekLineBasicInfo final : public CVirtualSetHistoryCandleBasic {
public:
	CSetWeekLineBasicInfo(const CString& strSchema = _T("ChinaMarket"), const CString& strTable = _T("weekline"), CDatabase* pDatabase = nullptr)
		: CVirtualSetHistoryCandleBasic(strSchema, strTable, pDatabase) {}
};
