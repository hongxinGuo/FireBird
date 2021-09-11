// CSetNaicsIndustry.cpp : CSetNaicsIndustry 类的实现

// CSetWorldStockDayLine 实现

// 代码生成在 2019年6月2日, 5:10

#include"pch.h"

#include "SetNaicsIndustry.h"

IMPLEMENT_DYNAMIC(CSetNaicsIndustry, CVirtualRecordset)

CSetNaicsIndustry::CSetNaicsIndustry(CString strSchema, CString strTable, CDatabase* pdb)
	: CVirtualRecordset(strSchema, strTable, pdb) {
	m_ID = 0;
	m_Naics = _T("");
	m_NationalIndustry = _T("");
	m_Sector = _T("");
	m_SubSector = _T("");
	m_nFields = 5;
}

void CSetNaicsIndustry::DoFieldExchange(CFieldExchange* pFX) {
	pFX->SetFieldType(CFieldExchange::outputColumn);
	// RFX_Text() 和 RFX_Int() 这类宏依赖的是
	// 成员变量的类型，而不是数据库字段的类型。
	// ODBC 尝试自动将列值转换为所请求的类型
	RFX_Long(pFX, _T("[ID]"), m_ID);
	RFX_Text(pFX, _T("[Naics]"), m_Naics);
	RFX_Text(pFX, _T("[NationalIndustry]"), m_NationalIndustry);
	RFX_Text(pFX, _T("[Sector]"), m_Sector);
	RFX_Text(pFX, _T("[SubSector]"), m_SubSector);
}
/////////////////////////////////////////////////////////////////////////////
// CSetNaicsIndustry 诊断

#ifdef _DEBUG
void CSetNaicsIndustry::AssertValid() const {
	CVirtualRecordset::AssertValid();
}

void CSetNaicsIndustry::Dump(CDumpContext& dc) const {
	CVirtualRecordset::Dump(dc);
}
#endif //_DEBUG