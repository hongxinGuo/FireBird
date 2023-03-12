#pragma once

#include"VirtualMarket.h"

#include"DataFinnhubStockExchange.h"
#include"DataFinnhubForexExchange.h"
#include"DataFinnhubCryptoExchange.h"
#include"containerFinnhubForexSymbol.h"
#include"containerFinnhubCryptoSymbol.h"
#include"DataFinnhubCountry.h"
#include"ContainerFinnhubEconomicCalendar.h"

#include"ContainerChosenStock.h"
#include"ContainerChosenForex.h"
#include"containerChosenCrypto.h"

#include"ContainerWorldStock.h"
#include"DataTiingoStock.h"

#include"ContainerTiingoCryptoSymbol.h"

#include "FinnhubWebSocket.h"
#include "TiingoIEXWebSocket.h"

#include"FinnhubCryptoSymbol.h"
#include"FinnhubForexSymbol.h"

#include<semaphore>
using std::binary_semaphore;
using std::dynamic_pointer_cast;

extern binary_semaphore gl_UpdateWorldMarketDB; // 此信号量用于更新WorldMarket数据库

class CWorldMarket : public CVirtualMarket {
public:
	CWorldMarket();
	// 只能有一个实例,不允许赋值、拷贝
	CWorldMarket(const CWorldMarket&) = delete;
	CWorldMarket& operator=(const CWorldMarket&) = delete;
	CWorldMarket(const CWorldMarket&&) noexcept = delete;
	CWorldMarket& operator=(const CWorldMarket&&) noexcept = delete;
	~CWorldMarket() override;
	void ResetMarket(void) final;

	bool PreparingExitMarket(void) final;

	void Reset(void);
	void ResetFinnhub(void);
	void ResetQuandl(void);
	void ResetTiingo(void);
	void ResetDataClass(void);

	bool IsTimeToResetSystem(long lCurrentTime) final {
		if ((lCurrentTime > 165759) && (lCurrentTime < 170501)) return true;
		return false;
	}

	bool SchedulingTask(void) override;

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

	void ClearEconomicCalendar(void) { m_dataFinnhubEconomicCalendar.Reset(); }

	bool UpdateEconomicCalendar(vector<CEconomicCalendarPtr> vEconomicCalendar) { return m_dataFinnhubEconomicCalendar.Update(vEconomicCalendar); }

	// 各种状态
	CFinnhubStockExchangePtr GetStockExchange(const long lIndex) const { return m_dataFinnhubStockExchange.GetExchange(lIndex); }
	CString GetStockExchangeCode(const long lIndex) const { return m_dataFinnhubStockExchange.GetExchange(lIndex)->m_strCode; }
	long GetStockExchangeSize(void) const noexcept { return m_dataFinnhubStockExchange.GetExchangeSize(); }

	bool IsUpdateStockProfileDB(void) { return m_containerStock.IsUpdateProfileDB(); }

	void AddStock(const CWorldStockPtr pStock) { m_containerStock.Add(pStock); }
	bool DeleteStock(const CWorldStockPtr pStock) { return m_containerStock.Delete(pStock); }
	size_t GetStockSize(void) const noexcept { return m_containerStock.Size(); }
	void AddTiingoStock(const CTiingoStockPtr pTiingoStock) { m_dataTiingoStock.Add(pTiingoStock); }
	bool DeleteTiingoStock(const CTiingoStockPtr pStock) { return m_dataTiingoStock.Delete(pStock); }
	size_t GetTotalTiingoStock(void) const noexcept { return m_dataTiingoStock.GetTotalStock(); }
	bool IsStock(const CString strSymbol) const { return m_containerStock.IsSymbol(strSymbol); }
	bool IsStock(const CWorldStockPtr pStock) const { return IsStock(pStock->GetSymbol()); }
	CWorldStockPtr GetStock(const size_t lIndex) { return m_containerStock.GetStock(lIndex); }
	CWorldStockPtr GetStock(const CString strSymbol) { return m_containerStock.GetStock(strSymbol); }
	size_t GetStockIndex(const CString strSymbol) const { return m_containerStock.GetOffset(strSymbol); }
	void SortStock(void) { m_containerStock.Sort(); }

