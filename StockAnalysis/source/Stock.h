#pragma once

enum {
  __NO_TRANSACTION__ = 0,
  __ATTACK_BUY__ = 1,
  __STRONG_BUY__ = 2,
  __ORDINARY_BUY__ = 3,
  __UNKNOWN_BUYSELL__ = 4,
  __ORDINARY_SELL__ = 5,
  __STRONG_SELL__ = 6,
  __ATTACK_SELL__ = 7
};

#include"SetDayLine.h"

#include"SetRealTimeData.h"
#include"RTData.h"
#include"DayLine.h"
#include"OneDeal.h"

using namespace std;
#include<vector>
#include<memory>
#include<deque>
#include<map>
#include<array>

// 证券名称数据包
class CStock : public CObject {
public:
  CStock(void);
  ~CStock(void);
  void Reset(void);

  void operator=(CStock &);

public:
  WORD GetMarket(void) noexcept { return m_wMarket; }
  void SetMarket(WORD wValue) noexcept { m_wMarket = wValue; }
  bool IsActive(void) noexcept { return m_fActive; }
  void SetActive(bool fFlag) noexcept { m_fActive = fFlag; }
  CString GetStockCode(void) { return m_strStockCode; }
  void SetStockCode(CString str) { m_strStockCode = str; }
  long GetCode(void) noexcept { return m_iStockCode; }
  void SetCode(long lValue) { m_iStockCode = lValue; }
  CString GetStockName(void) { return m_strStockName; }
  void SetStockName(CString str) { m_strStockName = str; }

  time_t GetTime(void) noexcept { return m_Time; }
  void SetTime(time_t time) noexcept { m_Time = time; }
  long GetLastClose(void) noexcept { return m_lLastClose; }
  void SetLastClose(long lValue) noexcept { m_lLastClose = lValue; }
  long GetOpen(void) noexcept { return m_lOpen; }
  void SetOpen(long lValue) noexcept { m_lOpen = lValue; }
  long GetHigh(void) noexcept { return m_lHigh; }
  void SetHigh(long lValue) noexcept { m_lHigh = lValue; }
  long GetLow(void) noexcept { return m_lLow; }
  void SetLow(long lValue) noexcept { m_lLow = lValue; }
  long GetNew(void) noexcept { return m_lNew; }
  void SetNew(long lValue) noexcept { m_lNew = lValue; }
  INT64 GetAmount(void) noexcept { return m_lAmount; }
  void SetAmount(INT64 llValue) noexcept { m_lAmount = llValue; }
  INT64 GetVolume(void) noexcept { return m_lVolume; }
  void SetVolume(INT64 llValue) noexcept { m_lVolume = llValue; }
  long GetPBuy(int iIndex) { return m_lPBuy.at(iIndex); }
  long GetVBuy(int iIndex) { return m_lVBuy.at(iIndex); }
  long GetPSell(int iIndex) { return m_lPSell.at(iIndex); }
  long GetVSell(int iIndex) { return m_lVSell.at(iIndex); }

  INT64 GetAttackBuyAmount(void) noexcept { return m_lAttackBuyAmount; }
  INT64 GetAttackSellAmount(void) noexcept { return m_lAttackSellAmount; }
  INT64 GetOrdinaryBuyVolume(void) noexcept { return m_lOrdinaryBuyVolume; }
  INT64 GetOrdinarySellVolume(void) noexcept { return m_lOrdinarySellVolume; }
  INT64 GetAttackBuyVolume(void) noexcept { return m_lAttackBuyVolume; }		// 向上买入。成交价高于卖一价但低于卖二价。次数量包括下面的强买量。
  INT64 GetStrongBuyVolume(void) noexcept { return m_lStrongBuyVolume; }		// 向上强力买入,成交价超过之前的卖二报价
  INT64 GetCurrentAttackBuy(void) noexcept { return m_lCurrentAttackBuy; }
  long GetCurrentStrongBuy(void) noexcept { return m_lCurrentStrongBuy; }
  INT64 GetAttackSellVolume(void) noexcept { return m_lAttackSellVolume; }			// 向下卖出。成交价低于买一价但高于买二价。
  INT64 GetStrongSellVolume(void) noexcept { return m_lStrongSellVolume; }
  long GetCurrentAttackSell(void) noexcept { return m_lCurrentAttackSell; }
  long GetCurrentStrongSell(void) noexcept { return m_lCurrentStrongSell; }
  INT64 GetUnknownVolume(void) noexcept { return m_lUnknownVolume; }
  long GetCurrentUnknown(void) noexcept { return m_lCurrentUnknown; }
  INT64 GetCancelBuyVolume(void) noexcept { return m_lCancelBuyVolume; }
  INT64 GetCancelSellVolume(void) noexcept { return m_lCancelSellVolume; }

