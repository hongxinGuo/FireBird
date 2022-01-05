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

#include"DataFinnhubStockExchange.h"
#include"DataFinnhubForexExchange.h"
#include"DataFinnhubCryptoExchange.h"
#include"DataFinnhubForexSymbol.h"
#include"DataFinnhubCryptoSymbol.h"
#include"DataFinnhubCountry.h"
#include"DataFinnhubEconomicCalendar.h"

#include"DataChoicedStock.h"
#include"DataChoicedForex.h"
#include"DataChoicedCrypto.h"

#include"DataWorldStock.h"
#include"DataTiingoStock.h"

#include"DataTiingoIndustry.h"
#include"DataTiingoCryptoSymbol.h"
#include"DataSICIndustry.h"
#include"DataNaicsIndustry.h"

#include"DataFinnhubWebSocket.h"
#include"DataTiingoIEXWebSocket.h"
#include"DataTiingoForexWebSocket.h"
#include"DataTiingoCryptoWebSocket.h"

#include"WebSocketData.h"
#include"WebSocket.h"

#include <ixwebsocket/IXWebSocket.h>
using namespace ix;

using namespace MyLib;

// Finnhub�������ʹ��룬����˻��޷�����Premium�����Ϣ
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

	__STOCK_PRICE_QUOTE__ = 301, // ʵʱ�������ȼ����
	__STOCK_PRICE_CANDLES__ = 302, // ��ʷ�������ȼ���
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
	__FOREX_CANDLES__ = 603, // ��ʷ�������ȼ��� Premium
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

