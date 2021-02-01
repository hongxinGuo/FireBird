#pragma once
#include"stdafx.h"

#include"SetFinnhubForexSymbol.h"
#include"SetForexDayLine.h"

using namespace std;
#include<memory>
#include<atomic>
#include<vector>

class CFinnhubForexSymbol : public CObject {
public:
  CFinnhubForexSymbol();

  void Load(CSetFinnhubForexSymbol& setForexSymbol);
  void Append(CSetFinnhubForexSymbol& setForexSymbol);
  void Update(CSetFinnhubForexSymbol& setForexSymbol);
  void Save(CSetFinnhubForexSymbol& setForexSymbol);

  bool SaveDayLine(void);

  CString GetDescription(void) { return m_strDescription; }
  void SetDescription(CString strDescription) { m_strDescription = strDescription; }
  CString GetExchange(void) { return m_strExchange; }
  void SetExchange(CString strExchange) { m_strExchange = strExchange; }
  CString GetSymbol(void) { return m_strSymbol; }
  void SetSymbol(CString strSymbol) { m_strSymbol = strSymbol; }
  CString GetDisplaySymbol(void) { return m_strDisplaySymbol; }
  void SetDisplaySymbol(CString strSymbol) { m_strDisplaySymbol = strSymbol; }

  long GetDayLineStartDate(void) noexcept { return m_lDayLineStartDate; }
  void SetDayLineStartDate(long lDate) noexcept { m_lDayLineStartDate = lDate; }
  long GetDayLineEndDate(void) noexcept { return m_lDayLineEndDate; }
  void SetDayLineEndDate(long lDate) noexcept { m_lDayLineEndDate = lDate; }
  bool IsDayLineNeedUpdate(void) noexcept { return m_fDayLineNeedUpdate; }
  void SetDayLineNeedUpdate(bool fFlag) noexcept { m_fDayLineNeedUpdate = fFlag; }
  bool IsDayLineNeedSaving(void) noexcept { return m_fDayLineNeedSaving; }
  void SetDayLineNeedSaving(bool fFlag) noexcept { m_fDayLineNeedSaving = fFlag; }
  bool IsDayLineNeedSavingAndClearFlag(void);
  long GetIPOStatus(void) noexcept { return m_lIPOStatus; }
  void SetIPOStatus(long lValue) noexcept { m_lIPOStatus = lValue; }

  void SetCheckingDayLineStatus(void);

  CString GetFinnhubDayLineInquiryString(time_t tCurrentTime);

  void UpdateDayLine(vector<CDayLinePtr>& vDayLine);
  void UnloadDayLine(void) { m_vDayLine.resize(0); }
  size_t GetDayLineSize(void) { return m_vDayLine.size(); }
  void UpdateDayLineStartEndDate(void);
  bool HaveNewDayLineData(void);

public:
  CString m_strDescription;
  CString m_strDisplaySymbol;
  CString m_strExchange;
  CString m_strSymbol;
  long m_lDayLineStartDate;
  long m_lDayLineEndDate;
  long m_lIPOStatus;

  vector<CDayLinePtr> m_vDayLine;

  bool m_fUpdateDatabase; // 要求更新此数据
  bool m_fDayLineNeedUpdate;
  atomic_bool m_fDayLineNeedSaving; // 日线历史数据已处理，等待存储。
};

typedef shared_ptr<CFinnhubForexSymbol> CForexSymbolPtr;
