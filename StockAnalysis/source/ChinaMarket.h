#pragma once

#include"stdafx.h"
#include"globedef.h"
#include"Accessory.h"
#include"Semaphore.h"

#include"VirtualMarket.h"

#include"WebRTDataContainer.h"

#include "ChinaStock.h"

using namespace MyLib;

using namespace std;
#include<vector>
#include<map>
#include<atomic>
#include<queue>
#include<set>

// 信号量必须声明为全局变量（为了初始化）
extern Semaphore gl_SaveOneStockDayLine;  // 此信号量用于生成日线历史数据库
extern Semaphore gl_SemaphoreBackGroundTaskThreads; // 后台工作线程数。最大为8
extern Semaphore gl_ProcessSinaRTDataQueue;
extern Semaphore gl_ProcessTengxunRTDataQueue;
extern Semaphore gl_ProcessNeteaseRTDataQueue;

extern CWebRTDataContainer gl_WebRTDataContainer;

constexpr int c_SelectedStockStartPosition = 0;
constexpr int c_10DaysRSStockSetStartPosition = 10; // 十日相对强度股票集起始位置（10-19为十日相对强对股票集，共十个）

class CChinaMarket : public CVirtualMarket
{
public:
  // 只能有一个实例
  CChinaMarket(void);
  virtual ~CChinaMarket(void);
  virtual void ResetMarket(void) override;
  void Reset(void);

#ifdef _DEBUG
  virtual	void AssertValid() const;
  virtual	void Dump(CDumpContext& dc) const;
#endif
public:
  // 定时更新，完成具体调度任务。由主线程CMainFrame的OnTimer函数调用。其后跟随各被调度函数
  virtual bool SchedulingTask(void) override; // 由程序的定时器调度，大约每100毫秒一次
  bool SchedulingTaskPerSecond(long lSecondNumber); // 每秒调度一次
  bool SchedulingTaskPer10Seconds(long lSecondNumber, long lCurrentTime); // 每十秒调度一次
  bool SchedulingTaskPerMinute(long lSecondNumber, long lCurrentTime); // 每一分钟调度一次
  bool SchedulingTaskPer5Minutes(long lSecondNumber, long lCurrentTime); // 每五分钟调度一次
  bool SchedulingTaskPerHour(long lSecondNumber, long lCurrentTime); // 每小时调度一次

  // 各种任务
  bool TaskGetRTDataFromWeb(void);
  bool TaskGetNeteaseDayLineFromWeb(void);

  bool TaskProcessTengxunRTData(void);  // 处理腾讯实时数据
  bool TaskSetCheckActiveStockFlag(long lCurrentTime);
  bool TaskChoice10RSStrong1StockSet(long lCurrentTime);
  bool TaskChoice10RSStrong2StockSet(long lCurrentTime);
  bool TaskChoice10RSStrongStockSet(long lCurrentTime);
  bool TaskProcessTodayStock(long lCurrentTime);
  bool TaskCheckDayLineDB(void);
  bool TaskCheckStartReceivingData(long lCurrentTime);
  bool TaskCheckMarketOpen(long lCurrentTime);
  bool TaskResetMarket(long lCurrentTime);
  bool TaskResetMarketAgain(long lCurrentTime);

  bool TaskUpdateStockCodeDB(void);
  bool TaskUpdateOptionDB(void);
  bool TaskUpdateChoicedStockDB(void);

  bool TaskShowCurrentTransaction(void);

  bool TaskSaveChoicedRTData(void);
  bool TaskClearChoicedRTDataSet(long lCurrentTime);

  //处理个股票的实时数据，计算挂单变化等。由工作线程ThreadCalculatingRTDataProc调用。
  bool TaskProcessRTData(void);

  // 是否所有股票的历史日线数据都查询过一遍了
  bool TaskProcessDayLineGetFromNeeteaseServer(void);

  // 装载当前股票日线任务
  bool TaskLoadCurrentStockHistoryData(void);

