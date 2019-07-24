// CSetDayLine.h : CSetDayLine 的声明

#pragma once

#include"afxdb.h"

// 代码生成在 2019年6月2日, 5:10

class CSetDayLineToday : public CRecordset
{
public:
  CSetDayLineToday(CDatabase* pDatabase = NULL);
  DECLARE_DYNAMIC(CSetDayLineToday)

  // 字段/参数数据

  // 以下字符串类型(如果存在)反映数据库字段(ANSI 数据类型的 CStringA 和 Unicode
  // 数据类型的 CStringW)的实际数据类型。
  //  这是为防止 ODBC 驱动程序执行可能
  // 不必要的转换。如果希望，可以将这些成员更改为
  // CString 类型，ODBC 驱动程序将执行所有必要的转换。
  // (注意: 必须使用 3.5 版或更高版本的 ODBC 驱动程序
  // 以同时支持 Unicode 和这些转换)。

  long        m_ID;  // 索引用，自动生成。
  long	      m_Time;	//成交时间，以日期表示，如20090222即2009年2月22日
  long	      m_Market;	//市场（上海，深圳）
  CString 	  m_StockCode;	//八位股票代码
  CStringW 	  m_StockName;	//六位股票名称   这里必须采用CStringW(Unicode)制式，否则有些中文无法显示，导致数据库无法存储
  double  	  m_LastClose;	//昨天收盘价。单位：0.001元
  double  	  m_Open;	//今天开盘价
  double  	  m_High;	//今天最高价
  double  	  m_Low;	//今天最低价
  double  	  m_Close;	//今日收盘价
  double      m_Volume;	//今日总成交数量。单位：股。
  double      m_Amount;	//今日总成交金额。单位：元
  double      m_UpAndDown;	//今日涨跌
  double      m_UpDownRate; // 今日涨跌幅度，单位1%。
  double      m_ChangeHandRate; // 今日换手率
  double      m_TotalValue;	//总市值。单位：元
  double      m_CurrentValue;	//流通市值。单位：元
  double	    m_RelativeStrong;	//相对于总市场的强度
  long        m_TransactionNumber; // 本日的盘口变化数
  long        m_TransactionNumberBelow5000;
  long        m_TransactionNumberBelow50000;
  long        m_TransactionNumberBelow200000;
  long        m_TransactionNumberAbove200000;
  double      m_OrdinaryBuyVolume; // 一般性买入股数。单位：股
  double      m_OrdinarySellVolume; // 一般性卖出股数
  double      m_AttackBuyVolume;	//攻击性买盘数量
  double      m_AttackSellVolume;	//攻击性买盘数量
  double      m_StrongBuyVolume;	//强攻击性买盘数量
  double      m_StrongSellVolume;	//强攻击性买盘数量
  double	    m_UnknownVolume;	//未知买卖盘数量
  double	    m_CancelBuyVolume;	//买单撤单数量
  double	    m_CancelSellVolume;	//卖单撤单数量
  double	    m_AttackBuyBelow50000;
  double	    m_AttackBuyBelow200000;
  double      m_AttackBuyAbove200000;
  double	    m_AttackSellBelow50000;
  double      m_AttackSellBelow200000;
  double      m_AttackSellAbove200000;

  // 重写
    // 向导生成的虚函数重写
public:
  virtual CString GetDefaultConnect();	// 默认连接字符串

  virtual CString GetDefaultSQL(); 	// 记录集的默认 SQL
  virtual void DoFieldExchange(CFieldExchange* pFX);	// RFX 支持

// 实现
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

};


