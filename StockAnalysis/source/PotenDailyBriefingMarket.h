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

  virtual bool SchedulingTask(void) override; // �ɳ���Ķ�ʱ�����ȣ���Լÿ100����һ��
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
  long GetNewestUpdateDay(void) noexcept { return m_lNewestUpdatedDay; }
  void SetNewestUpdateDay(long lDay) noexcept { m_lNewestUpdatedDay = lDay; }
  long GetNewestDatabaseDay(void) noexcept { return m_lNewestDatabaseDay; }
  void SetNewestDatabaseDay(long lDay) noexcept { m_lNewestDatabaseDay = lDay; }

  long GetDatabaseSize(void) noexcept { return m_vPotenDailyBriefing.size(); }
  void ClearDatabase(void) noexcept { m_vPotenDailyBriefing.resize(0); }

protected:
  bool m_fDataBaseLoaded;
  bool m_fTodayDataUpdated;
  vector<CPotenDailyBriefingPtr> m_vPotenDailyBriefing;
  map<long, bool> m_mapDataLoadedDays;
  long m_lNewestUpdatedDay;
  long m_lNewestDatabaseDay;
};