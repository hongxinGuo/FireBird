#pragma once
#include"stdafx.h"

#include"SetForexSymbol.h"

using namespace std;
#include<memory>

class CForexSymbol : public CObject {
public:
  CForexSymbol();

  void Load(CSetForexSymbol& setForexSymbol);
  void Append(CSetForexSymbol& setForexSymbol);
  void Save(CSetForexSymbol& setForexSymbol);

  CString GetDescription(void) { return m_strDescription; }
  void SetDescription(CString strDescription) { m_strDescription = strDescription; }
  CString GetExchange(void) { return m_strExchange; }
  void SetExchange(CString strExchange) { m_strExchange = strExchange; }
  CString GetSymbol(void) { return m_strSymbol; }
  void SetSymbol(CString strSymbol) { m_strSymbol = strSymbol; }

public:
  CString m_strDescription;
  CString m_strDisplaySymbol;
  CString m_strExchange;
  CString m_strSymbol;
};

typedef shared_ptr<CForexSymbol> CForexSymbolPtr;
