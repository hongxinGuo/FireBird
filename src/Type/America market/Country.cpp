#include"pch.h"
#include"Country.h"

CCountry::CCountry() {
	Reset();
}

void CCountry::Reset() {
	m_strCode2 = " ";
	m_strCode3 = " ";
	m_strCodeNo = " ";
	m_strCountry = " ";
	m_strCurrency = " ";
	m_strCurrencyCode = " ";
}
