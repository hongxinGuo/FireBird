#pragma once
#include"VirtualRecordset.h"

class CSetRSStrongStock : public CVirtualRecordset {
public:
	CSetRSStrongStock(long lIndex, CString strSchema = _T("ChinaMarket"), CString strTable = _T("selected_rs_"), CDatabase* pDatabase = nullptr);

	// 字段/参数数据

	// 以下字符串类型(如果存在)反映数据库字段(ANSI 数据类型的 CStringA 和 Unicode
	// 数据类型的 CStringW)的实际数据类型。
	//  这是为防止 ODBC 驱动程序执行可能
	// 不必要的转换。如果希望，可以将这些成员更改为
	// CString 类型，ODBC 驱动程序将执行所有必要的转换。
	// (注意: 必须使用 3.5 版或更高版本的 ODBC 驱动程序
	// 以同时支持 Unicode 和这些转换)。

	CString m_Symbol;

	// 重写
	// 向导生成的虚函数重写
public:
	CString GetDefaultSQL() override; 	// 记录集的默认 SQL
	void DoFieldExchange(CFieldExchange* pFX) override;	// RFX 支持

	// 实现
	long m_lIndex; // 数据表索引
};
