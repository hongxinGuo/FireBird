#pragma once

#include"stdafx.h"

#include"VirtualMarket.h"

#include"PotenDailyBriefing.h"

using namespace std;
#include<vector>

class CPotenDailyBriefingMarket : public CVirtualMarket {
public:
  CPotenDailyBriefingMarket(void);
  ~CPotenDailyBriefingMarket(void);
  virtual bool SchedulingTask(void) override; // �ɳ���Ķ�ʱ�����ȣ���Լÿ100����һ��
  bool SchedulingTaskPerSecond(long lSecond);

  bool LoadDatabase(void);

  bool IsDatabaseLoaded(void) noexcept { return m_fDataBaseLoaded; }
  void SetDatabaseLoaded(bool fFlag) noexcept { m_fDataBaseLoaded = fFlag; }
  long GetNewestUpdateDay(void) noexcept { return m_lNewestUpdatedDay; }
  void SetNewestUpdateDay(long lDay) noexcept { m_lNewestUpdatedDay = lDay; }

protected:
  bool m_fDataBaseLoaded;
  vector<CPotenDailyBriefingPtr> m_vPotenDailyBriefing;
  long m_lNewestUpdatedDay;
};