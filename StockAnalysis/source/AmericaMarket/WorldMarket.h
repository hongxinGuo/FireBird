#pragma once

#include"VirtualMarket.h"

#include"FinnhubFactory.h"
#include"TiingoFactory.h"
#include"QuandlFactory.h"

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

#include"FinnhubWebSocket.h"
#include"TiingoIEXWebSocket.h"
#include"TiingoForexWebSocket.h"
#include"TiingoCryptoWebSocket.h"

#include"FinnhubDataSource.h"
#include"TiingoDataSource.h"

using namespace std;
#include<map>
#include<vector>
#include<queue>
#include<atomic>
#include<semaphore>
#include<semaphore>

extern binary_semaphore gl_UpdateWorldMarketDB;  // 此信号量用于更新WorldMarket数据库

class CWorldMarket : public CVirtualMarket {
public:
	DECLARE_DYNCREATE(CWorldMarket)
		CWorldMarket();
	// 只能有一个实例,不允许赋值、拷贝
	CWorldMarket(const CWorldMarket&) = delete;
	CWorldMarket& operator=(const CWorldMarket&) = delete;
	CWorldMarket(const CWorldMarket&&) noexcept = delete;
	CWorldMarket& operator=(const CWorldMarket&&) noexcept = delete;
	virtual ~CWorldMarket();
	virtual void ResetMarket(void) override final;

	virtual bool PreparingExitMarket(void) override final;

	void Reset(void);
	void ResetFinnhub(void);
	void ResetQuandl(void);
	void ResetTiingo(void);
	void ResetDataClass(void);
	virtual bool IsTimeToResetSystem(long lCurrentTime)  override final { if ((lCurrentTime > 165759) && (lCurrentTime < 170501)) return true; else return false; }

	virtual bool SchedulingTask(void) override final;
	// 由程序的定时器调度，大约每100毫秒一次
	void ProcessMessageAndReceivedData(long lCurrentTime);

	bool SchedulingTaskPerSecond(long lSecond, long lCurrentTime);
	bool SchedulingTaskPer10Seconds(long lCurrentTime);
	bool SchedulingTaskPerMinute(long lCurrentTime);
	bool SchedulingTaskPer5Minute(long lCurrentTime);
	bool SchedulingTaskPerHour(long lCurrentTime);
	bool TaskResetMarket(long lCurrentTime);

	virtual bool TaskUpdateTiingoIndustry(void);
	virtual bool TaskUpdateSICIndustry(void);
	virtual bool TaskUpdateNaicsIndustry(void);

	bool TaskUpdateStockProfileDB(void);
	bool TaskUpdateCompanyNewsDB(void);
	bool TaskUpdateBasicFinancialDB(void);
	bool TaskUpdateDayLineDB(void);
	bool TaskUpdateForexExchangeDB(void);
	bool TaskUpdateForexSymbolDB(void);
	bool TaskUpdateForexDayLineDB(void);
	bool TaskUpdateCryptoExchangeDB(void);
	bool TaskUpdateFinnhubCryptoSymbolDB(void);
	bool TaskUpdateCryptoDayLineDB(void);
	bool TaskUpdateCountryListDB(void);
	bool TaskUpdateEPSSurpriseDB(void);
	bool TaskUpdateEconomicCalendarDB(void);
	bool TaskUpdateInsiderTransactionDB(void);
	bool TaskUpdateInsiderSentimentDB(void);
	bool TaskUpdateTiingoStockDB(void);
	bool TaskUpdateTiingoCryptoSymbolDB(void);

	bool TaskCheckSystemReady(void);

	bool TaskUpdateDayLineStartEndDate(void);

	void ClearEconomicCanendar(void) { m_dataFinnhubEconomicCalendar.Reset(); }
	bool UpdateEconomicCalendar(vector<CEconomicCalendarPtr> vEconomicCalendar) { return m_dataFinnhubEconomicCalendar.Update(vEconomicCalendar); }

	// 各种状态
	bool IsFinnhubInquiring(void) noexcept { return gl_pDataSourceFinnhub->IsInquiring(); }
	void SetFinnhubInquiring(bool fFlag) noexcept { gl_pDataSourceFinnhub->SetInquiring(fFlag); }
	void SetFinnhubDataReceived(bool fFlag) noexcept { gl_pDataSourceFinnhub->SetDataReceived(fFlag); }
	bool IsFinnhubDataReceived(void) noexcept { return gl_pDataSourceFinnhub->IsDataReceived(); }

	bool IsTiingoInquiring(void) noexcept { return gl_pDataSourceTiingo->IsInquiring(); }
	void SetTiingoInquiring(bool fFlag) noexcept { gl_pDataSourceTiingo->SetInquiring(fFlag); }
	void SetTiingoDataReceived(bool fFlag) noexcept { gl_pDataSourceTiingo->SetDataReceived(fFlag); }
	bool IsTiingoDataReceived(void) noexcept { return gl_pDataSourceTiingo->IsDataReceived(); }

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

