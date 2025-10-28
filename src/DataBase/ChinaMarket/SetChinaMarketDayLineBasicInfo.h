// CSetChinaMarketDayLneBasicInfo.h : CSetChinaMarketDayLneBasicInfo µÄÉùÃ÷

#pragma once

#include"VirtualSetHistoryCandleBasic.h"

class CSetChinaMarketDayLneBasicInfo : public CVirtualSetHistoryCandleBasic {
public:
	CSetChinaMarketDayLneBasicInfo(const CString& strSchema = _T("ChinaMarket"), const CString& strTable = _T("dayline"), CDatabase* pDatabase = nullptr)
		: CVirtualSetHistoryCandleBasic(strSchema, strTable, pDatabase) {}
};
