// SetWorldChoicedStock.h : CSetChoicedStock µÄÉùÃ÷
#pragma once

#include"VirtualSetChoicedStake.h"

class CSetWorldChoicedStock : public CVirtualSetChoicedStake
{
public:
	CSetWorldChoicedStock(CString strTableName = _T("choiced_stock"), CDatabase* pDatabase = nullptr)
		: CVirtualSetChoicedStake(m_pDatabase) {
		sm_TableName = strTableName;
	}
	DECLARE_DYNAMIC(CSetWorldChoicedStock)
};
