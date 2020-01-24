#pragma once

#include"stdafx.h"

#include"CrweberIndex.h"
#include"VirtualMarket.h"

using namespace std;
#include<map>

class CCrweberIndexMarket final : public CVirtualMarket {
public:
  CCrweberIndexMarket();
  ~CCrweberIndexMarket();
  virtual bool SchedulingTask(void) override; // 由程序的定时器调度，大约每100毫秒一次
  virtual void ResetMarket(void) override;
  void Reset(void);

  bool SchedulingTaskPer5Minute(long lSecond, long lCurrentTime);
  bool TaskProcessWebRTDataGetFromCrweberdotcom(void);

  bool LoadDatabase(void);
  bool SaveDatabase(void);
  bool SaveCrweberIndexData(CCrweberIndexPtr pCrweberIndex);

  bool IsDatabaseLoaded(void) noexcept { return m_fDataBaseLoaded; }
  void SetDatabaseLoaded(bool fFlag) noexcept { m_fDataBaseLoaded = fFlag; }
  long GetNewestUpdateDay(void) noexcept { return m_lNewestUpdatedDay; }
  void SetNewestUpdateDay(long lDay) noexcept { m_lNewestUpdatedDay = lDay; }

protected:
  vector<CCrweberIndexPtr> m_vCrweberIndex; // crweber.com网站上的油运指数

  CCrweberIndex m_CrweberIndex;
  CCrweberIndex m_CrweberIndexLast;

  bool m_fDataBaseLoaded;
  bool m_fTodayDataUupdated;
  long m_lNewestUpdatedDay;
  long m_lNewestDatabaseDay;
};