	bool IsCompanyNewsNeedUpdate(void) { return m_dataWorldStock.IsCompanyNewsNeedUpdate(); }
	bool IsBasicFinancialNeedUpdate(void) { return m_dataWorldStock.IsBasicFinancialNeedUpdate(); }

	CWorldStockPtr GetChoicedStock(long lIndex) { return m_dataChoicedStock.GetStock(lIndex); }
	size_t GetChoicedStockSize(void) noexcept { return m_dataChoicedStock.GetSize(); }

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
	void AddFinnhubCryptoSymbol(CFinnhubCryptoSymbolPtr pCryptoSymbol) { m_dataFinnhubCryptoSymbol.Add(pCryptoSymbol); }
	bool DeleteFinnhubCryptoSymbol(CFinnhubCryptoSymbolPtr pCryptoSysbol) { return m_dataFinnhubCryptoSymbol.Delete(pCryptoSysbol); }
	CFinnhubCryptoSymbolPtr GetFinnhubCryptoSymbol(long lIndex) { return m_dataFinnhubCryptoSymbol.GetCryptoSymbol(lIndex); }
	CFinnhubCryptoSymbolPtr GetFinnhubCryptoSymbol(CString strSymbol) { return m_dataFinnhubCryptoSymbol.GetCryptoSymbol(strSymbol); }
	size_t GetFinnhubCryptoSymbolSize(void) noexcept { return m_dataFinnhubCryptoSymbol.GetCryptoSymbolSize(); }

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
	CCountryPtr GetCountry(CString strCountry) { return m_dataFinnhubCountry.GetCountry(strCountry); }

	size_t GetFinnhubInquiryQueueSize(void) noexcept { return gl_pDataSourceFinnhub->GetInquiryQueueSize(); }
	void StoreFinnhubInquiry(CVirtualProductWebDataPtr p) { gl_pDataSourceFinnhub->StoreInquiry(p); }
	CVirtualProductWebDataPtr GetFinnhubInquiry(void) { return gl_pDataSourceFinnhub->GetInquiry(); }

	CString GetCurrentFunction(void) { return m_strCurrentFunction; }
	void SetCurrentFunction(CString str) { m_strCurrentFunction = str; }

	bool IsNeedUpdateForexExchangeDB(void) noexcept { return m_dataFinnhubForexExchange.IsNeedUpdate(); }
	bool IsNeedUpdateForexSymbolDB(void) noexcept { return m_dataFinnhubForexSymbol.IsNeedUpdate(); }
	bool IsNeedUpdateCryptoExchangeDB(void) noexcept { return m_dataFinnhubCryptoExchange.IsNeedUpdate(); }
	bool IsNeedUpdateCryptoSymbolDB(void) noexcept { return m_dataFinnhubCryptoSymbol.IsNeedUpdate(); }
	bool IsNeedUpdateInsiderTransactionDB(void) noexcept { return m_dataWorldStock.IsNeedSaveInsiderTransaction(); }
	bool IsNeedUpdateInsiderSentimentDB(void) noexcept { return m_dataWorldStock.IsNeedSaveInsiderSentiment(); }
	bool IsNeedSaveStockDayLineDB(void) noexcept { return m_dataWorldStock.IsNeedSaveDayLine(); }
	bool IsNeedUpdateExonomicCalendarDB(void) noexcept { return m_dataFinnhubEconomicCalendar.IsNeedUpdate(); }

	bool IsNeedUpdateTiingoStock(void) noexcept { return m_dataTiingoStock.IsNeedUpdate(); }
	bool IsNeedUpdateTiingoCryptoSymbol(void) noexcept { return m_dataTiingoCryptoSymbol.IsNeedUpdate(); }

	bool UpdateToken(void);

	// 数据库操作
	bool LoadWorldExchangeDB(void) { return m_dataFinnhubStockExchange.LoadDB(); }

	bool LoadStockDB(void) { return m_dataWorldStock.LoadDB(); }
	bool LoadWorldChoicedStock(void) { return m_dataChoicedStock.LoadDB(); }

