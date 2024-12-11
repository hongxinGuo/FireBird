#pragma once

#include"VirtualMarket.h"
import FireBird.Container.ChinaStock;

import FireBird.HistoryCandle.VirtualExtend;
import FireBird.Stock.ChinaStock;
import FireBird.RelativeStrongReference;
import FireBird.Container.ChinaStockDayLine;
using std::set;
using std::atomic_int64_t;

constexpr int c_SelectedStockStartPosition = 0;
constexpr int c_10DaysRSStockSetStartPosition = 10; // 十日相对强度股票集起始位置（10-19为十日相对强对股票集，共十个）

class CChinaMarket;
using CChinaMarketPtr = shared_ptr<CChinaMarket>;

// 计算股票相对强度线程。此线程调用线程ThreadCalculateRSAtThisDay执行具体任务，最多生成8个工作线程。
UINT ThreadBuildDayLineRS(const CChinaMarketPtr& pMarket, long startCalculatingDate); // 此工作线程返回值为11, 参数为当前最后计算日期
// 计算股票相对强度线程。此线程调用线程ThreadCalculateRSAtThisDate执行具体任务，最多生成8个工作线程。
UINT ThreadBuildWeekLineRS(const CChinaMarketPtr& pMarket, long startCalculatingDate); // 此工作线程返回值为30, 参数为当前最后计算日期

