#pragma once

#include"VirtualMarket.h"

#include"ContainerChinaStock.h"

constexpr int c_SelectedStockStartPosition = 0;

class CChinaMarket : public CVirtualMarket {
public:
	CChinaMarket();
	// 只能有一个实例,不允许赋值。
	CChinaMarket(const CChinaMarket&) = delete;
	CChinaMarket& operator=(const CChinaMarket&) = delete;
	CChinaMarket(const CChinaMarket&&) noexcept = delete;
	CChinaMarket& operator=(const CChinaMarket&&) noexcept = delete;
	~CChinaMarket() override;

	void ResetMarket() final;
	chrono::local_seconds GetResetTime() final { return toLocalTime(30000); } // chinaMarket重置时间为每日91300和92600，无需暂停任务。设为凌晨3点即可。
	void Reset();

	void PrepareToCloseMarket() final;

	bool IsTimeToResetSystem(chrono::local_seconds ls) final;
	bool IsOrdinaryTradeTime() final { return IsOrdinaryTradeTime(GetMarketTime()); } // 日常交易时间
	bool IsOrdinaryTradeTime(chrono::local_seconds lTime) final;
	bool IsWorkingTime() final { return IsWorkingTime(GetMarketTime()); }
	bool IsWorkingTime(chrono::local_seconds lTime) final;
	bool IsDummyTime() final { return !IsWorkingTime(); }
	bool IsDummyTime(chrono::local_seconds lTime) final { return !IsWorkingTime(lTime); }

	int ProcessTask() override; // 每日定时任务调度,由基类的ScheduleTask调度
	int ProcessCurrentImmediateTask() override; // 即时任务调度，由ScheduleTask调度

	// 各种任务
	virtual void TaskCreateTask();
	virtual void TaskExitSystem();
	bool TaskCheckMarketReady();
	virtual bool TaskResetMarket();
	bool TaskCheckSystem();
	void TaskDistributeAndCalculateRTData();
	void TaskProcessAndSaveDayLine();
	void TaskPerSecond(); // 每秒一次的辅助任务
	void TaskAccessoryPerMinuteTask(); // 每分钟重复执行的辅助任务
	void TaskPreparingMarketOpen();
	void TaskSetCurrentStock();

	bool SetCheckActiveStockFlag();
	bool TaskProcessTodayStock();

	bool IsSavingDayLineDBTaskFinished();
	void EnableDayLineDataSource();
	bool CheckFastReceivingData();
	static bool IsWebBusy();
	long long GetHTTPStatus();
	long long GetDayLineHTTPStatus();
	bool IsWebReaTimeDataError();
	long long GetWebRealTimeDataErrorCode();
	bool IsDayLineDataSourceEnable();
	bool IsWebDayLineDataError();
	long long GetWebDayLineDataErrorCode();

	void ProcessTodayStock();
	bool CheckMarketOpen();

	bool TaskUpdateStockProfileDB();
	bool TaskUpdateOptionDB();
	bool TaskUpdateChosenStockDB();

	bool TaskUpdateStockSection(); //

	bool ProcessDayLine();
	void UpdateOneYearStockDayLine();
	void UpdateAllStockDayLine();

	// 各工作线程调用包裹函数

	//处理个股票的实时数据，计算挂单变化等。
	void DistributeRTData();
	void CalculateRTData();

	// 实时数据读取
	string GetSinaStockInquiringStr(long lTotalNumber, bool fUsingTotalStockSet);

	static size_t IncreaseStockInquiringIndex(size_t& lIndex, size_t lEndPosition);

	auto GetCurrentSelectedPosition() const noexcept { return m_lCurrentSelectedPosition; }
	void SetCurrentSelectedPosition(const long lIndex) noexcept { m_lCurrentSelectedPosition = lIndex; }
	auto GetCurrentSelectedStockSet() const noexcept { return m_lCurrentSelectedStockSet; }
	void SetCurrentSelectedStockSet(const long lIndex) noexcept { m_lCurrentSelectedStockSet = lIndex; }
	CChinaStockPtr GetCurrentSelectedStock();

	bool IsChosen10RSStrong1StockSet() const noexcept { return m_fChosen10RSStrong1StockSet; }
	void SetChosen10RSStrong1StockSet(const bool fFlag) noexcept { m_fChosen10RSStrong1StockSet = fFlag; }
	bool IsChosen10RSStrongStockSet() const noexcept { return m_fChosen10RSStrongStockSet; }
	void SetChosen10RSStrongStockSet(const bool fFlag) noexcept { m_fChosen10RSStrongStockSet = fFlag; }
	bool IsCalculateChosen10RS() const noexcept { return m_fCalculateChosen10RS; }
	void SetCalculateChosen10RS(const bool fFlag) noexcept { m_fCalculateChosen10RS = fFlag; }

