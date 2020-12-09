#pragma once

#include"stdafx.h"

#include"DayLine.h"
#include"SetCompanyProfile.h"

using namespace std;
#include<memory>
#include<atomic>
#include<vector>

class CCompanyProfile : public CObject {
public:
  CCompanyProfile();
public:
  void Load(CSetCompanyProfile& setCompanyProfile);
  bool CheckDayLineUpdateStatus();
  void Save(CSetCompanyProfile& setCompanyProfile);
  void Update(CSetCompanyProfile& setCompanyProfile);

  void UpdateDayLine(vector<CDayLinePtr>& vDayLine);

  long GetDayLineStartDate(void) noexcept { return m_lDayLineStartDate; }
  void SetDayLineStartDate(long lDate) noexcept { m_lDayLineStartDate = lDate; }
  long GetDayLineEndDate(void) noexcept { return m_lDayLineEndDate; }
  void SetDayLineEndDate(long lDate) noexcept { m_lDayLineEndDate = lDate; }

  bool IsDayLineNeedUpdate(void) noexcept { return m_fDayLineNeedUpdate; }
  void SetDayLineNeedUpdate(bool fFlag) noexcept { m_fDayLineNeedUpdate = fFlag; }
  bool IsDayLineNeedSaving(void) noexcept { return m_fDayLineNeedSaving; }
  void SetDayLineNeedSaving(bool fFlag) noexcept { m_fDayLineNeedSaving = fFlag; }

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
  double m_dMarketCapitalization;
  double m_dShareOutstanding;
  CString m_strName;
  CString m_strTicker;
  CString m_strWebURL;
  CString m_strLogo; // 不存储
  CString m_strFinnhubIndustry;
  long m_lCompanyProfileUpdateDate;
  long m_lDayLineStartDate;
  long m_lDayLineEndDate;

  vector<CDayLinePtr> m_vDayLine;
  // 无需存储数据区

  bool m_fUpdateDatabase; // 要求更新此数据
  bool m_fInquiryCompanyProfile; // 要求更新公司简介
  bool m_fDayLineNeedUpdate; // 日线需要更新。默认为真
  atomic_bool m_fDayLineNeedSaving; // 日线历史数据已处理，等待存储。
};

typedef shared_ptr<CCompanyProfile> CCompanyProfilePtr;