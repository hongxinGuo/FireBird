#pragma once

#include"stdafx.h"

#include"CrweberIndex.h"
#include"VirtualMarket.h"

class CCrweberIndexMarket final : public CVirtualMarket {
public:
  CCrweberIndexMarket();
  ~CCrweberIndexMarket();
  virtual bool SchedulingTask(void) override; // 由程序的定时器调度，大约每100毫秒一次
  virtual void ResetMarket(void) override { Reset(); }
  void Reset(void) {}

protected:
  bool m_fDataBaseLoaded;
  bool m_fTodayDataUupdated;
  vector<CCrweberIndexPtr> m_vPotenDailyBriefing;
  map<long, bool> m_mapDataLoadedDays;
  long m_lNewestUpdatedDay;
  long m_lNewestDatabaseDay;
};
