#pragma once

#include"VirtualMarket.h"

#include"StockSection.h"

#include"DataStockSymbol.h"
#include"DataChinaStock.h"

#include<gsl/gsl>
using namespace gsl;

using namespace std;
#include<vector>
#include<map>
#include<atomic>
#include<queue>
#include<set>
#include<semaphore>

extern counting_semaphore<8> gl_BackGroundTaskThread;// 后台工作线程数。最大为8

constexpr int c_SelectedStockStartPosition = 0;
constexpr int c_10DaysRSStockSetStartPosition = 10; // 十日相对强度股票集起始位置（10-19为十日相对强对股票集，共十个）

class CChinaMarket : public CVirtualMarket {
public:
	DECLARE_DYNCREATE(CChinaMarket)
		CChinaMarket(void);
	// 只能有一个实例,不允许赋值。
	CChinaMarket(const CChinaMarket&) = delete;
	CChinaMarket& operator=(const CChinaMarket&) = delete;
	CChinaMarket(const CChinaMarket&&) noexcept = delete;
	CChinaMarket& operator=(const CChinaMarket&&) noexcept = delete;
	virtual ~CChinaMarket(void);
	virtual void ResetMarket(void) override final;
	void Reset(void);

	virtual bool PreparingExitMarket(void) override final;

	virtual bool IsTimeToResetSystem(long lCurrentTime)  override final;

	virtual bool IsOrdinaryTradeTime(void) override final; // 日常交易时间
	virtual bool IsOrdinaryTradeTime(long) override final;
	virtual bool IsWorkingTime(void) override final;
	virtual bool IsWorkingTime(long lTime) override final;
	virtual bool IsDummyTime(void) override final;
	virtual bool IsDummyTime(long lTime) override final;

#ifdef _DEBUG
	virtual	void AssertValid() const;
	virtual	void Dump(CDumpContext& dc) const;
#endif
public:
	// 定时更新，完成具体调度任务。由主线程CMainFrame的OnTimer函数调用。其后跟随各被调度函数
	virtual bool SchedulingTask(void) override final; // 由程序的定时器调度，大约每100毫秒一次

	bool SchedulingTaskPerSecond(long lSecondNumber, long lCurrentTime); // 每秒调度一次
	bool SchedulingTaskPer10Seconds(long lCurrentTime); // 每十秒调度一次
	bool SchedulingTaskPerMinute(long lCurrentTime); // 每一分钟调度一次
	bool SchedulingTaskPer5Minutes(long lCurrentTime); // 每五分钟调度一次
	bool SchedulingTaskPerHour(long lCurrentTime); // 每小时调度一次

	// 各种任务
	bool TaskProcessTengxunRTData(void);  // 处理腾讯实时数据
	bool TaskSetCheckActiveStockFlag(long lCurrentTime);
	bool TaskChoice10RSStrong1StockSet(long lCurrentTime);
	bool TaskChoice10RSStrong2StockSet(long lCurrentTime);
	bool TaskChoice10RSStrongStockSet(long lCurrentTime);
	bool TaskProcessTodayStock(long lCurrentTime);
	void ProcessTodayStock(void);
	bool TaskCheckDayLineDB(void);
	bool TaskCheckFastReceivingData(long lCurrentTime);
	bool TaskCheckMarketOpen(long lCurrentTime);
	bool TaskResetMarket(long lCurrentTime);
	bool TaskResetMarketAgain(long lCurrentTime);

	bool TaskUpdateStockCodeDB(void);
	bool TaskUpdateOptionDB(void);
	bool TaskUpdateChoicedStockDB(void);

	bool TaskShowCurrentTransaction(void);

	bool TaskSaveChoicedRTData(void);
	bool TaskClearChoicedRTDataSet(long lCurrentTime);

	bool TaskSaveStockSection(void); //

	void TaskGetActiveStockSize(void);

	//处理个股票的实时数据，计算挂单变化等。由工作线程ThreadCalculatingRTDataProc调用。
	bool TaskProcessRTData(void) { return m_dataChinaStock.TaskProcessRTData(); }

	// 是否所有股票的历史日线数据都查询过一遍了
	bool TaskProcessDayLineGetFromNeeteaseServer(void);

	// 装载当前股票日线任务
	bool TaskLoadCurrentStockHistoryData(void);

