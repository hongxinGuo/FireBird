#pragma once
#include"Semaphore.h"

#include"VirtualMarket.h"
#include"WorldStock.h"
#include"WebData.h"
#include"FinnhubStockExchange.h"
#include"FinnhubForexSymbol.h"
#include"FinnhubCryptoSymbol.h"
#include"Country.h"
#include"EconomicCalendar.h"
#include"InsiderTransaction.h"

#include"TiingoStock.h"

#include"TiingoIndustry.h"
#include"SICIndustry.h"
#include"NaicsIndustry.h"

#include"QuandlWebInquiry.h"
#include"TiingoWebInquiry.h"

#include"FinnhubFactory.h"
#include"TiingoFactory.h"
#include"QuandlFactory.h"

#include"ProductWebData.h"
#include"ProductTiingoStockSymbol.h"
#include"ProductTiingoStockDayLine.h"

#include"WebSocketData.h"
#include"WebSocket.h"

#include <ixwebsocket/IXWebSocket.h>
using namespace ix;

using namespace MyLib;

// Finnhub申请类别和代码，免费账户无法申请Premium类的信息
enum {
	__WEBSOCKET__TRADES__ = 1,
	__WEBSOCKET__NEWS__ = 2, // Premium

	__STOCK_SYMBOLS__ = 101,
	__COMPANY_PROFILE__ = 102, //Premium
	__COMPANY_PROFILE_CONCISE__ = 103,
	__COMPANY_EXECTIVE__ = 104, //Premium
	__MARKET_NEWS__ = 105,
	__COMPANY_NEWS__ = 106,
	__PRESS_RELEASE__ = 107, //Premium
	__NEWS_SENTIMENT__ = 108,
	__PEERS__ = 109,
	__BASIC_FINANCIALS__ = 110,
	__OWNERSHIP__ = 111, //Premium
	__FUND_OWNERSHIP__ = 112, //Premium
	__INSIDER_TRANSACTION__ = 113,
	__FINANCIAL__ = 114, //Premium
	__FINAICIAL_AS_REPORTED__ = 115,
	__REVENUE_BREAKDOWN__ = 116,
	__SEC_FILINGS__ = 117,
	__INTERNATIONAL_FILINGS__ = 118, //Premium
	__SEC_SENTIMENT_ANALYSIS__ = 119, //Premium
	__SIMILARITY_INDEX__ = 120, //Premium
	__IPO_CALENDAR__ = 121,
	__DIVIDENDS__ = 122, //Premium

	__STOCK_ESTIMATES_RECOMMENDATION_TRENDS__ = 201,
	__STOCK_ESTIMATES_PRICE_TARGET__ = 202,
	__STOCK_ESTIMATES_UPGRADE_DOWNGRADE__ = 203, // Primium
	__STOCK_ESTIMATES_REVENUE_EXTIMATES__ = 204, // Premium
	__STOCK_ESTIMATES_EPS_EXTIMATES__ = 205, // Primium
	__STOCK_ESTIMATES_EPS_SURPRISE__ = 206,
	__STOCK_ESTIMATES_EARNING_CALENDAR__ = 207,

	__STOCK_PRICE_QUOTE__ = 301, // 实时数据优先级最低
	__STOCK_PRICE_CANDLES__ = 302, // 历史数据优先级低
	__STOCK_PRICE_TICK_DATA__ = 303, //Premium
	__STOCK_PRICE_HISTORICAL_NBBO__ = 304, // Premium
	__STOCK_PRICE_LAST_BID_ASK__ = 305, //Premium
	__STOCK_PRICE_SPLITS__ = 306, // Premium
	__STOCK_PRICE_DIVIDEND2__ = 307, // Premium

	__ETFS_INDICES_CONSTITUENTS__ = 401,
	__ETFS_HISTORY_CONSTITUENTS__ = 402, // Premium
	__ETFS_PROFILE__ = 403, // Premiun
	__ETFS_HOLDINGS__ = 404, // Premium
	__ETFS_SECTOR__ = 405, // Premium
	__ETFS_COUNTRY__ = 406, //Premium