  // 各工作线程调用包裹函数
  virtual bool RunningThreadSaveChoicedRTData(void);
  virtual bool RunningThreadProcessTodayStock(void);
  virtual bool RunningThreadBuildDayLineRS(long lStartCalculatingDay);
  virtual bool RunningThreadBuildDayLineRSOfDate(long lThisDay);
  virtual bool RunningThreadSaveTempRTData(void);
  virtual bool RunningThreadSaveDayLineBasicInfoOfStock(CChinaStockPtr pStock);
  virtual bool RunningThreadLoadDayLine(CChinaStockPtr pCurrentStock);
  virtual bool RunningThreadLoadWeekLine(CChinaStockPtr pCurrentStock);
  virtual bool RunningThreadUpdateStockCodeDB(void);
  virtual bool RunningThreadUpdateOptionDB(void);
  virtual bool RunningThreadAppendChoicedStockDB(void);
  virtual bool RunningThreadChoice10RSStrong2StockSet(void);
  virtual bool RunningThreadChoice10RSStrong1StockSet(void);
  virtual bool RunningThreadChoice10RSStrongStockSet(void);
  virtual bool RunningThreadCalculate10RSStrongStock(vector<CChinaStockPtr>* pv10RSStrongStock, CRSReference* pRef, CChinaStockPtr pStock);
  virtual bool RunningThreadCalculate10RSStrong1Stock(vector<CChinaStockPtr>* pv10RSStrongStock, CChinaStockPtr pStock);
  virtual bool RunningThreadCalculate10RSStrong2Stock(vector<CChinaStockPtr>* pv10RSStrongStock, CChinaStockPtr pStock);
  virtual bool RunningThreadBuildWeekLine(long lStartDate);
  virtual bool RunningThreadBuildWeekLineOfStock(CChinaStockPtr pStock, long lStartDate);
  virtual bool RunningThreadBuildWeekLineRS(void);
  virtual bool RunningThreadBuildWeekLineRSOfDate(long lThisDay);
  virtual bool RunningThreadBuildWeekLineOfCurrentWeek(void);
  virtual bool RunningThreadBuildCurrentWeekWeekLineTable(void);
  // interface function
public:
  // 系统状态区

  // 初始化市场

  // 实时数据读取
  CString GetSinaInquiringStockStr(long lTotalNumber, bool fSkipUnactiveStock = true);
  CString GetTengxunInquiringStockStr(long lTotalNumber, bool fSkipUnactiveStock = true);
  CString	GetNeteaseInquiringStockStr(long lTotalNumber = 700, bool fSkipUnactiveStock = true);
  bool CheckValidOfNeteaseDayLineInquiringStr(CString str);
  CString GetNextInquiringMiddleStr(long& iStockIndex, CString strPostfix, long lTotalNumber, bool fSkipUnactiveStock = true);
  bool StepToActiveStockIndex(long& lStockIndex);

  //日线历史数据读取
  bool CreateNeteaseDayLineInquiringStr(CString& strReturn);

  long IncreaseStockInquiringIndex(long& lIndex);

  bool IsAStock(CChinaStockPtr pStock); // 是否为沪深A股
  bool IsAStock(CString strStockCode); // 是否为沪深A股
  bool IsStock(CString  strStockCode);	// 是否为正确的股票代码

  CString GetStockName(CString strStockCode);

  // 得到股票索引
  bool GetStockIndex(CString strStockCode, long& lIndex);
  // 得到股票指针
  CChinaStockPtr GetStock(CString strStockCode);
  CChinaStockPtr GetStock(long lIndex);

  void IncreaseActiveStockNumber(void);

  // 得到当前显示股票
  CChinaStockPtr GetCurrentStock(void) noexcept { return m_pCurrentStock; }
  void SetCurrentStock(CString strStockCode);
  void SetCurrentStock(CChinaStockPtr pStock);
  void ResetCurrentStock(void);
  bool IsCurrentStockChanged(void) noexcept { return m_fCurrentStockChanged; }
  void SetCurrentStockChanged(bool fFlag) noexcept { m_fCurrentStockChanged = fFlag; }

