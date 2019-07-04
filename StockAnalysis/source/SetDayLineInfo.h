#pragma once

// 代码生成在 2019年6月17日, 5:10

class CSetDayLineInfo : public CRecordset
{
public:
  CSetDayLineInfo(CDatabase* pDatabase = NULL);
  DECLARE_DYNAMIC(CSetDayLineInfo)

  // 字段/参数数据

  // 以下字符串类型(如果存在)反映数据库字段(ANSI 数据类型的 CStringA 和 Unicode
  // 数据类型的 CStringW)的实际数据类型。
  //  这是为防止 ODBC 驱动程序执行可能
  // 不必要的转换。如果希望，可以将这些成员更改为
  // CString 类型，ODBC 驱动程序将执行所有必要的转换。
  // (注意: 必须使用 3.5 版或更高版本的 ODBC 驱动程序
  // 以同时支持 Unicode 和这些转换)。

  long m_ID;  // 索引用，自动生成。
  long	m_Time;	//成交时间，以日期表示，如20090222即2009年2月22日
  long	m_Market;	//市场（上海，深圳）
  CStringW	m_StockCode;	//六位股票代码
  CStringW	m_StockName;	//六位股票代码
  double	m_RelativeStrong;	//相对于总市场的强度
  long  m_TransactionNumber; // 本日的盘口变化数
  long  m_TransactionNumberBelow5000; // 本日的盘口变化数
  long  m_TransactionNumberBelow50000; // 本日的盘口变化数
  long  m_TransactionNumberBelow200000; // 本日的盘口变化数
  long  m_TransactionNumberAbove200000; // 本日的盘口变化数
  long	m_AttackBuyVolume;	//攻击性买盘数量
  long	m_AttackSellVolume;	//攻击性买盘数量
  long	m_StrongBuyVolume;	//强攻击性买盘数量
  long	m_StrongSellVolume;	//强攻击性买盘数量
  long  m_OrdinaryBuyVolume;
  long  m_OrdinarySellVolume;
  long	m_UnknownVolume;	//未知买卖盘数量
  long	m_CancelBuyVolume;	//买单撤单数量
  long	m_CancelSellVolume;	//卖单撤单数量
  long	m_AttackBuyBelow50000;
  long	m_AttackBuyBelow200000;
  long	m_AttackBuyAbove200000;
  long	m_AttackSellBelow50000;
  long	m_AttackSellBelow200000;
  long	m_AttackSellAbove200000;

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