	__MUTUAL_FUND_PROFILE__ = 501, // Premium
	__MUTUAL_FUND_HOLDINGS__ = 502, // Premium
	__MUTUAL_FUND_SECTOR__ = 503, // Premium
	__MUTUAL_FUND_COUNTRY__ = 504, // Premium

	__FOREX_EXCHANGE__ = 601,
	__FOREX_SYMBOLS__ = 602,
	__FOREX_CANDLES__ = 603, // 历史数据优先级低 Premium
	__FOREX_ALL_RATES__ = 604, // Premium

	__CRYPTO_EXCHANGE__ = 701,
	__CRYPTO_SYMBOLS__ = 702,
	__CRYPTO_PROFILE__ = 703, // Premium
	__CRYPTO_CANDLES__ = 704,

	__TECHNICAL_ANALYSIS_PATTERN_RECOGNITION__ = 801, // Premium
	__TECHNICAL_ANALYSIS_SURPORT_RESISTANCE__ = 802, // Premium
	__TECHNICAL_ANALYSIS_AGGREGATE_INDICATORS__ = 803, // Premium
	__TECHNICAL_ANALYSIS_TECHNICAL_INDICATORS__ = 804,

	__ALTERNATIVE_DATA_TRANSCRIPT_LIST__ = 901, //Premium
	__ALTERNATIVE_DATA_TRANSCRIPT__ = 902, //Premium
	__ALTERNATIVE_DATA_SOCIAL_SETIMENT__ = 903,
	__ALTERNATIVE_DATA_INVEST_THEMES__ = 904, // Premium
	__ALTERNATIVE_DATA_SUPPLY_CHAIN__ = 905, // Premium
	__ALTERNATIVE_DATA_COMPANY_ESG__ = 906, // Premium
	__ALTERNATIVE_DATA_EARNING_QUALITY_SCORE__ = 907, // Premium
	__ALTERNATIVE_DATA_COVID_19__ = 908,
	__ALTERNATIVE_DATA_FAD_CALENDAR__ = 909,

	__ECONOMIC_COUNTRY_LIST__ = 1001,
	__ECONOMIC_CALENDAR__ = 1002, //Premium
	__ECONOMIC_CODES__ = 1003, //Premium
	__ECONOMIC__ = 1004, //Premium
};

extern Semaphore gl_UpdateWorldMarketDB;  // 此信号量用于生成日线历史数据库

using namespace std;
#include<map>
#include<vector>
#include<queue>
#include<atomic>

class CWorldMarket : public CVirtualMarket {
public:
	DECLARE_DYNCREATE(CWorldMarket)
	CWorldMarket();
	virtual ~CWorldMarket();
	virtual void ResetMarket(void) override;

	virtual bool PreparingExitMarket(void) override final;

	void Reset(void);
	void ResetFinnhub(void);
	void ResetQuandl(void);
	void ResetTiingo(void);

	virtual bool SchedulingTask(void) override final; // 由程序的定时器调度，大约每100毫秒一次
	bool ProcessFinnhubInquiringMessage(void);
	bool ProcessFinnhubWebDataReceived(void);

	bool ProcessTiingoInquiringMessage(void);
	bool ProcessTiingoWebDataReceived(void);

	bool SchedulingTaskPerSecond(long lSecond, long lCurrentTime);
	bool SchedulingTaskPer10Seconds(long lCurrentTime);
	bool SchedulingTaskPerMinute(long lCurrentTime);
	bool SchedulingTaskPer5Minute(long lCurrentTime);
	bool SchedulingTaskPerHour(long lCurrentTime);
	bool TaskResetMarket(long lCurrentTime);

