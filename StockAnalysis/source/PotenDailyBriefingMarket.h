#pragma once

#include"stdafx.h"

#include"VirtualMarket.h"

#include"PotenDailyBriefing.h"

using namespace std;
#include<vector>
#include<map>

class CPotenDailyBriefingMarket final : public CVirtualMarket {
public:
  CPotenDailyBriefingMarket(void);
  ~CPotenDailyBriefingMarket(void);

  virtual bool SchedulingTask(void) override; // 由程序的定时器调度，大约每100毫秒一次
  virtual void ResetMarket(void) override;

  bool SchedulingTaskPerSecond(long lSecond, long lCurrentTime);
  bool TaskResetMarket(long lCurrentTime);
  void Reset(void);

  bool LoadDatabase(void);
  bool SaveDatabase(void);

  bool UpdateStatus(void);

  void SetNextInquiringDay(void);

  bool ProcessData(void);

  bool IsDatabaseLoaded(void) noexcept { return m_fDataBaseLoaded; }
  void SetDatabaseLoaded(bool fFlag) noexcept { m_fDataBaseLoaded = fFlag; }
  long GetCurrentInquiringDay(void) noexcept { return m_lCurrentInquiringDay; }
  void SetCurrentInquiringDay(long lDay) noexcept { m_lCurrentInquiringDay = lDay; }
  long GetNewestDatabaseDay(void) noexcept { return m_lNewestDatabaseDay; }
  void SetNewestDatabaseDay(long lDay) noexcept { m_lNewestDatabaseDay = lDay; }

  long GetDatabaseSize(void) noexcept { return m_vPotenDailyBriefing.size(); }
  void ClearDatabase(void) noexcept { m_vPotenDailyBriefing.resize(0); }

protected:
  bool m_fDataBaseLoaded;
  bool m_fTodayDataUpdated;
  vector<CPotenDailyBriefingPtr> m_vPotenDailyBriefing;
  map<long, bool> m_mapDataLoadedDays;
  long m_lCurrentInquiringDay;
  long m_lNewestDatabaseDay;
};