  long GetTotalActiveStock(void) noexcept { return m_lTotalActiveStock; }
  void SetTotalActiveStock(long lValue) noexcept { m_lTotalActiveStock = lValue; }

  long GetMinLineOffset(time_t tMarket);

  bool IsSystemReady(void) noexcept { return m_fSystemReady; }
  void SetSystemReady(bool fFlag) noexcept { m_fSystemReady = fFlag; }

  bool IsTodayStockNotProcessed(void) noexcept { if (m_iTodayStockProcessed == 0) return true; else return false; }
  bool IsProcessingTodayStock(void) noexcept { if (m_iTodayStockProcessed == 1) return true; else return false; }
  bool IsTodayStockProcessed(void) noexcept { if (m_iTodayStockProcessed == 0) return false; else return true; }
  void SetProcessingTodayStock(void) noexcept { m_iTodayStockProcessed = 1; }
  void SetTodayStockProcessed(bool fFlag) noexcept { if (fFlag) m_iTodayStockProcessed = 2; else m_iTodayStockProcessed = 0; }

  long GetCurrentSelectedPosition(void) noexcept { return m_lCurrentSelectedPosition; }
  void SetCurrentSelectedPosition(long lIndex) noexcept { m_lCurrentSelectedPosition = lIndex; }
  long GetCurrentSelectedStockSet(void) noexcept { return m_lCurrentSelectedStockSet; }
  void SetCurrentSelectedStockSet(long lIndex) noexcept { m_lCurrentSelectedStockSet = lIndex; }
  CChinaStockPtr GetCurrentSelectedStock(void);

  bool IsChoiced10RSStrong1StockSet(void) noexcept { return m_fChoiced10RSStrong1StockSet; }
  void SetChoiced10RSStrong1StockSet(bool fFlag) noexcept { m_fChoiced10RSStrong1StockSet = fFlag; }
  bool IsChoiced10RSStrong2StockSet(void) noexcept { return m_fChoiced10RSStrong2StockSet; }
  void SetChoiced10RSStrong2StockSet(bool fFlag) noexcept { m_fChoiced10RSStrong2StockSet = fFlag; }
  bool IsChoiced10RSStrongStockSet(void) noexcept { return m_fChoiced10RSStrongStockSet; }
  void SetChoiced10RSStrongStockSet(bool fFlag) noexcept { m_fChoiced10RSStrongStockSet = fFlag; }
  bool IsCalculateChoiced10RS(void) noexcept { return m_fCalculateChoiced10RS; }
  void Setm_fCalculateChoiced10RS(bool fFlag) noexcept { m_fCalculateChoiced10RS = fFlag; }

  // 数据库读取存储操作
  virtual bool SaveRTData(void);  // 实时数据处理函数，将读取到的实时数据存入数据库中
  bool TaskSaveDayLineData(void);  // 日线历史数据处理函数，将读取到的日线历史数据存入数据库中
  virtual bool UpdateStockCodeDB(void);
  void LoadStockCodeDB(void);
  virtual bool UpdateOptionDB(void);
  void LoadOptionDB(void);
  void LoadOptionChinaStockMarketDB(void);
  virtual bool UpdateOptionChinaStockMarketDB(void);
  bool UpdateChoicedStockDB(void);
  virtual bool AppendChoicedStockDB(void);
  void LoadChoicedStockDB(void);
  bool UpdateTempRTData(void);
  virtual bool UpdateTodayTempDB(void);
  bool DeleteTodayTempDB(void);
  bool LoadTodayTempDB(void);
  bool LoadDayLine(CDayLineContainer& dayLineContainer, long lDate);
  bool LoadWeekLineBasicInfo(CWeekLineContainer& weekLineContainer, long lMondayOfWeek);
  bool SaveWeekLine(CWeekLineContainer& weekLineContainer);

  bool DeleteWeekLine(void);
  bool DeleteWeekLineBasicInfo(void);
  bool DeleteWeekLineExtendInfo(void);
  bool DeleteWeekLine(long lMonday);
  bool DeleteWeekLineBasicInfo(long lMonday);
  bool DeleteWeekLineExtendInfo(long lMonday);