	bool TaskInquiryFinnhub(long lCurrentTime); // 这个函数做为总括，所有查询Finnhub的任务皆位于此函数内。
	virtual bool TaskInquiryFinnhubCountryList(void);
	virtual bool TaskInquiryFinnhubCompanySymbol(void);
	virtual bool TaskInquiryFinnhubCompanyProfileConcise(void);
	virtual bool TaskInquiryFinnhubDayLine(void);
	virtual bool TaskInquiryFinnhubRTQuote(void);
	virtual bool TaskInquiryFinnhubPeer(void);
	virtual bool TaskInquiryFinnhubInsiderTransaction(void);
	virtual bool TaskInquiryFinnhubEconomicCalendar(void);
	virtual bool TaskInquiryFinnhubEPSSurprise(void);
	virtual bool TaskInquiryFinnhubForexExchange(void);
	virtual bool TaskInquiryFinnhubForexSymbol(void);
	virtual bool TaskInquiryFinnhubForexDayLine(void);
	virtual bool TaskInquiryFinnhubCryptoExchange(void);
	virtual bool TaskInquiryFinnhubCryptoSymbol(void);
	virtual bool TaskInquiryFinnhubCryptoDayLine(void);

	bool TaskInquiryTiingo(void);// 这个函数做为总括，所有查询Tiingo的任务皆位于此函数内。
	virtual bool TaskInquiryTiingoCompanySymbol(void);
	virtual bool TaskInquiryTiingoDayLine(void);

	virtual bool TaskUpdateTiingoIndustry(void);
	virtual bool TaskUpdateSICIndustry(void);
	virtual bool TaskUpdateNaicsIndustry(void);

	bool TaskUpdateStockProfileDB(void);
	bool TaskUpdateDayLineDB(void);
	bool TaskUpdateForexExchangeDB(void);
	bool TaskUpdateForexSymbolDB(void);
	bool TaskUpdateForexDayLineDB(void);
	bool TaskUpdateCryptoExchangeDB(void);
	bool TaskUpdateCryptoSymbolDB(void);
	bool TaskUpdateCryptoDayLineDB(void);
	bool TaskUpdateCountryListDB(void);
	bool TaskUpdateEPSSurpriseDB(void);
	bool TaskUpdateEconomicCalendarDB(void);
	bool TaskUpdateInsiderTransactionDB(void);
	bool TaskUpdateTiingoStockDB(void);

	bool TaskCheckSystemReady(void);

	bool TaskUpdateDayLineStartEndDate(void);

	// 各工作线程调用包裹函数
	virtual bool CreatingthreadUpdateDayLneStartEndDate(void);
	virtual bool CreatingThreadUpdateDayLineDB(void);
	virtual bool CreatingThreadUpdateStockProfileDB(void);
	virtual bool CreatingThreadUpdateForexExchangeDB(void);
	virtual bool CreatingThreadUpdateForexSymbolDB(void);
	virtual bool CreatingThreadUpdateForexDayLineDB(CProductFinnhubForexSymbol* pSymbol);
	virtual bool CreatingThreadUpdateCryptoExchangeDB(void);
	virtual bool CreatingThreadUpdateCryptoSymbolDB(void);
	virtual bool CreatingThreadUpdateCryptoDayLineDB(CFinnhubCryptoSymbol* pSymbol);
	virtual bool CreatingThreadUpdateCountryListDB(void);
	virtual bool CreatingThreadUpdateEPSSurpriseDB(CWorldStock* pStock);
	virtual bool CreatingThreadUpdateInsiderTransactionDB(void);
	virtual bool CreatingThreadUpdateTiingoStockDB(void);
	virtual bool CreatingThreadUpdateTiingoIndustry(void);
	virtual bool CreatingThreadUpdateSICIndustry(void);
	virtual bool CreatingThreadUpdateNaicsIndustry(void);
	virtual bool CreatingThreadUpdateEconomicCalendarDB(void);

	bool UpdateEconomicCalendar(vector<CEconomicCalendarPtr> vEconomicCalendar);

	// 各种状态
	CWebSourceDataProductPtr GetCurrentFinnhubInquiry(void) { return m_pCurrentFinnhubProduct; }
	void SetCurrentFinnhubInquiry(CWebSourceDataProductPtr p) { m_pCurrentFinnhubProduct = p; }

