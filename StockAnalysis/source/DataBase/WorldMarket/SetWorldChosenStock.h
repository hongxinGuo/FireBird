// SetWorldChosenStock.h : CSetWorldChosenStock µÄÉùÃ÷
#pragma once

#include"VirtualSetChosenStake.h"

class CSetWorldChosenStock : public CVirtualSetChosenStake
{
public:
	CSetWorldChosenStock(CString strSchema = _T("WorldMarket"), CString strTable = _T("choiced_stock"), CDatabase* pDatabase = nullptr)
		: CVirtualSetChosenStake(strSchema, strTable, pDatabase) {
	}
	DECLARE_DYNAMIC(CSetWorldChosenStock)
};
