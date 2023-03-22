#pragma once

#include"VirtualMarket.h"

#include"DayLineWebData.h"

#include"ContainerStockSymbol.h"
#include"ContainerChinaStock.h"

#include<semaphore>
#include<set>
#include<atomic>
using std::binary_semaphore;
using std::set;
using std::atomic_int64_t;

extern counting_semaphore<8> gl_BackGroundTaskThread; // 后台工作线程数。最大为8
extern binary_semaphore gl_ProcessChinaMarketRTData; // 处理中国市场的实时数据时，不允许同时存储之。

constexpr int c_SelectedStockStartPosition = 0;
constexpr int c_10DaysRSStockSetStartPosition = 10; // 十日相对强度股票集起始位置（10-19为十日相对强对股票集，共十个）

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
	void Reset();

	bool PreparingExitMarket() final;

	bool IsTimeToResetSystem(long lCurrentTime) final;

	bool IsOrdinaryTradeTime() final; // 日常交易时间
	bool IsOrdinaryTradeTime(long) final;
	bool IsWorkingTime() final;
	bool IsWorkingTime(long lTime) final;
	bool IsDummyTime() final;
	bool IsDummyTime(long lTime) final;

public:
	bool ProcessTask(long lCurrentTime) override; // 每日定时任务调度,由SchedulingTaskPerSecond调度

	// 各种任务
	virtual bool TaskCreateTask(long lCurrentTime);
	bool TaskCheckMarketReady(long lCurrentTime);
	virtual bool TaskResetMarket(long lCurrentTime);
	virtual void TaskDistributeAndCalculateRTData(long lCurrentTime);
	bool TaskProcessAndSaveDayLine(long lCurrentTime);
	void TaskSaveTempData(long lCurrentTime);
	void TaskLoadCurrentStockHistoryData();// 装载当前股票日线
	void TaskAccessoryTask(long lCurrentTime); // 其他辅助任务
	void TaskChoiceRSSet(long lCurrentTime);

	bool SetCheckActiveStockFlag(long lCurrentTime);
	bool TaskChoice10RSStrong1StockSet(long lCurrentTime);
	bool TaskChoice10RSStrong2StockSet(long lCurrentTime);
	bool TaskChoice10RSStrongStockSet(long lCurrentTime);
	bool TaskProcessTodayStock(long lCurrentTime);
	void ProcessTodayStock();
	bool CheckDayLineDB();
	bool CheckFastReceivingData(long lCurrentTime);
	bool CheckMarketOpen(long lCurrentTime);

	bool TaskUpdateStockProfileDB(long lCurrentTime);
	bool TaskUpdateOptionDB(long lCurrentTime);
	bool TaskUpdateChosenStockDB();

	bool TaskShowCurrentTransaction();

	bool TaskUpdateStockSection(); //

	bool ProcessDayLine();

	// 各工作线程调用包裹函数
	virtual void CreateThreadProcessRTData();
	virtual void CreateThreadBuildDayLineRS(long lStartCalculatingDay);
	virtual void CreateThreadBuildDayLineRSOfDate(long lThisDay);
	virtual void CreateThreadBuildWeekLine(long lStartDate);
	virtual void CreateThreadBuildWeekLineOfStock(CChinaStock* pStock, long lStartDate);
	virtual void CreateThreadBuildWeekLineRS();
	virtual void CreateThreadBuildWeekLineRSOfDate(long lThisDay);
	virtual void CreateThreadBuildWeekLineOfCurrentWeek();
	virtual void CreateThreadBuildCurrentWeekWeekLineTable();
	virtual void CreateThreadChoice10RSStrong1StockSet();
	virtual void CreateThreadChoice10RSStrongStockSet();
	virtual void CreateThreadChoice10RSStrong2StockSet();
	virtual void CreateThreadLoadDayLine(CChinaStock* pCurrentStock);
	virtual void CreateThreadLoadWeekLine(CChinaStock* pCurrentStock);
	virtual void CreateThreadProcessTodayStock();
	virtual void CreateThreadUpdateStockProfileDB();
	virtual void CreateThreadUpdateOptionDB();
	virtual void CreateThreadUpdateTempRTData();
	virtual void CreateThreadLoadTempRTData(long lTheDay);

	// interface function

