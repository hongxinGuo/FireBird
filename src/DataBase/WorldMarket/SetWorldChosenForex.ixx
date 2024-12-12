// CSetWorldChosenForex.h : CSetChinaChosenForex µÄÉùÃ÷
#include"pch.h"
export module FireBird.Set.WorldChosenForex;

import FireBird.Set.VirtualChosenStake;

export {
	class CSetWorldChosenForex : public CVirtualSetChosenStake {
	public:
		CSetWorldChosenForex(const CString& strSchema = _T("WorldMarket"), const CString& strTable = _T("choice_forex"), CDatabase* pDatabase = nullptr)
			: CVirtualSetChosenStake(strSchema, strTable, pDatabase) {
		}
	};
}