  int  GetCurrentTransactionType(void) noexcept { return m_nCurrentTransactionType; }
  INT64 GetCurrentTransationVolume(void) noexcept { return m_lCurrentGuadanTransactionVolume; }

  double GetRelativeStrong(void) noexcept { return m_dRelativeStrong; }
  long GetTransactionNumber(void) noexcept { return m_lTransactionNumber; }
  long GetTransactionNumberBelow5000(void) noexcept { return m_lTransactionNumberBelow5000; }
  long GetTransactionNumberBelow50000(void) noexcept { return m_lTransactionNumberBelow50000; }
  long GetTransactionNumberBelow200000(void) noexcept { return m_lTransactionNumberBelow200000; }
  long GetTransactionNumberAbove200000(void) noexcept { return m_lTransactionNumberAbove200000; }

  INT64 GetAttackBuyBelow50000(void) noexcept { return m_lAttackBuyBelow50000; }
  INT64 GetAttackBuyBelow200000(void) noexcept { return m_lAttackBuyBelow200000; }
  INT64 GetAttackBuyAbove200000(void) noexcept { return m_lAttackBuyAbove200000; }
  INT64 GetAttackSellBelow50000(void) noexcept { return m_lAttackSellBelow50000; }
  INT64 GetAttackSellBelow200000(void) noexcept { return m_lAttackSellBelow200000; }
  INT64 GetAttackSellAbove200000(void) noexcept { return m_lAttackSellAbove200000; }

  double GetCurrentGuaDanTransactionPrice(void) noexcept { return m_dCurrentGuaDanTransactionPrice; }
  long GetGuaDan(long lPrice) noexcept { return m_mapGuaDan.at(lPrice); }
  void SetGuaDan(long lPrice, long lVolume) noexcept { m_mapGuaDan[lPrice] = lVolume; }
  bool HaveGuaDan(long lPrice) noexcept { if (m_mapGuaDan.find(lPrice) == m_mapGuaDan.end()) return false; return true; }

	bool IsChoiced( void ) noexcept { return m_fChoiced; }
	void SetChoicedFlag( bool fChoiced ) noexcept { m_fChoiced = fChoiced; }

	bool IsMinLineUpdated( void ) noexcept { return ( m_fMinLineUpdated ); }
	void SetMinLineUpdated( bool fUpdate ) noexcept { m_fMinLineUpdated = fUpdate; }

	bool IsDayLineUpdated(void) noexcept { return ( m_fDayKLineUpdated ); }
	void SetDayLineUpdated(bool fUpdate) noexcept { m_fDayKLineUpdated = fUpdate; }

  bool IsDayLineLoaded(void) noexcept { return m_fDayLineLoaded; }
  void SetDayLineLoaded(bool fFlag) noexcept { m_fDayLineLoaded = fFlag; }

  bool IsStartCalculating(void) noexcept { return m_fStartCalculating; }
  bool SetStartCalculating(bool fFlag) noexcept { if (m_fStartCalculating || !fFlag) return false; m_fStartCalculating = fFlag; return true; }

	void UpdataCurrentStatus(CStockRTDataPtr pRTData);
	bool LoadDayLine(CSetDayLine * psetDayLine);
	
  // 计算实时数据, 由工作线程ClientThreadCalculatingRTDataProc调用
  bool CalculateRTData(void);
  bool CalculateOneRTData(CStockRTDataPtr pData);
  bool AnalysisingGuaDan(CStockRTDataPtr pCurrentRTData, CStockRTDataPtr pLastRTData, int nTransactionType, long lCurrentTransactionPrice);
  void ReportGuaDanTransaction(void);
  void ReportGuaDan(void);

	bool SaveRealTimeData(CSetRealTimeData * psetRT);

  // 采用同步机制存取实时数据
  void PushRTStockData(CStockRTDataPtr pData);
  CStockRTDataPtr PopRTStockData(void);
  long GetRTDataDequeSize(void);

  // 由于处理日线历史数据的函数位于不同的线程中，故而需要同步机制设置标识
  void SetDayLineNeedSavingFlag(bool fFlag);
  bool IsDayLineNeedSaving(void);
		
#ifdef _DEBUG
	virtual	void AssertValid() const;
	virtual	void Dump(CDumpContext& dc) const;
#endif

public:
  // 测试专用函数
  void TestSetGuaDanDeque(long lPrice, long lVolume) { m_mapGuaDan[lPrice] = lVolume; } // 预先设置挂单。

public:

	vector<CDayLinePtr>				m_vDayLine;			// 日线数据容器
 
protected:
  // 基本信息
  WORD      m_wMarket;                // 市场索引
  bool      m_fActive;                // 今天是否接受到实时数据
  CString		m_strStockCode;						// 证券代码，
  CString		m_strStockName;						// 证券名称
  int       m_iStockCode;             // 证券代码值
  short			m_nHand;									// 每手股数


