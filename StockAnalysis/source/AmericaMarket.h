#pragma once

#include"stdafx.h"
#include"Semaphore.h"

#include"VirtualMarket.h"
#include"AmericaStake.h"
#include"WebData.h"
#include"ForexSymbol.h"
#include"Country.h"
#include"EconomicCalendar.h"

#include"QuandlWebInquiry.h"
#include"TiingoWebInquiry.h"

using namespace MyLib;

class CFinnhub;

// Finnhub申请类别和代码，免费账户无法申请Premium类的信息
enum {
  __WEBSOCKET__TRADES__ = 1,
  __WEBSOCKET__NEWS__ = 2, // Premium

  __COMPANY_PROFILE__ = 100, //Premium
  __COMPANY_PROFILE2__ = 101,
  __COMPANY_SYMBOLS__ = 102,
  __COMPANY_EXECTIVE__ = 103, //Premium
  __MARKET_NEWS__ = 104,
  __COMPANY_NEWS__ = 105,
  __PRESS_RELEASE__ = 106, //Premium
  __NEWS_SENTIMENT__ = 107,
  __PEERS__ = 108,
  __BASIC_FINANCIALS__ = 109,
  __OWNERSHIP__ = 110, //Premium
  __FUND_OWNERSHIP__ = 111, //Premium
  __FINANCIAL__ = 112, //Premium
  __FINAICIAL_AS_REPORT__ = 113,
  __SEC_FILINGS__ = 114,
  __INTERNATIONAL_FILINGS__ = 115, //Premium
  __SEC_SENTIMENT_ANALYSIS__ = 116, //Premium
  __SIMILARITY_INDEX__ = 117, //Premium
  __IPO_CALENDER__ = 118,
  __DIVIDENDS__ = 119, //Premium

  __STOCK_RECOMMENDATION_TRENDS__ = 200,
  __STOCK_PRICE_TARGET__ = 201,
  __STOCK_UPGRADE_DOWNGRADE__ = 202, // Primium
  __STOCK_REVENUE_EXTIMATES__ = 203, // Premium
  __STOCK_EPS_EXTIMATES__ = 204, // Primium
  __STOCK_EPS_SURPRISE__ = 205,
  __STOCK_EARNING_CALENDER__ = 206,

  __STOCK_QUOTE__ = 300, // 实时数据优先级最低
  __STOCK_CANDLES__ = 301, // 历史数据优先级低
  __STOCK_TICK_DATA__ = 302, //Premium
  __STOCK_LAST_BID_ASK__ = 303, //Premium
  __STOCK_SPLITS__ = 304,

  __FOREX_EXCHANGE__ = 501,
  __FOREX_SYMBOLS__ = 502,
  __FOREX_CANDLES__ = 503, // 历史数据优先级低
  __FOREX_ALL_RATES__ = 504,

  __CRYPTO_EXCHANGE__ = 601,
  __CRYPTO_SYMBOL__ = 602,
  __CRYPTO_CANDLES__ = 603,

  __PATTERN_RECOGNITION__ = 701,
  __SURPORT_RESISTANCE__ = 702,
  __AGGREGATE_INDICATORS__ = 703,
  __TECHNICAL_INDICATORS__ = 704,

  __TRANSCRIPT_LIST__ = 801, //Premium
  __TRANSCRIPT__ = 802, //Premium

  __ECONOMIC_COUNTRY_LIST__ = 901,
  __ECONOMIC_CALENDAR__ = 902,
  __ECONOMIC_CODES__ = 903, //Premium
  __ECONOMIC__ = 904, //Premium
};

struct WebInquiry {
public:
  void Reset(void) {
    m_iPriority = 0;
    m_lInquiryIndex = 0;
    m_lStakeIndex = 0;
  }
public:
  int m_iPriority; // 优先级
  long m_lInquiryIndex; // 指令索引
  long m_lStakeIndex; // 股票集当前位置
  bool operator() (WebInquiry temp1, WebInquiry temp2) {
    return temp1.m_iPriority < temp2.m_iPriority; // 优先级大的位于前列
  }
};

extern Semaphore gl_SaveAmericaOneStockDayLine;  // 此信号量用于生成日线历史数据库
extern Semaphore gl_SaveEPSSurprise;  // 此信号量用于生成日线历史数据库
extern Semaphore gl_SaveForexDayLine;  // 此信号量用于生成日线历史数据库

