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

#ifdef _DEBUG
	virtual	void AssertValid() const;
	virtual	void Dump(CDumpContext& dc) const;
#endif

// interface function
public:
	// 初始化市场

  bool            IsAStock(CStockPtr pStock);			// 是否为沪深A股
  bool            IsAStock(CString strStockCode);			// 是否为沪深A股
  bool            IsStock( CString  strStockCode, CStockPtr & pStock );	// 是否为正确的股票代码

	CString			    GetStockName( CString strStockCode );

	// 得到股票索引
	bool			      GetStockIndex(CString strStockCode, long & lIndex );
	// 得到股票指针
	CStockPtr			  GetStockPtr(CString strStockCode);
	CStockPtr       GetStockPtr( long lIndex );

  // 得到股票ID指针
  bool            GetStockIDPtr(CString strStockCode, StockIDPtr & pStockIDPtr);

	// 得到当前显示股票
	CStockPtr       GetShowStock( void ) { return m_pCurrentStock; }
  void					  SetShowStock(CStockPtr pStock);
  bool            IsCurrentStockChanged(void) { return m_fCurrentStockChanged; }

	void					SetShowStock( CString strStockCode );

	long					GetTotalStock( void ) { return m_lTotalActiveStock; }

	long					GetMinLineOffset( CStockID sID, time_t Time );

	bool					MarketReady( void ) { return m_fMarketReady; }
  void          SetMarketReadyFlag(bool fFlag) { m_fMarketReady = fFlag; }

	bool					SaveDayLine(CSetDayLine * psetDayLine, CSetStockCode * psetStockCode,
                            CStockPtr pStock, vector<CDayLinePtr> & vectorDayLine, bool fReversed = true );
  bool          SaveOneRecord(CSetDayLine * psetDayLine, CDayLinePtr pDayLine);

  // 实时数据处理函数，将读取到的实时数据存入数据库中
  bool          SaveRTData( CSetRealTimeData * psetDayLine );

	  // 日线数据处理函数，将读取到的日线数据存入数据库中
  bool          SaveDayLineData(void);
  
  bool          ClearAllDayLineVector(void);

  // 是否所有股票的历史日线数据都检查过了。
  bool          IsTotalStockDayLineChecked(void);

  bool          CompileCurrentTradeDayStocks(long lCurrentTradeDay);

	bool					IsLoadSelectedStock( void ) { return m_fLoadedSelectedStock; }
	void					SetLoadedSelectedStock( bool fLoad ) { m_fLoadedSelectedStock = fLoad; }

	long long	    GetTotalAttackBuyAmount( void );
	long long     GetTotalAttackSellAmount( void );

  //处理实时股票变化等
  bool          ProcessRTData(void);
  int						GetInquiringStockStr(CString& str);

  //处理个股票的实时数据，计算挂单变化等。由工作线程ClientThreadCalculatingRTDataProc调用。
  bool          CalculateRTData(void);

	bool					ProcessDayLineData(char * buffer, long lLength);
	bool					ProcessOneItemDayLineData(CDayLinePtr pDayLine, char *& pCurrentPos, long & lLength);
	bool					ReadOneValue(char *& pCurrentPos, char * buffer, long& iReadNumber);
  bool          ReadOneValueExceptperiod(char *& pCurrentPos, char * buffer, long & lCounter);

  // 调度函数，处理本类中的定时更新任务。由注线程的OnTimerha函数调用
  bool          SchedulingTask(void);

public :
  CStockPtr                   m_pCurrentStock;          // 当前显示的股票
  char                        m_aStockCodeTemp[30];
  bool                        m_fCurrentEditStockChanged;

protected :
	map<CString, long>          m_mapActiveStockToIndex;		// 将市场活跃的股票代码映射为偏移量
	vector<CStockPtr>						m_vActiveStock;									//当天股票数据

  long												m_lTotalActiveStock;						// 当天股票总数
  vector<CStockPtr>::iterator  m_itStock;

	vector<CStockPtr>            m_vpSelectedStock;		// 当前选择的股票
	bool												m_fLoadedSelectedStock;
	
	bool												m_fMarketReady;					// 市场初始态已经设置好

  bool                        m_fCurrentStockChanged;   // 当前选择的股票改变了
	long long										m_lTotalMarketBuy;				// 沪深市场中的A股向上买入金额
	long long										m_lTotalMarketSell;					// 沪深市场中的A股向下卖出金额 
	
private:


};

#endif

