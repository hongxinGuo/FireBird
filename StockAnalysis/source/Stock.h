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

// 证券名称数据包
class CStock : public CObject {
public:
	CStock( void );
	~CStock ( void );
	void Reset( void );

	void operator=( CStock & );

public:

	bool IsChoiced( void ) { return m_fChoiced; }
	void SetChoicedFlag( bool fChoiced ) { m_fChoiced = fChoiced; }

	bool IsMinLineUpdated( void ) { return ( m_fMinLineUpdated ); }
	void SetMinLineUpdated( bool fUpdate ) { m_fMinLineUpdated = fUpdate; }

	bool IsDayLineUpdated( void ) { return ( m_fDayKLineUpdated ); }
	void SetDayLineUpdated( bool fUpdate ) { m_fDayKLineUpdated = fUpdate; }

	void UpdataCurrentStatus(CStockRTDataPtr pRTData);
	bool LoadDayLine(CSetDayLine * psetDayLine);
	
  // 计算实时数据, 由工作线程ClientThreadCalculatingRTDataProc调用
  bool CalculateRTData(void);
  bool AnalysisingGuaDan(CStockRTDataPtr pCurrentRTData, CStockRTDataPtr pLastRTData, int nTransactionType, double dCurrentTransactionPrice);

	bool SaveRealTimeData(CSetRealTimeData * psetRT);

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
  // 基本信息
	WORD      m_wMarket;                // 市场索引
  bool      m_fActive;                // 今天是否接受到实时数据
	CString		m_strStockCode;						// 证券代码，
	CString		m_strStockName;						// 证券名称
  int       m_iStockCode;             // 证券代码值
	short			m_nHand;									// 每手股数

	bool			m_fDayLineLoaded;					// 是否装入了日线数据

	vector<CDayLinePtr>				m_vDayLine;			// 日线数据容器

  bool      m_fStartCalculating;  // 实时数据开始计算标识。第一个实时数据只能用来初始化系统，不能用于计算。从第二个数据开始计算才有效。

	// 实时数据
	time_t    m_Time;
	long	    m_lLastClose;		// 以0.001元计的收盘价
	long		  m_lOpen;				// 以0.001元计的开盘价		
	long		  m_lHigh;				// 以0.001元计的最高价
	long		  m_lLow;					// 以0.001元计的最低价
	long		  m_lNew;					// 以0.001元计的最新价
	long long	m_lVolume;			// 以1股计的成交量
	long long m_lAmount;			// 以元计的成交金额
  double    m_dRelativeStrong; // 单位：1%
  long      m_lTransactionNumber; // 本交易日的成交笔数
  long      m_lTransactionNumberBelow5000; // 本交易日低于5000股的成交笔数
  long      m_lTransactionNumberBelow50000; // 
  long      m_lTransactionNumberBelow200000; // 
  long      m_lTransactionNumberAbove200000; // 
	long		  m_lPBuy[5];			// 买盘价。单位：0.001元
	long			m_lVBuy[5];			// 买盘量。单位：股
	long		  m_lPSell[5];		// 卖盘价。单位：0.001元
	long			m_lVSell[5];		// 卖盘量。单位：股

	// 以下变量用于分析买入卖出具体情况
  long      m_lSellPrice;     // 竞卖价格
  long      m_lBuyPrice;      // 竞买价格

	long long	m_lAttackBuyAmount;		// 向上买入金额
	long long m_lAttackSellAmount;	// 向下卖出金额
	long			m_lCurrentVolume;
  long      m_lOrdinaryBuyVolume;
  long      m_lOrdinarySellVolume;
	long			m_lAttackBuyVolume;		// 向上买入。成交价高于卖一价但低于卖二价。次数量包括下面的强买量。
	long			m_lStrongBuyVolume;		// 向上强力买入,成交价超过之前的卖二报价
	long			m_lCurrentAttackBuy;
	long			m_lCurrentStrongBuy;
	long			m_lAttackSellVolume;			// 向下卖出。成交价低于买一价但高于买二价。
	long			m_lStrongSellVolume;		// 向下强力卖出,成交价低于之前的买二报价
	long			m_lCurrentAttackSell;
	long			m_lCurrentStrongSell;
	long			m_lUnknownVolume;
	long			m_lCurrentUnknown;
	long			m_lCancelBuyVolume;		// 买单撤单量
	long			m_lCancelSellVolume;			// 卖单撤单量

	long			m_lAttackBuyBelow50000;
	long			m_lAttackBuyBelow200000;
	long			m_lAttackBuyAbove200000;
	long			m_lAttackSellBelow50000;
	long			m_lAttackSellBelow200000;
	long			m_lAttackSellAbove200000;

	//
	long long	m_lFirstDataVolume;			  // 用于存储第一次实时数据的成交量
 
public:

  deque<COneDealPtr>    m_dequeDeal;        // 具体成交信息队列

  // 挂单的具体情况。
  map<long, long>       m_mapGuaDan;        // 采用map结构存储挂单的具体情况。索引为价位，内容为挂单量。
  long                  m_lLastClose2;        // 昨收盘。单位：0.01元。
  CStockRTDataPtr       m_pLastRTData;        // 从m_dequeRTData读出的上一个实时数据。
  long                  m_lCurrentGuadanTransactionVolume; // 当前挂单交易量（不是目前的时间，而是实时数据队列最前面数据的时间）
  double                m_dCurrentGuaDanTransactionPrice; // 当前成交价格
  int                   m_nCurrentTransactionType; // 当前交易类型（强买、进攻型买入。。。。）
  long                  m_lCurrentCanselSellVolume;
  long                  m_lCurrentCanselBuyVolume;
protected:

  bool                      m_fDayLineNeededSaving;   // 日线数据是否需要存储
  CCriticalSection          m_DayLineNeedSacingLock;

  deque<CStockRTDataPtr>		m_dequeRTStockData;  // 实时数据队列
  CCriticalSection          m_RTDataLock;   // 实时数据队列的同步锁

 	
	bool			                m_fChoiced;									// 此股票是否是自选股票.
	bool			                m_fMinLineUpdated;					// 今天的分钟资料是否更新过.
	bool			                m_fDayKLineUpdated;					// 今天的日线资料是否更新过.

};

typedef shared_ptr<CStock> CStockPtr;

