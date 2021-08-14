#pragma once
#include"Semaphore.h"

#include"VirtualMarket.h"
#include"WorldStock.h"
#include"WebData.h"
#include"FinnhubExchange.h"
#include"FinnhubForexSymbol.h"
#include"Country.h"
#include"EconomicCalendar.h"
#include"InsiderTransaction.h"

#include"TiingoStock.h"

#include"TiingoIndustry.h"
#include"SICIndustry.h"
#include"NaicsIndustry.h"

#include"QuandlWebInquiry.h"
#include"TiingoWebInquiry.h"

#include"TiingoWebSocketData.h"

#include <ixwebsocket/IXWebSocket.h>
using namespace ix;

using namespace MyLib;

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
	__IPO_CALENDAR__ = 118,
	__DIVIDENDS__ = 119, //Premium
	__INSIDER_TRANSACTION__ = 120,

	__STOCK_RECOMMENDATION_TRENDS__ = 200,
	__STOCK_PRICE_TARGET__ = 201,
	__STOCK_UPGRADE_DOWNGRADE__ = 202, // Primium
	__STOCK_REVENUE_EXTIMATES__ = 203, // Premium
	__STOCK_EPS_EXTIMATES__ = 204, // Primium
	__STOCK_EPS_SURPRISE__ = 205,
	__STOCK_EARNING_CALENDAR__ = 206,

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
public:
	CWorldMarket();
	virtual ~CWorldMarket();
	virtual void ResetMarket(void) override;

	virtual bool PreparingExitMarket(void) override final;

	void Reset(void);
	void ResetFinnhub(void);
	void ResetQuandl(void);
	void ResetTiingo(void);

	void InitialFinnhubInquiryStr(void);
	void InitialTiingoInquiryStr(void);

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

	bool TaskInquiryTiingo(void);// ���������Ϊ���������в�ѯTiingo�������λ�ڴ˺����ڡ�
	virtual bool TaskInquiryTiingoCompanySymbol(void);
	virtual bool TaskInquiryTiingoDayLine(void);

	virtual bool TaskUpdateTiingoIndustry(void);
	virtual bool TaskUpdateSICIndustry(void);
	virtual bool TaskUpdateNaicsIndustry(void);

	// Finnhub���ݴ�����
	virtual bool ProcessFinnhubStockProfile(CWebDataPtr pWebData, CWorldStockPtr& pStock);
	virtual bool ProcessFinnhubStockProfileConcise(CWebDataPtr pWebData, CWorldStockPtr& pStock);
	virtual bool ProcessFinnhubStockSymbol(CWebDataPtr pWebData, vector<CWorldStockPtr>& vStock);
	virtual bool ProcessFinnhubStockCandle(CWebDataPtr pWebData, CWorldStockPtr& pStock);
	virtual bool ProcessFinnhubStockQuote(CWebDataPtr pWebData, CWorldStockPtr& pStock);
	virtual bool ProcessFinnhubForexExchange(CWebDataPtr pWebData, vector<CString>& vExchange);
	virtual bool ProcessFinnhubForexSymbol(CWebDataPtr pWebData, vector<CForexSymbolPtr>& vForexSymbol);
	virtual bool ProcessFinnhubForexCandle(CWebDataPtr pWebData, CForexSymbolPtr& pForexSymbol);
	virtual bool ProcessFinnhubCountryList(CWebDataPtr pWebData, vector<CCountryPtr>& vCountry);
	virtual bool ProcessFinnhubStockPeer(CWebDataPtr pWebData, CWorldStockPtr& pStock);
	virtual bool ProcessFinnhubStockInsiderTransaction(CWebDataPtr pWebData, vector<CInsiderTransactionPtr>& vInsiderTransaction);
	virtual bool ProcessFinnhubEconomicCalendar(CWebDataPtr pWebData, vector<CEconomicCalendarPtr>& m_vEconomicCalendar);
	virtual bool ProcessFinnhubEPSSurprise(CWebDataPtr pWebData, vector<CEPSSurprisePtr>& vEPSSurprise);

	// Tiingo���ݴ�����
	virtual bool ProcessTiingoStockSymbol(CWebDataPtr pWebData, vector<CTiingoStockPtr>& vTiingoStock);
	virtual bool ProcessTiingoStockDayLine(CWebDataPtr pWebData, CWorldStockPtr& pStock);

	bool TaskUpdateStockProfileDB(void);
	bool TaskUpdateDayLineDB(void);
	bool TaskUpdateForexExchangeDB(void);
	bool TaskUpdateForexSymbolDB(void);
	bool TaskUpdateForexDayLineDB(void);
	bool TaskUpdateCountryListDB(void);
	bool TaskUpdateEPSSurpriseDB(void);
	bool TaskUpdateEconomicCalendarDB(void);
	bool TaskUpdateInsiderTransactionDB(void);
	bool TaskUpdateTiingoStockDB(void);

	bool TaskCheckSystemReady(void);

	bool TaskUpdateDayLineStartEndDate(void);

	// �������̵߳��ð�������
	virtual bool RunningthreadUpdateDayLneStartEndDate(void);
	virtual bool CreatingThreadUpdateDayLineDB(void);
	virtual bool CreatingThreadUpdateStockProfileDB(void);
	virtual bool CreatingThreadUpdateForexDayLineDB(CFinnhubForexSymbol* pSymbol);
	virtual bool CreatingThreadUpdateForexSymbolDB(void);
	virtual bool CreatingThreadUpdateCountryListDB(void);
	virtual bool CreatingThreadUpdateEPSSurpriseDB(CWorldStock* pStock);
	virtual bool CreatingThreadUpdateInsiderTransactionDB(void);
	virtual bool CreatingThreadUpdateTiingoStockDB(void);
	virtual bool CreatingThreadUpdateTiingoIndustry(void);
	virtual bool CreatingThreadUpdateSICIndustry(void);
	virtual bool CreatingThreadUpdateNaicsIndustry(void);
	virtual bool CreatingThreadUpdateForexExchangeDB(void);
	virtual bool CreatingThreadUpdateEconomicCalendarDB(void);

	bool UpdateEconomicCalendar(vector<CEconomicCalendarPtr> vEconomicCalendar);

	// ����״̬
	WebInquiry GetCurrentFinnhubInquiry(void) noexcept { return m_CurrentFinnhubInquiry; }
	void SetCurrentFinnhubInquiry(WebInquiry inquiry) noexcept { m_CurrentFinnhubInquiry = inquiry; }
	WebInquiry GetCurrentTiingoInquiry(void) noexcept { return m_CurrentTiingoInquiry; }
	void SetCurrentTiingoInquiry(WebInquiry inquiry) noexcept { m_CurrentTiingoInquiry = inquiry; }
	WebInquiry GetCurrentQuandlInquiry(void) noexcept { return m_CurrentQuandlInquiry; }
	void SetCurrentQuandlInquiry(WebInquiry inquiry) noexcept { m_CurrentQuandlInquiry = inquiry; }

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
	CWorldStockPtr GetStock(CString strSymbol) {
		try {
			return m_vWorldStock.at(m_mapWorldStock.at(strSymbol));
		}
		catch (exception&) {
			return nullptr;
		}
	}
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

	size_t GetTotalCountry(void) noexcept { return m_vCountry.size(); }
	bool IsCountry(CString strCountry);
	bool IsCountry(CCountryPtr pCountry);
	void AddCountry(CCountryPtr pCountry);
	bool DeleteCountry(CCountryPtr pCountry);

	size_t GetFinnhubInquiryQueueSize(void) noexcept { return m_qFinnhubWebInquiry.size(); }
	void PushFinnhubInquiry(WebInquiry inquiry) { m_qFinnhubWebInquiry.push(inquiry); }
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

	size_t GetTiingoInquiryQueueSize(void) noexcept { return m_qTiingoWebInquiry.size(); }
	void PushTiingoInquiry(WebInquiry inquiry) { m_qTiingoWebInquiry.push(inquiry); }
	WebInquiry GetTiingoInquiry(void);

	void StopReceivingWebSocket(void);

	// ���ݿ����
	bool LoadOption(void);
	bool LoadWorldExchangeDB(void); // װ�����罻������Ϣ
	bool LoadStockDB(void);
	bool LoadWorldChoicedStock(void);
	virtual bool UpdateCountryListDB(void);
	virtual bool UpdateStockProfileDB(void);
	virtual bool UpdateStockDayLineDB(void);
	virtual bool UpdateForexSymbolDB(void);
	virtual bool UpdateForexExchangeDB(void);
	virtual bool UpdateInsiderTransactionDB(void);
	virtual bool UpdateEconomicCalendarDB(void);
	virtual bool UpdateTiingoStockDB(void);
	virtual bool UpdateTiingoIndustry(void);
	virtual bool UpdateSICIndustry(void);
	virtual bool UpdateNaicsIndustry(void);

	bool LoadForexExchange(void);
	bool LoadForexSymbol(void);
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

	bool ConnectTiingoIEXWebSocket(void);
	bool ConnectTiingoCryptoWebSocket(void);
	bool ConnectTiingoForexWebSocket(void);

	bool SendTiingoIEXWebSocketMessage(void);
	bool SendTiingoCryptoWebSocketMessage(void);
	bool SendTiingoForexWebSocketMessage(void);

	bool TaskProcessWebSocketData(void);
	bool ProcessFinnhubWebSocketData();
	bool ProcessOneFinnhubWebSocketData(shared_ptr<string> data);
	bool ProcessTiingoIEXWebSocketData();
	bool ProcessOneTiingoIEXWebSocketData(shared_ptr<string> data);
	bool ProcessTiingoCryptoWebSocketData();
	bool ProcessOneTiingoCryptoWebSocketData(shared_ptr<string> data);
	bool ProcessTiingoForexWebSocketData();
	bool ProcessOneTiingoForexWebSocketData(shared_ptr<string> data);

	bool TaskUpdateWorldStockFromWebSocketData(void);
	bool UpdateWorldStockFromTiingoIEXWebSocketData(CTiingoIEXWebSocketDataPtr pTiingoIEXbData);
	bool UpdateWorldStockFromFinnhubWebSocketData(CFinnhubWebSocketDataPtr pFinnhubData);

	int GetWebSocketDataReceivedPerSecond(void) { return m_iWebSocketDataReceivedPerSecond; }
	void ClearWebSocketDataReceived(void) { m_iWebSocketDataReceivedPerSecond = 0; }

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
	long m_lCurrentUpdateInsiderTransactionPos;
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

	vector<CTiingoStockPtr> m_vTiingoStock;
	map<CString, long> m_mapTiingoStock;
	long m_lLastTotalTiingoStock;

	bool m_fFinnhubSymbolUpdated; // ÿ�ո��¹�˾�����
	bool m_fFinnhubStockProfileUpdated; // ÿ�ո��¹�˾���
	bool m_fFinnhubDayLineUpdated; // ÿ�ո��¹�˾��������
	bool m_fFinnhubForexExchangeUpdated; // ÿ�ո���Forex������
	bool m_fFinnhubForexSymbolUpdated; // ÿ�ո���Forex����������
	bool m_fFinnhubForexDayLineUpdated; // ÿ�ո���Forex��������
	bool m_fFinnhubPeerUpdated; // ÿ�¸���Peers����
	bool m_fFinnhubInsiderTransactionUpdated; // ÿ�¸���Peers����
	bool m_fFinnhubEconomicCalendarUpdated; // ÿ�ո��¾�����������
	bool m_fFinnhubEPSSurpriseUpdated;

	bool m_fTiingoSymbolUpdated; // ÿ�ո��¹�˾�����
	bool m_fTiingoDayLineUpdated; // ÿ�ո��¹�˾��������

	vector<CTiingoIndustryPtr> m_vTiingoIndustry;
	map<CString, long> m_mapTiingoIndustry;
	long m_lLastTotalTiingoIndustry;

	vector<CSICIndustryPtr> m_vSICIndustry;
	map<CString, long> m_mapSICIndustry;
	long m_lLastTotalSICIndustry;

	vector<CNaicsIndustryPtr> m_vNaicsIndustry;
	map<CString, long> m_mapNaicsIndustry;
	long m_lLastTotalNaicsIndustry;

	// WebSocket����
	WebSocket m_FinnhubWebSocket;
	WebSocket m_TiingoIEXWebSocket;
	WebSocket m_TiingoCryptoWebSocket;
	WebSocket m_TiingoForexWebSocket;

	int m_iTiingoIEXSubscriptionId;
	int m_iTiingoCryptoSubscriptionId;
	int m_iTiingoForexSubscriptionId;

	queue<CFinnhubWebSocketDataPtr> m_qFinnhubWebSocketData;
	queue<CTiingoCryptoWebSocketDataPtr> m_qTiingoCryptoWebSocketData;
	queue<CTiingoForexWebSocketDataPtr> m_qTiingoForexWebSocketData;
	queue<CTiingoIEXWebSocketDataPtr> m_qTiingoIEXWebSockerData;

	size_t m_iWebSocketDataReceivedPerSecond;
	string m_strMessage;

	//
	bool m_fRebulidDayLine;
	// �ؽ�������ʷ���ݡ�
};

typedef shared_ptr<CWorldMarket> CWorldMarketPtr;
