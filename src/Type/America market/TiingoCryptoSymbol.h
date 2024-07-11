#pragma once

#include"SetTiingoCrypto.h"

#include<memory>
#include<vector>

class CTiingoCryptoSymbol final {
public:
	CTiingoCryptoSymbol();
	~CTiingoCryptoSymbol() = default;

	void Reset();

	void Load(const CSetTiingoCrypto& setTiingoCrypto);
	void Append(CSetTiingoCrypto& setTiingoCrypto);
	void Save(CSetTiingoCrypto& setTiingoCrypto);

	CString m_strTicker;
	CString m_strName;
	CString m_strDescription; // 此项已废弃不用了。
	CString m_strBaseCurrency;
	CString m_strQuoteCurrency;

	// 无需存储数据区
};

using CTiingoCryptoSymbolPtr = shared_ptr<CTiingoCryptoSymbol>;
using CTiingoCryptosPtr = shared_ptr<vector<CTiingoCryptoSymbolPtr>>;
