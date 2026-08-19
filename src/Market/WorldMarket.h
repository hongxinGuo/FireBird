#pragma once

#include"VirtualMarket.h"

class CTiingoIEXSocket;
class CFinnhubSocket;

class CMarketStatus;
class CMarketHoliday;

class CTiingoStock;

using std::literals::chrono_literals::operator ""h;
using std::literals::chrono_literals::operator ""min;
using std::literals::chrono_literals::operator ""s;

using std::atomic_int;

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
	local_seconds GetResetTime() override;

	void PrepareToCloseMarket() final;
	void CloseAllThread();

	void Reset();
	void ResetFinnhub();
	void ResetTiingo();
	void ResetDataContainer();

	bool IsTimeToResetSystem(local_seconds ls) final;
	int ProcessTask() override; // 每日定时任务调度,由ScheduleTask调度
	int ProcessCurrentImmediateTask() override; // 即时任务调度，由ScheduleTask调度

	// 各项任务
	void TaskCreateTask();
	void TaskResetMarket();
	bool TaskCheckMarketReady();
	void TaskProcessWebSocketData();
	void TaskMonitorWebSocket();
	void TaskUpdateWorldMarketDB();

	bool TaskUpdateTiingoIndustry();
	bool TaskUpdateSicIndustry();
	bool TaskUpdateNaicsIndustry();
	bool TaskRebuildTiingoStockSplitDB();
	bool TaskRebuildTiingoIndustryRS();

	int TaskUpdateTiingoStockDayLineDB(std::stop_token st);

	bool TaskUpdateForexDayLineDB(std::stop_token st);
	bool TaskUpdateCryptoDayLineDB(std::stop_token st);

	void TaskCreateTiingoTradeDayDayLine();
	void TaskProcessTiingoDayLine();

	void TaskPerSecond();

	void TaskMainTainTiingoDayLineDB();

	bool UpdateEPSSurpriseDB(std::stop_token st);
	void UpdateSECFilingsDB(std::stop_token st);

	void TaskCalculateNasdaq100MA200UpDownRate(); // 计算Nasdaq100 200日平均线位于收盘价之上的百分比
	concurrencpp::result<bool> LoadNasdaq100StocksDayLine(std::stop_token st);
	void CalculateNasdaq100StocksMA(int length) const;
	void calculateNasdaq100MA200UpDownRate();

	void calculateStockYearHigherRate(); // 计算股票年内再次新高的数量和比例
	void AddNewHighHigher(long lNewHighHigher) { m_iNewHighHigher += lNewHighHigher; }
	long GetNewHighHigher() const { return m_iNewHighHigher; }
	void AddNoNewHighHigher(long lNoNewHighHigher) { m_iNoNewHighHigher += lNoNewHighHigher; }
	long GetNoNewHighHigher() const { return m_iNoNewHighHigher; }
	void ResetNewHighHigher() {
		m_iNewHighHigher = 0;
		m_iNoNewHighHigher = 0;
	}

	// 各种状态

	static bool UpdateToken();

	// 数据库操作
	virtual bool UpdateCompanyNewsDB(std::stop_token st);
	virtual bool UpdateFinnhubStockDayLineDB(std::stop_token st);
	virtual bool UpdateInsiderSentimentDB(std::stop_token st);
	virtual bool UpdateTiingoIndustry(std::stop_token st);
	virtual bool UpdateSicIndustry();
	virtual bool UpdateNaicsIndustry();

	void RebuildStockDayLineDB();
	void RebuildEPSSurprise();
	void RebuildPeer();
	void RebuildBasicFinancial();
	void RebuildTiingoStockSplitDB(std::stop_token st);

	void UpdateTiingoStockDayLine(local_days startDate);
	void UpdateTiingoAllStockDayLine();

	void RebuildIndustryRS(std::stop_token st);
	void BuildIndustry();
	void CalculateIndustryTotalValue();
	void CalculateStockTotalValue(const vector<shared_ptr<CTiingoStock>>& vStocks);

	vector<string> GetFinnhubWebSocketSymbols();

	static void DisconnectAllWebSocket(); // 停止WebSocket。此函数等待其停止后方返回。是系统退出前的准备工作。

	static void ProcessWebSocketData();
	static void ProcessFinnhubWebSocketData();
	static void ProcessTiingoIEXWebSocketData();
	static void ProcessTiingoCryptoWebSocketData();
	static void ProcessTiingoForexWebSocketData();

	void UpdateFinnhubStockFromWebSocket();
	void UpdateFinnhubStockFromTiingoIEXSocket(const shared_ptr<CTiingoIEXSocket>& pTiingoIEXbData);
	void UpdateFinnhubStockFromFinnhubSocket(const shared_ptr<CFinnhubSocket>& pFinnhub);

	void UpdateMarketStatus(const shared_ptr<vector<CMarketStatus>>& pv) const;
	void UpdateMarketHoliday(const shared_ptr<vector<CMarketHoliday>>& pv) const;

	void DeleteTiingoDelistedStock(std::stop_token st);
	static void DeleteTiingoDayLine(const shared_ptr<CTiingoStock>& pStock);
	static void DeleteTiingoFinancialStatement(const shared_ptr<CTiingoStock>& pStock);

	bool IsReadyToInquireWebData() override { return !IsResetTime(); }

	void SetPermitUpdateTiingoFundamentalDefinitionDB(bool fFlag) noexcept { m_fPermitUpdateTiingoFundamentalDefinitionDB = fFlag; }
	bool IsPermitUpdateTiingoFundamentalDefinitionDB() const noexcept { return m_fPermitUpdateTiingoFundamentalDefinitionDB; }

	bool IsBuildTodayTiingoDayLine() const noexcept { return m_bBuildTodayTiingoDayLine; }
	void SetBuildTodayTiingoDayLine(bool fFlag) noexcept { m_bBuildTodayTiingoDayLine = fFlag; }
	bool IsDeleteTiingoDelistedStock() const noexcept { return m_bDeleteTiingoDelistedStock; }
	void SetDeleteTiingoDelistedStock(bool fFlag) noexcept { m_bDeleteTiingoDelistedStock = fFlag; }

	void ChangeToPrevStock();
	void ChangeToNextStock();

