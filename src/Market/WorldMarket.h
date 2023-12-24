#pragma once

#include"VirtualMarket.h"

#include"ContainerFinnhubStockExchange.h"
#include"ContainerFinnhubForexExchange.h"
#include"ContainerFinnhubCryptoExchange.h"

#include"ContainerWorldStock.h"

#include "FinnhubWebSocket.h"
#include "TiingoIEXWebSocket.h"

#include"FinnhubCryptoSymbol.h"
#include"FinnhubForexSymbol.h"

#include "MarketStatus.h"
#include"MarketHoliday.h"

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
	void ResetMarket() final;

	void PreparingExitMarket() final;

	void Reset();
	void ResetFinnhub();
	static void ResetQuandl();
	static void ResetTiingo();
	void ResetDataClass();

	bool IsTimeToResetSystem(long lCurrentTime) final {
		if ((lCurrentTime > 165759) && (lCurrentTime < 170501)) return true;
		return false;
	}

	bool ProcessTask(long lCurrentTime) override; // 每日定时任务调度,由SchedulingTaskPerSecond调度

	void TaskCreateTask(long lCurrentTime);
	void TaskResetMarket(long lCurrentTime);
	bool TaskCheckMarketReady(long lCurrentTime);
	void TaskProcessWebSocketData(long lCurrentTime);
	void TaskMonitoringWebSocketStatus(long lCurrentTime);
	void TaskUpdateStockProfileDB(long lCurrentTime);

	virtual bool CreateThreadUpdateTiingoIndustry();
	virtual bool CreateThreadUpdateSICIndustry();
	virtual bool CreateThreadUpdateNaicsIndustry();

	virtual void CreateThreadUpdateCompanyNewsDB();
	virtual void CreateThreadUpdateBasicFinancialDB();
	virtual void CreateThreadUpdateDayLineDB();
	virtual void CreateThreadUpdateForexExchangeDB();
	virtual void CreateThreadUpdateForexSymbolDB();
	virtual void CreateThreadUpdateCryptoExchangeDB();
	virtual void CreateThreadUpdateFinnhubCryptoSymbolDB();
	virtual void CreateThreadUpdateCountryListDB();
	virtual void CreateThreadUpdateEconomicCalendarDB();
	virtual void CreateThreadUpdateInsiderTransactionDB();
	virtual void CreateThreadUpdateInsiderSentimentDB();
	virtual void CreateThreadUpdateTiingoStockDB();
	virtual void CreateThreadUpdateTiingoCryptoSymbolDB();
	virtual void CreateThreadUpdateDayLineStartEndDate();
	virtual void CreateThreadUpdateStockProfileDB();
	virtual void CreateThreadUpdateForexDayLineDB(CForexSymbolPtr pSymbol);
	virtual void CreateThreadUpdateCryptoDayLineDB(CFinnhubCryptoSymbolPtr pSymbol);
	virtual void CreateThreadUpdateEPSSurpriseDB(CWorldStockPtr pStock);
	virtual void CreateThreadUpdateEPSSurpriseDB2();

	bool UpdateForexDayLineDB();
	bool UpdateCryptoDayLineDB();
	bool UpdateEPSSurpriseDB();

	// 各种状态
	CFinnhubStockExchangePtr GetStockExchange(const long lIndex) const { return m_dataFinnhubStockExchange.GetExchange(lIndex); }
	CString GetStockExchangeCode(const long lIndex) const { return m_dataFinnhubStockExchange.GetExchange(lIndex)->m_strCode; }
	long GetStockExchangeSize() const noexcept { return m_dataFinnhubStockExchange.GetExchangeSize(); }

	bool IsUpdateStockProfileDB() { return m_containerStock.IsUpdateProfileDB(); }

	void AddStock(const CWorldStockPtr& pStock) { m_containerStock.Add(pStock); }
	void DeleteStock(const CWorldStockPtr& pStock) { m_containerStock.Delete(pStock); }
	size_t GetStockSize() noexcept { return m_containerStock.Size(); }
	bool IsStock(const CString& strSymbol) const { return m_containerStock.IsSymbol(strSymbol); }
	bool IsStock(const CWorldStockPtr& pStock) const { return IsStock(pStock->GetSymbol()); }
	CWorldStockPtr GetStock(const size_t lIndex) { return m_containerStock.GetStock(lIndex); }
	CWorldStockPtr GetStock(const CString& strSymbol) { return m_containerStock.GetStock(strSymbol); }
	size_t GetStockIndex(const CString& strSymbol) const { return m_containerStock.GetOffset(strSymbol); }
	void SortStock() { m_containerStock.Sort(); }

	bool IsUpdateCompanyNewsDB() { return m_containerStock.IsUpdateCompanyNewsDB(); }
	bool IsUpdateBasicFinancialDB() { return m_containerStock.IsUpdateBasicFinancialDB(); }

	bool IsForexExchange(const CString& strForexExchange) const { return m_dataFinnhubForexExchange.IsForexExchange(strForexExchange); }

	void AddForexExchange(const CString& strForexExchange) { m_dataFinnhubForexExchange.Add(strForexExchange); }
	bool DeleteForexExchange(const CString& strForexExchange) { return m_dataFinnhubForexExchange.Delete(strForexExchange); }
	size_t GetForexExchangeSize() const noexcept { return m_dataFinnhubForexExchange.GetForexExchangeSize(); }
	CString GetForexExchange(const size_t lIndex) const { return m_dataFinnhubForexExchange.GetForexExchange(lIndex); }

	bool IsCryptoExchange(CString strCryptoExchange) const { return m_dataFinnhubCryptoExchange.IsCryptoExchange(strCryptoExchange.GetBuffer()); }

	void AddCryptoExchange(CString strCryptoExchange) { m_dataFinnhubCryptoExchange.Add(strCryptoExchange.GetBuffer()); }
	bool DeleteCryptoExchange(CString strCryptoExchange) { return m_dataFinnhubCryptoExchange.Delete(strCryptoExchange.GetBuffer()); }
	size_t GetCryptoExchangeSize() const noexcept { return m_dataFinnhubCryptoExchange.GetCryptoExchangeSize(); }
	CString GetCryptoExchange(const size_t lIndex) const { return m_dataFinnhubCryptoExchange.GetCryptoExchange(lIndex).c_str(); }

	CString GetCurrentFunction() { return m_strCurrentFunction; }
	void SetCurrentFunction(const CString& str) { m_strCurrentFunction = str; }

	bool IsUpdateForexExchangeDB() const noexcept { return m_dataFinnhubForexExchange.IsNeedUpdate(); }
	bool IsUpdateCryptoExchangeDB() const noexcept { return m_dataFinnhubCryptoExchange.IsNeedUpdate(); }
	bool IsUpdateInsiderTransactionDB() noexcept { return m_containerStock.IsSaveInsiderTransaction(); }
	bool IsUpdateInsiderSentimentDB() noexcept { return m_containerStock.IsSaveInsiderSentiment(); }
	bool IsSaveStockDayLineDB() noexcept { return m_containerStock.IsDayLineNeedSaving(); }
	bool IsUpdateEPSSurpriseDB() noexcept { return m_containerStock.IsSaveEPSSurpriseDB(); }

	static bool UpdateToken();

	// 数据库操作
	bool LoadWorldExchangeDB() { return m_dataFinnhubStockExchange.LoadDB(); }

	void LoadStockDB() { m_containerStock.LoadDB(); }

	virtual void UpdateStockProfileDB() { m_containerStock.UpdateProfileDB(); }
	virtual bool UpdateCompanyNewsDB();
	virtual void UpdateBasicFinancialDB() { m_containerStock.UpdateBasicFinancialDB(); }
	virtual bool UpdateStockDayLineDB();
	virtual bool UpdateForexExchangeDB() { return m_dataFinnhubForexExchange.UpdateDB(); }
	virtual bool UpdateCryptoExchangeDB() { return m_dataFinnhubCryptoExchange.UpdateDB(); }
	virtual bool UpdateInsiderTransactionDB();
	virtual bool UpdateInsiderSentimentDB();
	virtual bool UpdateTiingoIndustry();
	virtual bool UpdateSICIndustry();
	virtual bool UpdateNaicsIndustry();

	void LoadForexExchange() { m_dataFinnhubForexExchange.LoadDB(); }
	void LoadCryptoExchange() { m_dataFinnhubCryptoExchange.LoadDB(); }

	void RebuildStockDayLineDB();
	virtual void UpdateStockDayLineStartEndDate();
	void RebuildEPSSurprise();
	void RebuildPeer();
	void RebuildBasicFinancial();

	vectorString GetFinnhubWebSocketSymbolVector();
	vectorString GetTiingoIEXWebSocketSymbolVector();
	vectorString GetTiingoCryptoWebSocketSymbolVector();
	vectorString GetTiingoForexWebSocketSymbolVector();

	void StartAllWebSocket();
	virtual void StartFinnhubWebSocket();
	virtual void StartTiingoIEXWebSocket();
	virtual void StartTiingoCryptoWebSocket();
	virtual void StartTiingoForexWebSocket();
	static void DisconnectAllWebSocket(); // 停止WebSocket。此函数等待其停止后方返回。是系统退出前的准备工作。
	static void StopFinnhubWebSocketIfTimeOut();
	static void StopTiingoIEXWebSocketIfTimeOut();
	static void StopTiingoCryptoWebSocketIfTimeOut();
	static void StopTiingoForexWebSocketIfTimeOut();

	static void ProcessWebSocketData();
	static void ProcessFinnhubWebSocketData();
	static void ProcessTiingoIEXWebSocketData();
	static void ProcessTiingoCryptoWebSocketData();
	static void ProcessTiingoForexWebSocketData();

	void UpdateWorldStockFromWebSocket();
	void UpdateWorldStockFromTiingoIEXSocket(const CTiingoIEXSocketPtr& pTiingoIEXbData);
	void UpdateWorldStockFromFinnhubSocket(const CFinnhubSocketPtr& pFinnhub);

	void UpdateMarketStatus(const CMarketStatusVectorPtr& pv) const;
	void UpdateMarketHoliday(const CMarketHolidayVectorPtr& pv) const;

	// check function

protected:
	long m_lCurrentUpdateDayLinePos; // 由于更新一次日线数据超过24小时，故而将此计数器声明为类变量，且无需每日重置。
	long m_lCurrentUpdateEPSSurprisePos; // 此变量无需每日更新

	CContainerFinnhubStockExchange m_dataFinnhubStockExchange;
	CContainerFinnhubForexExchange m_dataFinnhubForexExchange;
	CContainerFinnhubCryptoExchange m_dataFinnhubCryptoExchange;

	CContainerWorldStock m_containerStock;

	CMarketStatusVectorPtr m_pvMarketStatus;
	CMarketHolidayVectorPtr m_pvMarketHoliday;

	CString m_strCurrentFunction; // 当前任务和处理的证券名称

	bool m_bFinnhubWebSiteAccessible; // 由于finnhub.io不时被墙，故而需要此标识。
};

using CWorldMarketPtr = shared_ptr<CWorldMarket>;

extern CWorldMarketPtr gl_pWorldMarket; // 股票市场。 单一实例变量，仅允许存在一个实例。
