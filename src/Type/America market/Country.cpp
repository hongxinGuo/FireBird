#include"pch.h"
#include"Country.h"

#include "CharSetTransfer.h"

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

void CCountry::Append(CSetCountry& setCountry) const {
	setCountry.AddNew();
	setCountry.m_Code2 = m_strCode2.c_str();
	setCountry.m_Code3 = m_strCode3.c_str();
	setCountry.m_CodeNo = m_strCodeNo.c_str();
	setCountry.m_Country = m_strCountry.c_str();
	setCountry.m_Currency = m_strCurrency.c_str();
	setCountry.m_CurrencyCode = m_strCurrencyCode.c_str();
	setCountry.Update();
}

void CCountry::Load(const CSetCountry& setCountry) {
	m_strCode2 = T2Utf8(setCountry.m_Code2);
	m_strCode3 = T2Utf8(setCountry.m_Code3);
	m_strCodeNo = T2Utf8(setCountry.m_CodeNo);
	m_strCountry = T2Utf8(setCountry.m_Country);
	m_strCurrency = T2Utf8(setCountry.m_Currency);
	m_strCurrencyCode = T2Utf8(setCountry.m_CurrencyCode);
}