	CWebSourceDataProductPtr GetCurrentTiingoInquiry(void) { return m_pCurrentTiingoProduct; }
	void SetCurrentTiingoInquiry(CWebSourceDataProductPtr p) { m_pCurrentTiingoProduct = p; }

	bool IsFinnhubInquiring(void) noexcept { return m_fFinnhubInquiring; }
	void SetFinnhubInquiring(bool fFlag) noexcept { m_fFinnhubInquiring = fFlag; }
	void SetFinnhubDataReceived(bool fFlag) noexcept { m_fFinnhubDataReceived = fFlag; }
	bool IsFinnhubDataReceived(void) noexcept { const bool f = m_fFinnhubDataReceived; return f; }
	void SetQuandlInquiring(bool fFlag) noexcept { m_fQuandlInquiring = fFlag; }
	void SetQuandlDataReceived(bool fFlag) noexcept { m_fQuandlDataReceived = fFlag; }
	bool IsQuandlDataReceived(void) noexcept { const bool f = m_fQuandlDataReceived; return f; }
	bool IsTiingoInquiring(void) noexcept { return m_fTiingoInquiring; }
	void SetTiingoInquiring(bool fFlag) noexcept { m_fTiingoInquiring = fFlag; }
	void SetTiingoDataReceived(bool fFlag) noexcept { m_fTiingoDataReceived = fFlag; }
	bool IsTiingoDataReceived(void) noexcept { const bool f = m_fTiingoDataReceived; return f; }

	CFinnhubExchangePtr GetExchange(long lIndex) { return m_vFinnhubExchange.at(lIndex); }
	CString GetExchangeCode(long lIndex) { return m_vFinnhubExchange.at(lIndex)->m_strCode; }
	size_t GetExchangeSize(void) noexcept { return m_mapFinnhubExchange.size(); }

	bool IsStockProfileNeedUpdate(void);
	void AddStock(CWorldStockPtr pStock);
	bool DeleteStock(CWorldStockPtr pStock);
	size_t GetTotalStock(void) noexcept { return m_vWorldStock.size(); }
	void AddTiingoStock(CTiingoStockPtr pTiingoStock);
	bool DeleteTiingoStock(CTiingoStockPtr pStock);
	size_t GetTotalTiingoStock(void) noexcept { return m_vTiingoStock.size(); }
	bool IsStock(CString strSymbol) { if (m_mapWorldStock.find(strSymbol) == m_mapWorldStock.end()) return false; else return true; }
	bool IsStock(CWorldStockPtr pStock) { return IsStock(pStock->GetSymbol()); }
	CWorldStockPtr GetStock(long lIndex) { return m_vWorldStock.at(lIndex); }
	CWorldStockPtr GetStock(CString strSymbol) { return m_vWorldStock.at(m_mapWorldStock.at(strSymbol)); }
	long GetStockIndex(CString strSymbol) { return m_mapWorldStock.at(strSymbol); }

	CWorldStockPtr GetChoicedStock(long lIndex) { return m_vWorldChoicedStock.at(lIndex); }

	bool IsTiingoStock(CString strSymbol) { if (m_mapTiingoStock.find(strSymbol) == m_mapTiingoStock.end()) return false; else return true; }
	bool IsTiingoStock(CWorldStockPtr pStock) { return IsTiingoStock(pStock->GetSymbol()); }
	bool IsTiingoStock(CTiingoStockPtr pTiingoStock) { return IsTiingoStock(pTiingoStock->m_strTicker); }
	CTiingoStockPtr GetTiingoStock(long lIndex) { return m_vTiingoStock.at(lIndex); }
	CTiingoStockPtr GetTiingoStock(CString strTicker) { return m_vTiingoStock.at(m_mapTiingoStock.at(strTicker)); }

	bool IsForexExchange(CString strExchange) { if (m_mapForexExchange.find(strExchange) == m_mapForexExchange.end()) return false; else return true; }
	void AddForexExchange(CString strForexExchange);
	bool DeleteForexExchange(CString strForexExchange);
	size_t GetForexExchangeSize(void) noexcept { return m_vForexExchange.size(); }
	CString GetForexExchange(long lIndex) { return m_vForexExchange.at(lIndex); }

