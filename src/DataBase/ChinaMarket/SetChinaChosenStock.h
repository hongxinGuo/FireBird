// SetChinaChosenStock.h : CSetChinaChosenStock µÄÉùÃ÷

#pragma once
#include"VirtualSetChosenStake.h"

class CSetChinaChosenStock : public CVirtualSetChosenStake
{
public:
	CSetChinaChosenStock(CString strSchema = _T("ChinaMarket"), CString strTable = _T("choiced_stock"), CDatabase* pDatabase = nullptr)
		: CVirtualSetChosenStake(strSchema, strTable, pDatabase) {
	}
	DECLARE_DYNAMIC(CSetChinaChosenStock)
};
