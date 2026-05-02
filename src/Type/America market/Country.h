#pragma once

#include"SetCountry.h"

using std::vector;
using std::shared_ptr;

class CCountry final {
public:
	CCountry();
	CCountry(const CCountry& other) = default;
	CCountry& operator=(const CCountry& other) = default;
	CCountry(CCountry&& other) noexcept = default;
	CCountry& operator=(CCountry&& other) noexcept = default;
	~CCountry() = default;

	void Append(CSetCountry& setCountry) const;
	void Load(const CSetCountry& setCountry);

public:
	string m_strCode2;
	string m_strCode3;
	string m_strCodeNo;
	string m_strCountry;
	string m_strCurrency;
	string m_strCurrencyCode;
};

using CCountryPtr = shared_ptr<CCountry>;
using CCountriesPtr = shared_ptr<vector<CCountry>>;
