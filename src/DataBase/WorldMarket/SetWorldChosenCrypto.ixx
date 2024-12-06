// SetWorldChosenCrypto.h : CSetWorldChosenCrypto µÄÉùÃ÷
#include"pch.h"
module;
export module FireBird.Set.WorldChosenCrypto;

import FireBird.Set.VirtualChosenStake;

export {
	class CSetWorldChosenCrypto : public CVirtualSetChosenStake {
	public:
		CSetWorldChosenCrypto(const CString& strSchema = _T("WorldMarket"), const CString& strTable = _T("choice_crypto"), CDatabase* pDatabase = nullptr)
			: CVirtualSetChosenStake(strSchema, strTable, pDatabase) {
		}
	};
}
