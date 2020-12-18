#include"Country.h"

CCountry::CCountry() : CObject() {
  m_strCode2 = _T(" ");
  m_strCode3 = _T(" ");
  m_strCodeNo = _T(" ");
  m_strCountry = _T(" ");
  m_strCurrency = _T(" ");
  m_strCurrencyCode = _T(" ");
}

void CCountry::Append(CSetCountry& setCountry) {
  setCountry.AddNew();
  setCountry.m_Code2 = m_strCode2;
  setCountry.m_Code3 = m_strCode3;
  setCountry.m_CodeNo = m_strCodeNo;
  setCountry.m_Country = m_strCountry;
  setCountry.m_Currency = m_strCurrency;
  setCountry.m_CurrencyCode = m_strCurrencyCode;
  setCountry.Update();
}

void CCountry::Load(CSetCountry& setCountry) {
  m_strCode2 = setCountry.m_Code2;
  m_strCode3 = setCountry.m_Code3;
  m_strCodeNo = setCountry.m_CodeNo;
  m_strCountry = setCountry.m_Country;
  m_strCurrency = setCountry.m_Currency;
  m_strCurrencyCode = setCountry.m_CurrencyCode;
}