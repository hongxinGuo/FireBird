#pragma once

#include"VirtualSetHistoryCandleExtend.h"

class CSetChinaMarketDayLneExtendInfo : public CVirtualSetHistoryCandleExtend {
public:
	CSetChinaMarketDayLneExtendInfo(const CString& strSchema = _T("ChinaMarket"), const CString& strTable = _T("daylineinfo"), CDatabase* pDatabase = nullptr)
		: CVirtualSetHistoryCandleExtend(strSchema, strTable, pDatabase) {}
};
