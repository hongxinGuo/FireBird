#pragma once

#include"stdafx.h"
#include"globedef.h"
#include"Accessory.h"
#include"Semaphore.h"

#include"VirtualMarket.h"

#include"RTDataContainer.h"

#include "ChinaStock.h"

using namespace MyLib;

using namespace std;
#include<vector>
#include<map>
#include<atomic>
#include<queue>

// 信号量必须声明为全局变量（为了初始化）
extern Semaphore gl_SaveOneStockDayLine;  // 此信号量用于生成日线历史数据库
extern Semaphore gl_SemaphoreCalculateDayLineRS;
extern Semaphore gl_ProcessSinaRTDataQueue;
extern Semaphore gl_ProcessTengxunRTDataQueue;
extern Semaphore gl_ProcessNeteaseRTDataQueue;

extern CRTDataContainer gl_RTDataContainer;

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
  bool TaskLoadCurrentStockDayLine(void);

  // 各工作线程调用包裹函数
  virtual bool RunningThreadSaveChoicedRTData(void);
  virtual bool RunningThreadProcessTodayStock(void);
  virtual bool RunningThreadCalculateRelativeStrong(long lStartCalculatingDay);
  virtual bool RunningThreadCalculateThisDayRS(long lThisDay);
  virtual bool RunningThreadSaveTempRTData(void);
  virtual bool RunningThreadSaveDayLineOfOneStock(CChinaStockPtr pStock);
  virtual bool RunningThreadLoadDayLine(CChinaStockPtr pCurrentStock);
  virtual bool RunningThreadUpdateStockCodeDB(void);
  virtual bool RunningThreadUpdateOptionDB(void);
  virtual bool RunningThreadAppendChoicedStockDB(void);
  virtual bool RunningThreadChoice10RSStrong2StockSet(void);
  virtual bool RunningThreadChoice10RSStrong1StockSet(void);

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
  CString CreateNeteaseDayLineInquiringStr(void);

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

  bool IsTodayStockProcessed(void) noexcept { return m_fTodayStockProcessed; }
  void SetTodayStockProcessed(bool fFlag) noexcept { m_fTodayStockProcessed = fFlag; }

  bool IsChoiced10RSStrong1StockSet(void) noexcept { return m_fChoiced10RSStrong1StockSet; }
  void SetChoiced10RSStrong1StockSet(bool fFlag) noexcept { m_fChoiced10RSStrong1StockSet = fFlag; }
  bool IsChoiced10RSStrong2StockSet(void) noexcept { return m_fChoiced10RSStrong2StockSet; }
  void SetChoiced10RSStrong2StockSet(bool fFlag) noexcept { m_fChoiced10RSStrong2StockSet = fFlag; }

  // 数据库读取存储操作
  virtual bool SaveRTData(void);  // 实时数据处理函数，将读取到的实时数据存入数据库中
  bool SaveDayLineData(void);  // 日线历史数据处理函数，将读取到的日线历史数据存入数据库中
  virtual bool UpdateStockCodeDB(void);
  void LoadStockCodeDB(void);
  virtual bool UpdateOptionDB(void);
  void LoadOptionDB(void);
  bool UpdateChoicedStockDB(void);
  virtual bool AppendChoicedStockDB(void);
  void LoadChoicedStockDB(void);
  bool UpdateTempRTData(void);
  virtual bool UpdateTodayTempDB(void);
  bool LoadTodayTempDB(void);
  bool Load10DayRSStrong1StockSet(void);
  bool Load10DayRSStrong2StockSet(void);

  bool UnloadDayLine(void);

  // 股票历史数据处理
  virtual bool Choice10RSStrong2StockSet(void); // 选择10日强势股票集（两次峰值）
  virtual bool Choice10RSStrong1StockSet(void); // 选择10日强势股票集（一次峰值）

  bool IsDayLineNeedUpdate(void);
  bool IsDayLineNeedSaving(void);

  virtual long ProcessCurrentTradeDayStock(long lCurrentTradeDay);
  bool CalculateOneDayRelativeStrong(long lDay);
  double GetUpDownRate(CString strClose, CString StrLastClose);

  bool IsLoadSelectedStock(void) noexcept { return m_fLoadedSelectedStock; }
  void SetLoadSelectedStock(bool fLoad) noexcept { m_fLoadedSelectedStock = fLoad; }

  bool IsCheckActiveStock(void) noexcept { return m_fCheckActiveStock; }
  void SetCheckActiveStock(bool fFlag) noexcept { m_fCheckActiveStock = fFlag; }

  bool IsTodayTempRTDataLoaded(void) noexcept { return m_fTodayTempDataLoaded; }
  void SetTodayTempRTDataLoaded(bool fFlag) noexcept { m_fTodayTempDataLoaded = fFlag; }

  bool IsDayLineDBUpdated(void);
  void ClearDayLineDBUpdatedFlag(void);

  long GetRelativeStrongStartDay(void) noexcept { return m_lRelativeStrongStartDay; }
  void SetRelativeStrongStartDay(long lDay) noexcept { m_lRelativeStrongStartDay = lDay; }
  long GetRelativeStrongEndDay(void) noexcept { return m_lRelativeStrongEndDay; }
  void SetRelativeStrongEndDay(long lDay) noexcept { m_lRelativeStrongEndDay = lDay; }
  long GetLastLoginDay(void) noexcept { return m_lLastLoginDay; }
  void SetLastLoginDay(long lDay) noexcept { m_lLastLoginDay = lDay; }
  long GetUpdatedDayFor10DayRS1(void) noexcept { return m_lUpdatedDayFor10DayRS1; }
  void SetUpdatedDayFor10DayRS1(long lDay) noexcept { m_lUpdatedDayFor10DayRS1 = lDay; }
  long GetUpdatedDayFor10DayRS2(void) noexcept { return m_lUpdatedDayFor10DayRS2; }
  void SetUpdatedDayFor10DayRS2(long lDay) noexcept { m_lUpdatedDayFor10DayRS2 = lDay; }

  INT64 GetTotalAttackBuyAmount(void);
  INT64 GetTotalAttackSellAmount(void);

  size_t GetTotalStockMapIndexSize(void) noexcept { return m_mapChinaMarketAStock.size(); }
  long GetTotalStockIndex(CString str) { return m_mapChinaMarketAStock.at(str); }

  void SetStockCodeForInquiringSinaRTData(CString strStockCode) noexcept { m_strStockCodeForInquiringSinaRTData = strStockCode; }
  CString GetStockCodeForInquiringSinaRTData(void) noexcept { return m_strStockCodeForInquiringSinaRTData; }
  void SetReadingTengxunRTDataTime(clock_t tt) noexcept { m_ReadingTengxunRTDataTime = tt; }
  clock_t GetReadingTengxunRTDataTime(void) noexcept { return m_ReadingTengxunRTDataTime; }
  void SetStockCodeForInquiringNeteaseDayLine(CString strStockCode) noexcept { m_strStockCodeForInquiringNeteaseDayLine = strStockCode; }
  CString GetStockCodeForInquiringNeteaseDayLine(void) noexcept { return m_strStockCodeForInquiringNeteaseDayLine; }

  // 处理网络上提取的实时股票数据
  bool TaskProcessWebRTDataGetFromSinaServer(void);
  void StoreChoiceRTData(CRTDataPtr pRTData);
  bool TaskProcessWebRTDataGetFromTengxunServer(void);
  bool IsInvalidTengxunRTData(CWebDataPtr pWebDataReceived);
  void CheckTengxunRTData(CRTDataPtr pRTData);
  bool TaskProcessWebRTDataGetFromNeteaseServer(void);
  bool IsInvalidNeteaseRTData(CWebDataPtr pWebDataReceived);
  bool IsValidNeteaseRTDataPrefix(CWebDataPtr pWebDataReceived);
  void CheckNeteaseRTData(CRTDataPtr pRTData);
  bool TaskProcessNeteaseRTData(void);

  //处理实时股票变化等
  bool TaskDistributeSinaRTDataToProperStock(void);

  void TaskSaveTempDataIntoDB(long lCurrentTime);

  // 状态反馈
  bool IsUsingSinaRTDataReceiver(void) noexcept { return m_fUsingSinaRTDataReceiver; }
  bool IsUsingNeteaseRTDataReceiver(void) noexcept { return m_fUsingNeteaseRTDataReceiver; }
  void SetUsingNeteaseRTDataReceiver(bool fFlag) noexcept { m_fUsingNeteaseRTDataReceiver = fFlag; }
  bool IsUsingTengxunRTDataReceiver(void) noexcept { return m_fUsingTengxunRTDataReceiver; }
  void SetUsingTengxunRTDataReceiver(bool fFlag) noexcept { m_fUsingTengxunRTDataReceiver = fFlag; }
  int GetCountDownNeteaseNumber(void) noexcept { return m_iCountDownNeteaseNumber; }
  void SetCountDownNeteaseNumber(int iValue) noexcept { m_iCountDownNeteaseNumber = iValue; }
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

  int GetCountDownSlowReadingRTData(void) noexcept { return m_iCountDownSlowReadingRTData; }
  bool IsCurrentEditStockChanged(void) noexcept { return m_fCurrentEditStockChanged; }
  void SetCurrentEditStockChanged(bool fFlag) noexcept { m_fCurrentEditStockChanged = fFlag; }

  bool AddChoicedStock(CChinaStockPtr pStock);
  bool DeleteChoicedStock(CChinaStockPtr pStock);
  long GetChoicedStockSize(void) noexcept { return m_vChoicedStock.size(); }
  void ClearChoiceStockContainer(void) noexcept { m_vChoicedStock.clear(); }
  long GetChoicedRTDataSize(void) noexcept { return m_qRTData.size(); }
  void ClearChoicedRTDataQueue(void) noexcept { while (m_qRTData.size() > 0) m_qRTData.pop(); }

  void ResetSinaRTDataInquiringIndex(void) noexcept { m_lSinaRTDataInquiringIndex = 0; }
  void ResetTengxunRTDataInquiringIndex(void) noexcept { m_lTengxunRTDataInquiringIndex = 0; }
  void ResetNeteaseRTDataInquiringIndex(void) noexcept { m_lNeteaseRTDataInquiringIndex = 0; }
  void ResetNeteaseDayLineDataInquiringIndex(void) noexcept { m_lNeteaseDayLineDataInquiringIndex = 0; }
  long GetSinaRTDataInquiringIndex(void) noexcept { return m_lSinaRTDataInquiringIndex; }
  long GetTengxunRTDataInquiringIndex(void) noexcept { return m_lTengxunRTDataInquiringIndex; }
  long GetNeteaseRTDataInquiringIndex(void) noexcept { return m_lNeteaseRTDataInquiringIndex; }
  long GetNeteaseDayLineDataInquiringIndex(void) noexcept { return m_lNeteaseDayLineDataInquiringIndex; }

  int GetDayLineNeedUpdateNumber(void) { int i = m_iDayLineNeedUpdate; return i; }
  void SetDayLineNeedUpdateNumber(int i) { m_iDayLineNeedUpdate = i; }
  int GetDayLineNeedProcessNumber(void) { int i = m_iDayLineNeedProcess; return i; }
  void SetDayLineNeedProcessNumber(int i) { m_iDayLineNeedProcess = i; }
  int GetDayLineNeedSaveNumber(void) { int i = m_iDayLineNeedSave; return i; }
  void SetDayLineNeedSaveNumber(int i) { m_iDayLineNeedSave = i; }
  void IncreaseNeteaseDayLineNeedUpdateNumber(int iNumber = 1) { m_iDayLineNeedUpdate += iNumber; }
  void DecreaseNeteaseDayLineNeedUpdateNumber(int iNumber = 1) { if (m_iDayLineNeedUpdate >= iNumber) m_iDayLineNeedUpdate -= iNumber; }
  void IncreaseNeteaseDayLineNeedProcessNumber(int iNumber = 1) { m_iDayLineNeedProcess += iNumber; }
  void DecreaseNeteaseDayLineNeedProcessNumber(int iNumber = 1) { if (m_iDayLineNeedProcess >= iNumber) m_iDayLineNeedProcess -= iNumber; }
  void IncreaseNeteaseDayLineNeedSaveNumber(int iNumber = 1) { m_iDayLineNeedSave += iNumber; }
  void DecreaseNeteaseDayLineNeedSaveNumber(int iNumber = 1) { if (m_iDayLineNeedSave >= iNumber) m_iDayLineNeedSave -= iNumber; }

  void SetRecordRTData(bool fFlag) noexcept { m_fSaveRTData = fFlag; }
  bool IsRecordingRTData(void) noexcept { if (m_fSaveRTData) return true; else return false; }

  void SetUpdateStockCodeDB(bool fFlag) noexcept { m_fUpdateStockCodeDB = fFlag; }
  bool IsUpdateStockCodeDB(void) noexcept { bool fFlag = m_fUpdateStockCodeDB; return fFlag; }
  void SetUpdateOptionDB(bool fFlag) noexcept { m_fUpdateOptionDB = fFlag; }
  bool IsUpdateOptionDB(void) noexcept { bool fFlag = m_fUpdateOptionDB; return fFlag; }
  void SetUpdateChoicedStockDB(bool fFlag) noexcept { m_fUpdateChoicedStockDB = fFlag; }
  bool IsUpdateChoicedStockDB(void) noexcept { bool fFlag = m_fUpdateChoicedStockDB; return fFlag; }

  INT64 GetRTDataReceived(void) noexcept { return m_llRTDataReceived; }
  void SetRTDataReceived(INT64 llValue) noexcept { m_llRTDataReceived = llValue; }

  bool CheckMarketReady(void) noexcept;

  bool ChangeCurrentStockToNextStock(void);
  bool ChangeCurrentStockToPrevStock(void);

