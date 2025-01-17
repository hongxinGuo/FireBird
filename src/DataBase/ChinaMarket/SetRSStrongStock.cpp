#include"pch.h"

#include "SetRSStrongStock.h"

CSetRSStrongStock::CSetRSStrongStock(long lIndex, const CString& strSchema, const CString& strTable, CDatabase* pdb)
	: CVirtualRecordset(strSchema, strTable, pdb) {
	ASSERT((m_lIndex >= 0) && (m_lIndex < 10));
	m_lIndex = lIndex;
	m_Symbol = _T("");
	m_nFields = 1;
}

CString CSetRSStrongStock::GetDefaultSQL() {
	string s = _T("[");
	s += m_Table;

	ASSERT((m_lIndex >= 0) && (m_lIndex < 10));
	s += fmt::format("{:Ld}]", m_lIndex);

	return s.c_str();
}

void CSetRSStrongStock::DoFieldExchange(CFieldExchange* pFX) {
	pFX->SetFieldType(CFieldExchange::outputColumn);
	// RFX_Text() 和 RFX_Int() 这类宏依赖的是
	// 成员变量的类型，而不是数据库字段的类型。
	// ODBC 尝试自动将列值转换为所请求的类型
	RFX_Text(pFX, _T("[Symbol]"), m_Symbol);
}