	bool IsUpdateCompanyNewsDB(void) { return m_containerStock.IsUpdateCompanyNewsDB(); }
	bool IsUpdateBasicFinancialDB(void) { return m_containerStock.IsUpdateBasicFinancialDB(); }

	CWorldStockPtr GetChosenStock(const long lIndex) { return m_containerChosenStock.GetStock(lIndex); }
	size_t GetChosenStockSize(void) const noexcept { return m_containerChosenStock.Size(); }

	bool IsTiingoStock(const CString strSymbol) const { return m_dataTiingoStock.IsStock(strSymbol); }
	bool IsTiingoStock(const CWorldStockPtr pStock) const { return m_dataTiingoStock.IsStock(pStock); }
	bool IsTiingoStock(const CTiingoStockPtr pTiingoStock) const { return m_dataTiingoStock.IsStock(pTiingoStock); }
	CTiingoStockPtr GetTiingoStock(const size_t lIndex) const { return m_dataTiingoStock.GetStock(lIndex); }
	CTiingoStockPtr GetTiingoStock(const CString strTicker) const { return m_dataTiingoStock.GetStock(strTicker); }

	bool IsForexExchange(const CString strForexExchange) const { return m_dataFinnhubForexExchange.IsForexExchange(strForexExchange); }

	void AddForexExchange(const CString strForexExchange) { m_dataFinnhubForexExchange.Add(strForexExchange); }
	bool DeleteForexExchange(const CString strForexExchange) { return m_dataFinnhubForexExchange.Delete(strForexExchange); }
	size_t GetForexExchangeSize(void) const noexcept { return m_dataFinnhubForexExchange.GetForexExchangeSize(); }
	CString GetForexExchange(const size_t lIndex) const { return m_dataFinnhubForexExchange.GetForexExchange(lIndex); }

	bool IsForexSymbol(const CString strForexSymbol) const { return m_dataFinnhubForexSymbol.IsSymbol(strForexSymbol); }
	bool IsForexSymbol(const CForexSymbolPtr pForexSymbol) const { return IsForexSymbol(pForexSymbol->GetSymbol()); }
	void AddForexSymbol(const CForexSymbolPtr pForexSymbol) { m_dataFinnhubForexSymbol.Add(pForexSymbol); }
	bool DeleteForexSymbol(const CForexSymbolPtr pForexSymbol) { return m_dataFinnhubForexSymbol.Delete(pForexSymbol); }
	CForexSymbolPtr GetForexSymbol(const size_t lIndex) { return m_dataFinnhubForexSymbol.GetSymbol(lIndex); }
	CForexSymbolPtr GetForexSymbol(CString strSymbol) { return m_dataFinnhubForexSymbol.GetSymbol(strSymbol); }
	size_t GetForexSymbolSize(void) const noexcept { return m_dataFinnhubForexSymbol.Size(); }

	bool IsCryptoExchange(CString strCryptoExchange) const { return m_dataFinnhubCryptoExchange.IsCryptoExchange(strCryptoExchange.GetBuffer()); }

	void AddCryptoExchange(CString strCryptoExchange) { m_dataFinnhubCryptoExchange.Add(strCryptoExchange.GetBuffer()); }
	bool DeleteCryptoExchange(CString strCryptoExchange) { return m_dataFinnhubCryptoExchange.Delete(strCryptoExchange.GetBuffer()); }
	size_t GetCryptoExchangeSize(void) const noexcept { return m_dataFinnhubCryptoExchange.GetCryptoExchangeSize(); }
	CString GetCryptoExchange(const size_t lIndex) const { return m_dataFinnhubCryptoExchange.GetCryptoExchange(lIndex).c_str(); }

