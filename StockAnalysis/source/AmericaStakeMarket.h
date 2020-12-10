#pragma once

#include"stdafx.h"

#include"VirtualMarket.h"
#include"AmericaStake.h"
#include"WebData.h"

// Finnhub申请类别，其值作为优先级的判断标准（数值大的优先级高）
enum {
  __COMPANY_PROFILE__ = 100,
  __COMPANY_PROFILE2__ = 101,
  __COMPANY_SYMBOLS__ = 102,
  __MARKET_NEWS__ = 103,
  __COMPANY_NEWS__ = 104,
  __NEWS_SETIMENTS__ = 105,
  __PEERS__ = 106,
  __BASIC_FINANCIALS__ = 107,
  __SEC_FILINGS__ = 108,

  __STAKE_QUOTE__ = 1, // 实时数据优先级最低
  __STAKE_CANDLES__ = 2, // 历史数据优先级低

  __FOREX_EXCHANGE__ = 201,
  __FOREX_SYMBOLS__ = 202,
  __FOREX_CANDLES__ = 3, // 历史数据优先级低
  __FOREX_ALL_RATES__ = 204,

  __CRYPTO_EXCHANGE__ = 301,
  __CRYPTO_SYMBOL__ = 302,
  __CRYPTO_CANDLES__ = 303,
};

struct FinnhubInquiry {
public:
  int m_iPriority; // 优先级
  long m_iIndex; // 指令
  bool operator() (FinnhubInquiry temp1, FinnhubInquiry temp2) {
    return temp1.m_iPriority < temp2.m_iPriority; // 优先级大的位于前列
  }
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
  void GetFinnhubDataFromWeb(void);
  virtual void ResetMarket(void) override;
  void Reset(void);

  bool SchedulingTaskPerSecond(long lSecond, long lCurrentTime);
  bool SchedulingTaskPer10Seconds(long lSecond, long lCurrentTime);
  bool SchedulingTaskPer1Minute(long lSecond, long lCurrentTime);
  bool SchedulingTaskPer1Hour(long lSecond, long lCurrentTime);
  bool TaskResetMarket(long lCurrentTime);

  bool TaskUpdateTodaySymbol(void);
  bool TaskSaveStakeSymbolDB(void);

  bool TaskUpdateAmericaStake(void);
  bool TaskUpdateAmericaStakeDB(void);

  bool TaskUpdateDayLine(void);

  bool TaskSaveDayLineData(void);

  // 各工作线程调用包裹函数
  virtual bool RunningThreadSaveStakeDayLine(CAmericaStakePtr pStake);

  bool IsAmericaStake(CString strProfile);
  bool IsAmericaStakeUpdated(void);
  CAmericaStakePtr GetAmericaStake(CString strTicker);
  CAmericaStakePtr GetCurrentProcessingAmericaStake(void) { return m_vAmericaStake.at(m_lCurrentProfilePos); }
  void AddAmericaStake(CAmericaStakePtr pStake);

  // 各种状态
  long GetCurrentPrefixIndex(void) noexcept { return m_lPrefixIndex; }

  void SetWaitingFinnhubData(bool fFlag) noexcept { m_fWaitingFinnhubData = fFlag; }
  bool IsWaitingFinHubData(void) noexcept { bool f = m_fWaitingFinnhubData; return f; }

  void SetFinnInquiry(long lOrder);
  long GetFinnInquiry(void);

  // 数据库操作
  bool LoadAmericaStake(void);
  bool SaveCompnayProfile(void);

protected:
  vector<CAmericaStakePtr> m_vAmericaStake;
  map<CString, long> m_mapAmericaStake;
  long m_lLastTotalAmericaStake;
  long m_lTotalAmericaStake;
  long m_lCurrentProfilePos;
  long m_lCurrentUpdateDayLinePos;

  vector<CString> m_vFinnhubInquiringStr;
  long m_lPrefixIndex; // 当前查询状态
  priority_queue<FinnhubInquiry, vector<FinnhubInquiry>, FinnhubInquiry> m_qWebInquiry; // 网络数据查询命令队列(有优先级）
  atomic_bool m_fWaitingFinnhubData;

  bool m_fAmericaStakeUpdated; // 每日更新公司简介
  bool m_fStakeDayLineUpdated; // 每日更新公司简介
  bool m_fSymbolUpdated; // 每日更新公司代码库
  bool m_fSymbolProceeded;

  bool m_fInquiringStakeProfileData; // 查询公司简介中
  bool m_fInquiringStakeCandle;
};

typedef shared_ptr<CAmericaStakeMarket> CAmericaStakeMarketPtr;