	// 各工作线程调用包裹函数
	virtual bool CreatingThreadBuildDayLineRS(long lStartCalculatingDay);
	virtual bool CreatingThreadBuildDayLineRSOfDate(long lThisDay);
	virtual bool CreatingThreadBuildWeekLine(long lStartDate);
	virtual bool CreatingThreadBuildWeekLineOfStock(CChinaStock* pStock, long lStartDate);
	virtual bool CreatingThreadBuildWeekLineRS(void);
	virtual bool CreatingThreadBuildWeekLineRSOfDate(long lThisDay);
	virtual bool CreatingThreadBuildWeekLineOfCurrentWeek(void);
	virtual bool CreatingThreadBuildCurrentWeekWeekLineTable(void);
	virtual bool CreatingThreadChoice10RSStrong1StockSet(void);
	virtual bool CreatingThreadChoice10RSStrongStockSet(void);
	virtual bool CreatingThreadChoice10RSStrong2StockSet(void);
	virtual bool CreatingThreadLoadDayLine(CChinaStock* pCurrentStock);
	virtual bool CreatingThreadLoadWeekLine(CChinaStock* pCurrentStock);
	virtual bool CreatingThreadProcessTodayStock(void);
	virtual bool CreatingThreadUpdateStockCodeDB(void);
	// interface function

public:
	// 系统状态区
	bool IsFastReceivingRTData(void) noexcept { return m_fFastReceivingRTData; }

	// 初始化市场

	// 实时数据读取
	CString GetSinaStockInquiringStr(long lTotalNumber, bool fUsingTotalStockSet);
	CString GetNeteaseStockInquiringMiddleStr(long lTotalNumber, bool fUsingTotalStockSet);
	CString	GetNextNeteaseStockInquiringMiddleStr(long lTotalNumber) { return m_dataChinaStock.GetNextNeteaseStockInquiringMiddleStr(lTotalNumber); }
	bool CheckValidOfNeteaseDayLineInquiringStr(CString str);
	CString GetNextSinaStockInquiringMiddleStrFromTotalStockSet(long lTotalNumber) { return m_dataStockSymbol.GetNextSinaStockInquiringMiddleStr(lTotalNumber); }
	CString GetNextNeteaseStockInquiringMiddleStrFromTotalStockSet(long lTotalNumber) { return m_dataStockSymbol.GetNextNeteaseStockInquiringMiddleStr(lTotalNumber); }
	CString GetNextSinaStockInquiringMiddleStr(long lTotalNumber) { return m_dataChinaStock.GetNextSinaStockInquiringMiddleStr(lTotalNumber); }
	CString GetNextTengxunStockInquiringMiddleStr(long lTotalNumber) { return m_dataChinaStock.GetNextTengxunStockInquiringMiddleStr(lTotalNumber); }
	//日线历史数据读取
	bool CreateNeteaseDayLineInquiringStr(CString& strReturn) { return m_dataChinaStock.CreateNeteaseDayLineInquiringStr(strReturn); }
	long IncreaseStockInquiringIndex(long& lIndex, long lEndPosition);

	bool IsAStock(not_null<CChinaStockPtr> pStock) { return m_dataChinaStock.IsAStock(pStock); } // 是否为沪深A股
	bool IsAStock(CString strStockCode) { return m_dataChinaStock.IsAStock(strStockCode); } // 是否为沪深A股
	bool IsStock(CString strStockCode) { return m_dataChinaStock.IsStock(strStockCode); }	// 是否为正确的股票代码

	CString GetStockName(CString strStockCode) { return m_dataChinaStock.GetStockName(strStockCode); }

	// 得到股票指针
	CChinaStockPtr GetStock(CString strStockCode) { return m_dataChinaStock.GetStock(strStockCode); }
	CChinaStockPtr GetStock(long lIndex) { return m_dataChinaStock.GetStock(lIndex); }

	// 得到当前显示股票
	CChinaStockPtr GetCurrentStock(void) const noexcept { return m_pCurrentStock; }
	void SetCurrentStock(CString strStockCode);
	void SetCurrentStock(CChinaStockPtr pStock);
	void ResetCurrentStock(void);
	bool IsCurrentStockChanged(void) const noexcept { return m_fCurrentStockChanged; }
	void SetCurrentStockChanged(bool fFlag) noexcept { m_fCurrentStockChanged = fFlag; }

	long GetMinLineOffset(time_t tUTC);