using namespace std;
#include<map>
#include<vector>
#include<queue>
#include<atomic>

class CAmericaMarket : public CVirtualMarket {
  friend class CFinnhub;
public:
  CAmericaMarket();
  void InitialFinnhubInquiryStr(void);
  void InitialTiingoInquiryStr(void);
  virtual ~CAmericaMarket();

  virtual bool SchedulingTask(void) override; // 由程序的定时器调度，大约每100毫秒一次
  bool ProcessFinnhubInquiringMessage(void);
  bool ProcessFinnhubWebDataReceived(void);
  bool ProcessTiingoInquiringMessage(void);
  bool ProcessTiingoWebDataReceived(void);
  virtual void ResetMarket(void) override;
  void Reset(void);
  void ResetFinnhub(void);
  void ResetQuandl(void);
  void ResetTiingo(void);

  bool SchedulingTaskPerSecond(long lSecond, long lCurrentTime);
  bool SchedulingTaskPer10Seconds(long lCurrentTime);
  bool SchedulingTaskPer1Minute(long lCurrentTime);
  bool SchedulingTaskPer1Hour(long lCurrentTime);
  bool TaskResetMarket(long lCurrentTime);

  bool TaskInquiryFinnhub(long lCurrentTime); // 这个函数做为总括，所有的查询Finnhub任务皆位于此。
  bool TaskInquiryFinnhubCountryList(void);
  bool TaskInquiryFinnhubCompanySymbol(void);
  bool TaskInquiryFinnhubCompanyProfile2(void);
  bool TaskInquiryFinnhubDayLine(void);
  bool TaskInquiryFinnhubRTQuote(void);
  bool TaskInquiryFinnhubPeer(void);
  bool TaskInquiryFinnhubEconomicCalender(void);
  bool TaskInquiryFinnhubEPSSurprise(void);
  bool TaskInquiryFinnhubForexExchange(void);
  bool TaskInquiryFinnhubForexSymbol(void);
  bool TaskInquiryFinnhubForexDayLine(void);

  bool TaskInquiryTiingoDayLine(void);

  bool TaskSaveStakeSymbolDB(void);
  bool TaskUpdateStakeDB(void);
  bool TaskUpdateDayLineDB(void);
  bool TaskUpdateForexExchangeDB(void);
  bool TaskUpdateForexSymbolDB(void);
  bool TaskUpdateForexDayLineDB(void);
  bool TaskUpdateCountryListDB(void);
  bool TaskUpdateEPSSurpriseDB(void);
  bool TaskUpdateEconomicCalendar(void);
  bool TaskCheckSystemReady(void);

  // 各工作线程调用包裹函数
  virtual bool RunningThreadUpdateDayLineDB(CAmericaStakePtr pStake);
  virtual bool RunningTaskThreadUpdateStakeDB(void);
  virtual bool RunningThreadUpdateForexDayLineDB(CForexSymbolPtr pSymbol);
  virtual bool RunningThreadUpdateForexSymbolDB(void);
  virtual bool RunningThreadUpdateCountryListDB(void);
  virtual bool RunningThreadUpdateEPSSurpriseDB(CAmericaStakePtr pStake);

  bool IsAmericaStake(CString strProfile);
  bool IsAmericaStakeUpdated(void);
  CAmericaStakePtr GetAmericaStake(CString strTicker);
  void AddAmericaStake(CAmericaStakePtr pStake);

  bool UpdateEconomicCalendar(vector<CEconomicCalendarPtr> vEconomicCalendar);

  // 各种状态
  long GetCurrentPrefixIndex(void) noexcept { return m_CurrentFinnhubInquiry.m_lInquiryIndex; }

  void SetFinnhubInquiring(bool fFlag) noexcept { m_fFinnhubInquiring = fFlag; }
  void SetFinnhubDataReceived(bool fFlag) noexcept { m_fFinnhubDataReceived = fFlag; }
  bool IsFinnhubDataReceived(void) noexcept { bool f = m_fFinnhubDataReceived; return f; }
  void SetQuandlInquiring(bool fFlag) noexcept { m_fQuandlInquiring = fFlag; }
  void SetQuandlDataReceived(bool fFlag) noexcept { m_fQuandlDataReceived = fFlag; }
  bool IsQuandlDataReceived(void) noexcept { bool f = m_fQuandlDataReceived; return f; }
  void SetTiingoInquiring(bool fFlag) noexcept { m_fTiingoInquiring = fFlag; }
  void SetTiingoDataReceived(bool fFlag) noexcept { m_fTiingoDataReceived = fFlag; }
  bool IsTiingoDataReceived(void) noexcept { bool f = m_fTiingoDataReceived; return f; }