	bool IsFinnhubCryptoSymbol(const CString strSymbol) const { return m_dataFinnhubCryptoSymbol.IsSymbol(strSymbol); }
	bool IsFinnhubCryptoSymbol(CFinnhubCryptoSymbolPtr pCryptoSymbol) const { return IsFinnhubCryptoSymbol(pCryptoSymbol->GetSymbol()); }
	void AddFinnhubCryptoSymbol(CFinnhubCryptoSymbolPtr pCryptoSymbol) { m_dataFinnhubCryptoSymbol.Add(pCryptoSymbol); }
	bool DeleteFinnhubCryptoSymbol(const CFinnhubCryptoSymbolPtr pCryptoSymbol) { return m_dataFinnhubCryptoSymbol.Delete(pCryptoSymbol); }
	CFinnhubCryptoSymbolPtr GetFinnhubCryptoSymbol(const size_t lIndex) { return m_dataFinnhubCryptoSymbol.GetSymbol(lIndex); }
	CFinnhubCryptoSymbolPtr GetFinnhubCryptoSymbol(CString strSymbol) { return m_dataFinnhubCryptoSymbol.GetSymbol(strSymbol); }
	size_t GetFinnhubCryptoSymbolSize(void) const noexcept { return m_dataFinnhubCryptoSymbol.Size(); }

	bool IsTiingoCryptoSymbol(const CString strSymbol) const { return m_dataTiingoCryptoSymbol.IsTiingoCryptoSymbol(strSymbol); }
	bool IsTiingoCryptoSymbol(CTiingoCryptoSymbolPtr pCryptoSymbol) const { return IsTiingoCryptoSymbol(pCryptoSymbol->m_strTicker); }
	void AddTiingoCryptoSymbol(const CTiingoCryptoSymbolPtr pCryptoSymbol) { m_dataTiingoCryptoSymbol.Add(pCryptoSymbol); }
	bool DeleteTiingoCryptoSymbol(const CTiingoCryptoSymbolPtr pCryptoSymbol) { return m_dataTiingoCryptoSymbol.Delete(pCryptoSymbol); }
	CTiingoCryptoSymbolPtr GetTiingoCryptoSymbol(const long lIndex) const { return m_dataTiingoCryptoSymbol.GetCryptoSymbol(lIndex); }
	CTiingoCryptoSymbolPtr GetTiingoCryptoSymbol(const CString strSymbol) const { return m_dataTiingoCryptoSymbol.GetCryptoSymbol(strSymbol); }
	size_t GetTiingoCryptoSymbolSize(void) const noexcept { return m_dataTiingoCryptoSymbol.GetCryptoSymbolSize(); }

	size_t GetTotalCountry(void) const noexcept { return m_dataFinnhubCountry.GetTotalCountry(); }
	bool IsCountry(const CString strCountry) const { return m_dataFinnhubCountry.IsCountry(strCountry); }
	bool IsCountry(const CCountryPtr pCountry) const { return m_dataFinnhubCountry.IsCountry(pCountry); }
	void AddCountry(const CCountryPtr pCountry) { m_dataFinnhubCountry.Add(pCountry); }
	bool DeleteCountry(const CCountryPtr pCountry) { return m_dataFinnhubCountry.Delete(pCountry); }
	CCountryPtr GetCountry(const CString strCountry) { return m_dataFinnhubCountry.GetCountry(strCountry); }

	CString GetCurrentFunction(void) { return m_strCurrentFunction; }
	void SetCurrentFunction(const CString str) { m_strCurrentFunction = str; }

	bool IsUpdateForexExchangeDB(void) const noexcept { return m_dataFinnhubForexExchange.IsNeedUpdate(); }
	bool IsUpdateForexSymbolDB(void) noexcept { return m_dataFinnhubForexSymbol.IsUpdateProfileDB(); }
	bool IsUpdateCryptoExchangeDB(void) const noexcept { return m_dataFinnhubCryptoExchange.IsNeedUpdate(); }
	bool IsUpdateCryptoSymbolDB(void) noexcept { return m_dataFinnhubCryptoSymbol.IsUpdateProfileDB(); }
	bool IsUpdateInsiderTransactionDB(void) noexcept { return m_containerStock.IsSaveInsiderTransaction(); }
	bool IsUpdateInsiderSentimentDB(void) noexcept { return m_containerStock.IsSaveInsiderSentiment(); }
	bool IsSaveStockDayLineDB(void) noexcept { return m_containerStock.IsDayLineNeedSaving(); }
	bool IsUpdateEconomicCalendarDB(void) const noexcept { return m_dataFinnhubEconomicCalendar.IsNeedUpdate(); }

	bool IsNeedUpdateTiingoStock(void) const noexcept { return m_dataTiingoStock.IsNeedUpdate(); }
	bool IsNeedUpdateTiingoCryptoSymbol(void) const noexcept { return m_dataTiingoCryptoSymbol.IsNeedUpdate(); }