public:
	//处理个股票的实时数据，计算挂单变化等。
	bool ProcessRTData() { return m_containerChinaStock.ProcessRTData(); }
	bool ProcessTengxunRTData(); // 处理腾讯实时数据

	// 系统状态区
	bool IsFastReceivingRTData() noexcept { return m_fFastReceivingRTData; }

	// 初始化市场

	// 实时数据读取
	CString GetSinaStockInquiringStr(long lTotalNumber, bool fUsingTotalStockSet);
	CString GetNeteaseStockInquiringMiddleStr(long lTotalNumber, bool fUsingTotalStockSet);
	CString GetNextNeteaseStockInquiringMiddleStr(const long lTotalNumber) { return m_containerChinaStock.GetNextNeteaseStockInquiringMiddleStr(lTotalNumber); }
	bool CheckValidOfNeteaseDayLineInquiringStr(const CString& str);
	CString GetNextSinaStockInquiringMiddleStrFromTotalStockSet(const long lTotalNumber) { return m_containerStockSymbol.GetNextSinaStockInquiringMiddleStr(lTotalNumber); }
	CString GetNextNeteaseStockInquiringMiddleStrFromTotalStockSet(const long lTotalNumber) { return m_containerStockSymbol.GetNextNeteaseStockInquiringMiddleStr(lTotalNumber); }
	CString GetNextSinaStockInquiringMiddleStr(const long lTotalNumber) { return m_containerChinaStock.GetNextSinaStockInquiringMiddleStr(lTotalNumber); }
	CString GetNextTengxunStockInquiringMiddleStr(const long lTotalNumber) { return m_containerChinaStock.GetNextTengxunStockInquiringMiddleStr(lTotalNumber); }

	//日线历史数据读取
	CString CreateNeteaseDayLineInquiringStr() { return m_containerChinaStock.CreateNeteaseDayLineInquiringStr(); }
	CString CreateTengxunDayLineInquiringStr() { return m_containerChinaStock.CreateTengxunDayLineInquiringStr(); }

	long IncreaseStockInquiringIndex(long& lIndex, long lEndPosition);

	bool IsAStock(const not_null<CChinaStockPtr> pStock) const { return m_containerChinaStock.IsAStock(pStock->GetSymbol()); } // 是否为沪深A股
	bool IsAStock(const CString& strStockCode) const { return m_containerChinaStock.IsAStock(strStockCode); } // 是否为沪深A股
	bool IsStock(const CString& strStockCode) const { return m_containerChinaStock.IsSymbol(strStockCode); } // 是否为正确的股票代码

	CString GetStockName(const CString& strStockCode) { return m_containerChinaStock.GetStockName(strStockCode); }

	// 得到股票指针
	CChinaStockPtr GetStock(const CString& strStockCode) { return m_containerChinaStock.GetStock(strStockCode); }
	CChinaStockPtr GetStock(const size_t lIndex) { return m_containerChinaStock.GetStock(lIndex); }
	size_t GetStockIndex(const CChinaStockPtr pStock) const { return m_containerChinaStock.GetOffset(pStock->GetSymbol()); }

	// 得到当前显示股票
	CChinaStockPtr GetCurrentStock() const noexcept { return m_pCurrentStock; }
	void SetCurrentStock(const CString& strStockCode);
	void SetCurrentStock(CChinaStockPtr pStock);
	void ResetCurrentStock();
	bool IsCurrentStockChanged() const noexcept { return m_fCurrentStockChanged; }
	void SetCurrentStockChanged(const bool fFlag) noexcept { m_fCurrentStockChanged = fFlag; }

	long GetMinLineOffset(time_t tUTC);

	long GetCurrentSelectedPosition() const noexcept { return m_lCurrentSelectedPosition; }
	void SetCurrentSelectedPosition(const long lIndex) noexcept { m_lCurrentSelectedPosition = lIndex; }
	long GetCurrentSelectedStockSet() const noexcept { return m_lCurrentSelectedStockSet; }
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
	bool SaveDayLineData() { return m_containerChinaStock.SaveDayLineData(); } // 日线历史数据处理函数，将读取到的日线历史数据存入数据库中
	virtual bool UpdateStockProfileDB() { return m_containerChinaStock.UpdateStockProfileDB(); }
	void LoadStockProfileDB() { m_containerChinaStock.LoadStockProfileDB(); }

	virtual bool UpdateOptionDB();
	void LoadOptionDB();
	bool UpdateChosenStockDB();
	virtual bool AppendChosenStockDB();
	void LoadChosenStockDB();
	virtual bool UpdateTodayTempDB() { return m_containerChinaStock.UpdateTodayTempDB(); }
	bool TaskLoadTempRTData(long lTheDay, long lCurrentTime);
	void LoadTempRTData(long lTheDay);
	bool LoadDayLine(CContainerChinaDayLine& dataChinaDayLine, long lDate);
	virtual bool SaveStockSection() { return m_containerStockSymbol.UpdateStockSectionDB(); }

	bool ChangeDayLineStockCodeToStandard();

	virtual bool DeleteWeekLine();
	bool DeleteWeekLineBasicInfo();
	bool DeleteWeekLineExtendInfo();
	virtual bool DeleteWeekLine(long lMonday);
	bool DeleteWeekLineBasicInfo(long lMonday);
	bool DeleteWeekLineExtendInfo(long lMonday);

	void MaintainDayLine() const { m_containerChinaStock.SetDayLineNeedMaintain(); }

	bool DeleteDayLine(long lDate);
	bool DeleteDayLineBasicInfo(long lDate);
	bool DeleteDayLineExtendInfo(long lDate);

	virtual bool DeleteCurrentWeekWeekLine();
	bool DeleteCurrentWeekWeekLineBeforeTheDate(long lCutOffDate);

	bool Load10DaysRSStrong1StockSet();
	bool Load10DaysRSStrong2StockSet();

	bool LoadCalculatingRSOption();
	void SaveCalculatingRSOption();

	bool Load10DaysRSStrongStockDB();
	bool LoadOne10DaysRSStrongStockDB(long lIndex);

	virtual bool BuildWeekLine(const long lStartDate) { return m_containerChinaStock.BuildWeekLine(lStartDate); }
	virtual bool BuildWeekLineOfCurrentWeek();
	bool CreateStockCodeSet(set<CString>& setStockCode, not_null<vector<CVirtualHistoryCandleExtendPtr>*> pvData);
	virtual bool BuildCurrentWeekWeekLineTable(); // 使用周线表构建当前周周线表

	// 股票历史数据处理
	virtual bool Choice10RSStrong2StockSet() { return m_containerChinaStock.Choice10RSStrong2StockSet(); }
	// 选择10日强势股票集（两次峰值）
	virtual bool Choice10RSStrong1StockSet() { return m_containerChinaStock.Choice10RSStrong1StockSet(); }
	// 选择10日强势股票集（一次峰值）
	virtual bool Choice10RSStrongStockSet(CRSReference* pRef, const int iIndex) { return m_containerChinaStock.Choice10RSStrongStockSet(pRef, iIndex); }

	bool IsDayLineNeedUpdate() noexcept { return m_containerChinaStock.IsDayLineNeedUpdate(); }
	bool IsDayLineNeedProcess();
	bool IsDayLineNeedSaving() { return m_containerChinaStock.IsDayLineNeedSaving(); }
	long GetDayLineNeedUpdateNumber() const { return m_containerChinaStock.GetDayLineNeedUpdateNumber(); }
	long GetDayLineNeedSaveNumber() const { return m_containerChinaStock.GetDayLineNeedSaveNumber(); }

	virtual long BuildDayLine(const long lCurrentTradeDay) { return m_containerChinaStock.BuildDayLine(lCurrentTradeDay); }
	virtual bool BuildDayLineRS(const long lDate) { return m_containerChinaStock.BuildDayLineRS(lDate); }
	virtual bool BuildWeekLineRS(const long lDate) { return m_containerChinaStock.BuildWeekLineRS(lDate); }

	bool IsLoadSelectedStock() const noexcept { return m_fLoadedSelectedStock; }
	void SetLoadSelectedStock(const bool fLoad) noexcept { m_fLoadedSelectedStock = fLoad; }

	bool IsCheckingActiveStock() const noexcept { return m_fCheckActiveStock; }
	void SetCheckActiveStock(const bool fFlag) noexcept { m_fCheckActiveStock = fFlag; }

	bool IsTodayTempRTDataLoaded() const noexcept { return m_fTodayTempDataLoaded; }
	void SetTodayTempRTDataLoaded(const bool fFlag) noexcept { m_fTodayTempDataLoaded = fFlag; }

	bool IsDayLineDBUpdated() { return m_containerChinaStock.IsDayLineDBUpdated(); }
	void ClearDayLineDBUpdatedFlag() noexcept { m_containerChinaStock.ClearDayLineDBUpdatedFlag(); }

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

	INT64 GetTotalAttackBuyAmount() { return m_containerChinaStock.GetTotalAttackBuyAmount(); }
	INT64 GetTotalAttackSellAmount() { return m_containerChinaStock.GetTotalAttackSellAmount(); }

	size_t GetStockOffset(const CString& str) const { return m_containerChinaStock.GetOffset(str); }

	//处理实时股票变化等
	bool DistributeSinaRTDataToStock();
	bool DistributeNeteaseRTDataToStock();
	bool DistributeRTDataToStock(CWebRTDataPtr pRTData);

	long GetRTDataReceivedInOrdinaryTradeTime() const noexcept { return m_lRTDataReceivedInOrdinaryTradeTime; }
	long GetNewRTDataReceivedInOrdinaryTradeTime() const noexcept { return m_lNewRTDataReceivedInOrdinaryTradeTime; }

	// 状态反馈
	bool IsUsingSinaRTDataReceiver() const noexcept { return m_fUsingSinaRTDataReceiver; }
	bool IsUsingNeteaseRTDataReceiver() const noexcept { return m_fUsingNeteaseRTDataReceiver; }
	void SetUsingNeteaseRTDataReceiver(const bool fFlag) noexcept { m_fUsingNeteaseRTDataReceiver = fFlag; }
	bool IsUsingTengxunRTDataReceiver() const noexcept { return m_fUsingTengxunRTDataReceiver; }
	void SetUsingTengxunRTDataReceiver(const bool fFlag) noexcept { m_fUsingTengxunRTDataReceiver = fFlag; }
	int GetCountDownTengxunNumber() const noexcept { return m_iCountDownTengxunNumber; }
	void SetCountDownTengxunNumber(const int iValue) noexcept { m_iCountDownTengxunNumber = iValue; }

	size_t GetTotalStock() const noexcept { return m_containerChinaStock.Size(); }
	long GetTotalActiveStock() const noexcept { return m_containerChinaStock.GetActiveStockSize(); }
	long GetTotalLoadedStock() const noexcept { return m_containerChinaStock.GetLoadedStockSize(); }
	void SetNewestTransactionTime(const time_t tt) noexcept { m_ttNewestTransactionTime = tt; }
	time_t GetNewestTransactionTime() const noexcept { return m_ttNewestTransactionTime; }
	bool IsMarketOpened() const noexcept { return m_fMarketOpened; }
	void SetMarketOpened(const bool fFlag) noexcept { m_fMarketOpened = fFlag; }
	bool IsFastReceivingRTData() const noexcept { return m_fFastReceivingRTData; }
	bool IsRTDataSetCleared() const noexcept { return m_fRTDataSetCleared; }
	void SetRTDataSetCleared(const bool fFlag) noexcept { m_fRTDataSetCleared = fFlag; }
	bool IsSavingTempData() const noexcept { return m_fSaveTempData; }
	void SetSavingTempData(const bool fFlag) noexcept { m_fSaveTempData = fFlag; }

	bool IsCurrentEditStockChanged() const noexcept { return m_fCurrentEditStockChanged; }
	void SetCurrentEditStockChanged(const bool fFlag) noexcept { m_fCurrentEditStockChanged = fFlag; }

	size_t SinaRTSize() noexcept { return m_qSinaRT.Size(); }
	void PushSinaRT(const CWebRTDataPtr pData) noexcept { m_qSinaRT.PushData(pData); }
	CWebRTDataPtr PopSinaRT() { return m_qSinaRT.PopData(); }
	size_t NeteaseRTSize() noexcept { return m_qNeteaseRT.Size(); }
	void PushNeteaseRT(const CWebRTDataPtr pData) noexcept { m_qNeteaseRT.PushData(pData); }
	CWebRTDataPtr PopNeteaseRT() { return m_qNeteaseRT.PopData(); }
	size_t TengxunRTSize() noexcept { return m_qTengxunRT.Size(); }
	void PushTengxunRT(const CWebRTDataPtr pData) noexcept { m_qTengxunRT.PushData(pData); }
	CWebRTDataPtr PopTengxunRT() { return m_qTengxunRT.PopData(); }
	size_t DayLineQueueSize() noexcept { return m_qDayLine.Size(); }
	void PushDayLine(const CDayLineWebDataPtr pData) noexcept { m_qDayLine.PushData(pData); }
	CDayLineWebDataPtr PopDayLine() { return m_qDayLine.PopData(); }

	// 实时数据需要计算与否和设置
	void SetRTDataNeedCalculate(const bool fFlag) noexcept { m_RTDataNeedCalculate = fFlag; }
	bool IsRTDataNeedCalculate() const noexcept { return m_RTDataNeedCalculate; }
	// 计算若干天日线相对强度与否和设置
	void SetCalculatingDayLineRS(const bool fFlag) noexcept { m_CalculatingDayLineRS = fFlag; }
	bool IsCalculatingDayLineRS() const noexcept { return m_CalculatingDayLineRS; }
	// 计算若干天周线相对强度与否和设置
	void SetCalculatingWeekLineRS(const bool fFlag) noexcept { m_CalculatingWeekLineRS = fFlag; }
	bool IsCalculatingWeekLineRS() const noexcept { return m_CalculatingWeekLineRS; }

	bool AddChosenStock(CChinaStockPtr pStock);
	bool DeleteChosenStock(CChinaStockPtr pStock);
	size_t GetChosenStockSize() const { return m_avChosenStock.at(0).size(); }
	size_t GetChosenStockSize(const long lIndex) const { return m_avChosenStock.at(lIndex).size(); }
	void ClearChoiceStockContainer() { m_avChosenStock.at(0).clear(); }

	void SetSinaStockRTDataInquiringIndex(const long lIndex) noexcept { m_containerChinaStock.SetSinaRTDataInquiringIndex(lIndex); }
	long GetSinaStockRTDataInquiringIndex() const noexcept { return m_containerChinaStock.GetSinaRTDataInquiringIndex(); }
	void SetTengxunRTDataInquiringIndex(const long lIndex) noexcept { m_containerChinaStock.SetTengxunRTDataInquiringIndex(lIndex); }
	long GetTengxunRTDataInquiringIndex() const noexcept { return m_containerChinaStock.GetTengxunRTDataInquiringIndex(); }

	void ClearDayLineNeedUpdateStatus() const { m_containerChinaStock.ClearDayLineNeedUpdateStatus(); }

	bool IsUpdateStockProfileDB() { return m_containerChinaStock.IsUpdateProfileDB(); }
	void SetUpdateOptionDB(const bool fFlag) noexcept { m_fUpdateOptionDB = fFlag; }

	bool IsUpdateOptionDB() const noexcept {
		const bool fFlag = m_fUpdateOptionDB;
		return fFlag;
	}

	void SetUpdateChosenStockDB(const bool fFlag) noexcept { m_fUpdateChosenStockDB = fFlag; }
	bool IsUpdateChosenStockDB() const noexcept { return m_fUpdateChosenStockDB; }

	INT64 GetRTDataReceived() const noexcept { return m_llRTDataReceived; }
	void SetRTDataReceived(const INT64 llValue) noexcept { m_llRTDataReceived = llValue; }
	void IncreaseRTDataReceived(const INT64 llValue = 1) noexcept { m_llRTDataReceived += llValue; }

	bool ChangeToNextStock();
	bool ChangeToPrevStock();
	bool ChangeToPrevStockSet();
	bool ChangeToNextStockSet();

	bool IsTotalStockSetSelected() const noexcept {
		if (m_lCurrentSelectedStockSet == -1) return true;
		return false;
	}

	size_t GetCurrentStockSetSize();

	void SetUpdateStockSection(const bool fFlag) noexcept { m_containerStockSymbol.SetUpdateStockSection(fFlag); }
	bool IsUpdateStockSection() const noexcept { return m_containerStockSymbol.IsUpdateStockSection(); }

	bool AddStock(CChinaStockPtr pStock) { return m_containerChinaStock.Add(pStock); }
	bool DeleteStock(CChinaStockPtr pStock) { return m_containerChinaStock.Delete(pStock); }
	bool CreateStock(CString strStockCode, CString strStockName, bool fProcessRTData);

	void SetCurrentRSStrongIndex(const long lIndex) noexcept { m_lCurrentRSStrongIndex = lIndex; }