//各种计算用工作线程
// 计算10日强股票集（使用外部pRef提供的参数）
UINT ThreadChoice10RSStrongStockSet(CRSReference* pRef, int iIndex); // 此线程返回值为103
// 计算股票的10日强势与否
UINT ThreadCalculate10RSStrongStock(vector<CChinaStockPtr>* pv10RSStrongStock, const CRSReference* pRef, const CChinaStockPtr& pStock); // 此线程返回值为104
// 计算股票的10日强势与否1
UINT ThreadCalculate10RSStrong1Stock(vector<CChinaStockPtr>* pv10RSStrongStock, const CChinaStockPtr& pStock); // 此线程返回值为105
// 计算股票的10日强势与否2
UINT ThreadCalculate10RSStrong2Stock(vector<CChinaStockPtr>* pv10RSStrongStock, const CChinaStockPtr& pStock); // 此线程返回值为106

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
	long GetResetTime() final { return 30000; } // chinaMarket重置时间为每日91300和92600，无需暂停任务。设为凌晨3点即可。
	void Reset();

	void PrepareToCloseMarket() final;

	bool IsTimeToResetSystem(long lCurrentTime) final;
	bool IsOrdinaryTradeTime() final { return IsOrdinaryTradeTime(GetMarketTime()); } // 日常交易时间
	bool IsOrdinaryTradeTime(long lTime) final;
	bool IsWorkingTime() final { return IsWorkingTime(GetMarketTime()); }
	bool IsWorkingTime(long lTime) final;
	bool IsDummyTime() final { return !IsWorkingTime(); }
	bool IsDummyTime(long lTime) final { return !IsWorkingTime(lTime); }

	int XferMarketTimeToIndex() override;

	int ProcessTask(long lCurrentTime) override; // 每日定时任务调度,由基类的ScheduleTask调度
	int ProcessCurrentImmediateTask(long lMarketTime) override; // 即时任务调度，由ScheduleTask调度

	// 各种任务
	virtual void TaskCreateTask(long lCurrentTime);
	virtual void TaskExitSystem(long lCurrentTime);
	bool TaskCheckMarketReady(long lCurrentTime);
	virtual bool TaskResetMarket(long lCurrentTime);
	void TaskDistributeAndCalculateRTData(long lCurrentTime);
	void TaskProcessAndSaveDayLine(long lCurrentTime);
	void TaskUpdateTempRTDB(long lCurrentTime);
	void TaskLoadCurrentStockHistoryData() const;// 装载当前股票日线
	void TaskPerSecond(long lCurrentTime); // 每秒一次的辅助任务
	void TaskAccessoryPerMinuteTask(long lCurrentTime); // 每分钟重复执行的辅助任务
	void TaskPreparingMarketOpen(long lCurrentTime);
	void TaskChoiceRSSet(long lCurrentTime);
	void TaskSetCurrentStock();

	bool SetCheckActiveStockFlag(long lCurrentTime);
	bool TaskChoice10RSStrong1StockSet(long lCurrentTime);
	bool TaskChoice10RSStrong2StockSet(long lCurrentTime);
	bool TaskChoice10RSStrongStockSet(long lCurrentTime);
	bool TaskProcessTodayStock(long lCurrentTime);

	void ProcessTodayStock();
	bool IsTaskOfSavingDayLineDBFinished();
	bool CheckFastReceivingData(long lCurrentTime);
	static bool IsWebBusy();
	bool CheckMarketOpen(long lCurrentTime);

	bool TaskUpdateStockProfileDB(long lCurrentTime);
	bool TaskUpdateOptionDB(long lCurrentTime);
	bool TaskUpdateChosenStockDB();

	bool TaskUpdateStockSection(); //

	bool ProcessDayLine();

	// 各工作线程调用包裹函数
	virtual void Choice10RSStrongStockSet();

	//处理个股票的实时数据，计算挂单变化等。
	void DistributeRTData();
	void CalculateRTData();

	// 系统状态区
	bool IsFastReceivingRTData() noexcept { return m_fFastReceivingRTData; }

	// 实时数据读取
	CString GetSinaStockInquiringStr(long lTotalNumber, bool fUsingTotalStockSet);
	CString GetNeteaseStockInquiringMiddleStr(long lTotalNumber, bool fUsingTotalStockSet);
	bool CheckValidOfNeteaseDayLineInquiringStr(const CString& str) const;

	static size_t IncreaseStockInquiringIndex(size_t& lIndex, size_t lEndPosition);

	// 得到当前显示股票
	CChinaStockPtr GetCurrentStock() const noexcept { return m_pCurrentStock; }
	void SetCurrentStock(const CString& strStockCode);
	void SetCurrentStock(const CChinaStockPtr& pStock);
	void ClearCurrentStock() noexcept { m_pCurrentStock = nullptr; }
	bool IsCurrentStockChanged() const noexcept { return m_fCurrentStockChanged; }
	void SetCurrentStockChanged(const bool fFlag) noexcept { m_fCurrentStockChanged = fFlag; }

	long GetMinLineOffset(time_t tUTC) const;

	auto GetCurrentSelectedPosition() const noexcept { return m_lCurrentSelectedPosition; }
	void SetCurrentSelectedPosition(const long lIndex) noexcept { m_lCurrentSelectedPosition = lIndex; }
	auto GetCurrentSelectedStockSet() const noexcept { return m_lCurrentSelectedStockSet; }
	void SetCurrentSelectedStockSet(const long lIndex) noexcept { m_lCurrentSelectedStockSet = lIndex; }
	CChinaStockPtr GetCurrentSelectedStock();

	bool IsChosen10RSStrong1StockSet() const noexcept { return m_fChosen10RSStrong1StockSet; }
	void SetChosen10RSStrong1StockSet(const bool fFlag) noexcept { m_fChosen10RSStrong1StockSet = fFlag; }
	bool IsChosen10RSStrong2StockSet() const noexcept { return m_fChosen10RSStrong2StockSet; }
	void SetChosen10RSStrong2StockSet(const bool fFlag) noexcept { m_fChosen10RSStrong2StockSet = fFlag; }
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
	bool TaskLoadTempRTData(long lTheDate, long lCurrentTime);
	virtual void LoadTempRTData(long lTheDate);
	bool LoadDayLine(CContainerChinaDayLine& dataChinaDayLine, long lDate) const;

	static bool ChangeDayLineStockCodeTypeToStandard();

	virtual bool DeleteWeekLine();
	static void DeleteWeekLineBasicInfo();
	static void DeleteWeekLineExtendInfo();
	virtual void DeleteWeekLine(long lMonday);
	void DeleteWeekLineBasicInfo(long lMonday) const;
	void DeleteWeekLineExtendInfo(long lMonday) const;

	void DeleteDayLine(long lDate) const;
	void DeleteDayLineBasicInfo(long lDate) const;
	void DeleteDayLineExtendInfo(long lDate) const;

	virtual bool DeleteCurrentWeekWeekLine();
	static bool DeleteCurrentWeekWeekLineBeforeTheDate(long lCutOffDate);

	bool Load10DaysRSStrong1StockSet();
	bool Load10DaysRSStrong2StockSet();

	bool LoadCalculatingRSOption();
	void SaveCalculatingRSOption() const;

	bool Load10DaysRSStrongStockDB();
	bool LoadOne10DaysRSStrongStockDB(long lIndex);

	virtual bool BuildWeekLineOfCurrentWeek();
	static bool CreateStockCodeSet(set<CString>& setStockCode, vector<CVirtualHistoryCandleExtendPtr>* pvData);
	virtual bool BuildCurrentWeekWeekLineTable(); // 使用周线表构建当前周周线表

	// 股票历史数据处理

	static bool IsDayLineNeedProcess() { return gl_qDayLine.size_approx() > 0; }
	bool IsSelectedStockLoaded() const noexcept { return m_fSelectedStockLoaded; }
	void SetSelectedStockLoaded(const bool fLoad) noexcept { m_fSelectedStockLoaded = fLoad; }

	bool IsCheckingActiveStock() const noexcept { return m_fCheckActiveStock; }
	void SetCheckActiveStock(const bool fFlag) noexcept { m_fCheckActiveStock = fFlag; }

	bool IsTodayTempRTDataLoaded() const noexcept { return m_fTodayTempDataLoaded; }
	void SetTodayTempRTDataLoaded(const bool fFlag) noexcept { m_fTodayTempDataLoaded = fFlag; }

	long GetRSStartDate() const noexcept { return m_lRSStartDate; }
	void SetRSStartDate(const long lDate) noexcept { m_lRSStartDate = lDate; }
	long GetRSEndDate() const noexcept { return m_lRSEndDate; }
	void SetRSEndDate(const long lDate) noexcept { m_lRSEndDate = lDate; }
	long GetLastLoginDate() const noexcept { return m_lLastLoginDate; }
	void SetLastLoginDate(const long lDate) noexcept { m_lLastLoginDate = lDate; }
	void SetLastLoginTime(const long lTime) noexcept { m_lLastLoginTime = lTime; }
	long GetUpdatedDateFor10DaysRS1() const noexcept { return m_lUpdatedDateFor10DaysRS1; }
	void SetUpdatedDateFor10DaysRS1(const long lDate) noexcept { m_lUpdatedDateFor10DaysRS1 = lDate; }
	long GetUpdatedDateFor10DaysRS2() const noexcept { return m_lUpdatedDateFor10DaysRS2; }
	void SetUpdatedDateFor10DaysRS2(const long lDate) noexcept { m_lUpdatedDateFor10DaysRS2 = lDate; }
	long GetUpdatedDateFor10DaysRS() const noexcept { return m_lUpdatedDateFor10DaysRS; }
	void SetUpdatedDateFor10DaysRS(const long lDate) noexcept { m_lUpdatedDateFor10DaysRS = lDate; }

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
	bool IsUsingSinaRTDataReceiver() const noexcept { return m_fUsingSinaRTDataReceiver; }
	bool IsUsingNeteaseRTDataReceiver() const noexcept { return m_fUsingNeteaseRTDataReceiver; }
	void SetUsingNeteaseRTDataReceiver(const bool fFlag) noexcept { m_fUsingNeteaseRTDataReceiver = fFlag; }
	bool IsUsingTengxunRTDataReceiver() const noexcept { return m_fUsingTengxunRTDataReceiver; }
	void SetUsingTengxunRTDataReceiver(const bool fFlag) noexcept { m_fUsingTengxunRTDataReceiver = fFlag; }
	int GetCountDownTengxunNumber() const noexcept { return m_iCountDownTengxunNumber; }
	void SetCountDownTengxunNumber(const int iValue) noexcept { m_iCountDownTengxunNumber = iValue; }

	void SetTransactionTime(chrono::sys_seconds time) noexcept { m_tpNewTransactionTime = time; }
	void SetTransactionTime(const time_t tt) noexcept { m_tpNewTransactionTime = chrono::time_point_cast<chrono::seconds>(chrono::system_clock::from_time_t(tt)); }
	auto GetTransactionTimePoint() const noexcept { return m_tpNewTransactionTime; }
	time_t GetTransactionTime() const noexcept { return m_tpNewTransactionTime.time_since_epoch().count(); }
	bool IsMarketOpened() const noexcept { return m_fMarketOpened; }
	void SetMarketOpened(const bool fFlag) noexcept { m_fMarketOpened = fFlag; }
	bool IsFastReceivingRTData() const noexcept { return m_fFastReceivingRTData; }
	bool IsRTDataSetCleared() const noexcept { return m_fRTDataSetCleared; }
	void SetRTDataSetCleared(const bool fFlag) noexcept { m_fRTDataSetCleared = fFlag; }
	bool IsUpdateTempDataDB() const noexcept { return m_fUpdateTempDataDB; }
	void SetUpdateTempDataDB(const bool fFlag) noexcept { m_fUpdateTempDataDB = fFlag; }

	bool IsCurrentEditStockChanged() const noexcept { return m_fCurrentEditStockChanged; }
	void SetCurrentEditStockChanged(const bool fFlag) noexcept { m_fCurrentEditStockChanged = fFlag; }

	// 实时数据需要计算与否和设置
	void SetRTDataNeedCalculate(const bool fFlag) noexcept { m_RTDataNeedCalculate = fFlag; }
	bool IsRTDataNeedCalculate() const noexcept { return m_RTDataNeedCalculate; }
	// 计算若干天日线相对强度与否和设置
	void SetCalculatingDayLineRS(const bool fFlag) noexcept { m_CalculatingDayLineRS = fFlag; }
	bool IsCalculatingDayLineRS() const noexcept { return m_CalculatingDayLineRS; }
	// 计算若干天周线相对强度与否和设置
	void SetCalculatingWeekLineRS(const bool fFlag) noexcept { m_CalculatingWeekLineRS = fFlag; }
	bool IsCalculatingWeekLineRS() const noexcept { return m_CalculatingWeekLineRS; }

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

	void ChangeToNextStock();
	void ChangeToPrevStock();
	void ChangeToPrevStockSet();
	void ChangeToNextStockSet();

	bool IsTotalStockSetSelected() const noexcept {
		if (m_lCurrentSelectedStockSet == -1) return true;
		return false;
	}

	size_t GetCurrentStockSetSize() const;

	void CreateStock(const CString& strStockCode, const CString& strStockName, bool fProcessRTData);

	void SetCurrentRSStrongIndex(const long lIndex) noexcept { m_lCurrentRSStrongIndex = lIndex; }

	void ResetEffectiveRTDataRatio();

	void SetDistributeAndCalculateTime(time_t tt) { m_ttDistributeAndCalculateTime = tt; }

