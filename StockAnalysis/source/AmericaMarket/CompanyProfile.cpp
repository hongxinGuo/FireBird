#include "CompanyProfile.h"

CCompanyProfile::CCompanyProfile() : CObject() {
  m_lIPODate = 0;
  m_lLastUpdateDate = 19900101;
}

void CCompanyProfile::Load(CSetCompanyProfile& setCompanyProfile) {
  m_strSymbol = setCompanyProfile.m_Symbol;
  m_strCurrency = setCompanyProfile.m_Currency;
  m_strDescription = setCompanyProfile.m_Description;
  m_strDisplaySymbol = setCompanyProfile.m_DisplaySymbol;
  m_strType = setCompanyProfile.m_Type;
  m_strCountry = setCompanyProfile.m_Country;
  m_strExchange = setCompanyProfile.m_Exchange;
  m_lIPODate = setCompanyProfile.m_IPODate;
  m_strPhone = setCompanyProfile.m_Phone;
  m_strShareOutstanding = setCompanyProfile.m_ShareOutstanding;
  m_strMarketCapitalization = setCompanyProfile.m_MarketCapitalization;
  m_strName = setCompanyProfile.m_Name;
  m_strWebURL = setCompanyProfile.m_WebURL;
  m_strFinnHubIndustry = setCompanyProfile.m_FinnHubIndustry;
}

void CCompanyProfile::Save(CSetCompanyProfile& setCompanyProfile) {
  setCompanyProfile.AddNew();
  setCompanyProfile.m_Symbol = m_strSymbol;
  setCompanyProfile.m_Currency = m_strCurrency;
  setCompanyProfile.m_Description = m_strDescription;
  setCompanyProfile.m_DisplaySymbol = m_strDisplaySymbol;
  setCompanyProfile.m_Type = m_strType;
  setCompanyProfile.m_Country = m_strCountry;
  setCompanyProfile.m_Exchange = m_strExchange;
  setCompanyProfile.m_IPODate = m_lIPODate;
  setCompanyProfile.m_Phone = m_strPhone;
  setCompanyProfile.m_ShareOutstanding = m_strShareOutstanding;
  setCompanyProfile.m_MarketCapitalization = m_strMarketCapitalization;
  setCompanyProfile.m_Name = m_strName;
  setCompanyProfile.m_WebURL = m_strWebURL;
  setCompanyProfile.m_FinnHubIndustry = m_strFinnHubIndustry;
  setCompanyProfile.Update();
}

void CCompanyProfile::Update(CCompanySymbolPtr pSymbol) {
  m_strDescription = pSymbol->m_strDescription;
  m_strCurrency = pSymbol->m_strCurrency;
  m_strDisplaySymbol = pSymbol->m_strSymbol;
  m_lLastUpdateDate = pSymbol->m_iCompanyProfileUpdateDate;
  m_strSymbol = pSymbol->m_strSymbol;
  m_strType = pSymbol->m_strType;
}