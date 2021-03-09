#pragma once
#include"Semaphore.h"

#include"VirtualMarket.h"
#include"WorldStock.h"
#include"WebData.h"
#include"FinnhubExchange.h"
#include"FinnhubForexSymbol.h"
#include"Country.h"
#include"EconomicCalendar.h"

#include"QuandlWebInquiry.h"
#include"TiingoWebInquiry.h"

using namespace MyLib;

class CFinnhub;

// Finnhub�������ʹ��룬����˻��޷�����Premium�����Ϣ
enum {
  __WEBSOCKET__TRADES__ = 1,
  __WEBSOCKET__NEWS__ = 2, // Premium

  __COMPANY_PROFILE__ = 100, //Premium
  __COMPANY_PROFILE_CONCISE__ = 101,
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

struct WebInquiry {
public:
  void Reset(void) {
    m_iPriority = 0;
    m_lInquiryIndex = 0;
    m_lStockIndex = 0;
  }
public:
  int m_iPriority; // ���ȼ�
  long m_lInquiryIndex; // ָ������
  long m_lStockIndex; // ��Ʊ����ǰλ��
  bool operator() (WebInquiry temp1, WebInquiry temp2) {
    return temp1.m_iPriority < temp2.m_iPriority; // ���ȼ����λ��ǰ��
  }
};

extern Semaphore gl_UpdateWorldMarketDB;  // ���ź�����������������ʷ���ݿ�

using namespace std;
#include<map>
#include<vector>
#include<queue>
#include<atomic>

class CWorldMarket : public CVirtualMarket {
  friend class CFinnhub;
public:
  CWorldMarket();
  virtual ~CWorldMarket();
  void InitialFinnhubInquiryStr(void);
  void InitialTiingoInquiryStr(void);

  virtual bool SchedulingTask(void) override final; // �ɳ���Ķ�ʱ�����ȣ���Լÿ100����һ��
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
  bool SchedulingTaskPerMinute(long lCurrentTime);
  bool SchedulingTaskPer5Minute(long lCurrentTime);
  bool SchedulingTaskPerHour(long lCurrentTime);
  bool TaskResetMarket(long lCurrentTime);

  bool TaskInquiryFinnhub(long lCurrentTime); // ���������Ϊ���������еĲ�ѯFinnhub�����λ�ڴˡ�
  bool TaskInquiryFinnhubCountryList(void);
  bool TaskInquiryFinnhubCompanySymbol(void);
  bool TaskInquiryFinnhubCompanyProfileConcise(void);
  bool TaskInquiryFinnhubDayLine(void);
  bool TaskInquiryFinnhubRTQuote(void);
  bool TaskInquiryFinnhubPeer(void);
  bool TaskInquiryFinnhubEconomicCalender(void);
  bool TaskInquiryFinnhubEPSSurprise(void);
  bool TaskInquiryFinnhubForexExchange(void);
  bool TaskInquiryFinnhubForexSymbol(void);
  bool TaskInquiryFinnhubForexDayLine(void);

  void TaskInquiryTiingo(void);
  bool TaskInquiryTiingoCompanySymbol(void);
  bool TaskInquiryTiingoDayLine(void);

  bool TaskUpdateStockProfileDB(void);
  bool TaskUpdateDayLineDB(void);
  bool TaskUpdateForexExchangeDB(void);
  bool TaskUpdateForexSymbolDB(void);
  bool TaskUpdateForexDayLineDB(void);
  bool TaskUpdateCountryListDB(void);
  bool TaskUpdateEPSSurpriseDB(void);
  bool TaskUpdateEconomicCalendar(void);
  bool TaskCheckSystemReady(void);

  bool TaskUpdateDayLineStartEndDate(void);

  // �������̵߳��ð�������
  virtual bool RunningthreadUpdateDayLneStartEndDate(CWorldMarket* pMarket);
  virtual bool RunningThreadUpdateDayLineDB();
  virtual bool RunningThreadUpdateStockProfileDB(void);
  virtual bool RunningThreadUpdateForexDayLineDB(CFinnhubForexSymbol* pSymbol);
  virtual bool RunningThreadUpdateForexSymbolDB(void);
  virtual bool RunningThreadUpdateCountryListDB(void);
  virtual bool RunningThreadUpdateEPSSurpriseDB(CWorldStock* pStock);