public:
	atomic_int64_t m_ttDistributeAndCalculateTime; // 实时数据分配及处理时间

protected:
	vector<CChinaStockPtr> m_v10RSStrong1Stock; // 10日强势股票集
	vector<CChinaStockPtr> m_v10RSStrong2Stock; // 10日强势股票集
	vector<CRSReference> m_aRSStrongOption; // 用于计算RS的参数，最多十个。
	vector<vector<CChinaStockPtr>> m_avChosenStock; // 各种选择的股票集。0-9：自选股票集；10-19：10日RS股票集；20-29：股价变化股票集
	long m_lCurrentSelectedPosition; // 当前股票集的位置
	long m_lCurrentRSStrongIndex; // 仅用于传递当前的位置，以用于选择正确的数据表
	long m_lCurrentSelectedStockSet; // 当前选择的股票集（-1为整体股票集，1-10为10日RS特性股票集，以此类推）。
	bool m_fChosen10RSStrong1StockSet; // 本日的10日强势股票集已计算完成
	bool m_fChosen10RSStrong2StockSet; // 本日的10日强势股票集已计算完成
	bool m_fChosen10RSStrongStockSet; // 本日的10日强势股票集已计算完成
	bool m_fCalculateChosen10RS;

	atomic_int64_t m_llRTDataReceived; // 接收到的实时数据数量

	bool m_RTDataNeedCalculate;
	bool m_CalculatingDayLineRS;
	bool m_CalculatingWeekLineRS;

	bool m_fCurrentEditStockChanged;
	bool m_fMarketOpened; // 是否开市
	bool m_fFastReceivingRTData; // 是否开始接收实时数据
	bool m_fRTDataSetCleared; // 实时数据库已清除标识。九点三十分之前为假，之后设置为真。
	bool m_fUpdateTempDataDB; // 存储临时实时数据标识
	CChinaStockPtr m_pCurrentStock; // 当前显示的股票

	chrono::sys_seconds m_tpNewTransactionTime;

	bool m_fUsingSinaRTDataReceiver; // 使用新浪实时数据提取器
	bool m_fUsingNeteaseRTDataReceiver; // 使用网易实时数据提取器
	bool m_fUsingTengxunRTDataReceiver; // 使用腾讯实时数据提取器
	int m_iCountDownTengxunNumber;

	// Option各选项
	long m_lRSStartDate;
	long m_lRSEndDate;
	long m_lLastLoginDate; // 上次登录日期。如果此日期为昨日的话，则无需下载日线历史数据
	long m_lLastLoginTime;
	long m_lUpdatedDateFor10DaysRS2;
	long m_lUpdatedDateFor10DaysRS1;
	long m_lUpdatedDateFor10DaysRS;

	bool m_fSelectedStockLoaded;

	bool m_fCurrentStockChanged; // 当前选择的股票改变了
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

extern CChinaMarketPtr gl_pChinaMarket; // 中国股票市场。所有活跃的股票皆位于其中，单一实例变量，仅允许存在一个实例。