	bool IsTodayStockNotProcessed(void) const noexcept { if (m_iTodayStockProcessed == 0) return true; else return false; }
	bool IsProcessingTodayStock(void) const noexcept { if (m_iTodayStockProcessed == 1) return true; else return false; }
	bool IsTodayStockProcessed(void) const noexcept { if (m_iTodayStockProcessed == 0) return false; else return true; }
	void SetProcessingTodayStock(void) noexcept { m_iTodayStockProcessed = 1; }
	void SetTodayStockProcessed(bool fFlag) noexcept { if (fFlag) m_iTodayStockProcessed = 2; else m_iTodayStockProcessed = 0; }

	long GetCurrentSelectedPosition(void) const noexcept { return m_lCurrentSelectedPosition; }
	void SetCurrentSelectedPosition(long lIndex) noexcept { m_lCurrentSelectedPosition = lIndex; }
	long GetCurrentSelectedStockSet(void) const noexcept { return m_lCurrentSelectedStockSet; }
	void SetCurrentSelectedStockSet(long lIndex) noexcept { m_lCurrentSelectedStockSet = lIndex; }
	CChinaStockPtr GetCurrentSelectedStock(void);

	bool IsChoiced10RSStrong1StockSet(void) const noexcept { return m_fChoiced10RSStrong1StockSet; }
	void SetChoiced10RSStrong1StockSet(bool fFlag) noexcept { m_fChoiced10RSStrong1StockSet = fFlag; }
	bool IsChoiced10RSStrong2StockSet(void) const noexcept { return m_fChoiced10RSStrong2StockSet; }
	void SetChoiced10RSStrong2StockSet(bool fFlag) noexcept { m_fChoiced10RSStrong2StockSet = fFlag; }
	bool IsChoiced10RSStrongStockSet(void) const noexcept { return m_fChoiced10RSStrongStockSet; }
	void SetChoiced10RSStrongStockSet(bool fFlag) noexcept { m_fChoiced10RSStrongStockSet = fFlag; }
	bool IsCalculateChoiced10RS(void) const noexcept { return m_fCalculateChoiced10RS; }
	void SetCalculateChoiced10RS(bool fFlag) noexcept { m_fCalculateChoiced10RS = fFlag; }

	// 数据库读取存储操作
	virtual bool SaveRTData(void);  // 实时数据处理函数，将读取到的实时数据存入数据库中
	bool TaskSaveDayLineData(void) { return m_dataChinaStock.TaskSaveDayLineData(); }  // 日线历史数据处理函数，将读取到的日线历史数据存入数据库中
	virtual bool UpdateStockCodeDB(void) { return m_dataChinaStock.UpdateStockCodeDB(); }
	void LoadStockCodeDB(void) { m_lStockDayLineNeedUpdate = m_dataChinaStock.LoadStockCodeDB(); }

	virtual bool UpdateOptionDB(void);
	void LoadOptionDB(void);
	bool UpdateChoicedStockDB(void);
	virtual bool AppendChoicedStockDB(void);
	void LoadChoicedStockDB(void);
	bool UpdateTempRTData(void);
	virtual bool UpdateTodayTempDB(void) { return m_dataChinaStock.UpdateTodayTempDB(); }
	bool LoadTodayTempDB(long lTheDay);
	bool LoadDayLine(CDataChinaDayLine& dataChinaDayLine, long lDate);
	virtual bool SaveStockSection(void) { return m_dataStockSymbol.UpdateStockSectionDB(); }

	bool ChangeDayLineStockCodeToStandred(void);

	virtual bool DeleteWeekLine(void);
	bool DeleteWeekLineBasicInfo(void);
	bool DeleteWeekLineExtendInfo(void);
	virtual bool DeleteWeekLine(long lMonday);
	bool DeleteWeekLineBasicInfo(long lMonday);
	bool DeleteWeekLineExtendInfo(long lMonday);

	void MaintainDayLine(void) { m_dataChinaStock.SetAllDayLineNeedMaintain(); }

	bool DeleteDayLine(long lDate);
	bool DeleteDayLineBasicInfo(long lDate);
	bool DeleteDayLineExtendInfo(long lDate);

	virtual bool DeleteCurrentWeekWeekLine(void);
	bool DeleteCurrentWeekWeekLineBeforeTheDate(long lCutOffDate);

	bool Load10DaysRSStrong1StockSet(void);
	bool Load10DaysRSStrong2StockSet(void);

