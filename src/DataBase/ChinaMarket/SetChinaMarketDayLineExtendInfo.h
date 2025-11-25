#pragma once

#include"VirtualSetHistoryCandleExtend.h"

class CSetChinaMarketDayLneExtendInfo : public CVirtualSetHistoryCandleExtend {
public:
	CSetChinaMarketDayLneExtendInfo(const CString& strSchema = "ChinaMarket", const CString& strTable = "daylineinfo", CDatabase* pDatabase = nullptr)
		: CVirtualSetHistoryCandleExtend(strSchema, strTable, pDatabase) {}
};
