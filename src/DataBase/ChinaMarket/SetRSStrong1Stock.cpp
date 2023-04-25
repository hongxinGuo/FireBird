#include"pch.h"

#include "SetRSStrong1Stock.h"

CSetRSStrong1Stock::CSetRSStrong1Stock(const CString& strSchema, const CString& strTable, CDatabase* pdb)
	: CVirtualRecordset(strSchema, strTable, pdb) {
	m_Symbol = _T("");
	m_nFields = 1;
}

void CSetRSStrong1Stock::DoFieldExchange(CFieldExchange* pFX) {
	pFX->SetFieldType(CFieldExchange::outputColumn);
	// RFX_Text() 和 RFX_Int() 这类宏依赖的是
	// 成员变量的类型，而不是数据库字段的类型。
	// ODBC 尝试自动将列值转换为所请求的类型
	RFX_Text(pFX, _T("[Symbol]"), m_Symbol);
}
