// CSetWorldChoicedForex.h : CSetChoicedStock µÄÉùÃ÷
#pragma once
#include"VirtualSetChoicedStake.h"

class CSetWorldChoicedForex : public CVirtualSetChoicedStake
{
public:
	CSetWorldChoicedForex(CString strTableName = _T("choiced_forex"), CDatabase* pDatabase = nullptr)
		: CVirtualSetChoicedStake(m_pDatabase) {
		sm_TableName = strTableName;
	}
	DECLARE_DYNAMIC(CSetWorldChoicedForex)
};
