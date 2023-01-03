// CSetSICIndustry.cpp : CSetSICIndustry 类的实现

// CSetSICIndustry 实现

// 代码生成在 2021年3月19日, 5:10

#include"pch.h"

#include "SetSICIndustry.h"

IMPLEMENT_DYNAMIC(CSetSICIndustry, CVirtualRecordset)

CSetSICIndustry::CSetSICIndustry(CString strSchema, CString strTable, CDatabase* pdb)
	: CVirtualRecordset(strSchema, strTable, pdb) {
	m_ID = 0;
	m_Code = 0;
	m_Industry = _T("");
	m_Sector = _T("");
	m_nFields = 4;
}

void CSetSICIndustry::DoFieldExchange(CFieldExchange* pFX) {
	pFX->SetFieldType(CFieldExchange::outputColumn);
	// RFX_Text() 和 RFX_Int() 这类宏依赖的是
	// 成员变量的类型，而不是数据库字段的类型。
	// ODBC 尝试自动将列值转换为所请求的类型
	RFX_Long(pFX, _T("[ID]"), m_ID);
	RFX_Long(pFX, _T("[Code]"), m_Code);
	RFX_Text(pFX, _T("[Industry]"), m_Industry);
	RFX_Text(pFX, _T("[Sector]"), m_Sector);
}
/////////////////////////////////////////////////////////////////////////////
// CSetSICIndustry 诊断

#ifdef _DEBUG
void CSetSICIndustry::AssertValid() const {
	CVirtualRecordset::AssertValid();
}

void CSetSICIndustry::Dump(CDumpContext& dc) const {
	CVirtualRecordset::Dump(dc);
}
#endif //_DEBUG