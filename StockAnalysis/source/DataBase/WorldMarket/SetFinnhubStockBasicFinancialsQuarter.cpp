// CSetCountry.cpp : CSetCountry 类的实现

// 代码生成在 2019年6月2日, 5:10

#include"pch.h"

#include "SetFinnhubStockBasicFinancialQuarter.h"

IMPLEMENT_DYNAMIC(CSetFinnhubStockBasicFinancialQuarter, CVirtualRecordset)

CSetFinnhubStockBasicFinancialQuarter::CSetFinnhubStockBasicFinancialQuarter(CString strSchema, CString strTable, CDatabase* pdb)
	: CVirtualRecordset(strSchema, strTable, pdb) {
	m_ID = 0;
	m_symbol = _T("");
	m_type = _T("");
	m_date = 19800101;
	m_value = 0;

	m_nFields = 5;
}

void CSetFinnhubStockBasicFinancialQuarter::DoFieldExchange(CFieldExchange* pFX) {
	pFX->SetFieldType(CFieldExchange::outputColumn);
	// RFX_Text() 和 RFX_Int() 这类宏依赖的是
	// 成员变量的类型，而不是数据库字段的类型。
	// ODBC 尝试自动将列值转换为所请求的类型
	RFX_Long(pFX, _T("[ID]"), m_ID);
	RFX_Text(pFX, _T("Symbol"), m_symbol);
	RFX_Text(pFX, _T("Type"), m_type);
	RFX_Int(pFX, _T("Date"), m_date);
	RFX_Double(pFX, _T("Value"), m_value);
}
/////////////////////////////////////////////////////////////////////////////
// CSetCountry 诊断

#ifdef _DEBUG
void CSetFinnhubStockBasicFinancialQuarter::AssertValid() const {
	CVirtualRecordset::AssertValid();
}

void CSetFinnhubStockBasicFinancialQuarter::Dump(CDumpContext& dc) const {
	CVirtualRecordset::Dump(dc);
}
#endif //_DEBUG