	bool IsForexSymbol(CString strSymbol) { if (m_mapForexSymbol.find(strSymbol) == m_mapForexSymbol.end()) return false; else return true; }
	bool IsForexSymbol(CForexSymbolPtr pForexSymbol) { return IsForexSymbol(pForexSymbol->GetSymbol()); }
	void AddForexSymbol(CForexSymbolPtr pForexSymbol);
	bool DeleteForexSymbol(CForexSymbolPtr pForexSysbol);
	CForexSymbolPtr GetForexSymbol(long lIndex) { return m_vForexSymbol.at(lIndex); }
	CForexSymbolPtr GetForexSymbol(CString strSymbol) { return GetForexSymbol(m_mapForexSymbol.at(strSymbol)); }
	size_t GetForexSymbolSize(void) noexcept { return m_vForexSymbol.size(); }

	bool IsCryptoExchange(CString strExchange) { if (m_mapCryptoExchange.find(strExchange) == m_mapCryptoExchange.end()) return false; else return true; }
	void AddCryptoExchange(CString strCryptoExchange);
	bool DeleteCryptoExchange(CString strCryptoExchange);
	size_t GetCryptoExchangeSize(void) noexcept { return m_vCryptoExchange.size(); }
	CString GetCryptoExchange(long lIndex) { return m_vCryptoExchange.at(lIndex); }

	bool IsCryptoSymbol(CString strSymbol) { if (m_mapCryptoSymbol.find(strSymbol) == m_mapCryptoSymbol.end()) return false; else return true; }
	bool IsCryptoSymbol(CCryptoSymbolPtr pCryptoSymbol) { return IsCryptoSymbol(pCryptoSymbol->GetSymbol()); }
	void AddCryptoSymbol(CCryptoSymbolPtr pCryptoSymbol);
	bool DeleteCryptoSymbol(CCryptoSymbolPtr pCryptoSysbol);
	CCryptoSymbolPtr GetCryptoSymbol(long lIndex) { return m_vCryptoSymbol.at(lIndex); }
	CCryptoSymbolPtr GetCryptoSymbol(CString strSymbol) { return GetCryptoSymbol(m_mapCryptoSymbol.at(strSymbol)); }
	size_t GetCryptoSymbolSize(void) noexcept { return m_vCryptoSymbol.size(); }

	size_t GetTotalCountry(void) noexcept { return m_vCountry.size(); }
	bool IsCountry(CString strCountry);
	bool IsCountry(CCountryPtr pCountry);
	void AddCountry(CCountryPtr pCountry);
	bool DeleteCountry(CCountryPtr pCountry);

	size_t GetFinnhubInquiryQueueSize(void) noexcept { return m_qFinnhubProduct.size(); }
	void PushFinnhubInquiry(CWebSourceDataProductPtr p) { m_qFinnhubProduct.push(p); }
	CWebSourceDataProductPtr GetFinnhubInquiry(void);

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
	bool IsFinnhubCryptoExchangeUpdated(void) noexcept { return m_fFinnhubCryptoExchangeUpdated; }
	void SetFinnhubCryptoExchangeUpdated(bool fFlag) noexcept { m_fFinnhubCryptoExchangeUpdated = fFlag; }
	bool IsFinnhubCryptoSymbolUpdated(void) noexcept { return m_fFinnhubCryptoSymbolUpdated; }
	void SetFinnhubCryptoSymbolUpdated(bool fFlag) noexcept { m_fFinnhubCryptoSymbolUpdated = fFlag; }
	bool IsFinnhubCryptoDayLineUpdated(void) noexcept { return m_fFinnhubCryptoDayLineUpdated; }
	void SetFinnhubCryptoDayLineUpdated(bool fFlag) noexcept { m_fFinnhubCryptoDayLineUpdated = fFlag; }
	bool IsFinnhubPeerUpdated(void) noexcept { return m_fFinnhubPeerUpdated; }
	void SetFinnhubPeerUpdated(bool fFlag) noexcept { m_fFinnhubPeerUpdated = fFlag; }
	bool IsFinnhubInsiderTransactionUpdated(void) noexcept { return m_fFinnhubInsiderTransactionUpdated; }
	void SetFinnhubInsiderTransactionUpdated(bool fFlag) noexcept { m_fFinnhubInsiderTransactionUpdated = fFlag; }
	bool IsFinnhubEconomicCalendarUpdated(void) noexcept { return m_fFinnhubEconomicCalendarUpdated; }
	void SetFinnhubEconomicCalendarUpdated(bool fFlag) noexcept { m_fFinnhubEconomicCalendarUpdated = fFlag; }
	bool IsFinnhubEPSSurpriseUpdated(void) noexcept { return m_fFinnhubEPSSurpriseUpdated; }
	void SetFinnhubEPSSurpriseUpdated(bool fFlag) noexcept { m_fFinnhubEPSSurpriseUpdated = fFlag; }

