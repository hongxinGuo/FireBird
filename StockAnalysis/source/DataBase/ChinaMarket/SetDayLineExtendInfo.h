#pragma once

#include"VirtualSetHistoryCandleExtend.h"

class CSetDayLineExtendInfo : public CVirtualSetHistoryCandleExtend
{
public:
	CSetDayLineExtendInfo(CString strSchema = _T("ChinaMarket"), CString strTable = _T("daylineinfo"), CDatabase* pDatabase = nullptr)
		: CVirtualSetHistoryCandleExtend(strSchema, strTable, pDatabase) {
	}
	DECLARE_DYNAMIC(CSetDayLineExtendInfo)
};
