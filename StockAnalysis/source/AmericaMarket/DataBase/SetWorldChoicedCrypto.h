// SetWorldChoicedCrypto.h : CSetWorldChoicedCrypto µÄÉùÃ÷
#pragma once

#include"VirtualSetChoicedStake.h"

class CSetWorldChoicedCrypto : public CVirtualSetChoicedStake
{
public:
	CSetWorldChoicedCrypto(CString strSchema = _T("WorldMarket"), CString strTable = _T("choiced_crypto"), CDatabase* pDatabase = nullptr)
		: CVirtualSetChoicedStake(strSchema, strTable, pDatabase) {
	}
	DECLARE_DYNAMIC(CSetWorldChoicedCrypto)
};