  bool UpdateEconomicCalendar(vector<CEconomicCalendarPtr> vEconomicCalendar);

  // ����״̬
  long GetCurrentPrefixIndex(void) noexcept { return m_CurrentFinnhubInquiry.m_lInquiryIndex; }

  bool IsFinnhubInquiring(void) noexcept { return m_fFinnhubInquiring; }
  void SetFinnhubInquiring(bool fFlag) noexcept { m_fFinnhubInquiring = fFlag; }
  void SetFinnhubDataReceived(bool fFlag) noexcept { m_fFinnhubDataReceived = fFlag; }
  bool IsFinnhubDataReceived(void) noexcept { bool f = m_fFinnhubDataReceived; return f; }
  void SetQuandlInquiring(bool fFlag) noexcept { m_fQuandlInquiring = fFlag; }
  void SetQuandlDataReceived(bool fFlag) noexcept { m_fQuandlDataReceived = fFlag; }
  bool IsQuandlDataReceived(void) noexcept { bool f = m_fQuandlDataReceived; return f; }
  bool IsTiingoInquiring(void) noexcept { return m_fTiingoInquiring; }
  void SetTiingoInquiring(bool fFlag) noexcept { m_fTiingoInquiring = fFlag; }
  void SetTiingoDataReceived(bool fFlag) noexcept { m_fTiingoDataReceived = fFlag; }
  bool IsTiingoDataReceived(void) noexcept { bool f = m_fTiingoDataReceived; return f; }

  CFinnhubExchangePtr GetExchange(long lIndex) { return m_vFinnhubExchange.at(lIndex); }
  size_t GetExchangeSize(void) noexcept { return m_mapFinnhubExchange.size(); }

  bool IsStockProfileNeedUpdate(void);
  void AddStock(CWorldStockPtr pStock);
  bool DeleteStock(CWorldStockPtr pStock);
  size_t GetTotalStock(void) noexcept { return m_vWorldStock.size(); }
  bool IsStock(CString strSymbol) { if (m_mapWorldStock.find(strSymbol) == m_mapWorldStock.end()) return false; else return true; }
  bool IsStock(CWorldStockPtr pStock) { return IsStock(pStock->GetSymbol()); }
  CWorldStockPtr GetStock(long lIndex) { return m_vWorldStock.at(lIndex); }
  CWorldStockPtr GetStock(CString strSymbol) { return m_vWorldStock.at(m_mapWorldStock.at(strSymbol)); }
  long GetStockIndex(CString strSymbol) { return m_mapWorldStock.at(strSymbol); }

  CWorldStockPtr GetChoicedStock(long lIndex) { return m_vWorldChoicedStock.at(lIndex); }

  bool IsForexExchange(CString strExchange) { if (m_mapForexExchange.find(strExchange) == m_mapForexExchange.end()) return false; else return true; }
  void AddForexExchange(CString strForexExchange);
  bool DeleteForexExchange(CString strForexExchange);
  size_t GetForexExchangeSize(void) noexcept { return m_vForexExchange.size(); }

  bool IsForexSymbol(CString strSymbol) { if (m_mapForexSymbol.find(strSymbol) == m_mapForexSymbol.end()) return false; else return true; }
  bool IsForexSymbol(CForexSymbolPtr pForexSymbol) { return IsForexSymbol(pForexSymbol->m_strSymbol); }
  void AddForexSymbol(CForexSymbolPtr pForexSymbol);
  bool DeleteForexSymbol(CForexSymbolPtr pForexSysbol);
  CForexSymbolPtr GetForexSymbol(long lIndex) { return m_vForexSymbol.at(lIndex); }
  size_t GetForexSymbolSize(void) noexcept { return m_vForexSymbol.size(); }

