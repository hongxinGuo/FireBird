// CSetInsiderTransaction.h : CSetInsiderTransaction 的声明

#pragma once

#include"VirtualRecordset.h"

class CSetInsiderTransaction : public CVirtualRecordset {
public:
	CSetInsiderTransaction(const CString& strSchema = _T("WorldMarket"), const CString& strTable = _T("insider_transaction"), CDatabase* pDatabase = nullptr);

	// 字段/参数数据

	// 以下字符串类型(如果存在)反映数据库字段(ANSI 数据类型的 CStringA 和 Unicode
	// 数据类型的 CStringW)的实际数据类型。
	//  这是为防止 ODBC 驱动程序执行可能
	// 不必要的转换。如果希望，可以将这些成员更改为
	// CString 类型，ODBC 驱动程序将执行所有必要的转换。
	// (注意: 必须使用 3.5 版或更高版本的 ODBC 驱动程序
	// 以同时支持 Unicode 和这些转换)。

	long m_ID;  // 索引用，自动生成。
	CString m_Symbol;
	CString m_PersonName;
	CString m_Share;
	CString m_Change;
	long m_FilingDate;
	long m_TransactionDate;
	CString m_TransactionCode;
	double m_TransactionPrice;

	// 重写
	// 向导生成的虚函数重写
public:
	void DoFieldExchange(CFieldExchange* pFX) override;	// RFX 支持
};
