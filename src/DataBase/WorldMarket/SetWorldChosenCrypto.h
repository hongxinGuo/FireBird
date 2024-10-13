// SetWorldChosenCrypto.h : CSetWorldChosenCrypto µÄÉùÃ÷
#pragma once

#include"VirtualSetChosenStake.h"

class CSetWorldChosenCrypto : public CVirtualSetChosenStake {
public:
	CSetWorldChosenCrypto(const CString& strSchema = _T("WorldMarket"), const CString& strTable = _T("choice_crypto"), CDatabase* pDatabase = nullptr)
		: CVirtualSetChosenStake(strSchema, strTable, pDatabase) {}
};
