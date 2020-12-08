#pragma once

#include"stdafx.h"

#include"SetCompanyProfile.h"

using namespace std;
#include<memory>

class CCompanyProfile : public CObject {
public:
  CCompanyProfile();
public:
  void Load(CSetCompanyProfile& setCompanyProfile);
  void Save(CSetCompanyProfile& setCompanyProfile);
  void Update(CSetCompanyProfile& setCompanyProfile);

public:
  CString m_strDescription;
  CString m_strDisplaySymbol;
  CString m_strSymbol;
  CString m_strType;
  CString m_strCurrency;
  CString m_strCountry;
  CString m_strExchange;
  CString m_strIPODate;
  CString m_strPhone;
  double m_lMarketCapitalization;
  double m_dShareOutstanding;
  CString m_strName;
  CString m_strTicker; // 不存储
  CString m_strWebURL;
  CString m_strLogo; // 不存储
  CString m_strFinnhubIndustry;
  long m_lCompanyProfileUpdateDate;

  // 无需存储数据区
  bool m_fUpdateDatabase; // 要求更新此数据
  bool m_fInquiry; // 要求更新
};

typedef shared_ptr<CCompanyProfile> CCompanyProfilePtr;