extern Semaphore gl_UpdateWorldMarketDB;  // ���ź�����������������ʷ���ݿ�

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
	void ResetDataClass(void);
	virtual bool IsInResetSystemTime(long lCurrentTime)  override { if ((lCurrentTime < 165800) || (lCurrentTime > 170500)) return false; else return true; }

	virtual bool SchedulingTask(void) override final; // �ɳ���Ķ�ʱ�����ȣ���Լÿ100����һ��
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

	bool TaskInquiryFinnhub(long lCurrentTime); // ���������Ϊ���������в�ѯFinnhub�������λ�ڴ˺����ڡ�
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

	bool TaskInquiryTiingo(void);// ���������Ϊ���������в�ѯTiingo�������λ�ڴ˺����ڡ�
	virtual bool TaskInquiryTiingoCompanySymbol(void);
	virtual bool TaskInquiryTiingoCryptoSymbol(void);
	virtual bool TaskInquiryTiingoDayLine(void);

	virtual bool TaskUpdateTiingoIndustry(void);
	virtual bool TaskUpdateSICIndustry(void);
	virtual bool TaskUpdateNaicsIndustry(void);

	bool TaskUpdateStockProfileDB(void) { return CreatingThreadUpdateStockProfileDB(); }
	bool TaskUpdateDayLineDB(void) { return CreatingThreadUpdateDayLineDB(); }
	bool TaskUpdateForexExchangeDB(void) { return CreatingThreadUpdateForexExchangeDB(); }
	bool TaskUpdateForexSymbolDB(void);
	bool TaskUpdateForexDayLineDB(void);
	bool TaskUpdateCryptoExchangeDB(void) { return CreatingThreadUpdateCryptoExchangeDB(); }
	bool TaskUpdateCryptoSymbolDB(void);
	bool TaskUpdateCryptoDayLineDB(void);
	bool TaskUpdateCountryListDB(void) { return CreatingThreadUpdateCountryListDB(); }
	bool TaskUpdateEPSSurpriseDB(void);
	bool TaskUpdateEconomicCalendarDB(void) { return CreatingThreadUpdateEconomicCalendarDB(); }
	bool TaskUpdateInsiderTransactionDB(void) { return CreatingThreadUpdateInsiderTransactionDB(); }
	bool TaskUpdateTiingoStockDB(void) { return CreatingThreadUpdateTiingoStockDB(); }
	bool TaskUpdateTiingoCryptoDB(void) { return CreatingThreadUpdateTiingoCryptoDB(); }

	bool TaskCheckSystemReady(void);

	bool TaskUpdateDayLineStartEndDate(void) { return CreatingthreadUpdateDayLneStartEndDate(); }

	// �������̵߳��ð�������
	virtual bool CreatingthreadUpdateDayLneStartEndDate(void);
	virtual bool CreatingThreadUpdateDayLineDB(void);
	virtual bool CreatingThreadUpdateStockProfileDB(void);
	virtual bool CreatingThreadUpdateForexExchangeDB(void);
	virtual bool CreatingThreadUpdateForexSymbolDB(void);
	virtual bool CreatingThreadUpdateForexDayLineDB(CForexSymbol* pSymbol);
	virtual bool CreatingThreadUpdateCryptoExchangeDB(void);
	virtual bool CreatingThreadUpdateCryptoSymbolDB(void);
	virtual bool CreatingThreadUpdateCryptoDayLineDB(CFinnhubCryptoSymbol* pSymbol);
	virtual bool CreatingThreadUpdateCountryListDB(void);
	virtual bool CreatingThreadUpdateEPSSurpriseDB(CWorldStock* pStock);
	virtual bool CreatingThreadUpdateInsiderTransactionDB(void);
	virtual bool CreatingThreadUpdateTiingoStockDB(void);
	virtual bool CreatingThreadUpdateTiingoCryptoDB(void);
	virtual bool CreatingThreadUpdateTiingoIndustry(void);
	virtual bool CreatingThreadUpdateSICIndustry(void);
	virtual bool CreatingThreadUpdateNaicsIndustry(void);
	virtual bool CreatingThreadUpdateEconomicCalendarDB(void);

	bool UpdateEconomicCalendar(vector<CEconomicCalendarPtr> vEconomicCalendar) { return m_dataFinnhubEconomicCalendar.Update(vEconomicCalendar); }

	// ����״̬
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

	CFinnhubStockExchangePtr GetStockExchange(long lIndex) { return m_dataFinnhubStockExchange.GetExchange(lIndex); }
	CString GetStockExchangeCode(long lIndex) { return m_dataFinnhubStockExchange.GetExchange(lIndex)->m_strCode; }
	size_t GetStockExchangeSize(void) noexcept { return m_dataFinnhubStockExchange.GetExchangeSize(); }

	bool IsStockProfileNeedUpdate(void) { return m_dataWorldStock.IsStockProfileNeedUpdate(); }
	void AddStock(CWorldStockPtr pStock) { m_dataWorldStock.Add(pStock); }
	bool DeleteStock(CWorldStockPtr pStock) { return m_dataWorldStock.Delete(pStock); }
	size_t GetStockSize(void) noexcept { return m_dataWorldStock.GetStockSize(); }
	void AddTiingoStock(CTiingoStockPtr pTiingoStock) { m_dataTiingoStock.Add(pTiingoStock); }
	bool DeleteTiingoStock(CTiingoStockPtr pStock) { return m_dataTiingoStock.Delete(pStock); }
	size_t GetTotalTiingoStock(void) noexcept { return m_dataTiingoStock.GetTotalStock(); }
	bool IsStock(CString strSymbol) { return m_dataWorldStock.IsStock(strSymbol); }
	bool IsStock(CWorldStockPtr pStock) { return IsStock(pStock->GetSymbol()); }
	CWorldStockPtr GetStock(long lIndex) { return m_dataWorldStock.GetStock(lIndex); }
	CWorldStockPtr GetStock(CString strSymbol) { return m_dataWorldStock.GetStock(strSymbol); }
	long GetStockIndex(CString strSymbol) { return m_dataWorldStock.GetStockIndex(strSymbol); }

	CWorldStockPtr GetChoicedStock(long lIndex) { return m_dataChoicedStock.GetStock(lIndex); }

	bool IsTiingoStock(CString strSymbol) { return m_dataTiingoStock.IsStock(strSymbol); }
	bool IsTiingoStock(CWorldStockPtr pStock) { return m_dataTiingoStock.IsStock(pStock); }
	bool IsTiingoStock(CTiingoStockPtr pTiingoStock) { return m_dataTiingoStock.IsStock(pTiingoStock); }
	CTiingoStockPtr GetTiingoStock(long lIndex) { return m_dataTiingoStock.GetStock(lIndex); }
	CTiingoStockPtr GetTiingoStock(CString strTicker) { return m_dataTiingoStock.GetStock(strTicker); }

	bool IsForexExchange(CString strForexExchange) { return m_dataFinnhubForexExchange.IsForexExchange(strForexExchange); }
	void AddForexExchange(CString strForexExchange) { m_dataFinnhubForexExchange.Add(strForexExchange); }
	bool DeleteForexExchange(CString strForexExchange) { return m_dataFinnhubForexExchange.Delete(strForexExchange); }
	size_t GetForexExchangeSize(void) noexcept { return m_dataFinnhubForexExchange.GetForexExchangeSize(); }
	CString GetForexExchange(long lIndex) { return m_dataFinnhubForexExchange.GetForexExchange(lIndex); }

	bool IsForexSymbol(CString strForexSymbol) { return m_dataFinnhubForexSymbol.IsForexSymbol(strForexSymbol); }
	bool IsForexSymbol(CForexSymbolPtr pForexSymbol) { return IsForexSymbol(pForexSymbol->GetSymbol()); }
	void AddForexSymbol(CForexSymbolPtr pForexSymbol) { m_dataFinnhubForexSymbol.Add(pForexSymbol); }
	bool DeleteForexSymbol(CForexSymbolPtr pForexSysbol) { return m_dataFinnhubForexSymbol.Delete(pForexSysbol); }
	CForexSymbolPtr GetForexSymbol(long lIndex) { return m_dataFinnhubForexSymbol.GetForexSymbol(lIndex); }
	CForexSymbolPtr GetForexSymbol(CString strSymbol) { return m_dataFinnhubForexSymbol.GetForexSymbol(strSymbol); }
	size_t GetForexSymbolSize(void) noexcept { return m_dataFinnhubForexSymbol.GetForexSymbolSize(); }

	bool IsCryptoExchange(CString strCryptoExchange) { return m_dataFinnhubCryptoExchange.IsCryptoExchange(strCryptoExchange); }
	void AddCryptoExchange(CString strCryptoExchange) { m_dataFinnhubCryptoExchange.Add(strCryptoExchange); }
	bool DeleteCryptoExchange(CString strCryptoExchange) { return m_dataFinnhubCryptoExchange.Delete(strCryptoExchange); }
	size_t GetCryptoExchangeSize(void) noexcept { return m_dataFinnhubCryptoExchange.GetCryptoExchangeSize(); }
	CString GetCryptoExchange(long lIndex) { return m_dataFinnhubCryptoExchange.GetCryptoExchange(lIndex); }

	bool IsFinnhubCryptoSymbol(CString strSymbol) { return m_dataFinnhubCryptoSymbol.IsFinnhubCryptoSymbol(strSymbol); }
	bool IsFinnhubCryptoSymbol(CFinnhubCryptoSymbolPtr pCryptoSymbol) { return IsFinnhubCryptoSymbol(pCryptoSymbol->GetSymbol()); }
	void AddCryptoSymbol(CFinnhubCryptoSymbolPtr pCryptoSymbol) { m_dataFinnhubCryptoSymbol.Add(pCryptoSymbol); }
	bool DeleteCryptoSymbol(CFinnhubCryptoSymbolPtr pCryptoSysbol) { return m_dataFinnhubCryptoSymbol.Delete(pCryptoSysbol); }
	CFinnhubCryptoSymbolPtr GetCryptoSymbol(long lIndex) { return m_dataFinnhubCryptoSymbol.GetCryptoSymbol(lIndex); }
	CFinnhubCryptoSymbolPtr GetCryptoSymbol(CString strSymbol) { return m_dataFinnhubCryptoSymbol.GetCryptoSymbol(strSymbol); }
	size_t GetCryptoSymbolSize(void) noexcept { return m_dataFinnhubCryptoSymbol.GetCryptoSymbolSize(); }

	bool IsTiingoCryptoSymbol(CString strSymbol) { return m_dataTiingoCryptoSymbol.IsTiingoCryptoSymbol(strSymbol); }
	bool IsTiingoCryptoSymbol(CTiingoCryptoSymbolPtr pCryptoSymbol) { return IsTiingoCryptoSymbol(pCryptoSymbol->m_strTicker); }
	void AddTiingoCryptoSymbol(CTiingoCryptoSymbolPtr pCryptoSymbol) { m_dataTiingoCryptoSymbol.Add(pCryptoSymbol); }
	bool DeleteTiingoCryptoSymbol(CTiingoCryptoSymbolPtr pCryptoSysbol) { return m_dataTiingoCryptoSymbol.Delete(pCryptoSysbol); }
	CTiingoCryptoSymbolPtr GetTiingoCryptoSymbol(long lIndex) { return m_dataTiingoCryptoSymbol.GetCryptoSymbol(lIndex); }
	CTiingoCryptoSymbolPtr GetTiingoCryptoSymbol(CString strSymbol) { return m_dataTiingoCryptoSymbol.GetCryptoSymbol(strSymbol); }
	size_t GetTiingoCryptoSymbolSize(void) noexcept { return m_dataTiingoCryptoSymbol.GetCryptoSymbolSize(); }

	size_t GetTotalCountry(void) noexcept { return m_dataFinnhubCountry.GetTotalCountry(); }
	bool IsCountry(CString strCountry) { return m_dataFinnhubCountry.IsCountry(strCountry); }
	bool IsCountry(CCountryPtr pCountry) { return m_dataFinnhubCountry.IsCountry(pCountry); }
	void AddCountry(CCountryPtr pCountry) { m_dataFinnhubCountry.Add(pCountry); }
	bool DeleteCountry(CCountryPtr pCountry) { return m_dataFinnhubCountry.Delete(pCountry); }

	size_t GetFinnhubInquiryQueueSize(void) noexcept { return m_qFinnhubProduct.size(); }
	void PushFinnhubInquiry(CWebSourceDataProductPtr p) { m_qFinnhubProduct.push(p); }
	CWebSourceDataProductPtr GetFinnhubInquiry(void) { CWebSourceDataProductPtr p = m_qFinnhubProduct.front(); m_qFinnhubProduct.pop(); return p; }

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

	bool IsTiingoStockSymbolUpdated(void) noexcept { return m_fTiingoStockSymbolUpdated; }
	void SetTiingoStockSymbolUpdated(bool fFlag) noexcept { m_fTiingoStockSymbolUpdated = fFlag; }
	bool IsTiingoCryptoSymbolUpdated(void) noexcept { return m_fTiingoCryptoSymbolUpdated; }
	void SetTiingoCryptoSymbolUpdated(bool fFlag) noexcept { m_fTiingoCryptoSymbolUpdated = fFlag; }
	bool IsTiingoDayLineUpdated(void) noexcept { return m_fTiingoDayLineUpdated; }
	void SetTiingoDayLineUpdated(bool fFlag) noexcept { m_fTiingoDayLineUpdated = fFlag; }

	size_t GetTiingoInquiryQueueSize(void) noexcept { return m_qTiingoProduct.size(); }
	void PushTiingoInquiry(CWebSourceDataProductPtr p) { m_qTiingoProduct.push(p); }
	CWebSourceDataProductPtr GetTiingoInquiry(void) { CWebSourceDataProductPtr p = m_qTiingoProduct.front(); m_qTiingoProduct.pop(); return p; }

	void StopReceivingWebSocket(void);

	// ���ݿ����
	bool LoadOption(void);
	bool LoadWorldExchangeDB(void) { return m_dataFinnhubStockExchange.LoadDB(); }

	bool LoadStockDB(void) { return m_dataWorldStock.LoadDB(); }
	bool LoadWorldChoicedStock(void) { return m_dataChoicedStock.LoadDB(); }

	virtual bool UpdateCountryListDB(void) { return m_dataFinnhubCountry.UpdateDB(); }
	virtual bool UpdateStockProfileDB(void) { return m_dataWorldStock.UpdateProfileDB(); }
	virtual bool UpdateStockDayLineDB(void);
	virtual bool UpdateForexSymbolDB(void) { return m_dataFinnhubForexSymbol.UpdateDB(); }
	virtual bool UpdateForexExchangeDB(void) { return m_dataFinnhubForexExchange.UpdateDB(); }
	virtual bool UpdateCryptoSymbolDB(void) { return m_dataFinnhubCryptoSymbol.UpdateDB(); }
	virtual bool UpdateCryptoExchangeDB(void) { return m_dataFinnhubCryptoExchange.UpdateDB(); }
	virtual bool UpdateInsiderTransactionDB(void);
	virtual bool UpdateEconomicCalendarDB(void) { return m_dataFinnhubEconomicCalendar.UpdateDB(); }
	virtual bool UpdateTiingoStockDB(void) { return m_dataTiingoStock.UpdateDB(); }
	virtual bool UpdateTiingoCryptoDB(void) { return m_dataTiingoCryptoSymbol.UpdateDB(); }
	virtual bool UpdateTiingoIndustry(void);
	virtual bool UpdateSICIndustry(void);
	virtual bool UpdateNaicsIndustry(void);

	bool LoadForexExchange(void) { return m_dataFinnhubForexExchange.LoadDB(); }
	bool LoadForexSymbol(void) { return m_dataFinnhubForexSymbol.LoadDB(); }
	bool LoadCryptoExchange(void) { return m_dataFinnhubCryptoExchange.LoadDB(); }
	bool LoadCryptoSymbol(void) { return m_dataFinnhubCryptoSymbol.LoadDB(); }
	bool LoadWorldChoicedForex(void) { return m_dataChoicedForex.LoadDB(); }
	bool LoadWorldChoicedCrypto(void) { return m_dataChoicedCrypto.LoadDB(); }

	bool LoadCountryDB(void) { return m_dataFinnhubCountry.LoadDB(); }
	bool LoadEconomicCalendarDB(void) { return m_dataFinnhubEconomicCalendar.LoadDB(); }

	bool LoadTiingoStock(void) { return m_dataTiingoStock.LoadDB(); }

	bool RebuildStockDayLineDB(void);
	virtual bool UpdateStockDayLineStartEndDate(void);
	bool RebuildEPSSurprise(void);
	bool RebuildPeer(void);

	bool SortStock(void) { return m_dataWorldStock.SortStock(); }

	bool ConnectFinnhubWebSocket(void) { return m_dataFinnhubWebSocket.Connecting(); }
	bool SendFinnhubWebSocketMessage(void);

	bool ConnectTiingoIEXWebSocket(void) { return m_dataTiingoIEXWebSocket.Connecting(); }
	bool ConnectTiingoCryptoWebSocket(void) { return m_dataTiingoCryptoWebSocket.Connecting(); }
	bool ConnectTiingoForexWebSocket(void) { return m_dataTiingoForexWebSocket.Connecting(); }

	bool SendTiingoIEXWebSocketMessage(void);
	bool SendTiingoCryptoWebSocketMessage(void);
	bool SendTiingoForexWebSocketMessage(void);

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

	void SetCurrentFinnhubWebSocketStake(CString s) { m_strCurrentFinnhubWebSocketStake = s; }
	CString GetCurrentFinnhubWebSocketStake(void) { return m_strCurrentFinnhubWebSocketStake; }
	void SetCurrentTiingoWebSocketIEX(CString s) { m_strCurrentTiingoWebSocketIEX = s; }
	CString GetCurrentTiingoWebSocketIEX(void) { return m_strCurrentTiingoWebSocketIEX; }
	void SetCurrentTiingoWebSocketForex(CString s) { m_strCurrentTiingoWebSocketForex = s; }
	CString GetCurrentTiingoWebSocketForex(void) { return m_strCurrentTiingoWebSocketForex; }
	void SetCurrentTiingoWebSocketCrypto(CString s) { m_strCurrentTiingoWebSocketCrypto = s; }
	CString GetCurrentTiingoWebSocketCrypto(void) { return m_strCurrentTiingoWebSocketCrypto; }