  size_t GetTotalCountry(void) noexcept { return m_vCountry.size(); }
  bool IsCountry(CString strCountry);
  bool IsCountry(CCountryPtr pCountry);
  void AddCountry(CCountryPtr pCountry);
  bool DeleteCountry(CCountryPtr pCountry);

  size_t GetFinnhubInquiryQueueSize(void) noexcept { return m_qFinnhubWebInquiry.size(); }
  void PushFinnhubInquiry(WebInquiry inquiry) noexcept { m_qFinnhubWebInquiry.push(inquiry); }
  WebInquiry GetFinnhubInquiry(void);

  bool IsCountryListUpdated(void) noexcept { return m_fCountryListUpdated; }
  void SetCountryListUpdated(bool fFlag) noexcept { m_fCountryListUpdated = fFlag; }
  bool IsFinnhubSymbolUpdated(void) noexcept { return m_fFinnhubSymbolUpdated; }
  void SetFinnhubSymbolUpdated(bool fFlag) noexcept { m_fFinnhubSymbolUpdated = fFlag; }
  bool IsFinnhubStockProfileUpdated(void) noexcept { return m_fFinnhubStockProfileUpdated; }
  void SetFinnhubStockProfileUpdated(bool fFlag) noexcept { m_fFinnhubStockProfileUpdated = fFlag; }
  bool IsFinnhubDayLineUpdated(void) noexcept { return m_fFinnhubDayLineUpdated; }
  void SetFinnhubDayLineUpdated(bool fFlag) noexcept { m_fFinnhubDayLineUpdated = fFlag; }
  bool IsFinnhubForexExchangeUpdated(void) noexcept { return m_fFinnhubForexExchangeUpdated; }
  void SetFinnhubForexExchangeUpdated(bool fFlag) noexcept { m_fFinnhubForexExchangeUpdated = fFlag; }
  bool IsFinnhubForexSymbolUpdated(void) noexcept { return m_fFinnhubForexSymbolUpdated; }
  void SetFinnhubForexSymbolUpdated(bool fFlag) noexcept { m_fFinnhubForexSymbolUpdated = fFlag; }
  bool IsFinnhubForexDayLineUpdated(void) noexcept { return m_fFinnhubForexDayLineUpdated; }
  void SetFinnhubForexDayLineUpdated(bool fFlag) noexcept { m_fFinnhubForexDayLineUpdated = fFlag; }
  bool IsFinnhubPeerUpdated(void) noexcept { return m_fFinnhubPeerUpdated; }
  void SetFinnhubPeerUpdated(bool fFlag) noexcept { m_fFinnhubPeerUpdated = fFlag; }
  bool IsFinnhubEconomicCalendarUpdated(void) noexcept { return m_fFinnhubEconomicCalendarUpdated; }
  void SetFinnhubEconomicCalendarUpdated(bool fFlag) noexcept { m_fFinnhubEconomicCalendarUpdated = fFlag; }
  bool IsFinnhubEPSSurpriseUpdated(void) noexcept { return m_fFinnhubEPSSurpriseUpdated; }
  void SetFinnhubEPSSurpriseUpdated(bool fFlag) noexcept { m_fFinnhubEPSSurpriseUpdated = fFlag; }

  bool IsTiingoSymbolUpdated(void) noexcept { return m_fTiingoSymbolUpdated; }
  void SetTiingoSymbolUpdated(bool fFlag) noexcept { m_fTiingoSymbolUpdated = fFlag; }
  bool IsTiingoDayLineUpdated(void) noexcept { return m_fTiingoDayLineUpdated; }
  void SetTiingoDayLineUpdated(bool fFlag) noexcept { m_fTiingoDayLineUpdated = fFlag; }

  size_t GetTiingoInquiryQueueSize(void) noexcept { return m_qTiingoWebInquiry.size(); }
  void PushTiingoInquiry(WebInquiry inquiry) noexcept { m_qTiingoWebInquiry.push(inquiry); }
  WebInquiry GetTiingoInquiry(void);