	virtual bool UpdateCountryListDB(void) { return m_dataFinnhubCountry.UpdateDB(); }
	virtual bool UpdateStockProfileDB(void) { return m_dataWorldStock.UpdateProfileDB(); }
	virtual bool UpdateCompanyNewsDB(void);
	virtual bool UpdateBasicFinancialDB(void) { return m_dataWorldStock.UpdateBasicFinancialDB(); }
	virtual bool UpdateStockDayLineDB(void);
	virtual bool UpdateForexSymbolDB(void) { return m_dataFinnhubForexSymbol.UpdateDB(); }
	virtual bool UpdateForexExchangeDB(void) { return m_dataFinnhubForexExchange.UpdateDB(); }
	virtual bool UpdateFinnhubCryptoSymbolDB(void) { return m_dataFinnhubCryptoSymbol.UpdateDB(); }
	virtual bool UpdateCryptoExchangeDB(void) { return m_dataFinnhubCryptoExchange.UpdateDB(); }
	virtual bool UpdateInsiderTransactionDB(void);
	virtual bool UpdateInsiderSentimentDB(void);
	virtual bool UpdateEconomicCalendarDB(void) { return m_dataFinnhubEconomicCalendar.UpdateDB(); }
	virtual bool UpdateTiingoStockDB(void) { return m_dataTiingoStock.UpdateDB(); }
	virtual bool UpdateTiingoCryptoSymbolDB(void) { return m_dataTiingoCryptoSymbol.UpdateDB(); }
	virtual bool UpdateTiingoIndustry(void);
	virtual bool UpdateSICIndustry(void);
	virtual bool UpdateNaicsIndustry(void);

	bool LoadForexExchange(void) { return m_dataFinnhubForexExchange.LoadDB(); }
	bool LoadFinnhubForexSymbol(void) { return m_dataFinnhubForexSymbol.LoadDB(); }
	bool LoadCryptoExchange(void) { return m_dataFinnhubCryptoExchange.LoadDB(); }
	bool LoadFinnhubCryptoSymbol(void) { return m_dataFinnhubCryptoSymbol.LoadDB(); }
	bool LoadWorldChoicedForex(void) { return m_dataChoicedForex.LoadDB(); }
	bool LoadWorldChoicedCrypto(void) { return m_dataChoicedCrypto.LoadDB(); }

	bool LoadCountryDB(void) { return m_dataFinnhubCountry.LoadDB(); }
	bool LoadEconomicCalendarDB(void) { return m_dataFinnhubEconomicCalendar.LoadDB(); }

	bool LoadTiingoStock(void) { return m_dataTiingoStock.LoadDB(); }
	bool LoadTiingoCryptoSymbol(void) { return m_dataTiingoCryptoSymbol.LoadDB(); }

	bool RebuildStockDayLineDB(void);
	virtual bool UpdateStockDayLineStartEndDate(void);
	bool RebuildEPSSurprise(void);
	bool RebuildPeer(void);
	bool RebuildBasicFinancial(void);

	bool SortStock(void) { return m_dataWorldStock.SortStock(); }

	vector<CString> GetFinnhubWebSocketSymbolVector(void);
	vector<CString> GetTiingoIEXWebSocketSymbolVector(void);
	vector<CString> GetTiingoCryptoWebSocketSymbolVector(void);
	vector<CString> GetTiingoForexWebSocketSymbolVector(void);

	bool StartWebSocket(void);
	void StartFinnhubWebSocket(void);
	void StartTiingoWebSocket(void);
	bool RestartWebSocket(void);
	void RestartFinnhubWebSocket(void);
	void RestartTiingoWebSocket(void);
	void StopReceivingWebSocket(void);
	void DeconnectingAllWebSocket(void);

	bool TaskProcessWebSocketData(void);
	bool ProcessFinnhubWebSocketData();
	bool ProcessTiingoIEXWebSocketData();
	bool ProcessTiingoCryptoWebSocketData();
	bool ProcessTiingoForexWebSocketData();

	bool TaskUpdateWorldStockFromWebSocket(void);
	bool UpdateWorldStockFromTiingoIEXSocket(CTiingoIEXSocketPtr pTiingoIEX);
	bool UpdateWorldStockFromFinnhubSocket(CFinnhubSocketPtr pFinnhub);

	// check function

protected:
	long m_lCurrentUpdateDayLinePos; // 由于更新一次日线数据超过24小时，故而将此计数器声明为类变量，且无需每日重置。
	long m_lCurrentUpdateEPSSurprisePos; // 此变量无需每日更新

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

	CString m_strCurrentFunction; // 当前任务和处理的证券名称

	// WebSocket数据
	CFinnhubWebSocket m_finnhubWebSocket;
	CTiingoIEXWebSocket m_tiingoIEXWebSocket;
	CTiingoForexWebSocket m_tiingoForexWebSocket;
	CTiingoCryptoWebSocket m_tiingoCryptoWebSocket;

	int m_iWebSocketReceivedDataPerSecond; // 每秒接收到的数据量
	string m_strMessage;

	//
	bool m_fRebulidDayLine;	// 重建日线历史数据。

	bool m_bFinnhubWebSiteAccessible; // 由于finnhub.io不时被墙，故而需要此标识。

private:
};

typedef shared_ptr<CWorldMarket> CWorldMarketPtr;

extern CWorldMarketPtr gl_pWorldMarket;// 股票市场。 单一实例变量，仅允许存在一个实例。