	// 数据库读取存储操作

	virtual void UpdateOptionDB();
	void LoadOptionDB();
	void UpdateChosenStockDB() const;
	virtual void AppendChosenStockDB();
	void LoadChosenStockDB();

	void DeleteDayLine(chrono::local_days lDate) const;

	static bool CreateStockCodeSet(set<string>& setStockCode, const vector<CVirtualHistoryCandle>* pvData);

	// 股票历史数据处理

	static bool IsDayLineNeedProcess() { return gl_qDayLine.size_approx() > 0; }
	bool IsSelectedStockLoaded() const noexcept { return m_fSelectedStockLoaded; }
	void SetSelectedStockLoaded(const bool fLoad) noexcept { m_fSelectedStockLoaded = fLoad; }

	bool IsCheckingActiveStock() const noexcept { return m_fCheckActiveStock; }
	void SetCheckActiveStock(const bool fFlag) noexcept { m_fCheckActiveStock = fFlag; }

	bool IsTodayTempRTDataLoaded() const noexcept { return m_fTodayTempDataLoaded; }
	void SetTodayTempRTDataLoaded(const bool fFlag) noexcept { m_fTodayTempDataLoaded = fFlag; }

	chrono::local_days GetLastLoginDate() const noexcept { return m_lLastLoginDate; }
	void SetLastLoginDate(const chrono::local_days lDate) noexcept { m_lLastLoginDate = lDate; }
	void SetLastLoginTime(const chrono::local_seconds lTime) noexcept { m_lLastLoginTime = lTime; }

	//处理实时股票变化等
	bool DistributeRTDataToStock(const CWebRTDataPtr& pRTData);

	void ResetCurrentEffectiveRTDataRatio() noexcept {
		m_lRTDataReceivedInCurrentMinute = 0;
		m_lNewRTDataReceivedInCurrentMinute = 0;
	}
	double GetCurrentEffectiveRTDataRatio() const noexcept {
		if (m_lRTDataReceivedInCurrentMinute == 0) return 0.0;
		return static_cast<double>(m_lNewRTDataReceivedInCurrentMinute) / m_lRTDataReceivedInCurrentMinute;
	}

	// 状态反馈
	bool IsRealTimeDataSourceEnable() noexcept;
	void EnableRealTimeDataSource(bool fEnable) noexcept;
	bool IsUsingSinaRTDataReceiver() const noexcept { return m_fUsingSinaRTDataReceiver; }
	bool IsUsingTengxunRTDataReceiver() const noexcept { return m_fUsingTengxunRTDataReceiver; }
	void SetUsingTengxunRTDataReceiver(const bool fFlag) noexcept { m_fUsingTengxunRTDataReceiver = fFlag; }
	int GetCountDownTengxunNumber() const noexcept { return m_iCountDownTengxunNumber; }
	void SetCountDownTengxunNumber(const int iValue) noexcept { m_iCountDownTengxunNumber = iValue; }

	void SetTransactionTime(chrono::sys_seconds time) noexcept { m_tpNewTransactionTime = time; }
	void SetTransactionTime(const time_t tt) noexcept { m_tpNewTransactionTime = ::toSysTime(tt); }
	auto GetTransactionTimePoint() const noexcept { return m_tpNewTransactionTime; }
	time_t GetTransactionTime() const noexcept { return m_tpNewTransactionTime.time_since_epoch().count(); }
	bool IsMarketOpened() const noexcept { return m_fMarketOpened; }
	void SetMarketOpened(const bool fFlag) noexcept { m_fMarketOpened = fFlag; }
	bool IsFastReceivingRTData() const noexcept { return m_fFastReceivingRTData; }
	bool IsRTDataSetCleared() const noexcept { return m_fRTDataSetCleared; }
	void SetRTDataSetCleared(const bool fFlag) noexcept { m_fRTDataSetCleared = fFlag; }
	bool IsUpdateTempDataDB() const noexcept { return m_fUpdateTempDataDB; }
	void SetUpdateTempDataDB(const bool fFlag) noexcept { m_fUpdateTempDataDB = fFlag; }

	// 实时数据需要计算与否和设置
	void SetRTDataNeedCalculate(const bool fFlag) noexcept { m_RTDataNeedCalculate = fFlag; }
	bool IsRTDataNeedCalculate() const noexcept { return m_RTDataNeedCalculate; }

	bool AddChosenStock(const CChinaStockPtr& pStock);
	bool DeleteChosenStock(const CChinaStockPtr& pStock);
	size_t GetChosenStockSize() const { return m_avChosenStock.at(0).size(); }
	size_t GetChosenStockSize(const long lIndex) const { return m_avChosenStock.at(lIndex).size(); }
	void ClearChoiceStockContainer() { m_avChosenStock.at(0).clear(); }

