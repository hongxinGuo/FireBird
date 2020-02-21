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
  bool SchedulingTaskPer10Second(long lSecond, long lCurrentTime);
  bool SchedulingTaskPerMinute(long lSecond, long lCurrentTime);
  bool TaskResetMarket(long lCurrentTime);
  void Reset(void);

  bool LoadDatabase(void);
  bool SaveCurrentData(void);

  void ChoiceNextInquiringDay(void);
  bool ProcessData(void);
  bool CheckTodayDataUpdated(void);
  bool IsTodayDataUpdated(void) noexcept { return m_fTodayDataUpdated; }

  bool IsDatabaseLoaded(void) noexcept { return m_fDataBaseLoaded; }
  void SetDatabaseLoaded(bool fFlag) noexcept { m_fDataBaseLoaded = fFlag; }
  long GetCurrentInquiringDay(void) noexcept { return m_lCurrentInquiringDay; }
  void SetCurrentInquiringDay(long lDay) noexcept { m_lCurrentInquiringDay = lDay; }

  long GetDatabaseSize(void) noexcept { return m_vPotenDailyBriefing.size(); }
  void ClearDatabase(void) noexcept { m_vPotenDailyBriefing.resize(0); }

public:
  void __TEST_SetLoadedDay(long lDay, bool fFlag) noexcept { m_mapDataLoadedDays[lDay] = fFlag; }

protected:
  CPotenDailyBriefingPtr m_pDataToSaved;
  bool m_fDataBaseLoaded;
  bool m_fTodayDataUpdated;
  vector<CPotenDailyBriefingPtr> m_vPotenDailyBriefing;
  map<long, bool> m_mapDataLoadedDays;
  long m_lCurrentInquiringDay;
};