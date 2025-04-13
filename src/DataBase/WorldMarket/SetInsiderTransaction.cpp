// CSetInsiderTransaction.cpp : CSetInsiderTransaction 类的实现

#include"pch.h"

#include "SetInsiderTransaction.h"

CSetInsiderTransaction::CSetInsiderTransaction(const CString& strSchema, const CString& strTable, CDatabase* pdb)
	: CVirtualRecordset(strSchema, strTable, pdb) {
	m_nFields = 9;
}

void CSetInsiderTransaction::DoFieldExchange(CFieldExchange* pFX) {
	pFX->SetFieldType(CFieldExchange::outputColumn);
	// RFX_Text() 和 RFX_Int() 这类宏依赖的是
	// 成员变量的类型，而不是数据库字段的类型。
	// ODBC 尝试自动将列值转换为所请求的类型
	RFX_Long(pFX, _T("[ID]"), m_ID);
	RFX_Text(pFX, _T("[Symbol]"), m_Symbol);
	RFX_Text(pFX, _T("[PersonName]"), m_PersonName);
	RFX_Text(pFX, _T("[Share]"), m_Share);
	RFX_Text(pFX, _T("[Change]"), m_Change);
	RFX_Long(pFX, _T("[FilingDate]"), m_FilingDate);
	RFX_Long(pFX, _T("[TransactionDate]"), m_TransactionDate);
	RFX_Text(pFX, _T("[TransactionCode]"), m_TransactionCode);
	RFX_Double(pFX, _T("[TransactionPrice]"), m_TransactionPrice);
}
