#pragma once

#include"stdafx.h"

#include"DayLine.h"
#include"SetAmericaStake.h"

using namespace std;
#include<memory>
#include<atomic>
#include<vector>

class CAmericaStake : public CObject {
public:
  CAmericaStake();
public:
  void Load(CSetAmericaStake& setAmericaStake);
  bool CheckDayLineUpdateStatus();
  void Save(CSetAmericaStake& setAmericaStake);
  void Update(CSetAmericaStake& setAmericaStake);
  bool SaveDayLine(void);

  void UpdateDayLine(vector<CDayLinePtr>& vDayLine);

  long GetDayLineStartDate(void) noexcept { return m_lDayLineStartDate; }
  void SetDayLineStartDate(long lDate) noexcept { m_lDayLineStartDate = lDate; }
  long GetDayLineEndDate(void) noexcept { return m_lDayLineEndDate; }
  void SetDayLineEndDate(long lDate) noexcept { m_lDayLineEndDate = lDate; }
  void UpdateDayLineStartEndDate(void);
  long GetDayLineSize(void) noexcept { return m_vDayLine.size(); }
  void UnloadDayLine(void) noexcept { m_vDayLine.resize(0); }

  bool IsDayLineNeedUpdate(void) noexcept { return m_fDayLineNeedUpdate; }
  void SetDayLineNeedUpdate(bool fFlag) noexcept { m_fDayLineNeedUpdate = fFlag; }
  bool IsDayLineNeedSaving(void) noexcept { return m_fDayLineNeedSaving; }
  void SetDayLineNeedSaving(bool fFlag) noexcept { m_fDayLineNeedSaving = fFlag; }
  bool IsDayLineNeedSavingAndClearFlag(void);
  bool HaveNewDayLineData(void);
  bool IsDayLineDBUpdated(void) noexcept { return (m_fDayLineDBUpdated); }
  void SetDayLineDBUpdated(bool fUpdate) noexcept { m_fDayLineDBUpdated = fUpdate; }

  CString GetStakeSymbol(void) noexcept { return m_strSymbol; }

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
  long m_lAmericaStakeUpdateDate;
  long m_lDayLineStartDate;
  long m_lDayLineEndDate;

  vector<CDayLinePtr> m_vDayLine;
  // 无需存储数据区

  bool m_fDayLineDBUpdated;
  bool m_fUpdateDatabase; // 要求更新此数据
  bool m_fInquiryAmericaStake; // 要求更新公司简介
  bool m_fDayLineNeedUpdate; // 日线需要更新。默认为真
  atomic_bool m_fDayLineNeedSaving; // 日线历史数据已处理，等待存储。
};

typedef shared_ptr<CAmericaStake> CAmericaStakePtr;