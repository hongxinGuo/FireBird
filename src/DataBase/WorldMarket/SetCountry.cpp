// CSetCountry.cpp : CSetCountry 类的实现

// 代码生成在 2019年6月2日, 5:10

#include"pch.h"

import FireBird.Set.FinnhubCountry;

CSetCountry::CSetCountry(const CString& strSchema, const CString& strTable, CDatabase* pdb)
	: CVirtualRecordset(strSchema, strTable, pdb) {
	m_nFields = 7;
}

void CSetCountry::DoFieldExchange(CFieldExchange* pFX) {
	pFX->SetFieldType(CFieldExchange::outputColumn);
	// RFX_Text() 和 RFX_Int() 这类宏依赖的是
	// 成员变量的类型，而不是数据库字段的类型。
	// ODBC 尝试自动将列值转换为所请求的类型
	RFX_Long(pFX, _T("[ID]"), m_ID);
	RFX_Text(pFX, _T("Code2"), m_Code2);
	RFX_Text(pFX, _T("Code3"), m_Code3);
	RFX_Text(pFX, _T("CodeNo"), m_CodeNo);
	RFX_Text(pFX, _T("Country"), m_Country);
	RFX_Text(pFX, _T("Currency"), m_Currency);
	RFX_Text(pFX, _T("CurrencyCode"), m_CurrencyCode);
}
