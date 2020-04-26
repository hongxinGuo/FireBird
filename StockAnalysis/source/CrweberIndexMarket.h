#pragma once

#include"stdafx.h"

#include"CrweberIndex.h"
#include"VirtualMarket.h"

using namespace std;
#include<map>

class CCrweberIndexMarket : public CVirtualMarket {
public:
  CCrweberIndexMarket();
  virtual ~CCrweberIndexMarket();
  virtual bool SchedulingTask(void) override; // 由程序的定时器调度，大约每100毫秒一次
  virtual void ResetMarket(void) override;
  void Reset(void);

  bool SchedulingTaskPerSecond(long lSecond, long lCurrentTime);
  bool SchedulingTaskPer1Minute(long lSecond, long lCurrentTime);
  bool SchedulingTaskPer1Hour(long lSecond, long lCurrentTime);
  bool TaskResetMarket(long lCurrentTime);
  bool TaskProcessWebRTDataGetFromCrweberdotcom(void);

  virtual bool RunningMaintainDB(void);
  virtual bool RunningSaveCrweberDB(CCrweberIndexPtr pCrweberIndex);

  bool LoadDatabase(void);
  bool SaveDatabase(void);
  bool GetNewestDatabaseDayFromDB(void);
  bool SaveCrweberIndexData(CCrweberIndexPtr pCrweberIndex);

  bool IsDatabaseLoaded(void) noexcept { return m_fDataBaseLoaded; }
  void SetDatabaseLoaded(bool fFlag) noexcept { m_fDataBaseLoaded = fFlag; }
  long GetNewestUpdateDay(void) noexcept { return m_lNewestUpdatedDay; }
  void SetNewestUpdateDay(long lDay) noexcept { m_lNewestUpdatedDay = lDay; }
  long GetNewestDatabaseDay(void) noexcept { return m_lNewestDatabaseDay; }

  bool IsMaintainDatabase(void) noexcept { return m_fMaintainDatabase; }
  void SetMaintainDatabase(bool fFlag) noexcept { m_fMaintainDatabase = fFlag; }

protected:
  vector<CCrweberIndexPtr> m_vCrweberIndex; // crweber.com网站上的油运指数

  CCrweberIndex m_CrweberIndex;
  CCrweberIndex m_CrweberIndexLast;

  bool m_fDataBaseLoaded;
  bool m_fTodayDataUpdated;
  bool m_fMaintainDatabase;
  long m_lNewestUpdatedDay;
  long m_lNewestDatabaseDay;
};

typedef shared_ptr<CCrweberIndexMarket> CCrweberIndexMarketPtr;
