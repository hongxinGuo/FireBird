#include"globedef.h"
#include"accessory.h"
#include "CompanyProfile.h"

CCompanyProfile::CCompanyProfile() : CObject() {
  m_strIPODate = _T("");
  m_strCurrency = _T("");
  m_strType = _T("");
  m_lCompanyProfileUpdateDate = 19900101;
  m_lDayLineStartDate = 29900101;
  m_lDayLineEndDate = 19900101;

  m_dMarketCapitalization = 0;
  m_dShareOutstanding = 0;

  m_fUpdateDatabase = false;
  m_fInquiryCompanyProfile = true;
  m_fDayLineNeedUpdate = true;
  m_fDayLineNeedSaving = false;

  m_vDayLine.resize(0);
}

void CCompanyProfile::Load(CSetCompanyProfile& setCompanyProfile) {
  m_strSymbol = setCompanyProfile.m_Symbol;
  m_strCurrency = setCompanyProfile.m_Currency;
  m_strDescription = setCompanyProfile.m_Description;
  m_strDisplaySymbol = setCompanyProfile.m_DisplaySymbol;
  m_strType = setCompanyProfile.m_Type;
  m_strCountry = setCompanyProfile.m_Country;
  m_strExchange = setCompanyProfile.m_Exchange;
  m_strTicker = setCompanyProfile.m_Ticker;
  m_strIPODate = setCompanyProfile.m_IPODate;
  m_strPhone = setCompanyProfile.m_Phone;
  m_dShareOutstanding = atof(setCompanyProfile.m_ShareOutstanding);
  m_dMarketCapitalization = atof(setCompanyProfile.m_MarketCapitalization);
  m_strName = setCompanyProfile.m_Name;
  m_strWebURL = setCompanyProfile.m_WebURL;
  m_strFinnhubIndustry = setCompanyProfile.m_FinnhubIndustry;
  m_lCompanyProfileUpdateDate = setCompanyProfile.m_CompanyProfileUpdateDate;
  m_lDayLineStartDate = setCompanyProfile.m_DayLineStartDate;
  m_lDayLineEndDate = setCompanyProfile.m_DayLineEndDate;
  if ((m_strType.GetLength() < 2) || (m_strCurrency.GetLength() < 2)) {
    //m_fInquiryCompanyProfile = false;
    //m_lCompanyProfileUpdateDate = gl_pAmericaStakeMarket->GetFormatedMarketDate();
  }
}

bool CCompanyProfile::CheckDayLineUpdateStatus() {
  ASSERT(IsDayLineNeedUpdate()); // Ĭ��״̬Ϊ����������Ҫ����
  // ���ٸ����������ݱ��ϸ�������Ҫ�µĹ�Ʊ���������еĹ�Ʊ����ѯһ�飬�Է�ֹ�����¹�Ʊ�����ϵĹ�Ʊ���»�Ծ������
  if (gl_pAmericaStakeMarket->GetLastTradeDate() <= GetDayLineEndDate()) { // ������������Ϊ���ջ�����һ�������յ����ݡ�
    SetDayLineNeedUpdate(false); // �������ݲ���Ҫ����
  }
  return true;
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
  setCompanyProfile.m_Ticker = m_strTicker;
  setCompanyProfile.m_IPODate = m_strIPODate;
  setCompanyProfile.m_Phone = m_strPhone;
  setCompanyProfile.m_ShareOutstanding = ConvertValueToString(m_dShareOutstanding);
  setCompanyProfile.m_MarketCapitalization = ConvertValueToString(m_dMarketCapitalization);
  setCompanyProfile.m_Name = m_strName;
  setCompanyProfile.m_WebURL = m_strWebURL;
  setCompanyProfile.m_FinnhubIndustry = m_strFinnhubIndustry;
  setCompanyProfile.m_CompanyProfileUpdateDate = m_lCompanyProfileUpdateDate;
  setCompanyProfile.m_DayLineStartDate = m_lDayLineStartDate;
  setCompanyProfile.m_DayLineEndDate = m_lDayLineEndDate;
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
  setCompanyProfile.m_Ticker = m_strTicker;
  setCompanyProfile.m_IPODate = m_strIPODate;
  setCompanyProfile.m_Phone = m_strPhone;
  setCompanyProfile.m_ShareOutstanding = ConvertValueToString(m_dShareOutstanding);
  setCompanyProfile.m_MarketCapitalization = ConvertValueToString(m_dMarketCapitalization);
  setCompanyProfile.m_Name = m_strName;
  setCompanyProfile.m_WebURL = m_strWebURL;
  setCompanyProfile.m_FinnhubIndustry = m_strFinnhubIndustry;
  setCompanyProfile.m_CompanyProfileUpdateDate = m_lCompanyProfileUpdateDate;
  setCompanyProfile.m_DayLineStartDate = m_lDayLineStartDate;
  setCompanyProfile.m_DayLineEndDate = m_lDayLineEndDate;
  if (m_strWebURL.GetLength() > 50) {
    TRACE("%s�ַ���̫��%d\n", m_strSymbol.GetBuffer(), m_strWebURL.GetLength());
  }
  TRACE("���¹�Ʊ��%s\n", m_strSymbol.GetBuffer());
  setCompanyProfile.Update();
}

void CCompanyProfile::UpdateDayLine(vector<CDayLinePtr>& vDayLine) {
  m_vDayLine.resize(0);
  for (auto& pDayLine : vDayLine) {
    m_vDayLine.push_back(pDayLine);
  }
}