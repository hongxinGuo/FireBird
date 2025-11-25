// CSetChinaMarketDayLneBasicInfo.h : CSetChinaMarketDayLneBasicInfo µÄÉùÃ÷

#pragma once

#include"VirtualSetHistoryCandleBasic.h"

class CSetChinaMarketDayLneBasicInfo : public CVirtualSetHistoryCandleBasic {
public:
	CSetChinaMarketDayLneBasicInfo(const CString& strSchema = "ChinaMarket", const CString& strTable = "dayline", CDatabase* pDatabase = nullptr)
		: CVirtualSetHistoryCandleBasic(strSchema, strTable, pDatabase) {}
};
