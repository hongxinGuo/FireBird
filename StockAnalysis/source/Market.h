#ifndef __MARKET_H__
#define __MERKET_H__

#include"stdafx.h"

#include "Stock.h"
#include"SetStockCode.h"

#include"globedef.h"

using namespace std;
#include<vector>
#include<map>

class CMarket: public CObject 
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
	// 初始化市场
  bool          UpdateStockCodeDataBase(void);
  bool          UpdateOptionDataBase(void);

  // 实时数据和日线历史数据读取
  bool          CreateRTDataInquiringStr(CString& str);
  int						GetInquiringStockStr(CString& str);
  bool          GetSinaStockRTData(void);
  bool          CreateDayLineInquiringStr(CString& str, CString& strStartDay);
  bool          GetNetEaseStockDayLineData(void);


  bool          IsAStock(CStockPtr pStock);			// 是否为沪深A股
  bool          IsAStock(CString strStockCode);			// 是否为沪深A股
  bool          IsStock( CString  strStockCode, CStockPtr & pStock );	// 是否为正确的股票代码

	CString			  GetStockName( CString strStockCode );

	// 得到股票索引
	bool			    GetStockIndex(CString strStockCode, long & lIndex );
	// 得到股票指针
	CStockPtr			GetStockPtr(CString strStockCode);
	CStockPtr     GetStockPtr( long lIndex );

  // 存储新股票指针入活跃股票池
  void          AddStockToMarket(CStockPtr pStock);

  // 得到股票ID指针
  bool          GetStockIDPtr(CString strStockCode, StockIDPtr & pStockIDPtr);

	// 得到当前显示股票
	CStockPtr     GetShowStock( void ) noexcept { return m_pCurrentStock; }
  void					SetShowStock(CStockPtr pStock);
  bool          IsCurrentStockChanged(void);

	void					SetShowStock( CString strStockCode );

	long					GetTotalStock( void ) noexcept { return m_lTotalActiveStock; }

	long					GetMinLineOffset( CStockID sID, time_t Time );

	bool					MarketReady( void ) noexcept { return m_fMarketReady; }
  void          SetMarketReadyFlag(bool fFlag) noexcept { m_fMarketReady = fFlag; }

	bool					SaveDayLine(CSetDayLine * psetDayLine, CSetStockCode * psetStockCode,
                            CStockPtr pStock, vector<CDayLinePtr> & vectorDayLine, bool fReversed = true );
  bool          SaveOneRecord(CSetDayLine * psetDayLine, CDayLinePtr pDayLine);

  bool          IsTodayStockCompiled(void) noexcept { return m_fTodayStockCompiled; }
  void          SetTodayStockCompiledFlag(bool fFlag) noexcept { m_fTodayStockCompiled = fFlag; }

  // 实时数据处理函数，将读取到的实时数据存入数据库中
  bool          SaveRTData(void);

	  // 日线数据处理函数，将读取到的日线数据存入数据库中
  bool          SaveDayLineData(void);
  
  bool          ClearAllDayLineVector(void);

  // 是否所有股票的历史日线数据都检查过了。
  bool          IsTotalStockDayLineChecked(void);

  bool          CompileCurrentTradeDayStocks(long lCurrentTradeDay);
  bool          CalculateOneDayRelativeStrong(long lDay);

	bool					IsLoadSelectedStock( void ) noexcept { return m_fLoadedSelectedStock; }
	void					SetLoadSelectedStock( bool fLoad ) noexcept { m_fLoadedSelectedStock = fLoad; }

  CString       GetDownLoadingStockCodeStr(void) { return m_strCurrentStockDownLoading; }
  void          SetDownLoadingStockCodeStr(CString str) { m_strCurrentStockDownLoading = str; }

  long          GetRelativeStrongStartDay(void) noexcept { return m_lRelativeStrongStartDay; }
  void          SetRelativeStrongStartDay(long lDay) noexcept { m_lRelativeStrongStartDay = lDay; }
  long          GetRelativeStrongEndDay(void) noexcept { return m_lRelativeStrongEndDay; }
  void          SetRelativeStrongEndDay(long lDay) noexcept { m_lRelativeStrongEndDay = lDay; }

	INT64	        GetTotalAttackBuyAmount( void );
	INT64         GetTotalAttackSellAmount( void );


  //处理个股票的实时数据，计算挂单变化等。由工作线程ClientThreadCalculatingRTDataProc调用。
  bool          CalculateRTData(void);

  //处理实时股票变化等
  bool          ProcessRTData(void);
  // 处理日线历史数据
  bool					ProcessDayLineData(char * buffer, long lLength);
	bool					ProcessOneItemDayLineData(CDayLinePtr pDayLine, char *& pCurrentPos, long & lLength);
	bool					ReadOneValue(char *& pCurrentPos, char * buffer, long& iReadNumber);
  bool          ReadOneValueExceptperiod(char *& pCurrentPos, char * buffer, long & lCounter);

  // 定时更新，完成具体调度任务。由主线程的OnTimerha函数调用
  bool          SchedulingTask(void);
  bool          SchedulingTaskPerSecond(void);

