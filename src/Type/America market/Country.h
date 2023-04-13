#pragma once

#include"SetCountry.h"

#include<memory>

class CCountry final {
public:
	CCountry();
	~CCountry() = default;

	void Append(CSetCountry& setCountry);
	void Load(const CSetCountry& setCountry);

public:
	CString m_strCode2;
	CString m_strCode3;
	CString m_strCodeNo;
	CString m_strCountry;
	CString m_strCurrency;
	CString m_strCurrencyCode;
};

using CCountryPtr = shared_ptr<CCountry>;
using CCountryVectorPtr = shared_ptr<vector<CCountryPtr>>;