	bool IsTiingoSymbolUpdated(void) noexcept { return m_fTiingoSymbolUpdated; }
	void SetTiingoSymbolUpdated(bool fFlag) noexcept { m_fTiingoSymbolUpdated = fFlag; }
	bool IsTiingoDayLineUpdated(void) noexcept { return m_fTiingoDayLineUpdated; }
	void SetTiingoDayLineUpdated(bool fFlag) noexcept { m_fTiingoDayLineUpdated = fFlag; }

	size_t GetTiingoInquiryQueueSize(void) noexcept { return m_qTiingoProduct.size(); }
	void PushTiingoInquiry(CWebSourceDataProductPtr p) { m_qTiingoProduct.push(p); }
	CWebSourceDataProductPtr GetTiingoInquiry(void);

	void StopReceivingWebSocket(void);

	// 数据库操作
	bool LoadOption(void);
	bool LoadWorldExchangeDB(void); // 装入世界交易所信息

	bool LoadStockDB(void);
	bool LoadWorldChoicedStock(void);

	virtual bool UpdateCountryListDB(void);
	virtual bool UpdateStockProfileDB(void);
	virtual bool UpdateStockDayLineDB(void);
	virtual bool UpdateForexSymbolDB(void);
	virtual bool UpdateForexExchangeDB(void);
	virtual bool UpdateCryptoSymbolDB(void);
	virtual bool UpdateCryptoExchangeDB(void);
	virtual bool UpdateInsiderTransactionDB(void);
	virtual bool UpdateEconomicCalendarDB(void);
	virtual bool UpdateTiingoStockDB(void);
	virtual bool UpdateTiingoIndustry(void);
	virtual bool UpdateSICIndustry(void);
	virtual bool UpdateNaicsIndustry(void);

	bool LoadForexExchange(void);
	bool LoadForexSymbol(void);
	bool LoadCryptoExchange(void);
	bool LoadCryptoSymbol(void);
	bool LoadWorldChoicedForex(void);
	bool LoadWorldChoicedCrypto(void);

	bool LoadCountryDB(void);
	bool LoadEconomicCalendarDB(void);

	bool LoadTiingoStock(void);

	bool RebuildStockDayLineDB(void);
	virtual bool UpdateStockDayLineStartEndDate(void);
	bool RebuildEPSSurprise(void);
	bool RebuildPeer(void);

	bool SortStockVector(void);

	bool ConnectFinnhubWebSocket(void);
	bool SendFinnhubWebSocketMessage(void);
	string CreateFinnhubWebSocketString(CString strSymbol);

	bool ConnectTiingoIEXWebSocket(void);
	bool ConnectTiingoCryptoWebSocket(void);
	bool ConnectTiingoForexWebSocket(void);

	bool SendTiingoIEXWebSocketMessage(void);
	CString CreateTiingoIEXWebSocketSymbolString(void);
	bool SendTiingoCryptoWebSocketMessage(void);
	CString CreateTiingoCryptoWebSocketSymbolString(void);
	bool SendTiingoForexWebSocketMessage(void);
	CString CreateTiingoForexWebSocketSymbolString(void);

