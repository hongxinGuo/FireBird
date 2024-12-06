// SetChinaChosenStock.h : CSetChinaChosenStock µÄÉùÃ÷

#pragma once
import FireBird.Set.VirtualChosenStake;

class CSetChinaChosenStock : public CVirtualSetChosenStake {
public:
	CSetChinaChosenStock(const CString& strSchema = _T("ChinaMarket"), const CString& strTable = _T("choice_stock"), CDatabase* pDatabase = nullptr)
		: CVirtualSetChosenStake(strSchema, strTable, pDatabase) {}
};