private:
  // 初始化
  bool            CreateTotalStockContainer(void);    // 此函数是构造函数的一部分，不允许单独调用。


public :
  const CString gl_strRTStockSource = _T("http://hq.sinajs.cn/list=");
  const CString gl_strDayLineStockSource = _T("http://quotes.money.163.com/service/chddata.html?code=");
  const CString gl_strDayLinePostfix = _T("&fields=TCLOSE;HIGH;LOW;TOPEN;LCLOSE;CHG;TURNOVER;VOTURNOVER;VATURNOVER;TCAP;MCAP");
  
  CStockPtr                   m_pCurrentStock;          // 当前显示的股票
  char                        m_aStockCodeTemp[30];
  bool                        m_fCurrentEditStockChanged;
  bool                        m_fMarketOpened;          // 是否开市
  bool										    m_fGetRTStockData;
  bool										    m_fGetDayLineData;
  bool                        m_fCountDownRT;
  int                         m_iCountDownDayLine;        // 日线数据读取延时计数。
  int                         m_iCountDownRT;

  vector<StockIDPtr>	        gl_vTotalStock;             // 本系统允许的所有股票池（无论代码是否存在）
  size_t                      GetTotalStockMapIndexSize(void) { return gl_mapTotalStockToIndex.size(); }
  long                        GetTotalStockIndex(CString str) { return gl_mapTotalStockToIndex.at(str); }
protected :
  map<CString, long>	        gl_mapTotalStockToIndex;		// 将所有被查询的股票代码映射为偏移量（目前只接受A股信息）
  CString                     m_strCurrentStockDownLoading; // 目前正在下载日线历史数据的股票代码
  
  long                        m_lRelativeStrongStartDay;
  long                        m_lRelativeStrongEndDay;

  bool                        m_fTodayStockCompiled;        // 今日是否执行了股票收盘

	map<CString, long>          m_mapActiveStockToIndex;		// 将市场活跃的股票代码映射为偏移量
	vector<CStockPtr>						m_vActiveStock;									//当天股票数据

  long												m_lTotalActiveStock;						// 当天股票总数
  vector<CStockPtr>::iterator m_itStock;
  bool                        m_fResetm_ItStock;    // 重置m_itStock标识

	vector<CStockPtr>           m_vpSelectedStock;		// 当前选择的股票
	bool												m_fLoadedSelectedStock;
	
	bool												m_fMarketReady;					// 市场初始态已经设置好

  bool                        m_fCurrentStockChanged;   // 当前选择的股票改变了
	INT64										    m_lTotalMarketBuy;				// 沪深市场中的A股向上买入金额
	INT64										    m_lTotalMarketSell;					// 沪深市场中的A股向下卖出金额 
	
  bool                        m_fCheckTodayActiveStock; // 是否查询今日活跃股票代码


  bool                        m_fUpdatedStockCodeDataBase;

private:


};

#endif