	bool UpdateToken(void);

	// 数据库操作
	bool LoadWorldExchangeDB(void) { return m_dataFinnhubStockExchange.LoadDB(); }

	bool LoadStockDB(void) { return m_containerStock.LoadDB(); }
	bool LoadWorldChosenStock(void) { return m_containerChosenStock.LoadDB(); }

	virtual bool UpdateCountryListDB(void) { return m_dataFinnhubCountry.UpdateDB(); }
	virtual bool UpdateStockProfileDB(void) { return m_containerStock.UpdateProfileDB(); }
	virtual bool UpdateCompanyNewsDB(void);
	virtual bool UpdateBasicFinancialDB(void) { return m_containerStock.UpdateBasicFinancialDB(); }
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
	bool LoadWorldChosenForex(void) { return m_containerChosenForex.LoadDB(); }
	bool LoadWorldChosenCrypto(void) { return m_containerChosenCrypto.LoadDB(); }

	bool LoadCountryDB(void) { return m_dataFinnhubCountry.LoadDB(); }
	bool LoadEconomicCalendarDB(void) { return m_dataFinnhubEconomicCalendar.LoadDB(); }

	bool LoadTiingoStock(void) { return m_dataTiingoStock.LoadDB(); }
	bool LoadTiingoCryptoSymbol(void) { return m_dataTiingoCryptoSymbol.LoadDB(); }

	bool RebuildStockDayLineDB(void);
	virtual bool UpdateStockDayLineStartEndDate(void);
	bool RebuildEPSSurprise(void);
	bool RebuildPeer(void);
	bool RebuildBasicFinancial(void);

	vectorString GetFinnhubWebSocketSymbolVector(void);
	vectorString GetTiingoIEXWebSocketSymbolVector(void);
	vectorString GetTiingoCryptoWebSocketSymbolVector(void);
	vectorString GetTiingoForexWebSocketSymbolVector(void);

	bool StartAllWebSocket(void);
	void StartFinnhubWebSocket(void);
	void StartTiingoIEXWebSocket(void);
	void StartTiingoCryptoWebSocket(void);
	void StartTiingoForexWebSocket(void);
	void DisconnectAllWebSocket(void); // // 停止WebSocket。此函数是生成工作线程来停止WebSocket，不用等待其停止即返回。用于系统运行中的停止动作。
	void StopWebSocketsIfTimeOut(void); // 停止WebSocket。此函数是生成工作线程来停止WebSocket，不用等待其停止即返回。用于系统运行中的停止动作。
	void StopFinnhubWebSocketIfTimeOut(void);
	void StopTiingoIEXWebSocketIfTimeOut(void);
	void StopTiingoCryptoWebSocketIfTimeOut(void);
	void StopTiingoForexWebSocketIfTimeOut(void);

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
	CContainerFinnhubForexSymbol m_dataFinnhubForexSymbol;
	CContainerFinnhubCryptoSymbol m_dataFinnhubCryptoSymbol;
	CDataFinnhubCountry m_dataFinnhubCountry;
	CContainerFinnhubEconomicCalendar m_dataFinnhubEconomicCalendar;

	CContainerWorldStock m_containerStock;

	CDataTiingoStock m_dataTiingoStock;
	CContainerTiingoCryptoSymbol m_dataTiingoCryptoSymbol;

	CContainerChosenStock m_containerChosenStock;
	CContainerChosenForex m_containerChosenForex;
	CContainerChosenCrypto m_containerChosenCrypto;

	CString m_strCurrentFunction; // 当前任务和处理的证券名称

	bool m_bFinnhubWebSiteAccessible; // 由于finnhub.io不时被墙，故而需要此标识。

private:
	int m_iCount1Hour; // 与五分钟每次的错开11秒钟，与一分钟每次的错开22秒钟
	int m_iCount5Minute; // 与一分钟每次的错开11秒钟
	int m_iCount1Minute; // 与10秒每次的错开1秒钟
	int m_iCount10Second;
};

using CWorldMarketPtr = shared_ptr<CWorldMarket>;

extern CWorldMarketPtr gl_pWorldMarket; // 股票市场。 单一实例变量，仅允许存在一个实例。
