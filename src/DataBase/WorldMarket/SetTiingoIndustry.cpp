// CSetTiingoIndustry.cpp : CSetTiingoIndustry 类的实现

#include"pch.h"

#include "SetTiingoIndustry.h"

CSetTiingoIndustry::CSetTiingoIndustry(CString strSchema, CString strTable, CDatabase* pdb)
	: CVirtualRecordset(strSchema, strTable, pdb) {
	m_ID = 0;
	m_Industry = _T("");
	m_Sector = _T("");
	m_nFields = 3;
}

void CSetTiingoIndustry::DoFieldExchange(CFieldExchange* pFX) {
	pFX->SetFieldType(CFieldExchange::outputColumn);
	// RFX_Text() 和 RFX_Int() 这类宏依赖的是
	// 成员变量的类型，而不是数据库字段的类型。
	// ODBC 尝试自动将列值转换为所请求的类型
	RFX_Long(pFX, _T("[ID]"), m_ID);
	RFX_Text(pFX, _T("[Industry]"), m_Industry);
	RFX_Text(pFX, _T("[Sector]"), m_Sector);
}