  bool DeleteDayLine(void);
  bool DeleteDayLineBasicInfo(void);
  bool DeleteDayLineExtendInfo(void);
  bool DeleteDayLine(long lDate);
  bool DeleteDayLineBasicInfo(long lDate);
  bool DeleteDayLineExtendInfo(long lDate);

  bool SaveCurrentWeekLine(CWeekLineContainer& weekLineContainer);
  bool LoadCurrentWeekLine(CWeekLineContainer& weekLineContainer);
  bool DeleteCurrentWeekWeekLine(void);
  bool DeleteCurrentWeekWeekLineBeforeTheDate(long lCutOffDate);

  bool Load10DaysRSStrong1StockSet(void);
  bool Load10DaysRSStrong2StockSet(void);

  bool LoadCalculatingRSOption(void);
  void SaveCalculatingRSOption(void);

  bool Load10DaysRSStrongStockDB(void);
  bool LoadOne10DaysRSStrongStockDB(long lIndex);

  bool UnloadDayLine(void);

  bool BuildWeekLine(long lStartDate);
  virtual bool BuildWeekLineOfCurrentWeek(void);
  bool CreateStockCodeSet(set<CString>& setStockCode, vector<CChinaStockHistoryDataPtr>* pvData);
  virtual bool BuildCurrentWeekWeekLineTable(void); // 使用周线表构建当前周周线表

// 股票历史数据处理
  virtual bool Choice10RSStrong2StockSet(void); // 选择10日强势股票集（两次峰值）
  virtual bool Choice10RSStrong1StockSet(void); // 选择10日强势股票集（一次峰值）
  virtual bool Choice10RSStrongStockSet(CRSReference* pRef, int iIndex);

  bool IsDayLineNeedUpdate(void);
  bool IsDayLineNeedSaving(void);

  virtual long BuildDayLineOfDate(long lCurrentTradeDay);
  virtual bool BuildDayLineRSOfDate(long lDate);
  virtual bool BuildWeekLineRSOfDate(long lDate);
  double GetUpDownRate(CString strClose, CString StrLastClose);

  bool IsLoadSelectedStock(void) noexcept { return m_fLoadedSelectedStock; }
  void SetLoadSelectedStock(bool fLoad) noexcept { m_fLoadedSelectedStock = fLoad; }

  bool IsCheckActiveStock(void) noexcept { return m_fCheckActiveStock; }
  void SetCheckActiveStock(bool fFlag) noexcept { m_fCheckActiveStock = fFlag; }

  bool IsTodayTempRTDataLoaded(void) noexcept { return m_fTodayTempDataLoaded; }
  void SetTodayTempRTDataLoaded(bool fFlag) noexcept { m_fTodayTempDataLoaded = fFlag; }

  bool IsDayLineDBUpdated(void);
  void ClearDayLineDBUpdatedFlag(void);

  long GetRSStartDate(void) noexcept { return m_lRSStartDate; }
  void SetRSStartDate(long lDate) noexcept { m_lRSStartDate = lDate; }
  long GetRSEndDate(void) noexcept { return m_lRSEndDate; }
  void SetRSEndDate(long lDate) noexcept { m_lRSEndDate = lDate; }
  long GetLastLoginDate(void) noexcept { return m_lLastLoginDay; }
  void SetLastLoginDate(long lDate) noexcept { m_lLastLoginDay = lDate; }
  long GetUpdatedDateFor10DaysRS1(void) noexcept { return m_lUpdatedDateFor10DaysRS1; }
  void SetUpdatedDateFor10DaysRS1(long lDate) noexcept { m_lUpdatedDateFor10DaysRS1 = lDate; }
  long GetUpdatedDateFor10DaysRS2(void) noexcept { return m_lUpdatedDateFor10DaysRS2; }
  void SetUpdatedDateFor10DaysRS2(long lDate) noexcept { m_lUpdatedDateFor10DaysRS2 = lDate; }
  long GetUpdatedDateFor10DaysRS(void) noexcept { return m_lUpdatedDateFor10DaysRS; }
  void SetUpdatedDateFor10DaysRS(long lDate) noexcept { m_lUpdatedDateFor10DaysRS = lDate; }

