#pragma once

import FireBird.Set.FinnhubCountry;

using std::vector;
using std::shared_ptr;

class CCountry final {
public:
	CCountry();
	~CCountry() = default;

	void Append(CSetCountry& setCountry) const;
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
using CCountrysPtr = shared_ptr<vector<CCountryPtr>>;
