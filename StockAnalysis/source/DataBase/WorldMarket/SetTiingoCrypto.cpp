// CSetTiingoCrypto.cpp : CSetTiingoCrypto 类的实现

#include"pch.h"

#include "SetTiingoCrypto.h"

IMPLEMENT_DYNAMIC(CSetTiingoCrypto, CVirtualRecordset)

CSetTiingoCrypto::CSetTiingoCrypto(CString strSchema, CString strTable, CDatabase* pdb)
	: CVirtualRecordset(strSchema, strTable, pdb) {
	m_Ticker = _T("");
	m_Name = _T("");
	m_BaseCurrency = _T("");
	m_QuoteCurrency = _T("");
	m_Description = _T("");

	m_nFields = 6;
}

void CSetTiingoCrypto::DoFieldExchange(CFieldExchange* pFX) {
	pFX->SetFieldType(CFieldExchange::outputColumn);
	// RFX_Text() 和 RFX_Int() 这类宏依赖的是
	// 成员变量的类型，而不是数据库字段的类型。
	// ODBC 尝试自动将列值转换为所请求的类型
	RFX_Long(pFX, _T("[ID]"), m_ID);
	RFX_Text(pFX, _T("[Ticker]"), m_Ticker);
	RFX_Text(pFX, _T("[Name]"), m_Name);
	RFX_Text(pFX, _T("[BaseCurrency]"), m_BaseCurrency);
	RFX_Text(pFX, _T("[QuoteCurrency]"), m_QuoteCurrency);
	RFX_Text(pFX, _T("[Description]"), m_Description);
}
/////////////////////////////////////////////////////////////////////////////
// CSetTiingoCrypto 诊断

#ifdef _DEBUG
void CSetTiingoCrypto::AssertValid() const {
	CVirtualRecordset::AssertValid();
}

void CSetTiingoCrypto::Dump(CDumpContext& dc) const {
	CVirtualRecordset::Dump(dc);
}
#endif //_DEBUG