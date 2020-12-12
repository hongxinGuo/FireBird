#pragma once

#include"stdafx.h"
#include"globeSymbolDef.h"

#include"DayLine.h"
#include"SetAmericaStake.h"

using namespace std;
#include<memory>
#include<atomic>
#include<vector>

class CAmericaStake : public CObject {
public:
  CAmericaStake();
  void Reset(void);
public:
  void Load(CSetAmericaStake& setAmericaStake);
  void SetCheckingDayLineStatus(void);
  bool CheckDayLineUpdateStatus();
  void Save(CSetAmericaStake& setAmericaStake);
  void Update(CSetAmericaStake& setAmericaStake);
  bool SaveDayLine(void);

  void UpdateDayLine(vector<CDayLinePtr>& vDayLine);

  long GetDayLineStartDate(void) noexcept { return m_lDayLineStartDate; }
  void SetDayLineStartDate(long lDate) noexcept { m_lDayLineStartDate = lDate; }
  long GetDayLineEndDate(void) noexcept { return m_lDayLineEndDate; }
  void SetDayLineEndDate(long lDate) noexcept { m_lDayLineEndDate = lDate; }
  long GetLastRTDataUpdateDate(void) noexcept { return m_lLastRTDataUpdateDate; }
  void SetLastRTDataUpdateDate(long lDate) noexcept { m_lLastRTDataUpdateDate = lDate; }
  void UpdateDayLineStartEndDate(void);
  long GetDayLineSize(void) noexcept { return m_vDayLine.size(); }
  void UnloadDayLine(void) { m_vDayLine.resize(0); }

  bool IsDayLineNeedUpdate(void) noexcept { return m_fDayLineNeedUpdate; }
  void SetDayLineNeedUpdate(bool fFlag) noexcept { m_fDayLineNeedUpdate = fFlag; }
  bool IsDayLineNeedSaving(void) noexcept { return m_fDayLineNeedSaving; }
  void SetDayLineNeedSaving(bool fFlag) noexcept { m_fDayLineNeedSaving = fFlag; }
  bool IsDayLineNeedSavingAndClearFlag(void);
  bool HaveNewDayLineData(void);
  bool IsDayLineDBUpdated(void) noexcept { return (m_fDayLineDBUpdated); }
  void SetDayLineDBUpdated(bool fUpdate) noexcept { m_fDayLineDBUpdated = fUpdate; }

  long GetIPOStatus(void) noexcept { return m_lIPOStatus; }
  void SetIPOStatus(long lValue) noexcept { m_lIPOStatus = lValue; }
  bool IsDelisted(void) noexcept { return (m_lIPOStatus == __STAKE_DELISTED__); }
  bool IsNullStock(void) noexcept { return (m_lIPOStatus == __STAKE_NULL__); }
  bool IsIPOed(void) noexcept { return(m_lIPOStatus == __STAKE_IPOED__); }
  bool IsNotChecked(void) noexcept { return(m_lIPOStatus == __STAKE_NOT_CHECKED__); }

  CString GetSymbol(void) { return m_strSymbol; }
  CString GetTicker(void) { return m_strTicker; }

  CString GetDayLineInquiryString(time_t tCurrentTime);

public:
  // symbol��Ϣ
  CString m_strDescription;
  CString m_strDisplaySymbol;
  CString m_strSymbol;
  CString m_strType;
  CString m_strCurrency;

  // profile��Ϣ
  CString m_strAddress;
  CString m_strCity;
  CString m_strCountry;
  CString m_strCusip;
  CString m_strSedol;
  long m_lEmployeeTotal;
  CString m_strExchange;
  CString m_strGgroup;
  CString m_strGind;
  CString m_strGsector;
  CString m_strGsubind;
  CString m_strIPODate;
  CString m_strIsin;
  double m_dMarketCapitalization;
  CString m_strNaics;
  CString m_strNaicsNationalIndustry;
  CString m_strNaicsSector;
  CString m_strNaicsSubsector;
  CString m_strName;
  CString m_strPhone;
  double m_dShareOutstanding;
  CString m_strState;
  CString m_strTicker;
  CString m_strWebURL;
  CString m_strLogo; // ���洢
  CString m_strFinnhubIndustry;
  long m_lProfileUpdateDate; // ���¼���������
  long m_lDayLineStartDate;
  long m_lDayLineEndDate;
  long m_lLastRTDataUpdateDate; // ����ʵʱ���ݸ�������
  long m_lIPOStatus;

  vector<CDayLinePtr> m_vDayLine;

  // ����洢������
  bool m_fDayLineDBUpdated;
  bool m_fUpdateDatabase; // Ҫ����´�����
  bool m_fInquiryAmericaStake; // Ҫ����¹�˾���
  bool m_fDayLineNeedUpdate; // ������Ҫ���¡�Ĭ��Ϊ��
  atomic_bool m_fDayLineNeedSaving; // ������ʷ�����Ѵ����ȴ��洢��

protected:
  // ����洢������
};

typedef shared_ptr<CAmericaStake> CAmericaStakePtr;