#include"accessory.h"
#include "CompanyProfile.h"

CCompanyProfile::CCompanyProfile() : CObject() {
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
  m_tIPODate = setCompanyProfile.m_IPODate;
  m_strPhone = setCompanyProfile.m_Phone;
  m_dShareOutstanding = atof(setCompanyProfile.m_ShareOutstanding);
  m_lMarketCapitalization = atoi(setCompanyProfile.m_MarketCapitalization);
  m_strName = setCompanyProfile.m_Name;
  m_strWebURL = setCompanyProfile.m_WebURL;
  m_strFinnhubIndustry = setCompanyProfile.m_FinnhubIndustry;
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
  setCompanyProfile.m_IPODate = m_tIPODate;
  setCompanyProfile.m_Phone = m_strPhone;
  setCompanyProfile.m_ShareOutstanding = ConvertValueToString(m_dShareOutstanding, 6);
  setCompanyProfile.m_MarketCapitalization = ConvertValueToString(m_lMarketCapitalization, 0);
  setCompanyProfile.m_Name = m_strName;
  setCompanyProfile.m_WebURL = m_strWebURL;
  setCompanyProfile.m_FinnhubIndustry = m_strFinnhubIndustry;
  setCompanyProfile.Update();
}

void CCompanyProfile::Update(CCompanySymbolPtr pSymbol) {
  m_strDescription = pSymbol->m_strDescription;
  m_strCurrency = pSymbol->m_strCurrency;
  m_strDisplaySymbol = pSymbol->m_strSymbol;
  m_lLastUpdateDate = pSymbol->lCompanyProfileUpdateDate;
  m_strSymbol = pSymbol->m_strSymbol;
  m_strType = pSymbol->m_strType;
}