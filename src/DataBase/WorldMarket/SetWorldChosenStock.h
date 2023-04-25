// SetWorldChosenStock.h : CSetWorldChosenStock µÄÉùÃ÷
#pragma once

#include"VirtualSetChosenStake.h"

class CSetWorldChosenStock : public CVirtualSetChosenStake {
public:
	CSetWorldChosenStock(const CString& strSchema = _T("WorldMarket"), const CString& strTable = _T("choiced_stock"), CDatabase* pDatabase = nullptr)
		: CVirtualSetChosenStake(strSchema, strTable, pDatabase) { }
};