	bool TaskProcessWebSocketData(void);
	bool ProcessFinnhubWebSocketData();
	bool ParseFinnhubWebSocketData(shared_ptr<string> pData);
	bool ProcessTiingoIEXWebSocketData();
	bool ParseTiingoIEXWebSocketData(shared_ptr<string> pData);
	bool ProcessTiingoCryptoWebSocketData();
	bool ParseTiingoCryptoWebSocketData(shared_ptr<string> pData);
	bool ProcessTiingoForexWebSocketData();
	bool ParseTiingoForexWebSocketData(shared_ptr<string> pData);

	bool TaskUpdateWorldStockFromWebSocket(void);
	bool UpdateWorldStockFromTiingoIEXSocket(CTiingoIEXSocketPtr pTiingoIEX);
	bool UpdateWorldStockFromFinnhubSocket(CFinnhubSocketPtr pFinnhub);

	int GetWebSocketReceivedNumberPerSecond(void) noexcept { return (int)m_iWebSocketReceivedNumberPerSecond; }
	void ClearWebSocketReceivedNumber(void) noexcept { m_iWebSocketReceivedNumberPerSecond = 0; }

	int GetProcessedFinnhubWebSocket(void) noexcept { return m_iProcessedFinnhubWebSocket; }
	void ClearPorcessedFinnhubWebSocket(void) noexcept { m_iProcessedFinnhubWebSocket = 0; }
	int GetProcessedTiingoIEXWebSocket(void) noexcept { return m_iProcessedTiingoIEXWebSocket; }
	void ClearProcessedTiingoIEXWebSocket(void) noexcept { m_iProcessedTiingoIEXWebSocket = 0; }
	int GetProcessedTiingoCryptoWebSocket(void) noexcept { return m_iProcessedTiingoCryptoWebSocket; }
	void ClearProcessedTiingoCryptoWebSocket(void) noexcept { m_iProcessedTiingoCryptoWebSocket = 0; }
	int GetProcessedTiingoForexWebSocket(void) noexcept { return m_iProcessedTiingoForexWebSocket; }
	void ClearProcessedTiingoForexWebSocket(void) noexcept { m_iProcessedTiingoForexWebSocket = 0; }

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
	long m_lCurrentCryptoExchangePos;
	long m_lCurrentCryptoSymbolPos;
	long m_lCurrentUpdatePeerPos;
	long m_lCurrentUpdateInsiderTransactionPos;
	long m_lCurrentUpdateEPSSurprisePos;

	vector<CWorldStockPtr> m_vWorldChoicedStock;
	map<CString, long> m_mapWorldChoicedStock;
	long m_lChoicedStockPos;
	vector<CForexSymbolPtr> m_vWorldChoicedForex;
	map<CString, long> m_mapWorldChoicedForex;
	long m_lChoicedForexPos;
	vector<CCryptoSymbolPtr> m_vWorldChoicedCrypto;
	map<CString, long> m_mapWorldChoicedCrypto;
	long m_lChoicedCryptoPos;

	CFinnhubFactory m_FinnhubFactory;
	CTiingoFactory m_TiingoFactory;
	CQuandlFactory m_QuandlFactory;
	CWebSourceDataProductPtr m_pCurrentFinnhubProduct;
	CWebSourceDataProductPtr m_pCurrentTiingoProduct;
	CWebSourceDataProductPtr m_pCurrentQuandlProduct;

	queue<CWebSourceDataProductPtr, list<CWebSourceDataProductPtr>> m_qFinnhubProduct; // 网络查询命令队列
	bool m_fFinnhubInquiring;
	atomic_bool m_fFinnhubDataReceived;

	queue<CWebSourceDataProductPtr, list<CWebSourceDataProductPtr>> m_qTiingoProduct; // 网络查询命令队列
	bool m_fTiingoInquiring;
	atomic_bool m_fTiingoDataReceived;

	queue<CWebSourceDataProductPtr, list<CWebSourceDataProductPtr>> m_qQuandlProduct; // 网络查询命令队列
	bool m_fQuandlInquiring;
	atomic_bool m_fQuandlDataReceived;

