#ifndef __MARKET_H__
#define __MERKET_H__

#include"stdafx.h"
#include"globedef.h"
#include"CriticalSectionclock.h"

#include "Stock.h"
#include"SetStockCode.h"

#include"CrweberIndex.h"

using namespace std;
#include<vector>
#include<map>

class CMarket final : public CObject
{
public:
  // 只能有一个实例
  CMarket(void);
  ~CMarket(void);
  void Reset(void);

#ifdef _DEBUG
  virtual	void AssertValid() const;
  virtual	void Dump(CDumpContext& dc) const;
#endif

  // interface function
public:
  // 系统状态区
  bool IsPermitResetSystem(void) noexcept { return m_fPermitResetSystem; }
  void SetPermitResetSystem(bool fFlag) noexcept { m_fPermitResetSystem = fFlag; }

  // 初始化市场

  // 实时数据读取
  bool CreateSinaRTDataInquiringStr(CString& str);
  bool CreateTengxunRTDataInquiringStr(CString& str);
  int GetSinaInquiringStockStr(CString& str);
  int	GetTengxunInquiringStockStr(CString& str);
  int GetInquiringStr(CString& str, vector<CStockPtr>::iterator& itStock, CString strPostfix, long lTotalNumber);
  bool GetTengxunStockRTData(void);
  void ResetIT(void); //重置各迭代器

  //日线历史数据读取
  bool CreateNeteaseDayLineInquiringStr(CString& str, CString& strStartDay);

  bool IsAStock(CStockPtr pStock); // 是否为沪深A股
  bool IsAStock(CString strStockCode); // 是否为沪深A股
  bool IsStock(CString  strStockCode, CStockPtr& pStock);	// 是否为正确的股票代码

  CString GetStockName(CString strStockCode);

  // 得到股票索引
  bool GetStockIndex(CString strStockCode, long& lIndex);
  // 得到股票指针
  CStockPtr GetStockPtr(CString strStockCode);
  CStockPtr GetStockPtr(long lIndex);

  // 存储新股票指针入活跃股票池
  void AddStockToMarket(CStockPtr pStock);

  // 初始化实时数据计数器
  bool CountLoopRTDataInquiring(void) { if (++m_lCountLoopRTDataInquiring >= 3) return true; else return false; }

  // 得到当前显示股票
  CStockPtr GetShowStock(void) noexcept { return m_pCurrentStock; }
  void SetShowStock(CString strStockCode);
  void SetShowStock(CStockPtr pStock);
  bool IsCurrentStockChanged(void);

  long GetTotalActiveStock(void) noexcept { return m_lTotalActiveStock; }

  long GetMinLineOffset(CStock sID, time_t Time);

  bool SystemReady(void) noexcept { return m_fSystemReady; }
  void SetSystemReady(bool fFlag) noexcept { m_fSystemReady = fFlag; }

  bool IsTodayStockCompiled(void) noexcept { return m_fTodayStockCompiled; }
  void SetTodayStockCompiledFlag(bool fFlag) noexcept { m_fTodayStockCompiled = fFlag; }

  bool SaveDayLine(CStockPtr pStock);
  bool SaveOneRecord(CSetDayLine* psetDayLine, CDayLinePtr pDayLine);

  // 数据库读取存储操作
  bool SaveRTData(void);  // 实时数据处理函数，将读取到的实时数据存入数据库中
  bool SaveDayLineData(void);  // 日线数据处理函数，将读取到的日线数据存入数据库中
  bool SaveCrweberIndexData(void);  // crweber.com油运指数存储函数
  bool UpdateStockCodeDB(void);
  void LoadStockCodeDB(void);
  bool UpdateOptionDB(void);
  void LoadOptionDB(void);
  bool UpdateTempRTData(void);
  bool UpdateTodayTempDB(void);
  bool LoadTodayTempDB(void);

  bool ClearAllDayLineVector(void);

