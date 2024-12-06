// SetWorldChosenStock.h : CSetWorldChosenStock µÄÉùÃ÷
#pragma once

import FireBird.Set.VirtualChosenStake;

class CSetWorldChosenStock : public CVirtualSetChosenStake {
public:
	CSetWorldChosenStock(const CString& strSchema = _T("WorldMarket"), const CString& strTable = _T("choice_stock"), CDatabase* pDatabase = nullptr)
		: CVirtualSetChosenStake(strSchema, strTable, pDatabase) {}
};
