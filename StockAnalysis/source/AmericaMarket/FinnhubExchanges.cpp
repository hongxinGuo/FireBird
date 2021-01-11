#include"FinnhubExchanges.h"

CFinnhubExchanges::CFinnhubExchanges() : CObject() {
  m_strCode = _T(" ");
  m_strName = _T(" ");
  m_strMic = _T(" ");
  m_strTimeZone = _T(" ");
  m_strHour = _T(" ");
  m_strCloseDate = _T(" ");
  m_strCountry = _T("");
  m_strSource = _T("");
}

void CFinnhubExchanges::Append(CSetFinnhubExchanges& setFinnhubExchanges) {
  setFinnhubExchanges.AddNew();
  setFinnhubExchanges.m_Code = m_strCode;
  setFinnhubExchanges.m_Name = m_strName;
  setFinnhubExchanges.m_Mic = m_strMic;
  setFinnhubExchanges.m_TimeZone = m_strTimeZone;
  setFinnhubExchanges.m_Hour = m_strHour;
  setFinnhubExchanges.m_CloseDate = m_strCloseDate;
  setFinnhubExchanges.m_Country = m_strCountry;
  setFinnhubExchanges.m_Source = m_strSource;
  setFinnhubExchanges.Update();
}

void CFinnhubExchanges::Load(CSetFinnhubExchanges& setFinnhubExchanges) {
  m_strCode = setFinnhubExchanges.m_Code;
  m_strName = setFinnhubExchanges.m_Name;
  m_strMic = setFinnhubExchanges.m_Mic;
  m_strTimeZone = setFinnhubExchanges.m_TimeZone;
  m_strHour = setFinnhubExchanges.m_Hour;
  m_strCloseDate = setFinnhubExchanges.m_CloseDate;
  m_strCountry = setFinnhubExchanges.m_Country;
  m_strSource = setFinnhubExchanges.m_Source;
}