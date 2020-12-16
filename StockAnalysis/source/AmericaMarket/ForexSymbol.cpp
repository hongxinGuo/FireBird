#include "ForexSymbol.h"

CForexSymbol::CForexSymbol() : CObject() {
  m_strDescription = _T(" ");
  m_strDisplaySymbol = _T(" ");
  m_strExchange = _T(" ");
  m_strSymbol = _T(" ");
}

void CForexSymbol::Load(CSetForexSymbol& setForexSymbol) {
  m_strDescription = setForexSymbol.m_Description;
  m_strDisplaySymbol = setForexSymbol.m_DisplaySymbol;
  m_strExchange = setForexSymbol.m_Exchange;
  m_strSymbol = setForexSymbol.m_Symbol;
}

void CForexSymbol::Append(CSetForexSymbol& setForexSymbol) {
  setForexSymbol.AddNew();
  Save(setForexSymbol);
  setForexSymbol.Update();
}

void CForexSymbol::Save(CSetForexSymbol& setForexSymbol) {
  setForexSymbol.m_Description = m_strDescription;
  setForexSymbol.m_DisplaySymbol = m_strDisplaySymbol;
  setForexSymbol.m_Exchange = m_strExchange;
  setForexSymbol.m_Symbol = m_strSymbol;
}