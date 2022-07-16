// CSetInsiderSentiment.cpp : CSetInsiderSentiment 类的实现

// 代码生成在 2019年6月2日, 5:10

#include"pch.h"

#include "SetInsiderSentiment.h"

IMPLEMENT_DYNAMIC(CSetInsiderSentiment, CVirtualRecordset)

CSetInsiderSentiment::CSetInsiderSentiment(CString strSchema, CString strTable, CDatabase* pdb)
	: CVirtualRecordset(strSchema, strTable, pdb) {
	m_ID = 0;
	m_Symbol = _T(" ");
	m_Date = 19800101;
	m_Change = 0;
	m_mspr = 0.0;
	m_nFields = 5;
}

void CSetInsiderSentiment::DoFieldExchange(CFieldExchange* pFX) {
	pFX->SetFieldType(CFieldExchange::outputColumn);
	// RFX_Text() 和 RFX_Int() 这类宏依赖的是
	// 成员变量的类型，而不是数据库字段的类型。
	// ODBC 尝试自动将列值转换为所请求的类型
	RFX_Long(pFX, _T("[ID]"), m_ID);
	RFX_Text(pFX, _T("[Symbol]"), m_Symbol);
	RFX_Long(pFX, _T("[Date]"), m_Date);
	RFX_Long(pFX, _T("[Change]"), m_Change);
	RFX_Double(pFX, _T("[mspr]"), m_mspr);
}
/////////////////////////////////////////////////////////////////////////////
// CSetInsiderTransaction 诊断

#ifdef _DEBUG
void CSetInsiderSentiment::AssertValid() const {
	CVirtualRecordset::AssertValid();
}

void CSetInsiderSentiment::Dump(CDumpContext& dc) const {
	CVirtualRecordset::Dump(dc);
}
#endif //_DEBUG