	void SetUpdateOptionDB(const bool fFlag) noexcept { m_fUpdateOptionDB = fFlag; }

	bool IsUpdateOptionDB() const noexcept { return m_fUpdateOptionDB; }

	void SetUpdateChosenStockDB(const bool fFlag) noexcept { m_fUpdateChosenStockDB = fFlag; }
	bool IsUpdateChosenStockDB() const noexcept { return m_fUpdateChosenStockDB; }

	INT64 GetRTDataReceived() const noexcept { return m_llRTDataReceived.load(); }
	void SetRTDataReceived(const INT64 llValue) noexcept { m_llRTDataReceived = llValue; }
	void IncreaseRTDataReceived(const INT64 llValue = 1) noexcept { m_llRTDataReceived += llValue; }

	int GetRTDataCounter() const noexcept { return m_iCurrentDayRTDataCounter; }
	void IncreaseRTDataCounter(const int iValue = 1) noexcept { m_iCurrentDayRTDataCounter += iValue; }

	bool IsTotalStockSetSelected() const noexcept {
		if (m_lCurrentSelectedStockSet == -1) return true;
		return false;
	}

	size_t GetCurrentStockSetSize() const;

	void CreateStock(const string& strStockCode, const string& strStockName, bool fProcessRTData);

	void SetCurrentRSStrongIndex(const long lIndex) noexcept { m_lCurrentRSStrongIndex = lIndex; }

	void ResetEffectiveRTDataRatio();

	void SetDistributeAndCalculateTime(time_t tt) { m_ttDistributeAndCalculateTime = tt; }

public:
	atomic_int64_t m_ttDistributeAndCalculateTime; // 实时数据分配及处理时间

protected:
	vector<vector<CChinaStockPtr>> m_avChosenStock; // 各种选择的股票集。0-9：自选股票集；10-19：10日RS股票集；20-29：股价变化股票集
	long m_lCurrentSelectedPosition; // 当前股票集的位置
	long m_lCurrentRSStrongIndex; // 仅用于传递当前的位置，以用于选择正确的数据表
	long m_lCurrentSelectedStockSet; // 当前选择的股票集（-1为整体股票集，1-10为10日RS特性股票集，以此类推）。
	bool m_fChosen10RSStrong1StockSet; // 本日的10日强势股票集已计算完成
	bool m_fChosen10RSStrongStockSet; // 本日的10日强势股票集已计算完成
	bool m_fCalculateChosen10RS;

	atomic_int64_t m_llRTDataReceived; // 接收到的实时数据数量
	atomic_int m_iCurrentDayRTDataCounter{ 0 };

	bool m_RTDataNeedCalculate;

	bool m_fMarketOpened; // 是否开市
	bool m_fFastReceivingRTData; // 是否开始接收实时数据
	bool m_fRTDataSetCleared; // 实时数据库已清除标识。九点三十分之前为假，之后设置为真。
	bool m_fUpdateTempDataDB; // 存储临时实时数据标识

	chrono::sys_seconds m_tpNewTransactionTime{ chrono::duration<long long>(0) };

	bool m_fUsingSinaRTDataReceiver; // 使用新浪实时数据提取器
	bool m_fUsingTengxunRTDataReceiver; // 使用腾讯实时数据提取器
	int m_iCountDownTengxunNumber;

	// Option各选项
	chrono::local_days m_lLastLoginDate; // 上次登录日期。如果此日期为昨日的话，则无需下载日线历史数据
	chrono::local_seconds m_lLastLoginTime;

	bool m_fSelectedStockLoaded;

	INT64 m_lTotalMarketBuy; // 沪深市场中的A股向上买入金额
	INT64 m_lTotalMarketSell; // 沪深市场中的A股向下卖出金额

	// 系统状态区
	bool m_fCheckActiveStock; // 是否查询今日活跃股票代码
	bool m_fTodayTempDataLoaded; //今日暂存的临时数据是否加载标识。

	// 更新股票代码数据库标识
	atomic_bool m_fUpdateOptionDB;
	bool m_fUpdateChosenStockDB;

private:
	long m_lRTDataReceivedInCurrentMinute; // 每分钟接收到的实时数据数量
	long m_lNewRTDataReceivedInCurrentMinute; // 每分钟接收到的新实时数据数量
};

using CChinaMarketPtr = shared_ptr<CChinaMarket>;

extern CChinaMarketPtr gl_pChinaMarket; // 中国股票市场。所有活跃的股票皆位于其中，单一实例变量，仅允许存在一个实例。