  // 实时数据,需要不断更新
  time_t    m_Time;
  long	    m_lLastClose;		// 以0.001元计的收盘价
  long		  m_lOpen;				// 以0.001元计的开盘价		
  long		  m_lHigh;				// 以0.001元计的最高价
  long		  m_lLow;					// 以0.001元计的最低价
  long		  m_lNew;					// 以0.001元计的最新价
  INT64	    m_lVolume;			// 以1股计的成交量
  INT64     m_lAmount;			// 以元计的成交金额
  array<long, 5>      m_lPBuy;			// 买盘价。单位：0.001元
  array<long, 5>			m_lVBuy;			// 买盘量。单位：股
  array<long, 5>		  m_lPSell;		// 卖盘价。单位：0.001元
  array<long, 5>			m_lVSell;		// 卖盘量。单位：股

    // 以下变量用于分析买入卖出具体情况
  INT64	    m_lAttackBuyAmount;		// 向上买入金额
  INT64     m_lAttackSellAmount;	// 向下卖出金额
  INT64	    m_lCurrentVolume;

  double    m_dRelativeStrong; // 单位：1%
  long      m_lTransactionNumber; // 本交易日的成交笔数
  long      m_lTransactionNumberBelow5000; // 本交易日低于5000股的成交笔数
  long      m_lTransactionNumberBelow50000; // 
  long      m_lTransactionNumberBelow200000; // 
  long      m_lTransactionNumberAbove200000; // 

  INT64     m_lOrdinaryBuyVolume;
  INT64     m_lOrdinarySellVolume;
  INT64     m_lAttackBuyVolume;		// 向上买入。成交价高于卖一价但低于卖二价。次数量包括下面的强买量。
  INT64	    m_lStrongBuyVolume;		// 向上强力买入,成交价超过之前的卖二报价
  long    	m_lCurrentAttackBuy;
  long    	m_lCurrentStrongBuy;
  INT64     m_lAttackSellVolume;			// 向下卖出。成交价低于买一价但高于买二价。
  INT64     m_lStrongSellVolume;		// 向下强力卖出,成交价低于之前的买二报价
  long      m_lCurrentAttackSell;
  long      m_lCurrentStrongSell;
  INT64	    m_lUnknownVolume;
  long      m_lCurrentUnknown;
  INT64	    m_lCancelBuyVolume;		// 买单撤单量
  INT64     m_lCancelSellVolume;			// 卖单撤单量

  INT64	    m_lAttackBuyBelow50000;
  INT64	    m_lAttackBuyBelow200000;
  INT64	    m_lAttackBuyAbove200000;
  INT64	    m_lAttackSellBelow50000;
  INT64	    m_lAttackSellBelow200000;
  INT64	    m_lAttackSellAbove200000;

  //
  INT64	    m_lFirstDataVolume;			  // 用于存储第一次实时数据的成交量

  deque<COneDealPtr>    m_dequeDeal;        // 具体成交信息队列

  // 挂单的具体情况。
  map<long, long>       m_mapGuaDan;        // 采用map结构存储挂单的具体情况。索引为价位，内容为挂单量。
  CStockRTDataPtr       m_pLastRTData;        // 从m_dequeRTData读出的上一个实时数据。
  INT64                 m_lCurrentGuadanTransactionVolume; // 当前挂单交易量（不是目前的时间，而是实时数据队列最前面数据的时间）
  double                m_dCurrentGuaDanTransactionPrice; // 当前成交价格
  int                   m_nCurrentTransactionType; // 当前交易类型（强买、进攻型买入。。。。）
  INT64                 m_lCurrentCanselSellVolume;
  INT64                 m_lCurrentCanselBuyVolume;
  
  bool			            m_fDayLineLoaded;					// 是否装入了日线数据

  bool                  m_fDayLineNeededSaving;   // 日线数据是否需要存储
  CCriticalSection      m_DayLineNeedSavingLock;  // 上述标识的同步锁

  deque<CStockRTDataPtr>m_dequeRTStockData;  // 实时数据队列
  CCriticalSection      m_RTDataLock;   // 实时数据队列的同步锁

  bool                  m_fStartCalculating;  // 实时数据开始计算标识。第一个实时数据只能用来初始化系统，不能用于计算。从第二个数据开始计算才有效。
 	
	bool			            m_fChoiced;									// 此股票是否是自选股票.
	bool			            m_fMinLineUpdated;					// 今天的分钟资料是否更新过.
	bool			            m_fDayKLineUpdated;					// 今天的日线资料是否更新过.

};

typedef shared_ptr<CStock> CStockPtr;