	bool LoadCalculatingRSOption(void);
	void SaveCalculatingRSOption(void);

	bool Load10DaysRSStrongStockDB(void);
	bool LoadOne10DaysRSStrongStockDB(long lIndex);

	virtual bool BuildWeekLine(long lStartDate) { return m_dataChinaStock.BuildWeekLine(lStartDate); }
	virtual bool BuildWeekLineOfCurrentWeek(void);
	bool CreateStockCodeSet(set<CString>& setStockCode, not_null<vector<CVirtualHistoryCandleExtendPtr>*> pvData);
	virtual bool BuildCurrentWeekWeekLineTable(void); // 使用周线表构建当前周周线表

	// 股票历史数据处理
	virtual bool Choice10RSStrong2StockSet(void) { return m_dataChinaStock.Choice10RSStrong2StockSet(); } // 选择10日强势股票集（两次峰值）
	virtual bool Choice10RSStrong1StockSet(void) { return m_dataChinaStock.Choice10RSStrong1StockSet(); } // 选择10日强势股票集（一次峰值）
	virtual bool Choice10RSStrongStockSet(CRSReference* pRef, int iIndex) { return m_dataChinaStock.Choice10RSStrongStockSet(pRef, iIndex); }

	bool IsDayLineNeedUpdate(void) const noexcept { return m_dataChinaStock.IsDayLineNeedUpdate(); }
	bool IsDayLineNeedProcess(void) const noexcept;
	bool IsDayLineNeedSaving(void) const { return m_dataChinaStock.IsDayLineNeedSaving(); }
	long GetDayLineNeedUpdateNumber(void) { return m_dataChinaStock.GetDayLineNeedUpdateNumber(); }
	long GetDayLineNeedSaveNumber(void) { return m_dataChinaStock.GetDayLineNeedSaveNumber(); }

	virtual long BuildDayLine(long lCurrentTradeDay) { return m_dataChinaStock.BuildDayLine(lCurrentTradeDay); }
	virtual bool BuildDayLineRS(long lDate) { return m_dataChinaStock.BuildDayLineRS(lDate); }
	virtual bool BuildWeekLineRS(long lDate) { return m_dataChinaStock.BuildWeekLineRS(lDate); }

	bool IsLoadSelectedStock(void) const noexcept { return m_fLoadedSelectedStock; }
	void SetLoadSelectedStock(bool fLoad) noexcept { m_fLoadedSelectedStock = fLoad; }

	bool IsCheckingActiveStock(void) const noexcept { return m_fCheckActiveStock; }
	void SetCheckActiveStock(bool fFlag) noexcept { m_fCheckActiveStock = fFlag; }

	bool IsTodayTempRTDataLoaded(void) noexcept { return m_fTodayTempDataLoaded; }
	void SetTodayTempRTDataLoaded(bool fFlag) noexcept { m_fTodayTempDataLoaded = fFlag; }

	bool IsDayLineDBUpdated(void) noexcept { return m_dataChinaStock.IsDayLineDBUpdated(); }
	void ClearDayLineDBUpdatedFlag(void) noexcept { m_dataChinaStock.ClearDayLineDBUpdatedFlag(); }

	long GetRSStartDate(void) const noexcept { return m_lRSStartDate; }
	void SetRSStartDate(long lDate) noexcept { m_lRSStartDate = lDate; }
	long GetRSEndDate(void) const noexcept { return m_lRSEndDate; }
	void SetRSEndDate(long lDate) noexcept { m_lRSEndDate = lDate; }
	long GetLastLoginDate(void) const noexcept { return m_lLastLoginDate; }
	void SetLastLoginDate(long lDate) noexcept { m_lLastLoginDate = lDate; }
	void SetLastLoginTime(long lTime) noexcept { m_lLastLoginTime = lTime; }
	long GetUpdatedDateFor10DaysRS1(void) const noexcept { return m_lUpdatedDateFor10DaysRS1; }
	void SetUpdatedDateFor10DaysRS1(long lDate) noexcept { m_lUpdatedDateFor10DaysRS1 = lDate; }
	long GetUpdatedDateFor10DaysRS2(void) const noexcept { return m_lUpdatedDateFor10DaysRS2; }
	void SetUpdatedDateFor10DaysRS2(long lDate) noexcept { m_lUpdatedDateFor10DaysRS2 = lDate; }
	long GetUpdatedDateFor10DaysRS(void) const noexcept { return m_lUpdatedDateFor10DaysRS; }
	void SetUpdatedDateFor10DaysRS(long lDate) noexcept { m_lUpdatedDateFor10DaysRS = lDate; }