protected:
	// 初始化

public:
	// 测试专用函数

protected:
	// 本市场各选项

	// 变量区
protected:
	CContainerChinaStock m_containerChinaStock;
	CContainerStockSymbol m_containerStockSymbol;

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
	long m_lRTDataReceivedInOrdinaryTradeTime; // 本日正常交易时间内接收到的实时数据数量
	long m_lNewRTDataReceivedInOrdinaryTradeTime; // 本日正常交易时间内接收到的新实时数据数量

	// 处理后的各种数据
	CPriorityQueueWebRTData m_qSinaRT; // 中国市场实时数据队列。
	CPriorityQueueWebRTData m_qNeteaseRT; // 中国市场实时数据队列。
	CPriorityQueueWebRTData m_qTengxunRT; // 中国市场实时数据队列。
	CTemplateMutexAccessQueue<CDayLineWebData> m_qDayLine; // 网易日线数据

	bool m_RTDataNeedCalculate;
	bool m_CalculatingDayLineRS;
	bool m_CalculatingWeekLineRS;

	bool m_fCurrentEditStockChanged;
	bool m_fMarketOpened; // 是否开市
	bool m_fFastReceivingRTData; // 是否开始接收实时数据
	bool m_fRTDataSetCleared; // 实时数据库已清除标识。九点三十分之前为假，之后设置为真。
	bool m_fSaveTempData; // 存储临时实时数据标识
	CChinaStockPtr m_pCurrentStock; // 当前显示的股票

	time_t m_ttNewestTransactionTime;

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

	bool m_fLoadedSelectedStock;

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
};

using CChinaMarketPtr = shared_ptr<CChinaMarket>;

extern CChinaMarketPtr gl_pChinaMarket; // 中国股票市场。所有活跃的股票皆位于其中，单一实例变量，仅允许存在一个实例。
