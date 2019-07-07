// SetStockCode.h : CSetStockCode 的声明

#pragma once

// 代码生成在 2019年5月26日, 8:14

class CSetStockCode : public CRecordset
{
public:
	CSetStockCode(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CSetStockCode)

// 字段/参数数据

// 以下字符串类型(如果存在)反映数据库字段(ANSI 数据类型的 CStringA 和 Unicode
// 数据类型的 CStringW)的实际数据类型。
//  这是为防止 ODBC 驱动程序执行可能
// 不必要的转换。如果希望，可以将这些成员更改为
// CString 类型，ODBC 驱动程序将执行所有必要的转换。
// (注意: 必须使用 3.5 版或更高版本的 ODBC 驱动程序
// 以同时支持 Unicode 和这些转换)。

  long m_ID;
	long	m_Counter;
	int	m_StockType;
	CString	m_StockCode;
	CString	m_StockName;
  long m_DayLineStartDay;   //  日线的起始日
	long m_DayLineEndDay;     // 从网易历史日线服务器上得到的最新日
  long m_NewestDayLineDay;  // 执行所有日线处理得到的最新日（如果处理了实时数据，则此日期要晚于m_DayLineEndDay
	long m_IPOed;
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


