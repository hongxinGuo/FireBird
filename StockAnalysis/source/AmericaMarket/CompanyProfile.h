#pragma once

#include"stdafx.h"

#include"SetCompanyProfile.h"
#include"CompanySymbol.h"

using namespace std;
#include<memory>

class CCompanyProfile : public CObject {
public:
  CCompanyProfile();
public:
  void Load(CSetCompanyProfile& setCompanyProfile);
  void Save(CSetCompanyProfile& setCompanyProfile);

  void Update(CCompanySymbolPtr pSymbol);

public:
  CString m_strDescription;
  CString m_strDisplaySymbol;
  CString m_strSymbol;
  CString m_strType;
  CString m_strCurrency;
  CString m_strCountry;
  CString m_strExchange;
  long m_lIPODate;
  CString m_strPhone;
  CString m_strMarketCapitalization;
  CString m_strShareOutstanding;
  CString m_strName;
  CString m_strTicker; // ���洢
  CString m_strWebURL;
  CString m_strLogo; // ���洢
  CString m_strFinnHubIndustry;

  // ����洢����
  long m_lLastUpdateDate; // �ϴθ���ʱ��
};

typedef shared_ptr<CCompanyProfile> CCompanyProfilePtr;