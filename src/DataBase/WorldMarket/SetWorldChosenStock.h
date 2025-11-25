// SetWorldChosenStock.h : CSetWorldChosenStock µÄÉùÃ÷
#pragma once

#include"VirtualSetChosenStake.h"

class CSetWorldChosenStock : public CVirtualSetChosenStake {
public:
	CSetWorldChosenStock(const CString& strSchema = "WorldMarket", const CString& strTable = "choice_stock", CDatabase* pDatabase = nullptr)
		: CVirtualSetChosenStake(strSchema, strTable, pDatabase) {}
};
