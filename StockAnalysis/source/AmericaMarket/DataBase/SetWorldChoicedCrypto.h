// SetWorldChoicedCrypto.h : CSetChoicedStock µÄÉùÃ÷
#pragma once

#include"VirtualSetChoicedStake.h"

class CSetWorldChoicedCrypto : public CVirtualSetChoicedStake
{
public:
	CSetWorldChoicedCrypto(CString strTableName = _T("choiced_crypto"), CDatabase* pDatabase = nullptr)
		: CVirtualSetChoicedStake(m_pDatabase) {
		sm_TableName = strTableName;
	}
	DECLARE_DYNAMIC(CSetWorldChoicedCrypto)
};
