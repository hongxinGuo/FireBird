// VirtualSetChosenStake.h : VirtualSetChosenStake 类的实现

#include"pch.h"

#include "VirtualSetChosenStake.h"

CVirtualSetChosenStake::CVirtualSetChosenStake(CString strSchema, CString strTable, CDatabase* pDatabase)
	: CVirtualRecordset(strSchema, strTable, pDatabase) {
	m_ID = 0;
	m_Symbol = _T("");
	m_nFields = 2;
}

void CVirtualSetChosenStake::DoFieldExchange(CFieldExchange* pFX) {
	pFX->SetFieldType(CFieldExchange::outputColumn);
	// RFX_Text() 和 RFX_Int() 这类宏依赖的是
	// 成员变量的类型，而不是数据库字段的类型。
	// ODBC 尝试自动将列值转换为所请求的类型
	RFX_Long(pFX, _T("[ID]"), m_ID);
	RFX_Text(pFX, _T("[Symbol]"), m_Symbol);
}
