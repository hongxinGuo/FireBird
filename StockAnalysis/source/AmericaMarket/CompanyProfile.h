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
  CString m_strTicker; // ���洢
  CString m_strWebURL;
  CString m_strLogo; // ���洢
  CString m_strFinnhubIndustry;
  long m_lCompanyProfileUpdateDate;

  // ����洢������
  bool m_fUpdateDatabase; // Ҫ����´�����
  bool m_fInquiry; // Ҫ�����
};

typedef shared_ptr<CCompanyProfile> CCompanyProfilePtr;