  // ���ݿ����
  bool LoadOption(void);
  bool LoadWorldExchangeDB(void); // װ�����罻������Ϣ
  bool LoadStockDB(void);
  bool LoadWorldChoicedStock(void);
  virtual bool UpdateCountryListDB(void);
  virtual bool UpdateStockProfileDB(void);
  virtual bool UpdateForexSymbolDB(void);
  bool UpdateEconomicCalendarDB(void);

  bool LoadForexExchange(void);
  bool LoadForexSymbol(void);
  bool LoadCountryList(void);
  bool LoadEconomicCalendarDB(void);

  bool RebuildStockDayLineDB(void);
  virtual bool UpdateDayLineStartEndDate(void);
  bool RebuildEPSSurprise(void);
  bool ReBuildPeer(void);

  bool SortStockVector(void);

protected:
  vector<CFinnhubExchangePtr> m_vFinnhubExchange;
  map<CString, long> m_mapFinnhubExchange;
  long m_lCurrentExchangePos;

  vector<CWorldStockPtr> m_vWorldStock;
  map<CString, long> m_mapWorldStock;
  long m_lLastTotalWorldStock;
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
  vector<CWorldStockPtr> m_vWorldChoicedStock;
  map<CString, long> m_mapWorldChoicedStock;
  long m_lChoicedStockPos;

  vector<CString> m_vFinnhubInquiringStr;
  priority_queue<WebInquiry, vector<WebInquiry>, WebInquiry> m_qFinnhubWebInquiry; // �������ݲ�ѯ�������(�����ȼ���
  bool m_fFinnhubInquiring;
  atomic_bool m_fFinnhubDataReceived;

  vector<CString> m_vTiingoInquiringStr;
  priority_queue<WebInquiry, vector<WebInquiry>, WebInquiry> m_qTiingoWebInquiry; // �������ݲ�ѯ�������(�����ȼ���
  bool m_fTiingoInquiring;
  atomic_bool m_fTiingoDataReceived;

  vector<CString> m_vQuandlInquiringStr;
  priority_queue<WebInquiry, vector<WebInquiry>, WebInquiry> m_qQuandlWebInquiry; // �������ݲ�ѯ�������(�����ȼ���
  bool m_fQuandlInquiring;
  atomic_bool m_fQuandlDataReceived;

  vector<CString> m_vForexExchange;
  map<CString, long> m_mapForexExchange;
  long m_lLastTotalForexExchange;

  vector<CForexSymbolPtr> m_vForexSymbol;
  map<CString, long> m_mapForexSymbol;
  long m_lLastTotalForexSymbol;
  long m_lCurrentUpdateForexDayLinePos;

  vector<CCountryPtr> m_vCountry;
  map<CString, long> m_mapCountry;
  bool m_fCountryListUpdated;
  long m_lLastTotalCountry;

  vector<CEconomicCalendarPtr> m_vEconomicCalendar;
  map<CString, long> m_mapEconomicCalendar;
  long m_lLastTotalEconomicCalendar;

  bool m_fFinnhubSymbolUpdated; // ÿ�ո��¹�˾�����
  bool m_fFinnhubStockProfileUpdated; // ÿ�ո��¹�˾���
  bool m_fFinnhubDayLineUpdated; // ÿ�ո��¹�˾��������
  bool m_fFinnhubForexExchangeUpdated; // ÿ�ո���Forex������
  bool m_fFinnhubForexSymbolUpdated; // ÿ�ո���Forex����������
  bool m_fFinnhubForexDayLineUpdated; // ÿ�ո���Forex��������
  bool m_fFinnhubPeerUpdated; // ÿ�¸���Peers����
  bool m_fFinnhubEconomicCalendarUpdated; // ÿ�ո��¾�����������
  bool m_fFinnhubEPSSurpriseUpdated;

  bool m_fTiingoSymbolUpdated; // ÿ�ո��¹�˾�����
  bool m_fTiingoDayLineUpdated; // ÿ�ո��¹�˾��������
//
  bool m_fRebulidDayLine; // �ؽ�������ʷ���ݡ�
};

typedef shared_ptr<CWorldMarket> CWorldMarketPtr;
