// SetWorldChosenStock.h : CSetWorldChosenStock µÄÉùÃ÷
#include"pch.h"
export module FireBird.Set.WorldChosenStock;

import FireBird.Set.VirtualChosenStake;

export {
	class CSetWorldChosenStock : public CVirtualSetChosenStake {
	public:
		CSetWorldChosenStock(const CString& strSchema = _T("WorldMarket"), const CString& strTable = _T("choice_stock"), CDatabase* pDatabase = nullptr)
			: CVirtualSetChosenStake(strSchema, strTable, pDatabase) {
		}
	};
}
