// CSetWorldChosenForex.h : CSetChinaChosenForex µÄÉùÃ÷
#pragma once
#include"VirtualSetChosenStake.h"

class CSetWorldChosenForex : public CVirtualSetChosenStake {
public:
	CSetWorldChosenForex(CString strSchema = _T("WorldMarket"), CString strTable = _T("choiced_forex"), CDatabase* pDatabase = nullptr)
		: CVirtualSetChosenStake(strSchema, strTable, pDatabase) {
	}
};