	INT64 GetTotalAttackBuyAmount(void) { return m_dataChinaStock.GetTotalAttackBuyAmount(); }
	INT64 GetTotalAttackSellAmount(void) { return m_dataChinaStock.GetTotalAttackSellAmount(); }

	long GetStockOffset(CString str) { return m_dataChinaStock.GetIndex(str); }

	void StoreChoiceRTData(CWebRTDataPtr pRTData);

	bool TaskDiscardNeteaseRTData(void);
	bool TaskDiscardSinaRTData(void);
	bool TaskDiscardTengxunRTData(void);

	//处理实时股票变化等
	bool TaskDistributeSinaRTDataToStock(void);
	bool TaskDistributeNeteaseRTDataToStock(void);
	bool DistributeRTDataToStock(CWebRTDataPtr pData);

	long GetRTDataReceivedInOrdinaryTradeTime(void) noexcept { return m_lRTDataReceivedInOrdinaryTradeTime; }
	long GetNewRTDataReveivedInOrdinaryTradeTime(void) noexcept { return m_lNewRTDataReceivedInOrdinaryTradeTime; }

	void TaskSaveTempDataIntoDB(long lCurrentTime);

	// 状态反馈
	bool IsUsingSinaRTDataReceiver(void) const noexcept { return m_fUsingSinaRTDataReceiver; }
	bool IsUsingNeteaseRTDataReceiver(void) const noexcept { return m_fUsingNeteaseRTDataReceiver; }
	void SetUsingNeteaseRTDataReceiver(bool fFlag) noexcept { m_fUsingNeteaseRTDataReceiver = fFlag; }
	bool IsUsingTengxunRTDataReceiver(void) const noexcept { return m_fUsingTengxunRTDataReceiver; }
	void SetUsingTengxunRTDataReceiver(bool fFlag) noexcept { m_fUsingTengxunRTDataReceiver = fFlag; }
	int GetCountDownTengxunNumber(void) const noexcept { return m_iCountDownTengxunNumber; }
	void SetCountDownTengxunNumber(int iValue) noexcept { m_iCountDownTengxunNumber = iValue; }

	long GetTotalStock(void) noexcept { return m_dataChinaStock.GetStockSize(); }
	long GetTotalActiveStock(void) noexcept { return m_lTotalActiveStock; }
	long GetTotalLoadedStock(void) noexcept { return m_dataChinaStock.GetLoadedStockSize(); }
	void SetNewestTransactionTime(time_t tt) noexcept { m_ttNewestTransactionTime = tt; }
	time_t GetNewestTransactionTime(void) const noexcept { return m_ttNewestTransactionTime; }
	bool IsMarketOpened(void) const noexcept { return m_fMarketOpened; }
	void SetMarketOpened(bool fFlag) noexcept { m_fMarketOpened = fFlag; }
	bool IsFastReceivingRTData(void) const noexcept { return m_fFastReceivingRTData; }
	bool IsSaveDayLine(void) const noexcept { return m_fSaveDayLine; }
	void SetSaveDayLine(bool fFlag) noexcept { m_fSaveDayLine = fFlag; }
	bool IsRTDataSetCleared(void) const noexcept { return m_fRTDataSetCleared; }
	void SetRTDataSetCleared(bool fFlag) noexcept { m_fRTDataSetCleared = fFlag; }
	bool IsSavingTempData(void) const noexcept { return m_fSaveTempData; }
	void SetSavingTempData(bool fFlag) noexcept { m_fSaveTempData = fFlag; }

	bool IsCurrentEditStockChanged(void) const noexcept { return m_fCurrentEditStockChanged; }
	void SetCurrentEditStockChanged(bool fFlag) noexcept { m_fCurrentEditStockChanged = fFlag; }