	vector<CString> m_vForexExchange;
	map<CString, long> m_mapForexExchange;
	long m_lLastTotalForexExchange;

	vector<CForexSymbolPtr> m_vForexSymbol;
	map<CString, long> m_mapForexSymbol;
	long m_lLastTotalForexSymbol;
	long m_lCurrentUpdateForexDayLinePos;

	vector<CString> m_vCryptoExchange;
	map<CString, long> m_mapCryptoExchange;
	long m_lLastTotalCryptoExchange;

	vector<CCryptoSymbolPtr> m_vCryptoSymbol;
	map<CString, long> m_mapCryptoSymbol;
	long m_lLastTotalCryptoSymbol;
	long m_lCurrentUpdateCryptoDayLinePos;

	vector<CCountryPtr> m_vCountry;
	map<CString, long> m_mapCountry;
	bool m_fCountryListUpdated;
	long m_lLastTotalCountry;

	vector<CEconomicCalendarPtr> m_vEconomicCalendar;
	map<CString, long> m_mapEconomicCalendar;
	long m_lLastTotalEconomicCalendar;

	vector<CTiingoStockPtr> m_vTiingoStock;
	map<CString, long> m_mapTiingoStock;
	long m_lLastTotalTiingoStock;

	bool m_fFinnhubSymbolUpdated; // 每日更新公司代码库
	bool m_fFinnhubStockProfileUpdated; // 每日更新公司简介
	bool m_fFinnhubDayLineUpdated; // 每日更新公司日线数据
	bool m_fFinnhubForexExchangeUpdated; // 每日更新Forex交易所
	bool m_fFinnhubForexSymbolUpdated; // 每日更新Forex交易所代码
	bool m_fFinnhubForexDayLineUpdated; // 每日更新Forex日线数据
	bool m_fFinnhubCryptoExchangeUpdated; // 每日更新Crypto交易所
	bool m_fFinnhubCryptoSymbolUpdated; // 每日更新Crypto交易所代码
	bool m_fFinnhubCryptoDayLineUpdated; // 每日更新Crypto日线数据
	bool m_fFinnhubPeerUpdated; // 每月更新Peers数据
	bool m_fFinnhubInsiderTransactionUpdated; // 每月更新Peers数据
	bool m_fFinnhubEconomicCalendarUpdated; // 每日更新经济日历数据
	bool m_fFinnhubEPSSurpriseUpdated;

	bool m_fTiingoSymbolUpdated; // 每日更新公司代码库
	bool m_fTiingoDayLineUpdated; // 每日更新公司日线数据

	vector<CTiingoIndustryPtr> m_vTiingoIndustry;
	map<CString, long> m_mapTiingoIndustry;
	long m_lLastTotalTiingoIndustry;

	vector<CSICIndustryPtr> m_vSICIndustry;
	map<CString, long> m_mapSICIndustry;
	long m_lLastTotalSICIndustry;

	vector<CNaicsIndustryPtr> m_vNaicsIndustry;
	map<CString, long> m_mapNaicsIndustry;
	long m_lLastTotalNaicsIndustry;

	// WebSocket数据
	CWebSocket m_FinnhubWebSocket;
	CWebSocket m_TiingoIEXWebSocket;
	CWebSocket m_TiingoCryptoWebSocket;
	CWebSocket m_TiingoForexWebSocket;

	int m_iProcessedFinnhubWebSocket;
	int m_iProcessedTiingoCryptoWebSocket;
	int m_iProcessedTiingoForexWebSocket;
	int m_iProcessedTiingoIEXWebSocket;

	size_t m_iWebSocketReceivedNumberPerSecond; // 每秒接收到的数据个数
	int m_iWebSocketReceivedDataPerSecond; // 每秒接收到的数据量
	string m_strMessage;

	//
	bool m_fRebulidDayLine;
	// 重建日线历史数据。
};

typedef shared_ptr<CWorldMarket> CWorldMarketPtr;
