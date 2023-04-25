// SetChinaChosenStock.h : CSetChinaChosenStock ������

#pragma once
#include"VirtualSetChosenStake.h"

class CSetChinaChosenStock : public CVirtualSetChosenStake {
public:
	CSetChinaChosenStock(const CString& strSchema = _T("ChinaMarket"), const CString& strTable = _T("choiced_stock"), CDatabase* pDatabase = nullptr)
		: CVirtualSetChosenStake(strSchema, strTable, pDatabase) { }
};
