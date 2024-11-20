#include "pch.h"
#include "SetTiingoStockCurrentTrace.h"

CSetTiingoStockCurrentTrace::CSetTiingoStockCurrentTrace(const CString& strSchema, const CString& strTable, CDatabase* pDatabase)
	: CVirtualRecordset(strSchema, strTable, pDatabase) {
	m_nFields = 3;
}

void CSetTiingoStockCurrentTrace::DoFieldExchange(CFieldExchange* pFX) {
	pFX->SetFieldType(CFieldExchange::outputColumn);
	// RFX_Text() 和 RFX_Int() 这类宏依赖的是
	// 成员变量的类型，而不是数据库字段的类型。
	// ODBC 尝试自动将列值转换为所请求的类型
	RFX_Long(pFX, _T("[ID]"), m_ID);
	RFX_Long(pFX, _T("[Date]"), m_Date);
	RFX_Text(pFX, _T("[Symbol]"), m_Symbol);
}