	bool AddChoicedStock(CChinaStockPtr pStock);
	bool DeleteChoicedStock(CChinaStockPtr pStock);
	size_t GetChoicedStockSize(void) const { return m_avChoicedStock.at(0).size(); }
	size_t GetChoicedStockSize(long lIndex) const { return m_avChoicedStock.at(lIndex).size(); }
	void ClearChoiceStockContainer(void) { m_avChoicedStock.at(0).clear(); }
	size_t GetChoicedRTDataSize(void) const noexcept { return m_qRTData.size(); }
	void ClearChoicedRTDataQueue(void) noexcept { while (m_qRTData.size() > 0) m_qRTData.pop(); }

	void SetSinaStockRTDataInquiringIndex(long lIndex) noexcept { m_dataChinaStock.SetSinaRTDataInquiringIndex(lIndex); }
	long GetSinaStockRTDataInquiringIndex(void) noexcept { return m_dataChinaStock.GetSinaRTDataInquiringIndex(); }
	void SetTengxunRTDataInquiringIndex(long lIndex) noexcept { m_dataChinaStock.SetTengxunRTDataInquiringIndex(lIndex); }
	long GetTengxunRTDataInquiringIndex(void) noexcept { return m_dataChinaStock.GetTengxunRTDataInquiringIndex(); }
	void SetNeteaseDayLineDataInquiringIndex(long lIndex) noexcept { m_dataChinaStock.SetNeteaseDayLineDataInquiringIndex(lIndex); }
	long GetNeteaseDayLineDataInquiringIndex(void) noexcept { return m_dataChinaStock.GetNeteaseDayLineDataInquiringIndex(); }

	void ClearDayLineNeedUpdateStatus(void) { m_dataChinaStock.ClearDayLineNeedUpdateStatus(); }

	void SetRecordRTData(bool fFlag) noexcept { m_fSaveRTData = fFlag; }
	bool IsRecordingRTData(void) const noexcept { if (m_fSaveRTData) return true; else return false; }

	bool IsUpdateStockCodeDB(void) { return m_dataChinaStock.IsUpdateStockCodeDB(); }
	void SetUpdateOptionDB(bool fFlag) noexcept { m_fUpdateOptionDB = fFlag; }
	bool IsUpdateOptionDB(void) const noexcept { const bool fFlag = m_fUpdateOptionDB; return fFlag; }
	void SetUpdateChoicedStockDB(bool fFlag) noexcept { m_fUpdateChoicedStockDB = fFlag; }
	bool IsUpdateChoicedStockDB(void) const noexcept { const bool fFlag = m_fUpdateChoicedStockDB; return fFlag; }

	INT64 GetRTDataReceived(void) const noexcept { return m_llRTDataReceived; }
	void SetRTDataReceived(INT64 llValue) noexcept { m_llRTDataReceived = llValue; }
	void IncreaseRTDataReceived(INT64 llValue = 1) noexcept { m_llRTDataReceived += llValue; }

	bool CheckMarketReady(void);

	bool ChangeToNextStock(void);
	bool ChangeToPrevStock(void);
	bool ChangeToPrevStockSet(void);
	bool ChangeToNextStockSet(void);
	bool IsTotalStockSetSelected(void) const noexcept { if (m_lCurrentSelectedStockSet == -1) return true; else return false; }
	size_t GetCurrentStockSetSize(void);

	void SetStockDayLineNeedUpdate(long lValue) noexcept { m_lStockDayLineNeedUpdate = lValue; }
	bool TooManyStockDayLineNeedUpdate(void) const noexcept { if (m_lStockDayLineNeedUpdate > 1000) return true; else return false; }

	void SetUpdateStockSection(bool fFlag) noexcept { m_dataStockSymbol.SetUpdateStockSection(fFlag); }
	bool IsUpdateStockSection(void) noexcept { return m_dataStockSymbol.IsUpdateStockSection(); }

	bool AddStock(CChinaStockPtr pStock) { return m_dataChinaStock.Add(pStock); }
	bool DeleteStock(CChinaStockPtr pStock) { return m_dataChinaStock.Delete(pStock); }
	bool CreateStock(CString strStockCode, CString strStockName, bool fProcessRTData);

	void SetCurrentRSStrongIndex(long lIndex) noexcept { m_lCurrentRSStrongIndex = lIndex; }

protected:
	// 初始化

public:
	// 测试专用函数

protected:
	// 本市场各选项

// 变量区
protected:
	CDataChinaStock m_dataChinaStock;
	CDataStockSymbol m_dataStockSymbol;

