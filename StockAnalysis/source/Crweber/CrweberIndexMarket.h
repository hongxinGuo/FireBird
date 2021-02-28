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
  virtual bool SchedulingTask(void) override final; // 由程序的定时器调度，大约每100毫秒一次
  virtual void ResetMarket(void) override;
  void Reset(void);

  bool SchedulingTaskPerSecond(long lSecond, long lCurrentTime);
  bool SchedulingTaskPer1Minute(long lCurrentTime);
  bool SchedulingTaskPer1Hour(long lCurrentTime);
  bool TaskResetMarket(long lCurrentTime);
  bool TaskProcessWebRTDataGetFromCrweberdotcom(void);

  virtual bool RunningThreadMaintainDB(void);
  virtual bool RunningThreadSaveCrweberDB(CCrweberIndexPtr pCrweberIndex);

  virtual bool LoadDatabase(void);
  virtual bool SaveDatabase(void);
  bool GetNewestDatabaseDateFromDB(void);
  virtual bool SaveCrweberIndexData(CCrweberIndexPtr pCrweberIndex);

  bool IsDatabaseLoaded(void) noexcept { return m_fDataBaseLoaded; }
  void SetDatabaseLoaded(bool fFlag) noexcept { m_fDataBaseLoaded = fFlag; }
  long GetNewestUpdateDate(void) noexcept { return m_lNewestUPdateDate; }
  void SetNewestUpdateDate(long lDate) noexcept { m_lNewestUPdateDate = lDate; }
  long GetNewestDatabaseDate(void) noexcept { return m_lNewestDatabaseDate; }

  bool IsMaintainDatabase(void) noexcept { return m_fMaintainDatabase; }
  void SetMaintainDatabase(bool fFlag) noexcept { m_fMaintainDatabase = fFlag; }

protected:
  vector<CCrweberIndexPtr> m_vCrweberIndex; // crweber.com网站上的油运指数

  CCrweberIndex m_CrweberIndex;
  CCrweberIndex m_CrweberIndexLast;

  bool m_fDataBaseLoaded;
  bool m_fTodayDataUpdated;
  bool m_fMaintainDatabase;
  long m_lNewestUPdateDate;
  long m_lNewestDatabaseDate;
};

typedef shared_ptr<CCrweberIndexMarket> CCrweberIndexMarketPtr;