protected:
	long m_lCurrentUpdateDayLinePos{ 0 };
	long m_lCurrentUpdateEPSSurprisePos{ 0 };

	shared_ptr<vector<CMarketStatus>> m_pvMarketStatus;
	shared_ptr<vector<CMarketHoliday>> m_pvMarketHoliday;

	bool m_bFinnhubWebSiteAccessible{ true }; // 由于finnhub.io不时被墙，故而需要此标识。

	bool m_fPermitUpdateTiingoFundamentalDefinitionDB{ false };
	bool m_bBuildTodayTiingoDayLine{ false };
	bool m_bDeleteTiingoDelistedStock{ false };

	vector<shared_ptr<CTiingoStock>> m_vNasdaq100TiingoStock;
	atomic_int m_iNewHighHigher{ 0 };
	atomic_int m_iNoNewHighHigher{ 0 };

	array<vector<shared_ptr<CTiingoStock>>, 1000> m_aTiingoIndustryCode; // 行业代码，SIC三位代码共1000个

private:
	// 各thread的std::jthread变量，用于自动结束线程。
	std::jthread m_jtUpdateAlpacaStockDB;
	std::jthread m_jtRebuildStockSplitDB;

	std::jthread m_jtUpdateFinnhubIndustryDB;
	std::jthread m_jtUpdateFinnhubForexExchangeDB;
	std::jthread m_jtUpdateFinnhubCryptoExchangeDB;
	std::jthread m_jtUpdateFinnhubForexSymbolDB;
	std::jthread m_jtUpdateFinnhubCryptoSymbolDB;
	std::jthread m_jtUpdateFinnhubInsiderTransactionDB;
	std::jthread m_jtUpdateFinnhubInsiderSentimentDB;
	std::jthread m_jtUpdateFinnhubCompanyNewsDB;
	std::jthread m_jtUpdateFinnhubEconomicCalendarDB;
	std::jthread m_jtUpdateFinnhubStockDayLineDB;
	std::jthread m_jtUpdateFinnhubEPSSurpriseDB;
	std::jthread m_jtUpdateSECFilingsDB;
	std::jthread m_jtUpdateFinnhubStockProfileDB;
	std::jthread m_jtUpdateCryptoDayLineDB;
	std::jthread m_jtUpdateForexDayLineDB;

	std::jthread m_jtUpdateTiingoStockDayLineDB;
	std::jthread m_jtProcessTiingoDayLine;
	std::jthread m_jtUpdateTiingoStockProfileDB;
	std::jthread m_jtUpdateTiingoCryptoSymbolDB;
	std::jthread m_jtUpdateTiingoFundamentalDefinitionDB;
	std::jthread m_jtUpdateTiingoFinancialStateDB;
	std::jthread m_jtCreateTiingoTradeDayDayLineDB;
	std::jthread m_jtUpdateTiingoIndustryDB;
	std::jthread m_jtUpdateTiingoChosenStockDB;
	std::jthread m_jtDeleteDelistedTiingoStock;
	std::jthread m_jtRebuildTiingoIndustryRS;
	std::jthread m_jtUpdateNaicsIndustryDB;
	std::jthread m_jtUpdateSicIndustryDB;

	std::jthread m_jtUpdateFinnhubInaccessibleExchangeDB;
	std::jthread m_jtUpdateTiingoInaccessibleStockDB;
	std::jthread m_jtCalculateNasdaq100;
};

using CWorldMarketPtr = shared_ptr<CWorldMarket>;

extern CWorldMarketPtr gl_pWorldMarket; // 股票市场。 单一实例变量，仅允许存在一个实例。