  INT64 GetTotalAttackBuyAmount(void);
  INT64 GetTotalAttackSellAmount(void);

  size_t GetTotalStockMapIndexSize(void) noexcept { return m_mapChinaMarketAStock.size(); }
  long GetTotalStockIndex(CString str) { return m_mapChinaMarketAStock.at(str); }

  void SetStockCodeForInquiringRTData(CString strStockCode) { m_strStockCodeForInquiringRTData = strStockCode; }
  CString GetStockCodeForInquiringRTData(void) { return m_strStockCodeForInquiringRTData; }
  void SetReadingTengxunRTDataTime(clock_t tt) noexcept { m_ReadingTengxunRTDataTime = tt; }
  clock_t GetReadingTengxunRTDataTime(void) noexcept { return m_ReadingTengxunRTDataTime; }
  void SetStockCodeForInquiringNeteaseDayLine(CString strStockCode) { m_strStockCodeForInquiringNeteaseDayLine = strStockCode; }
  CString GetStockCodeForInquiringNeteaseDayLine(void) { return m_strStockCodeForInquiringNeteaseDayLine; }

  // 处理网络上提取的实时股票数据
  bool TaskProcessWebRTDataGetFromSinaServer(void);
  void StoreChoiceRTData(CWebRTDataPtr pRTData);
  bool TaskProcessWebRTDataGetFromTengxunServer(void);
  bool IsInvalidTengxunRTData(CWebDataPtr pWebDataReceived);
  void CheckTengxunRTData(CWebRTDataPtr pRTData);
  bool TaskProcessWebRTDataGetFromNeteaseServer(void);
  bool IsInvalidNeteaseRTData(CWebDataPtr pWebDataReceived);
  bool IsValidNeteaseRTDataPrefix(CWebDataPtr pWebDataReceived);
  bool ValidateNeteaseRTData(CWebRTDataPtr pRTData);

  bool TaskDiscardNeteaseRTData(void);
  bool TaskDiscardSinaRTData(void);
  bool TaskDiscardTengxunRTData(void);

  //处理实时股票变化等
  bool TaskDistributeSinaRTDataToProperStock(void);
  bool TaskDistributeNeteaseRTDataToProperStock(void);

  void TaskSaveTempDataIntoDB(long lCurrentTime);

  void SetUsingSinaRTDataServer(void) noexcept { m_iRTDataServer = 0; }
  void SetUsingNeteaseRTDataServer(void) noexcept { m_iRTDataServer = 1; }
  bool IsUsingSinaRTDataServer(void) noexcept { if (m_iRTDataServer == 0) return true; else return false; }
  bool IsUsingNeteaseRTDataServer(void) noexcept { if (m_iRTDataServer == 1) return true; else return false; }

  // 状态反馈
  bool IsUsingSinaRTDataReceiver(void) noexcept { return m_fUsingSinaRTDataReceiver; }
  bool IsUsingNeteaseRTDataReceiver(void) noexcept { return m_fUsingNeteaseRTDataReceiver; }
  void SetUsingNeteaseRTDataReceiver(bool fFlag) noexcept { m_fUsingNeteaseRTDataReceiver = fFlag; }
  bool IsUsingTengxunRTDataReceiver(void) noexcept { return m_fUsingTengxunRTDataReceiver; }
  void SetUsingTengxunRTDataReceiver(bool fFlag) noexcept { m_fUsingTengxunRTDataReceiver = fFlag; }
  int GetCountDownTengxunNumber(void) noexcept { return m_iCountDownTengxunNumber; }
  void SetCountDownTengxunNumber(int iValue) noexcept { m_iCountDownTengxunNumber = iValue; }