	vector<CChinaStockPtr> m_v10RSStrong1Stock; // 10日强势股票集
	vector<CChinaStockPtr> m_v10RSStrong2Stock; // 10日强势股票集
	vector<CRSReference> m_aRSStrongOption; // 用于计算RS的参数，最多十个。
	vector<vector<CChinaStockPtr> > m_avChoicedStock; // 各种选择的股票集。0-9：自选股票集；10-19：10日RS股票集；20-29：股价变化股票集
	long m_lCurrentSelectedPosition; // 当前股票集的位置
	long m_lCurrentRSStrongIndex; // 仅用于传递当前的位置，以用于选择正确的数据表
	long m_lCurrentSelectedStockSet; // 当前选择的股票集（-1为整体股票集，1-10为10日RS特性股票集，以此类推）。
	bool m_fChoiced10RSStrong1StockSet; // 本日的10日强势股票集已计算完成
	bool m_fChoiced10RSStrong2StockSet; // 本日的10日强势股票集已计算完成
	bool m_fChoiced10RSStrongStockSet; // 本日的10日强势股票集已计算完成
	bool m_fCalculateChoiced10RS;

	atomic_uint64_t m_llRTDataReceived; // 接收到的实时数据数量
	long m_lRTDataReceivedInOrdinaryTradeTime; // 本日正常交易时间内接收到的实时数据数量
	long m_lNewRTDataReceivedInOrdinaryTradeTime; // 本日正常交易时间内接收到的新实时数据数量

	long m_lStockDayLineNeedUpdate; // 股票历史日线今日需要更新数

	queue<CWebRTDataPtr> m_qRTData;
	bool m_fSaveRTData;

	int m_iMarketOpenOffset; // 开市的偏移量。以分钟为单位，0930 = 0，1129 = 120， 1300 = 121， 1459 = 240。

	bool m_fCurrentEditStockChanged;
	bool m_fMarketOpened; // 是否开市
	bool m_fFastReceivingRTData; // 是否开始接收实时数据
	bool m_fSaveDayLine; // 将读取的日线存入数据库标识
	bool m_fRTDataSetCleared; // 实时数据库已清除标识。九点三十分之前为假，之后设置为真。
	bool m_fSaveTempData; // 存储临时实时数据标识
	CChinaStockPtr m_pCurrentStock; // 当前显示的股票

	time_t m_ttNewestTransactionTime;

	bool m_fUsingSinaRTDataReceiver; // 使用新浪实时数据提取器
	bool m_fUsingNeteaseRTDataReceiver; // 使用网易实时数据提取器
	bool m_fUsingTengxunRTDataReceiver; // 使用腾讯实时数据提取器
	int m_iCountDownTengxunNumber;

	CString m_strSinaRTDataInquiringStr;
	CString m_strTengxunRTDataInquiringStr;
	CString m_strNeteaseRTDataInquiringStr;
	CString m_strNeteaseDayLineDataInquiringStr;

	// Option各选项
	long m_lRSStartDate;
	long m_lRSEndDate;
	long m_lLastLoginDate; // 上次登录日期。如果此日期为昨日的话，则无需下载日线历史数据
	long m_lLastLoginTime;
	long m_lUpdatedDateFor10DaysRS2;
	long m_lUpdatedDateFor10DaysRS1;
	long m_lUpdatedDateFor10DaysRS;

	vector<CChinaStockPtr> m_vpSelectedStock; // 当前选择的股票
	bool m_fLoadedSelectedStock;

	bool m_fCurrentStockChanged; // 当前选择的股票改变了
	INT64 m_lTotalMarketBuy; // 沪深市场中的A股向上买入金额
	INT64 m_lTotalMarketSell; // 沪深市场中的A股向下卖出金额

	// 系统状态区
	int m_iTodayStockProcessed; // 今日是否执行了股票收盘.0:尚未执行；1：正在执行中；2：已执行完。
	bool m_fCheckActiveStock; // 是否查询今日活跃股票代码
	bool m_fTodayTempDataLoaded; //今日暂存的临时数据是否加载标识。
	long m_lTotalActiveStock;

	// 更新股票代码数据库标识
	atomic_bool m_fUpdateOptionDB;
	bool m_fUpdateChoicedStockDB;

private:
};

typedef shared_ptr<CChinaMarket> CChinaMarketPtr;

extern CChinaMarketPtr gl_pChinaMarket; // 中国股票市场。所有活跃的股票皆位于其中，单一实例变量，仅允许存在一个实例。
