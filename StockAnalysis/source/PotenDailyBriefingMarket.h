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

  bool SchedulingTaskPerSecond(long lSecond);
  void Reset(void);

  bool LoadDatabase(void);
  bool SaveDatabase(void);

  bool UpdateStatus(void);

  void SetNextInquiringDay(void);

  bool ProcessData(void);

  bool IsDatabaseLoaded(void) noexcept { return m_fDataBaseLoaded; }
  void SetDatabaseLoaded(bool fFlag) noexcept { m_fDataBaseLoaded = fFlag; }
  long GetNewestUpdateDay(void) noexcept { return m_lNewestUpdatedDay; }
  void SetNewestUpdateDay(long lDay) noexcept { m_lNewestUpdatedDay = lDay; }

protected:
  bool m_fDataBaseLoaded;
  bool m_fTodayDataUupdated;
  vector<CPotenDailyBriefingPtr> m_vPotenDailyBriefing;
  map<long, bool> m_mapDataLoadedDays;
  long m_lNewestUpdatedDay;
  long m_lNewestDatabaseDay;
};