protected:
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

	CDataFinnhubStockExchange m_dataFinnhubStockExchange;
	CDataFinnhubForexExchange m_dataFinnhubForexExchange;
	CDataFinnhubCryptoExchange m_dataFinnhubCryptoExchange;
	CDataFinnhubForexSymbol m_dataFinnhubForexSymbol;
	CDataFinnhubCryptoSymbol m_dataFinnhubCryptoSymbol;
	CDataFinnhubCountry m_dataFinnhubCountry;
	CDataFinnhubEconomicCalendar m_dataFinnhubEconomicCalendar;

	CDataWorldStock m_dataWorldStock;

	CDataTiingoStock m_dataTiingoStock;
	CDataTiingoCryptoSymbol m_dataTiingoCryptoSymbol;

	CDataChoicedStock m_dataChoicedStock;
	CDataChoicedForex m_dataChoicedForex;
	CDataChoicedCrypto m_dataChoicedCrypto;

	long m_lCurrentUpdateForexDayLinePos;
	long m_lCurrentUpdateCryptoDayLinePos;

	CFinnhubFactory m_FinnhubFactory;
	CTiingoFactory m_TiingoFactory;
	CQuandlFactory m_QuandlFactory;
	CWebSourceDataProductPtr m_pCurrentFinnhubProduct;
	CWebSourceDataProductPtr m_pCurrentTiingoProduct;
	CWebSourceDataProductPtr m_pCurrentQuandlProduct;

	queue<CWebSourceDataProductPtr, list<CWebSourceDataProductPtr>> m_qFinnhubProduct; // �����ѯ�������
	bool m_fFinnhubInquiring;
	atomic_bool m_fFinnhubDataReceived;

	queue<CWebSourceDataProductPtr, list<CWebSourceDataProductPtr>> m_qTiingoProduct; // �����ѯ�������
	bool m_fTiingoInquiring;
	atomic_bool m_fTiingoDataReceived;

	queue<CWebSourceDataProductPtr, list<CWebSourceDataProductPtr>> m_qQuandlProduct; // �����ѯ�������
	bool m_fQuandlInquiring;
	atomic_bool m_fQuandlDataReceived;

	bool m_fCountryListUpdated;
	bool m_fFinnhubSymbolUpdated; // ÿ�ո��¹�˾�����
	bool m_fFinnhubStockProfileUpdated; // ÿ�ո��¹�˾���
	bool m_fFinnhubDayLineUpdated; // ÿ�ո��¹�˾��������
	bool m_fFinnhubForexExchangeUpdated; // ÿ�ո���Forex������
	bool m_fFinnhubForexSymbolUpdated; // ÿ�ո���Forex����������
	bool m_fFinnhubForexDayLineUpdated; // ÿ�ո���Forex��������
	bool m_fFinnhubCryptoExchangeUpdated; // ÿ�ո���Crypto������
	bool m_fFinnhubCryptoSymbolUpdated; // ÿ�ո���Crypto����������
	bool m_fFinnhubCryptoDayLineUpdated; // ÿ�ո���Crypto��������
	bool m_fFinnhubPeerUpdated; // ÿ�¸���Peers����
	bool m_fFinnhubInsiderTransactionUpdated; // ÿ�¸���Peers����
	bool m_fFinnhubEconomicCalendarUpdated; // ÿ�ո��¾�����������
	bool m_fFinnhubEPSSurpriseUpdated;

	bool m_fTiingoStockSymbolUpdated; // ÿ�ո��¹�˾�����
	bool m_fTiingoCryptoSymbolUpdated; // ÿ�ո���crypto�����
	bool m_fTiingoDayLineUpdated; // ÿ�ո��¹�˾��������

	bool m_fInquiringFinnhubStockSymbol;
	bool m_fInquiringFinnhubStockProfile;
	bool m_fInquiringFinnhubStockPeer;
	bool m_fInquiringFinnhubStockInsiderTransaction;
	bool m_fInquiringFinnhubForexDayLine;
	bool m_fInquiringFinnhubCryptoDayLine;
	bool m_fInquiringFinnhubStockDayLine;

	// WebSocket����
	CDataFinnhubWebSocket m_dataFinnhubWebSocket;
	CDataTiingoIEXWebSocket m_dataTiingoIEXWebSocket;
	CDataTiingoForexWebSocket m_dataTiingoForexWebSocket;
	CDataTiingoCryptoWebSocket m_dataTiingoCryptoWebSocket;

	int m_iProcessedFinnhubWebSocket;
	int m_iProcessedTiingoCryptoWebSocket;
	int m_iProcessedTiingoForexWebSocket;
	int m_iProcessedTiingoIEXWebSocket;

	size_t m_iWebSocketReceivedNumberPerSecond; // ÿ����յ������ݸ���
	int m_iWebSocketReceivedDataPerSecond; // ÿ����յ���������
	string m_strMessage;

	CString m_strCurrentFinnhubWebSocketStake;
	CString m_strCurrentTiingoWebSocketIEX;
	CString m_strCurrentTiingoWebSocketForex;
	CString m_strCurrentTiingoWebSocketCrypto;

	//
	bool m_fRebulidDayLine;
	// �ؽ�������ʷ���ݡ�
};

typedef shared_ptr<CWorldMarket> CWorldMarketPtr;
