// CSetDayLineBasicInfo.h : CSetDayLineBasicInfo µÄÉùÃ÷

#pragma once

#include"VirtualSetHistoryCandleBasic.h"

class CSetDayLineBasicInfo : public CVirtualSetHistoryCandleBasic {
public:
	CSetDayLineBasicInfo(const CString& strSchema = _T("ChinaMarket"), const CString& strTable = _T("dayline"), CDatabase* pDatabase = nullptr)
		: CVirtualSetHistoryCandleBasic(strSchema, strTable, pDatabase) { }
};