  long GetFinnInquiry(void);

  // 数据库操作
  bool LoadAmericaStake(void);
  bool SaveCompnayProfile(void);
  bool UpdateCountryListDB(void);
  bool UpdateStakeDB(void);
  bool UpdateForexSymbolDB(void);
  bool UpdateEconomicCalendarDB(void);

  bool LoadForexExchange(void);
  bool LoadForexSymbol(void);
  bool LoadCountryList(void);
  bool LoadEconomicCalendarDB(void);

  bool DeleteStakeSymbolDB(void);
  void DeleteEPSSurpriseDB(void);

  bool RebulidFinnhubDayLine(void);
  bool RebuildEPSSurprise(void);
  bool SortStakeTable(void);

protected:
  vector<CAmericaStakePtr> m_vAmericaStake;
  map<CString, long> m_mapAmericaStake;
  long m_lLastTotalAmericaStake;
  long m_lTotalAmericaStake;
  long m_lCurrentProfilePos;
  long m_lCurrentUpdateDayLinePos;
  long m_lCurrentRTDataQuotePos;
  long m_lCurrentForexExchangePos;
  long m_lCurrentForexSymbolPos;
  long m_lCurrentUpdatePeerPos;
  long m_lCurrentUpdateEPSSurprisePos;
  WebInquiry m_CurrentFinnhubInquiry;
  WebInquiry m_CurrentTiingoInquiry;
  WebInquiry m_CurrentQuandlInquiry;

  vector<CString> m_vFinnhubInquiringStr;
  priority_queue<WebInquiry, vector<WebInquiry>, WebInquiry> m_qFinnhubWebInquiry; // 网络数据查询命令队列(有优先级）
  bool m_fFinnhubInquiring;
  atomic_bool m_fFinnhubDataReceived;

  vector<CString> m_vTiingoInquiringStr;
  priority_queue<WebInquiry, vector<WebInquiry>, WebInquiry> m_qTiingoWebInquiry; // 网络数据查询命令队列(有优先级）
  bool m_fTiingoInquiring;
  atomic_bool m_fTiingoDataReceived;

  vector<CString> m_vQuandlInquiringStr;
  priority_queue<WebInquiry, vector<WebInquiry>, WebInquiry> m_qQuandlWebInquiry; // 网络数据查询命令队列(有优先级）
  bool m_fQuandlInquiring;
  atomic_bool m_fQuandlDataReceived;

  vector<CString> m_vForexExchange;
  map<CString, long> m_mapForexExchange;
  long m_lLastTotalForexExchange;
  long m_lTotalForexExchange;

  vector<CForexSymbolPtr> m_vForexSymbol;
  map<CString, long> m_mapForexSymbol;
  long m_lLastTotalForexSymbol;
  long m_lTotalForexSymbol;
  long m_lCurrentUpdateForexDayLinePos;

  vector<CCountryPtr> m_vCountry;
  map<CString, long> m_mapCountry;
  bool m_fCountryListUpdated;
  long m_lLastTotalCountry;
  long m_lTotalCountry;

  vector<CEconomicCalendarPtr> m_vEconomicCalendar;
  map<CString, long> m_mapEconomicCalendar;
  long m_lLastTotalEconomicCalendar;
  long m_lTotalEconomicCalendar;

  bool m_fSymbolUpdated; // 每日更新公司代码库
  bool m_fAmericaStakeUpdated; // 每日更新公司简介
  bool m_fStakeDayLineUpdated; // 每日更新公司日线数据
  bool m_fForexExhangeUpdated; // 每日更新Forex交易所
  bool m_fForexSymbolUpdated; // 每日更新Forex交易所代码
  bool m_fForexDayLineUpdated; // 每日更新Forex日线数据
  bool m_fPeerUpdated; // 每月更新Peers数据
  bool m_fEconomicCalendarUpdated; // 每日更新经济日历数据
  bool m_fEPSSurpriseUpdated;
  //
  bool m_fRebulidDayLine; // 重建日线历史数据。
};

typedef shared_ptr<CAmericaMarket> CAmericaMarketPtr;
