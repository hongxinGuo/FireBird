#pragma once

#include"stdafx.h"

#include"VirtualMarket.h"

// FinnHub申请类别，其值作为优先级的判断标准（数值大的优先级高）
enum {
  __COMPANY_PROFILE2__ = 101,
  __COMPANY_SYMBOLS__ = 102,
  __MARKET_NEWS__ = 103,
  __COMPANY_NEWS__ = 104,
  __NEWS_SETIMENTS__ = 105,
  __PEERS__ = 106,
  __BASIC_FINANCIALS__ = 107,

  __QUOTE__ = 1, // 实时数据优先级最低
  __CANDLES__ = 2, // 历史数据优先级低

  __FOREX_EXCHANGE__ = 201,
  __FOREX_SYMBOLS__ = 202,
  __FOREX_CANDLES__ = 3, // 历史数据优先级低
  __FOREX_ALL_RATES__ = 204,
};

using namespace std;
#include<map>
#include<vector>
#include<queue>
#include<atomic>

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

  bool TaskUpdateTodaySymbol(void);

  // 各种状态
  long GetCurrentPrefixIndex(void) noexcept { return m_lPrefixIndex; }

  void SetWaitingFinnHubData(bool fFlag) noexcept { m_fWaitingFinnHubData = fFlag; }
  bool IsWaitingFinHubData(void) noexcept { bool f = m_fWaitingFinnHubData; return f; }

protected:
  vector<CString> m_vFinnHubInquiringStr;
  long m_lPrefixIndex; // 当前查询状态
  priority_queue<int> m_qWebInquiry; // 网络数据查询命令队列(有优先级）
  atomic_bool m_fWaitingFinnHubData;

  bool m_fSymbolUpdated; // 每日更新公司代码库
};

typedef shared_ptr<CAmericaStakeMarket> CAmericaStakeMarketPtr;
