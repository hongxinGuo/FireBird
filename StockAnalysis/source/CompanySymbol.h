#pragma once

#include"stdafx.h"

#include"SetCompanySymbol.h"

using namespace std;
#include<memory>

class CCompanySymbol : public CObject {
public:
  void Load(CSetCompanySymbol& setCompanySymbol);
  void Save(CSetCompanySymbol& setCompanySymbol);
public:
  CString m_strDescription;
  CString m_strDisplaySymbol;
  CString m_strSymbol;
  CString m_strType;
  CString m_strCurrency;
};

typedef shared_ptr<CCompanySymbol> CCompanySymbolPtr;
