#pragma once

#include"SetWorldStock.h"
#include"SetTiingoCrypto.h"

using namespace std;
#include<memory>
#include<atomic>
#include<vector>

class CTiingoCryptoSymbol : public CObject {
public:
	CTiingoCryptoSymbol();
	void Reset(void);

	void Load(CSetTiingoCrypto& setTiingoCrypto);
	void Append(CSetTiingoCrypto& setTiingoCrypto);
	void Save(CSetTiingoCrypto& setTiingoCrypto);

public:
	CString m_strTicker;
	CString m_strName;
	CString m_strDescription;
	CString m_strBaseCurrency;
	CString m_strQuoteCurrency;

protected:
	// 无需存储数据区
};

typedef shared_ptr<CTiingoCryptoSymbol> CTiingoCryptoSymbolPtr;
typedef shared_ptr<vector<CTiingoCryptoSymbolPtr>> CTiingoCryptoVectorPtr;
