#pragma once

#include"stdafx.h"

#include"SetCountry.h"

using namespace std;
#include<memory>

class CCountry : public CObject {
public:
  CCountry();

  void Append(CSetCountry& setCountry);

  void Load(CSetCountry& setCountry);

public:
  CString m_strCode2;
  CString m_strCode3;
  CString m_strCodeNo;
  CString m_strCountry;
  CString m_strCurrency;
  CString m_strCurrencyCode;
};

typedef shared_ptr<CCountry> CCountryPtr;
