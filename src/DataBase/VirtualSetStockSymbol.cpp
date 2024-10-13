// VirtualSetStockSymbol.cpp : CVirtualSetStockSymbol 类的实现

#include"pch.h"

#include "VirtualSetStockSymbol.h"

CVirtualSetStockSymbol::CVirtualSetStockSymbol(const CString& strSchema, const CString& strTable, CDatabase* pdb)
	: CVirtualRecordset(strSchema, strTable, pdb) {
	m_nFields = 9;
}

void CVirtualSetStockSymbol::DoFieldExchange(CFieldExchange* pFX) {
	pFX->SetFieldType(CFieldExchange::outputColumn);
	// RFX_Text() 和 RFX_Int() 这类宏依赖的是
	// 成员变量的类型，而不是数据库字段的类型。
	// ODBC 尝试自动将列值转换为所请求的类型
	RFX_Long(pFX, _T("[ID]"), m_ID);
	RFX_Text(pFX, _T("[Description]"), m_Description);
	RFX_Text(pFX, _T("[Exchange]"), m_Exchange);
	RFX_Text(pFX, _T("[Symbol]"), m_Symbol);
	RFX_Text(pFX, _T("[DisplaySymbol]"), m_DisplaySymbol);
	RFX_Long(pFX, _T("[DayLineStartDate]"), m_DayLineStartDate);
	RFX_Long(pFX, _T("[DayLineEndDate]"), m_DayLineEndDate);
	RFX_Long(pFX, _T("[IPOStatus]"), m_IPOStatus);
	RFX_Text(pFX, _T("[UpdateDate]"), m_UpdateDate, 10000);
}