  // 是否有股票的历史日线数据需要更新。
  bool IsDayLineNeedUpdate(void);

  bool IsDayLineNeedSaving(void);

  // 是否所有股票的历史日线数据都查询过一遍了
  bool IsDayLineDataInquiringOnce(void);

  long CompileCurrentTradeDayStock(long lCurrentTradeDay);

  bool CalculateOneDayRelativeStrong(long lDay);

  bool IsLoadSelectedStock(void) noexcept { return m_fLoadedSelectedStock; }
  void SetLoadSelectedStock(bool fLoad) noexcept { m_fLoadedSelectedStock = fLoad; }

  bool IsCheckTodayActiveStock(void) noexcept { return m_fCheckTodayActiveStock; }

  bool IsTodayTempRTDataLoaded(void) noexcept { return m_fTodayTempDataLoaded; }
  void SetTodayTempRTDataLoaded(bool fFlag) noexcept { m_fTodayTempDataLoaded = fFlag; }

  bool IsCalculatingRS(void) noexcept { return m_fCalculatingRS; }
  void SetCalculatingRS(bool fFlag) noexcept { m_fCalculatingRS = fFlag; }

  long GetRelativeStrongStartDay(void) noexcept { return m_lRelativeStrongStartDay; }
  void SetRelativeStrongStartDay(long lDay) noexcept { m_lRelativeStrongStartDay = lDay; }
  long GetRelativeStrongEndDay(void) noexcept { return m_lRelativeStrongEndDay; }
  void SetRelativeStrongEndDay(long lDay) noexcept { m_lRelativeStrongEndDay = lDay; }
  long GetLastLoginDay(void) noexcept { return m_lLastLoginDay; }
  void SetLastLoginDay(long lDay) noexcept { m_lLastLoginDay = lDay; }

  INT64 GetTotalAttackBuyAmount(void);
  INT64 GetTotalAttackSellAmount(void);

  size_t GetTotalStockMapIndexSize(void) noexcept { return m_mapChinaMarketAStock.size(); }
  long GetTotalStockIndex(CString str) { return m_mapChinaMarketAStock.at(str); }

  void SetReadingSinaRTDataTime(clock_t tt) { m_ReadingSinaRTDataTime.SetTime(tt); }
  clock_t GetReadingSinaRTDataTime(void) { return m_ReadingSinaRTDataTime.GetTime(); }
  void SetReadingTengxunRTDataTime(clock_t tt) { m_ReadingTengxunRTDataTime.SetTime(tt); }
  clock_t GetReadingTengxunRTDataTime(void) { return m_ReadingTengxunRTDataTime.GetTime(); }
  void SetReadingNeteaseDayDataTime(clock_t tt) { m_ReadingNeteaseDayDataTime.SetTime(tt); }
  clock_t GetReadingNeteaseDayDataTime(void) { return m_ReadingNeteaseDayDataTime.GetTime(); }

  //处理个股票的实时数据，计算挂单变化等。由工作线程ThreadCalculatingRTDataProc调用。
  bool ProcessRTData(void);

  //处理实时股票变化等
  bool ProcessSinaRTDataReceivedFromWeb(void);
  // 处理日线历史数据
  bool ProcessNeteaseDayLineData(CNeteaseDayLineWebData* pWebData);
  bool ProcessOneItemDayLineData(CString strStockCode, CDayLinePtr pDayLine, char*& pCurrentPos, long& lLength);
  // 虽然这个函数与读取新浪实时数据的完全一样，但为了防止以后可能改变的缘故，还是分别实现。
  bool ReadOneValue(char*& pCurrentPos, char* buffer, long& iReadNumber);
  bool ReadOneValueExceptPeriod(char*& pCurrentPos, char* buffer, long& lCounter);

