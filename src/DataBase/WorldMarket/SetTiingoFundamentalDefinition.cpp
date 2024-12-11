// CSetWorldFundamentalDefinition.cpp : CSetWorldFundamentalDefinition 类的实现

#include"pch.h"

module FireBird.Set.TiingoFundamentalDefinition;

CSetTiingoFundamentalDefinition::CSetTiingoFundamentalDefinition(const CString& strSchema, const CString& strTable, CDatabase* pdb)
	: CVirtualRecordset(strSchema, strTable, pdb) {
	m_nFields = 6;
}

void CSetTiingoFundamentalDefinition::DoFieldExchange(CFieldExchange* pFX) {
	pFX->SetFieldType(CFieldExchange::outputColumn);
	// RFX_Text() 和 RFX_Int() 这类宏依赖的是
	// 成员变量的类型，而不是数据库字段的类型。
	// ODBC 尝试自动将列值转换为所请求的类型
	RFX_Long(pFX, _T("[ID]"), m_ID);
	RFX_Text(pFX, _T("dataCode"), m_dataCode);
	RFX_Text(pFX, _T("name"), m_name);
	RFX_Text(pFX, _T("description"), m_description, 1000); // Note 默认字符长度为255，更长的话需要设定
	RFX_Text(pFX, _T("statementType"), m_statementType);
	RFX_Text(pFX, _T("units"), m_units);
}
