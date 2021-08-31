// SetWorldChoicedStock.h : CSetChoicedStock µÄÉùÃ÷
#pragma once

#include"VirtualSetChoicedStake.h"

class CSetWorldChoicedStock : public CVirtualSetChoicedStake
{
public:
	CSetWorldChoicedStock(CString strSchema = _T("WorldMarket"), CString strTable = _T("choiced_stock"), CDatabase* pDatabase = nullptr)
		: CVirtualSetChoicedStake(strSchema, strTable, pDatabase) {
	}
	DECLARE_DYNAMIC(CSetWorldChoicedStock)
};
