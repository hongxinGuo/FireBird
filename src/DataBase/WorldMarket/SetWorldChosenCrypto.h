// SetWorldChosenCrypto.h : CSetWorldChosenCrypto µÄÉùÃ÷
#pragma once

#include"VirtualSetChosenStake.h"

class CSetWorldChosenCrypto : public CVirtualSetChosenStake {
public:
	CSetWorldChosenCrypto(CString strSchema = _T("WorldMarket"), CString strTable = _T("choiced_crypto"), CDatabase* pDatabase = nullptr)
		: CVirtualSetChosenStake(strSchema, strTable, pDatabase) {
	}
};
