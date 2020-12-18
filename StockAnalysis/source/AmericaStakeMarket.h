#pragma once

#include"stdafx.h"

#include"VirtualMarket.h"
#include"AmericaStake.h"
#include"WebData.h"
#include"ForexSymbol.h"
#include"Country.h"

class CFinnhub;

// Finnhub�������ʹ��룬����˻��޷�����Premium�����Ϣ
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

  __STOCK_QUOTE__ = 300, // ʵʱ�������ȼ����
  __STOCK_CANDLES__ = 301, // ��ʷ�������ȼ���
  __STOCK_TICK_DATA__ = 302, //Premium
  __STOCK_LAST_BID_ASK__ = 303, //Premium
  __STOCK_SPLITS__ = 304,

  __FOREX_EXCHANGE__ = 501,
  __FOREX_SYMBOLS__ = 502,
  __FOREX_CANDLES__ = 503, // ��ʷ�������ȼ���
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

struct FinnhubInquiry {
public:
  int m_iPriority; // ���ȼ�
  long m_lInquiryIndex; // ָ������
  long m_lStakeIndex; // ��Ʊ����ǰλ��
  bool operator() (FinnhubInquiry temp1, FinnhubInquiry temp2) {
    return temp1.m_iPriority < temp2.m_iPriority; // ���ȼ����λ��ǰ��
  }
};

using namespace std;
#include<map>
#include<vector>
#include<queue>
#include<atomic>

class CAmericaStakeMarket : public CVirtualMarket {
  friend class CFinnhub;
public:
  CAmericaStakeMarket();
  virtual ~CAmericaStakeMarket();

  virtual bool SchedulingTask(void) override; // �ɳ���Ķ�ʱ�����ȣ���Լÿ100����һ��
  bool ProcessFinnhubInquiringMessage(void);
  bool ProcessFinnhubWebDataReceived(void);
  virtual void ResetMarket(void) override;
  void Reset(void);

  bool SchedulingTaskPerSecond(long lSecond, long lCurrentTime);
  bool SchedulingTaskPer10Seconds(long lSecond, long lCurrentTime);
  bool SchedulingTaskPer1Minute(long lSecond, long lCurrentTime);
  bool SchedulingTaskPer1Hour(long lSecond, long lCurrentTime);
  bool TaskResetMarket(long lCurrentTime);

  bool TaskInquiryFinnhub(long lCurrentTime); // ���������Ϊ���������еĲ�ѯFinnhub�����λ�ڴˡ�
  bool TaskInquiryCountryList(void);
  bool TaskInquiryTodaySymbol(void);
  bool TaskInquiryAmericaStake(void);
  bool TaskInquiryDayLine(void);
  bool TaskInquiryFinnhubRTQuote(void);

  bool TaskInquiryFinnhubForexExchange(void);
  bool TaskInquiryFinnhubForexSymbol(void);
  bool TaskInquiryFinnhubForexDayLine(void);

  bool TaskSaveStakeSymbolDB(void);
  bool TaskUpdateStakeDB(void);
  bool TaskUpdateDayLineDB(void);
  bool TaskUpdateForexExchangeDB(void);
  bool TaskUpdateForexSymbolDB(void);
  bool TaskUpdateForexDayLineDB(void);
  bool TaskUpdateCountryListDB(void);
  bool TaskCheckSystemReady(void);

  // �������̵߳��ð�������
  virtual bool RunningThreadUpdateDayLineDB(CAmericaStakePtr pStake);
  virtual bool RunningTaskThreadUpdateStakeDB(void);
  virtual bool RunningThreadUpdateForexDayLineDB(CForexSymbolPtr pSymbol);
  virtual bool RunningThreadUpdateForexSymbolDB(void);
  virtual bool RunningThreadUpdateCountryListDB(void);

  bool IsAmericaStake(CString strProfile);
  bool IsAmericaStakeUpdated(void);
  CAmericaStakePtr GetAmericaStake(CString strTicker);
  void AddAmericaStake(CAmericaStakePtr pStake);

  // ����״̬
  long GetCurrentPrefixIndex(void) noexcept { return m_CurrentFinnhubInquiry.m_lInquiryIndex; }

  void SetFinnhubInquiring(bool fFlag) noexcept { m_fFinnhubInquiring = fFlag; }
  void SetFinnhubDataReceived(bool fFlag) noexcept { m_fFinnhubDataReceived = fFlag; }
  bool IsFinnhubDataReceived(void) noexcept { bool f = m_fFinnhubDataReceived; return f; }

  long GetFinnInquiry(void);

  // ���ݿ����
  bool LoadAmericaStake(void);
  bool SaveCompnayProfile(void);
  bool UpdateCountryListDB(void);
  bool UpdateStakeDB(void);
  bool UpdateForexSymbolDB(void);
  bool RebulidFinnhubDayLine(void);
  bool SortStakeTable(void);

  bool LoadForexExchange(void);
  bool LoadForexSymbol(void);
  bool LoadCountryList(void);

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
  FinnhubInquiry m_CurrentFinnhubInquiry;

  vector<CString> m_vFinnhubInquiringStr;
  priority_queue<FinnhubInquiry, vector<FinnhubInquiry>, FinnhubInquiry> m_qWebInquiry; // �������ݲ�ѯ�������(�����ȼ���
  bool m_fFinnhubInquiring;
  atomic_bool m_fFinnhubDataReceived;

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
  map<CCountryPtr, long> m_mapCountry;
  bool m_fCountryListUpdated;
  long m_lLastTotalCountry;
  long m_lTotalCountry;

  bool m_fSymbolUpdated; // ÿ�ո��¹�˾�����
  bool m_fAmericaStakeUpdated; // ÿ�ո��¹�˾���
  bool m_fStakeDayLineUpdated; // ÿ�ո��¹�˾��������
  bool m_fForexExhangeUpdated; // ÿ�ո���Forex������
  bool m_fForexSymbolUpdated; // ÿ�ո���Forex����������
  bool m_fForexDayLineUpdated; // ÿ�ո���Forex��������
//
  bool m_fRebulidDayLine; // �ؽ�������ʷ���ݡ�
};

typedef shared_ptr<CAmericaStakeMarket> CAmericaStakeMarketPtr;