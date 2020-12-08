#include"accessory.h"
#include "CompanyProfile.h"

CCompanyProfile::CCompanyProfile() : CObject() {
  m_lCompanyProfileUpdateDate = 19900101;
  m_lMarketCapitalization = 0;
  m_dShareOutstanding = 0;

  m_fUpdateDatabase = false;
  m_fInquiry = true;
}

void CCompanyProfile::Load(CSetCompanyProfile& setCompanyProfile) {
  m_strSymbol = setCompanyProfile.m_Symbol;
  m_strCurrency = setCompanyProfile.m_Currency;
  m_strDescription = setCompanyProfile.m_Description;
  m_strDisplaySymbol = setCompanyProfile.m_DisplaySymbol;
  m_strType = setCompanyProfile.m_Type;
  m_strCountry = setCompanyProfile.m_Country;
  m_strExchange = setCompanyProfile.m_Exchange;
  m_strIPODate = setCompanyProfile.m_IPODate;
  m_strPhone = setCompanyProfile.m_Phone;
  m_dShareOutstanding = atof(setCompanyProfile.m_ShareOutstanding);
  m_lMarketCapitalization = atoi(setCompanyProfile.m_MarketCapitalization);
  m_strName = setCompanyProfile.m_Name;
  m_strWebURL = setCompanyProfile.m_WebURL;
  m_strFinnhubIndustry = setCompanyProfile.m_FinnhubIndustry;
  m_lCompanyProfileUpdateDate = setCompanyProfile.m_CompanyProfileUpdateDate;
  if ((m_strType.GetLength() < 2) || (m_strCurrency.GetLength() < 2)) m_fInquiry = false;
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
  setCompanyProfile.m_IPODate = m_strIPODate;
  setCompanyProfile.m_Phone = m_strPhone;
  setCompanyProfile.m_ShareOutstanding = ConvertValueToString(m_dShareOutstanding);
  setCompanyProfile.m_MarketCapitalization = ConvertValueToString(m_lMarketCapitalization);
  setCompanyProfile.m_Name = m_strName;
  setCompanyProfile.m_WebURL = m_strWebURL;
  setCompanyProfile.m_FinnhubIndustry = m_strFinnhubIndustry;
  setCompanyProfile.m_CompanyProfileUpdateDate = m_lCompanyProfileUpdateDate;
  setCompanyProfile.Update();
}

void CCompanyProfile::Update(CSetCompanyProfile& setCompanyProfile) {
  setCompanyProfile.Edit();
  setCompanyProfile.m_Symbol = m_strSymbol;
  setCompanyProfile.m_Currency = m_strCurrency;
  setCompanyProfile.m_Description = m_strDescription;
  setCompanyProfile.m_DisplaySymbol = m_strDisplaySymbol;
  setCompanyProfile.m_Type = m_strType;
  setCompanyProfile.m_Country = m_strCountry;
  setCompanyProfile.m_Exchange = m_strExchange;
  setCompanyProfile.m_IPODate = m_strIPODate;
  setCompanyProfile.m_Phone = m_strPhone;
  setCompanyProfile.m_ShareOutstanding = ConvertValueToString(m_dShareOutstanding);
  setCompanyProfile.m_MarketCapitalization = ConvertValueToString(m_lMarketCapitalization);
  setCompanyProfile.m_Name = m_strName;
  setCompanyProfile.m_WebURL = m_strWebURL;
  setCompanyProfile.m_FinnhubIndustry = m_strFinnhubIndustry;
  setCompanyProfile.m_CompanyProfileUpdateDate = m_lCompanyProfileUpdateDate;
  setCompanyProfile.Update();
}