  long GetTotalStock(void) noexcept { return m_lTotalStock; }
  void SetNewestTransactionTime(time_t tt) noexcept { m_ttNewestTransactionTime = tt; }
  time_t GetNewestTransactionTime(void) noexcept { return m_ttNewestTransactionTime; }
  bool IsMarketOpened(void) noexcept { return m_fMarketOpened; }
  void SetMarketOpened(bool fFlag) noexcept { m_fMarketOpened = fFlag; }
  bool IsStartReceivingData(void) noexcept { return m_fStartReceivingData; }
  bool IsGetRTData(void) noexcept { return m_fGetRTData; }
  bool IsSaveDayLine(void) noexcept { return m_fSaveDayLine; }
  void SetSaveDayLine(bool fFlag) noexcept { m_fSaveDayLine = fFlag; }
  bool IsRTDataSetCleared(void) noexcept { return m_fRTDataSetCleared; }
  void SetRTDataSetCleared(bool fFlag) noexcept { m_fRTDataSetCleared = fFlag; }
  bool IsSavingTempData(void) noexcept { return m_fSaveTempData; }
  void SetSavingTempData(bool fFlag) noexcept { m_fSaveTempData = fFlag; }

  int GetCountDownSlowReadingRTData(void) noexcept { return m_iCountDownSlowReadingRTData; }
  bool IsCurrentEditStockChanged(void) noexcept { return m_fCurrentEditStockChanged; }
  void SetCurrentEditStockChanged(bool fFlag) noexcept { m_fCurrentEditStockChanged = fFlag; }

  bool AddChoicedStock(CChinaStockPtr pStock);
  bool DeleteChoicedStock(CChinaStockPtr pStock);
  size_t GetChoicedStockSize(void) noexcept { return m_avChoicedStock[0].size(); }
  size_t GetStockSetSize(long lIndex) noexcept { return m_avChoicedStock[lIndex].size(); }
  void ClearChoiceStockContainer(void) noexcept { m_avChoicedStock[0].clear(); }
  size_t GetChoicedRTDataSize(void) noexcept { return m_qRTData.size(); }
  void ClearChoicedRTDataQueue(void) noexcept { while (m_qRTData.size() > 0) m_qRTData.pop(); }

  void ResetSinaRTDataInquiringIndex(void) noexcept { m_lSinaRTDataInquiringIndex = 0; }
  void ResetTengxunRTDataInquiringIndex(void) noexcept { m_lTengxunRTDataInquiringIndex = 0; }
  void ResetNeteaseRTDataInquiringIndex(void) noexcept { m_lNeteaseRTDataInquiringIndex = 0; }
  void ResetNeteaseDayLineDataInquiringIndex(void) noexcept { m_lNeteaseDayLineDataInquiringIndex = 0; }
  long GetSinaRTDataInquiringIndex(void) noexcept { return m_lSinaRTDataInquiringIndex; }
  long GetTengxunRTDataInquiringIndex(void) noexcept { return m_lTengxunRTDataInquiringIndex; }
  long GetNeteaseRTDataInquiringIndex(void) noexcept { return m_lNeteaseRTDataInquiringIndex; }
  long GetNeteaseDayLineDataInquiringIndex(void) noexcept { return m_lNeteaseDayLineDataInquiringIndex; }

  int GetDayLineNeedUpdateNumber(void) noexcept { const int i = m_iDayLineNeedUpdate; return i; }
  void SetDayLineNeedUpdateNumber(int i) noexcept { m_iDayLineNeedUpdate = i; }
  void ClearDayLineNeedUpdaeStatus(void);
  int GetDayLineNeedProcessNumber(void) noexcept { const int i = m_iDayLineNeedProcess; return i; }
  void SetDayLineNeedProcessNumber(int i) noexcept { m_iDayLineNeedProcess = i; }
  int GetDayLineNeedSaveNumber(void) noexcept { const int i = m_iDayLineNeedSave; return i; }
  void SetDayLineNeedSaveNumber(int i) noexcept { m_iDayLineNeedSave = i; }
  void IncreaseNeteaseDayLineNeedUpdateNumber(int iNumber = 1);
  void DecreaseNeteaseDayLineNeedUpdateNumber(int iNumber = 1);
  void IncreaseNeteaseDayLineNeedProcessNumber(int iNumber = 1) noexcept { m_iDayLineNeedProcess += iNumber; }
  void DecreaseNeteaseDayLineNeedProcessNumber(int iNumber = 1) noexcept { if (m_iDayLineNeedProcess >= iNumber) m_iDayLineNeedProcess -= iNumber; }
  void IncreaseNeteaseDayLineNeedSaveNumber(int iNumber = 1) noexcept { m_iDayLineNeedSave += iNumber; }
  void DecreaseNeteaseDayLineNeedSaveNumber(int iNumber = 1) noexcept { if (m_iDayLineNeedSave >= iNumber) m_iDayLineNeedSave -= iNumber; }

