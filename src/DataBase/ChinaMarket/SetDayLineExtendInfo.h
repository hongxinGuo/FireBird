#pragma once

#include"VirtualSetHistoryCandleExtend.h"

class CSetDayLineExtendInfo : public CVirtualSetHistoryCandleExtend {
public:
	CSetDayLineExtendInfo(const CString& strSchema = _T("ChinaMarket"), const CString& strTable = _T("daylineinfo"), CDatabase* pDatabase = nullptr)
		: CVirtualSetHistoryCandleExtend(strSchema, strTable, pDatabase) { }
};
