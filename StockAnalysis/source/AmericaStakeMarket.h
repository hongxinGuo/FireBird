#pragma once

#include"stdafx.h"

#include"VirtualMarket.h"

using namespace std;
#include<map>
#include<vector>

class CAmericaStakeMarket : public CVirtualMarket {
public:
  CAmericaStakeMarket();
  virtual ~CAmericaStakeMarket();

  virtual bool SchedulingTask(void) override; // 由程序的定时器调度，大约每100毫秒一次
  void GetFinnHubDataFromWeb(void);
  virtual void ResetMarket(void) override;
  void Reset(void);

  bool SchedulingTaskPerSecond(long lSecond, long lCurrentTime);
  bool SchedulingTaskPer1Minute(long lSecond, long lCurrentTime);
  bool SchedulingTaskPer1Hour(long lSecond, long lCurrentTime);
  bool TaskResetMarket(long lCurrentTime);

protected:
  vector<CString> m_vFinnHubInquiringStr;
  long m_lPrefixIndex; // 当前查询状态

  bool m_fSymbolUpdated; // 每日更新公司代码库
};

typedef shared_ptr<CAmericaStakeMarket> CAmericaStakeMarketPtr;
