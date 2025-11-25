// SetWorldChosenCrypto.h : CSetWorldChosenCrypto µÄÉùÃ÷
#pragma once

#include"VirtualSetChosenStake.h"

class CSetWorldChosenCrypto : public CVirtualSetChosenStake {
public:
	CSetWorldChosenCrypto(const CString& strSchema = "WorldMarket", const CString& strTable = "choice_crypto", CDatabase* pDatabase = nullptr)
		: CVirtualSetChosenStake(strSchema, strTable, pDatabase) {}
};
