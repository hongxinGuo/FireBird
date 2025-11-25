// CSetChinaMarketDayLneBasicInfo.h : CSetChinaMarketDayLneBasicInfo µÄÉùÃ÷

#pragma once

#include"VirtualSetHistoryCandleBasic.h"

class CSetWeekLineBasicInfo final : public CVirtualSetHistoryCandleBasic {
public:
	CSetWeekLineBasicInfo(const CString& strSchema = "ChinaMarket", const CString& strTable = "weekline", CDatabase* pDatabase = nullptr)
		: CVirtualSetHistoryCandleBasic(strSchema, strTable, pDatabase) {}
};
