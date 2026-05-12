// SetWorldChosenCrypto.h : CSetWorldChosenCrypto 的声明
#pragma once

#include"VirtualSetChosenStake.h"

class CSetWorldChosenCrypto : public CVirtualSetChosenStake {
public:
	CSetWorldChosenCrypto(const CString& strSchema = "stock_market", const CString& strTable = "world_choice_crypto", CDatabase* pDatabase = nullptr)
		: CVirtualSetChosenStake(strSchema, strTable, pDatabase) {}
	~CSetWorldChosenCrypto() override = default;
};
