// CSetCryptoDayLine.h : CSetCryptoDayLine 的声明

#pragma once

#include"afxdb.h"

// 代码生成在 2019年6月2日, 5:10

class CSetCryptoDayLine : public CRecordset
{
public:
	CSetCryptoDayLine(CDatabase* pDatabase = nullptr);
	DECLARE_DYNAMIC(CSetCryptoDayLine)

	// 字段/参数数据

	// 以下字符串类型(如果存在)反映数据库字段(ANSI 数据类型的 CStringA 和 Unicode
	// 数据类型的 CStringW)的实际数据类型。
	//  这是为防止 ODBC 驱动程序执行可能
	// 不必要的转换。如果希望，可以将这些成员更改为
	// CString 类型，ODBC 驱动程序将执行所有必要的转换。
	// (注意: 必须使用 3.5 版或更高版本的 ODBC 驱动程序
	// 以同时支持 Unicode 和这些转换)。

	long m_ID;  // 索引用，自动生成。
	long m_Date;	//成交时间，以日期表示，如20090222即2009年2月22日
	CString m_Exchange;	//市场（上海，深圳）
	CString m_Symbol;	//
	CString m_LastClose;	//昨天收盘价。单位：0.001元
	CString m_Open;	//今天开盘价
	CString m_High;	//今天最高价
	CString m_Low;	//今天最低价
	CString m_Close;	//今日收盘价
	CString m_Volume;	//今日总成交数量。单位：股。

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
