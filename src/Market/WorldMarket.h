#pragma once

#include"VirtualMarket.h"

#include "TiingoIEXWebSocket.h"
#include "FinnhubWebSocket.h"

#include "MarketStatus.h"
#include"MarketHoliday.h"
import FireBird.System.Configuration;
#include "TiingoStock.h"
import FireBird.Accessory.TimeConvert;

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
	long GetResetTime() override { return gl_systemConfiguration.GetWorldMarketResettingTime(); }

	void PrepareToCloseMarket() final;

	void Reset();
	void ResetFinnhub();
	void ResetQuandl();
	void ResetTiingo() const;
	void ResetDataContainer();

	bool IsTimeToResetSystem(long lCurrentTime) final { return (lCurrentTime > GetPrevTime(GetResetTime(), 0, 2, 1)) && (lCurrentTime < GetNextTime(GetResetTime(), 0, 5, 1)); }
	int ProcessTask(long lCurrentTime) override; // 每日定时任务调度,由ScheduleTask调度
	int ProcessCurrentImmediateTask(long lMarketTime) override; // 即时任务调度，由ScheduleTask调度

	int XferMarketTimeToIndex() override;

	void TaskCreateTask(long lCurrentTime);
	void TaskResetMarket(long lCurrentTime);
	bool TaskCheckMarketReady(long lCurrentTime);
	void TaskProcessWebSocketData(long lCurrentTime);
	void TaskMonitorWebSocket(long lCurrentTime);
	void TaskUpdateWorldMarketDB(long lCurrentTime);
	void TaskUpdateTiingoStock52WeekHighLowDB(long lCurrentTime);

	bool TaskUpdateTiingoIndustry();
	bool TaskUpdateSicIndustry();
	bool TaskUpdateNaicsIndustry();

	bool TaskUpdateTiingoStockDayLineDB();

	bool TaskUpdateForexDayLineDB();
	bool TaskUpdateCryptoDayLineDB();

	void TaskCreateTiingoTradeDayDayLine(long lCurrentTime);
	void TaskProcessTiingoDayLine(long lCurrentTime);
	void TaskTiingoCalculate(long lCurrentTime);

	static void TaskDeleteDelistedStock();

	void TaskPerSecond(long lCurrentTime);

	bool UpdateEPSSurpriseDB();
	void UpdateSECFilingsDB();

	// 各种状态

	static bool UpdateToken();

	// 数据库操作
	virtual bool UpdateCompanyNewsDB();
	virtual bool UpdateFinnhubStockDayLineDB();
	virtual bool UpdateInsiderTransactionDB();
	virtual bool UpdateInsiderSentimentDB();
	virtual bool UpdateTiingoIndustry();
	virtual bool UpdateSicIndustry();
	virtual bool UpdateNaicsIndustry();

	void RebuildStockDayLineDB();
	virtual void UpdateStockDayLineStartEndDate();
	void RebuildEPSSurprise();
	void RebuildPeer();
	void RebuildBasicFinancial();

	vectorString GetFinnhubWebSocketSymbols();

	static void DisconnectAllWebSocket(); // 停止WebSocket。此函数等待其停止后方返回。是系统退出前的准备工作。

	static void ProcessWebSocketData();
	static void ProcessFinnhubWebSocketData();
	static void ProcessTiingoIEXWebSocketData();
	static void ProcessTiingoCryptoWebSocketData();
	static void ProcessTiingoForexWebSocketData();

	void UpdateFinnhubStockFromWebSocket();
	void UpdateFinnhubStockFromTiingoIEXSocket(const CTiingoIEXSocketPtr& pTiingoIEXbData);
	void UpdateFinnhubStockFromFinnhubSocket(const CFinnhubSocketPtr& pFinnhub);

	void UpdateMarketStatus(const CMarketStatussPtr& pv) const;
	void UpdateMarketHoliday(const CMarketHolidaysPtr& pv) const;

	static void DeleteTiingoDelistedStock();
	static void DeleteTiingoDayLine(const CTiingoStockPtr& pStock);
	static void DeleteTiingoFinancialStatement(const CTiingoStockPtr& pStock);

	bool IsReadyToInquireWebData(long lCurrentMarketTime) override { return !IsResetTime(lCurrentMarketTime); }

	void SetPermitUpdateTiingoFundamentalDefinitionDB(bool fFlag) noexcept { m_fPermitUpdateTiingoFundamentalDefinitionDB = fFlag; }
	bool IsPermitUpdateTiingoFundamentalDefinitionDB() const noexcept { return m_fPermitUpdateTiingoFundamentalDefinitionDB; }

protected:
	long m_lCurrentUpdateDayLinePos{ 0 }; // 由于更新一次日线数据超过24小时，故而将此计数器声明为类变量，且无需每日重置。
	long m_lCurrentUpdateEPSSurprisePos{ 0 }; // 此变量无需每日更新

	CMarketStatussPtr m_pvMarketStatus;
	CMarketHolidaysPtr m_pvMarketHoliday;

	bool m_bFinnhubWebSiteAccessible{ true }; // 由于finnhub.io不时被墙，故而需要此标识。

	bool m_fPermitUpdateTiingoFundamentalDefinitionDB{ false };
};

using CWorldMarketPtr = shared_ptr<CWorldMarket>;

extern CWorldMarketPtr gl_pWorldMarket; // 股票市场。 单一实例变量，仅允许存在一个实例。