protected:
  // 初始化
  bool CreateTotalStockContainer(void); //此函数是构造函数的一部分，不允许单独调用。使用Mock类测试时，派生Mock类中将CChinaStock改为CMockChinaStock。

protected:
  vector<CChinaStockPtr> m_vChinaMarketAStock; // 本系统允许的所有股票池（无论代码是否存在）
  map<CString, long> m_mapChinaMarketAStock; // 将所有被查询的股票代码映射为偏移量（目前只接受A股信息）
  long m_lTotalStock; // 股票代码总数
  long m_lTotalActiveStock;	// 当天股票总数

  vector<CChinaStockPtr> m_vChoicedStock; // 自选股票池
  vector<CChinaStockPtr> m_v10RSStrong1Stock; // 10日强势股票集
  vector<CChinaStockPtr> m_v10RSStrong2Stock; // 10日强势股票集
  bool m_fChoiced10RSStrong1StockSet; // 本日的10日强势股票集已计算完成
  bool m_fChoiced10RSStrong2StockSet; // 本日的10日强势股票集已计算完成

  INT64 m_llRTDataReceived; // 接收到的实时数据数量

  queue<CRTDataPtr> m_qRTData;
  bool m_fSaveRTData;

  int m_iMarketOpenOffset; // 开市的偏移量。以分钟为单位，0930 = 0，1129 = 120， 1300 = 121， 1459 = 240。

  bool m_fCurrentEditStockChanged;
  int m_iCountDownSlowReadingRTData; // 慢速读取实时数据计数器
  bool m_fMarketOpened; // 是否开市
  bool m_fStartReceivingData; // 是否开始接收实时数据
  bool m_fGetRTData; // 读取实时数据标识
  bool m_fSaveDayLine; // 将读取的日线存入数据库标识
  bool m_fRTDataSetCleared; // 实时数据库已清除标识。九点三十分之前为假，之后设置为真。
  CChinaStockPtr m_pCurrentStock; // 当前显示的股票

  time_t m_ttNewestTransactionTime;

  bool m_fUsingSinaRTDataReceiver; // 使用新浪实时数据提取器
  bool m_fUsingNeteaseRTDataReceiver; // 使用网易实时数据提取器
  bool m_fUsingTengxunRTDataReceiver; // 使用腾讯实时数据提取器
  int m_iCountDownTengxunNumber;
  int m_iCountDownNeteaseNumber;

  CString m_strSinaRTDataInquiringStr;
  CString m_strTengxunRTDataInquiringStr;
  CString m_strNeteaseRTDataInquiringStr;
  CString m_strNeteaseDayLineDataInquiringStr;

  long m_lSinaRTDataInquiringIndex;
  long m_lTengxunRTDataInquiringIndex;
  long m_lNeteaseRTDataInquiringIndex;
  long m_lNeteaseDayLineDataInquiringIndex;

  // Option各选项
  long m_lRelativeStrongStartDay;
  long m_lRelativeStrongEndDay;
  long m_lLastLoginDay; // 上次登录日期。如果此日期为昨日的话，则无需下载日线历史数据
  long m_lUpdatedDayFor10DayRS2;
  long m_lUpdatedDayFor10DayRS1;

  vector<CChinaStockPtr> m_vpSelectedStock; // 当前选择的股票
  bool m_fLoadedSelectedStock;

  bool m_fCurrentStockChanged; // 当前选择的股票改变了
  INT64 m_lTotalMarketBuy; // 沪深市场中的A股向上买入金额
  INT64 m_lTotalMarketSell; // 沪深市场中的A股向下卖出金额

  // 系统状态区
  bool m_fSystemReady; // 市场初始态已经设置好
  bool m_fTodayStockProcessed; // 今日是否执行了股票收盘
  bool m_fCheckActiveStock; // 是否查询今日活跃股票代码
  bool m_fTodayTempDataLoaded; //今日暂存的临时数据是否加载标识。

  // 多线程读取之变量
  CString m_strStockCodeForInquiringSinaRTData;
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
