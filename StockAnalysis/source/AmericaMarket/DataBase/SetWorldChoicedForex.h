// CSetWorldChoicedForex.h : CSetChoicedStock µÄÉùÃ÷
#pragma once
#include"VirtualSetChoicedStake.h"

class CSetWorldChoicedForex : public CVirtualSetChoicedStake
{
public:
	CSetWorldChoicedForex(CString strSchema = _T("WorldMarket"), CString strTable = _T("choiced_forex"), CDatabase* pDatabase = nullptr)
		: CVirtualSetChoicedStake(strSchema, strTable, pDatabase) {
	}
	DECLARE_DYNAMIC(CSetWorldChoicedForex)
};
