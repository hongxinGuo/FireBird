// SetRealTimeData.h : CSetRealTimeData 的声明

#pragma once

// 代码生成在 2019年5月26日, 8:09

class CSetRealTimeData : public CRecordset
{
public:
	CSetRealTimeData(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CSetRealTimeData)

// 字段/参数数据

// 以下字符串类型(如果存在)反映数据库字段(ANSI 数据类型的 CStringA 和 Unicode
// 数据类型的 CStringW)的实际数据类型。
//  这是为防止 ODBC 驱动程序执行可能
// 不必要的转换。如果希望，可以将这些成员更改为
// CString 类型，ODBC 驱动程序将执行所有必要的转换。
// (注意: 必须使用 3.5 版或更高版本的 ODBC 驱动程序
// 以同时支持 Unicode 和这些转换)。

	long	m_Time;	//成交时间，以秒计算，从1970年1月1日起
  long  m_lMarket; // 上海为1，深圳为2， 。。。
	CString	m_StockName;	//四位股票简称
	CString	m_StockCode;	//六位股票代码
	double	m_LastClose;	//昨天收盘价
	double	m_Open;	//今天开盘价
	double	m_High;	//今天最高价
	double	m_Low;	//今天最低价
	double	m_CurrentPrice;	//最新成交价格
	long	m_Volume;	//目前总成交数量
	double	m_Amount;	//当前总成交金额
	long	m_Stroke;	//当前总成交笔数
	double	m_PBuy5;	//买价5
	long	m_VBuy5;	//买价5上的手数
	double	m_PBuy4;
	long	m_VBuy4;
	double	m_PBuy3;
	long	m_VBuy3;
	double	m_PBuy2;
	long	m_VBuy2;
	double	m_PBuy1;
	long	m_VBuy1;
	double	m_PSell1;
	long	m_VSell1;
	double	m_PSell2;
	long	m_VSell2;
	double	m_PSell3;
	long	m_VSell3;
	double	m_PSell4;
	long	m_VSell4;
	double	m_PSell5;	//卖价5
	long	m_VSell5;	//卖价5上的手数

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


