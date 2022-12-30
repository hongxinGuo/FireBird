#pragma once

#include"SetTiingoCrypto.h"

#include<memory>
#include<vector>

class CTiingoCryptoSymbol final {
public:
	CTiingoCryptoSymbol();
	virtual ~CTiingoCryptoSymbol() = default;

	void Reset(void);

	void Load(CSetTiingoCrypto& setTiingoCrypto);
	void Append(CSetTiingoCrypto& setTiingoCrypto);
	void Save(CSetTiingoCrypto& setTiingoCrypto);

public:
	CString m_strTicker;
	CString m_strName;
	CString m_strDescription; // 此项已废弃不用了。
	CString m_strBaseCurrency;
	CString m_strQuoteCurrency;

protected:
	// 无需存储数据区
};

using CTiingoCryptoSymbolPtr = shared_ptr<CTiingoCryptoSymbol>;
using CTiingoCryptoVectorPtr = shared_ptr<vector<CTiingoCryptoSymbolPtr>>;