  void SetRecordRTData(bool fFlag) noexcept { m_fSaveRTData = fFlag; }
  bool IsRecordingRTData(void) noexcept { if (m_fSaveRTData) return true; else return false; }

  void SetUpdateStockCodeDB(bool fFlag) noexcept { m_fUpdateStockCodeDB = fFlag; }
  bool IsUpdateStockCodeDB(void) noexcept { const bool fFlag = m_fUpdateStockCodeDB; return fFlag; }
  void SetUpdateOptionDB(bool fFlag) noexcept { m_fUpdateOptionDB = fFlag; }
  bool IsUpdateOptionDB(void) noexcept { const bool fFlag = m_fUpdateOptionDB; return fFlag; }
  void SetUpdateChoicedStockDB(bool fFlag) noexcept { m_fUpdateChoicedStockDB = fFlag; }
  bool IsUpdateChoicedStockDB(void) noexcept { const bool fFlag = m_fUpdateChoicedStockDB; return fFlag; }

  INT64 GetRTDataReceived(void) noexcept { return m_llRTDataReceived; }
  void SetRTDataReceived(INT64 llValue) noexcept { m_llRTDataReceived = llValue; }

  bool CheckMarketReady(void) noexcept;

  bool ChangeToNextStock(void);
  bool ChangeToPrevStock(void);
  bool ChangeToPrevStockSet(void);
  bool ChangeToNextStockSet(void);
  bool IsTotalStockSetSelected(void) noexcept { if (m_lCurrentSelectedStockSet == -1) return true; else return false; }
  size_t GetCurrentStockSetSize(void);

  void SetStockNeedUpdated(long lValue) noexcept { m_lStockNeedUpdated = lValue; }
  bool TooManyStocksNeedUpdated(void) noexcept { if (m_lStockNeedUpdated > 1000) return true; else return false; }

protected:
  // 初始化
  bool CreateTotalStockContainer(void); //此函数是构造函数的一部分，不允许单独调用。使用Mock类测试时，派生Mock类中将CChinaStock改为CMockChinaStock。

public:

protected:
  vector<CChinaStockPtr> m_vChinaMarketStock; // 本系统允许的所有股票池（无论代码是否存在）
  map<CString, long> m_mapChinaMarketAStock; // 将所有被查询的股票代码映射为偏移量（目前只接受A股信息）
  long m_lTotalStock; // 股票代码总数
  long m_lTotalActiveStock;	// 当天股票总数

  vector<CChinaStockPtr> m_v10RSStrong1Stock; // 10日强势股票集
  vector<CChinaStockPtr> m_v10RSStrong2Stock; // 10日强势股票集
  CRSReference m_aRSStrongOption[10]; // 用于计算RS的参数，最多十个。
  vector<CChinaStockPtr> m_avChoicedStock[30]; // 各种选择的股票集。0-9：自选股票集；10-19：10日RS股票集；20-29：股价变化股票集
  long m_lCurrentSelectedPosition; // 当前股票集的位置
  long m_lCurrentRSStrongIndex; // 仅用于传递当前的位置，以用于选择正确的数据表
  long m_lCurrentSelectedStockSet; // 当前选择的股票集（-1为整体股票集，1-10为10日RS特性股票集，以此类推）。
  bool m_fChoiced10RSStrong1StockSet; // 本日的10日强势股票集已计算完成
  bool m_fChoiced10RSStrong2StockSet; // 本日的10日强势股票集已计算完成
  bool m_fChoiced10RSStrongStockSet; // 本日的10日强势股票集已计算完成
  bool m_fCalculateChoiced10RS;

