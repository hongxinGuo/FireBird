// CSetEPSSurprise.cpp : CSetEPSSurprise 类的实现

// CSetWorldStockDayLine 实现

// 代码生成在 2019年6月2日, 5:10

#include"pch.h"

#include "SetEPSSurprise.h"

CSetEPSSurprise::CSetEPSSurprise(const CString& strSchema, const CString& strTable, CDatabase* pdb)
	: CVirtualRecordset(strSchema, strTable, pdb) {
	m_nFields = 5;
}

void CSetEPSSurprise::DoFieldExchange(CFieldExchange* pFX) {
	pFX->SetFieldType(CFieldExchange::outputColumn);
	// RFX_Text() 和 RFX_Int() 这类宏依赖的是
	// 成员变量的类型，而不是数据库字段的类型。
	// ODBC 尝试自动将列值转换为所请求的类型
	RFX_Long(pFX, _T("[ID]"), m_ID);
	RFX_Text(pFX, _T("[Symbol]"), m_Symbol);
	RFX_Long(pFX, _T("[Date]"), m_Date);
	RFX_Double(pFX, _T("[Actual]"), m_Actual);
	RFX_Double(pFX, _T("[Estimate]"), m_Estimate);
}
