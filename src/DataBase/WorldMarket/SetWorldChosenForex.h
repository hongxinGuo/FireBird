// CSetWorldChosenForex.h : CSetChinaChosenForex µÄÉùÃ÷
#pragma once
#include"VirtualSetChosenStake.h"

class CSetWorldChosenForex : public CVirtualSetChosenStake {
public:
	CSetWorldChosenForex(const CString& strSchema = _T("WorldMarket"), const CString& strTable = _T("choice_forex"), CDatabase* pDatabase = nullptr)
		: CVirtualSetChosenStake(strSchema, strTable, pDatabase) {}
};