  INT64 m_llRTDataReceived; // 接收到的实时数据数量

  long m_lStockNeedUpdated; // 股票历史日线今日需要更新数

  queue<CWebRTDataPtr> m_qRTData;
  bool m_fSaveRTData;

  int m_iMarketOpenOffset; // 开市的偏移量。以分钟为单位，0930 = 0，1129 = 120， 1300 = 121， 1459 = 240。

  bool m_fCurrentEditStockChanged;
  int m_iCountDownSlowReadingRTData; // 慢速读取实时数据计数器
  bool m_fMarketOpened; // 是否开市
  bool m_fStartReceivingData; // 是否开始接收实时数据
  bool m_fGetRTData; // 读取实时数据标识
  bool m_fSaveDayLine; // 将读取的日线存入数据库标识
  bool m_fRTDataSetCleared; // 实时数据库已清除标识。九点三十分之前为假，之后设置为真。
  bool m_fSaveTempData; // 存储临时实时数据标识
  CChinaStockPtr m_pCurrentStock; // 当前显示的股票

  time_t m_ttNewestTransactionTime;

  int m_iRTDataServer; // 实时数据服务器选择

  bool m_fUsingSinaRTDataReceiver; // 使用新浪实时数据提取器
  bool m_fUsingNeteaseRTDataReceiver; // 使用网易实时数据提取器
  bool m_fUsingTengxunRTDataReceiver; // 使用腾讯实时数据提取器
  int m_iCountDownTengxunNumber;

  CString m_strSinaRTDataInquiringStr;
  CString m_strTengxunRTDataInquiringStr;
  CString m_strNeteaseRTDataInquiringStr;
  CString m_strNeteaseDayLineDataInquiringStr;

  long m_lSinaRTDataInquiringIndex;
  long m_lTengxunRTDataInquiringIndex;
  long m_lNeteaseRTDataInquiringIndex;
  long m_lNeteaseDayLineDataInquiringIndex;

  // Option各选项
  long m_lRSStartDate;
  long m_lRSEndDate;
  long m_lLastLoginDay; // 上次登录日期。如果此日期为昨日的话，则无需下载日线历史数据
  long m_lUpdatedDateFor10DaysRS2;
  long m_lUpdatedDateFor10DaysRS1;
  long m_lUpdatedDateFor10DaysRS;

  vector<CChinaStockPtr> m_vpSelectedStock; // 当前选择的股票
  bool m_fLoadedSelectedStock;

  bool m_fCurrentStockChanged; // 当前选择的股票改变了
  INT64 m_lTotalMarketBuy; // 沪深市场中的A股向上买入金额
  INT64 m_lTotalMarketSell; // 沪深市场中的A股向下卖出金额

  // 系统状态区
  bool m_fSystemReady; // 市场初始态已经设置好
  int m_iTodayStockProcessed; // 今日是否执行了股票收盘.0:尚未执行；1：正在执行中；2：已执行完。
  bool m_fCheckActiveStock; // 是否查询今日活跃股票代码
  bool m_fTodayTempDataLoaded; //今日暂存的临时数据是否加载标识。

  // 多线程读取之变量
  CString m_strStockCodeForInquiringRTData;
  clock_t m_ReadingTengxunRTDataTime; // 每次读取腾讯实时数据的时间
  CString m_strStockCodeForInquiringNeteaseDayLine;

  // 更新股票代码数据库标识
  atomic_bool m_fUpdateStockCodeDB;
  atomic_bool m_fUpdateOptionDB;
  bool m_fUpdateChoicedStockDB;

  // 网易日线历史数据读取处理和存储计数器。
  atomic_int m_iDayLineNeedUpdate; // 日线需要更新的股票数量
  atomic_int m_iDayLineNeedProcess; // 日线需要处理的股票数量
  atomic_int m_iDayLineNeedSave; // 日线需要存储的股票数量

private:
};

typedef shared_ptr<CChinaMarket> CChinaMarketPtr;