  // 定时更新，完成具体调度任务。由主线程的OnTimer函数调用
  bool SchedulingTask(void);
  bool SchedulingTaskPerSecond(long lSecondNumber);
  bool SchedulingTaskPerHour(long lSecondNumber, long lCurrentTime);
  bool SchedulingTaskPer5Minutes(long lSecondNumber, long lCurrentTime);
  bool SchedulingTaskPer1Minute(long lSecondNumber, long lCurrentTime);
  bool SchedulingTaskPer10Seconds(long lSecondNumber, long lCurrentTime);
  void ResetSystemFlagAtMidnight(long lCurrentTime);
  void SaveTempDataIntoDB(long lCurrentTime);

private:
  // 初始化
  bool CreateTotalStockContainer(void); // 此函数是构造函数的一部分，不允许单独调用。

public:

  CStockPtr m_pCurrentStock; // 当前显示的股票
  char m_aStockCodeTemp[30];
  bool m_fCurrentEditStockChanged;
  bool m_fMarketOpened; // 是否开市
  bool m_fGetRTStockData; // 读取实时数据标识
  bool m_fReadingTengxunRTData; // 读取腾讯实时行情
  bool m_fGetDayLineData;  // 读取日线历史数据标识
  bool m_fSaveDayLine; // 将读取的日线存入数据库标识
  int m_iCountDownDayLine; // 日线数据读取延时计数。
  int m_iCountDownSlowReadingRTData; // 慢速读取实时数据计数器

  vector<CStockPtr> m_vChinaMarketAStock; // 本系统允许的所有股票池（无论代码是否存在）

  vector<CStockPtr> gl_vStockChoice; // 自选股票池

  vector<CrweberIndexPtr> gl_vCrweberIndex; // crweber.com网站上的油运指数

protected:

  map<CString, long> m_mapChinaMarketAStock; // 将所有被查询的股票代码映射为偏移量（目前只接受A股信息）

  // Option各选项
  long m_lRelativeStrongStartDay;
  long m_lRelativeStrongEndDay;
  long m_lLastLoginDay; // 上次登录日期。如果此日期为昨日的话，则无需下载日线历史数据

  map<CString, long> m_mapActiveStockToIndex;	// 将市场活跃的股票代码映射为偏移量
  vector<CStockPtr> m_vActiveStock;	//当天股票数据

  long m_lTotalActiveStock;	// 当天股票总数

  vector<CStockPtr>::iterator m_itSinaStock; // 新浪实时股票代码查询迭代器
  vector<CStockPtr>::iterator m_itTengxunStock; // 腾讯实时股票代码迭代器

  vector<CStockPtr> m_vpSelectedStock; // 当前选择的股票
  bool m_fLoadedSelectedStock;

  bool m_fCurrentStockChanged; // 当前选择的股票改变了
  INT64 m_lTotalMarketBuy; // 沪深市场中的A股向上买入金额
  INT64 m_lTotalMarketSell; // 沪深市场中的A股向下卖出金额

  long m_lCountLoopRTDataInquiring; // 全体股票池遍历计数器，用于初始化时

  bool m_fCalculatingRS;

  // 系统状态区
  bool m_fPermitResetSystem; // 允许重置系统（如果不断机多日运行的话，需要每日重置系统
  bool m_fSystemReady; // 市场初始态已经设置好
  bool m_fTodayStockCompiled; // 今日是否执行了股票收盘
  bool m_fUpdatedStockCodeDataBase; //是否更新了日线历史数据库标识
  bool m_fCheckTodayActiveStock; // 是否查询今日活跃股票代码
  bool m_fTodayTempDataLoaded; //今日暂存的临时数据是否加载标识。

  // 多线程读取之变量，需要设置同步机制
  CCriticalSectionClock m_ReadingSinaRTDataTime; // 每次读取新浪实时数据的时间
  CCriticalSectionClock m_ReadingTengxunRTDataTime; // 每次读取腾讯实时数据的时间
  CCriticalSectionClock m_ReadingNeteaseDayDataTime;    // 每次读取网易日线历史数据的时间

private:
};

#endif
