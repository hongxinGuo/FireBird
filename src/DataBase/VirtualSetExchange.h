// VirtualSetExchange.h : CVirtualSetExchange 的声明
//
// 存储交易所数据的默认制式
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
#include"VirtualRecordset.h"

class CVirtualSetExchange : public CVirtualRecordset {
public:
	CVirtualSetExchange(const CString& strSchema, const CString& strTable, CDatabase* pDatabase = nullptr);

	// 字段/参数数据

	// 以下字符串类型(如果存在)反映数据库字段(ANSI 数据类型的 CStringA 和 Unicode
	// 数据类型的 CStringW)的实际数据类型。
	//  这是为防止 ODBC 驱动程序执行可能
	// 不必要的转换。如果希望，可以将这些成员更改为
	// CString 类型，ODBC 驱动程序将执行所有必要的转换。
	// (注意: 必须使用 3.5 版或更高版本的 ODBC 驱动程序
	// 以同时支持 Unicode 和这些转换)。

	long m_ID;
	CString m_Code;
	CString m_Name;
	CString m_Mic;
	CString m_TimeZone;
	CString m_PreMarket;
	CString m_Hour;
	CString m_PostMarket;
	CString m_CloseDate;
	CString m_Country;
	CString m_CountryName;
	CString m_Source;
	CString m_MyUnknownColumn;

public:
	void DoFieldExchange(CFieldExchange* pFX) override;	// RFX 支持
};
