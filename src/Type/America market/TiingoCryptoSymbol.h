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
	CString m_strDescription; // �����ѷ��������ˡ�
	CString m_strBaseCurrency;
	CString m_strQuoteCurrency;

protected:
	// ����洢������
};

using CTiingoCryptoSymbolPtr = shared_ptr<CTiingoCryptoSymbol>;
using CTiingoCryptoVectorPtr = shared_ptr<vector<CTiingoCryptoSymbolPtr>>;
