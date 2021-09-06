// SetChinaChoicedStock.h : CSetChinaChoicedStock µÄÉùÃ÷

#pragma once
#include"VirtualSetChoicedStake.h"

class CSetChinaChoicedStock : public CVirtualSetChoicedStake
{
public:
	CSetChinaChoicedStock(CString strSchema = _T("ChinaMarket"), CString strTable = _T("choiced_stock"), CDatabase* pDatabase = nullptr)
		: CVirtualSetChoicedStake(strSchema, strTable, pDatabase) {
	}
	DECLARE_DYNAMIC(CSetChinaChoicedStock)
};
