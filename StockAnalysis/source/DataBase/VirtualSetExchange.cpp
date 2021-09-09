// CVirtualSetExchange.cpp : CVirtualSetExchange 类的实现

#include"pch.h"
#include"globedef.h"
#include"accessory.h"

#include "VirtualSetExchange.h"

IMPLEMENT_DYNAMIC(CVirtualSetExchange, CVirtualRecordset)

CVirtualSetExchange::CVirtualSetExchange(CString strSchema, CString strTable, CDatabase* pdb)
	: CVirtualRecordset(strSchema, strTable, pdb) {
	m_Code = _T("");
	m_Name = _T("");
	m_Mic = _T("");
	m_TimeZone = _T("");
	m_Hour = _T("");
	m_CloseDate = _T("");
	m_Country = _T("");
	m_Source = _T("");
	m_nFields = 9;
}

void CVirtualSetExchange::DoFieldExchange(CFieldExchange* pFX) {
	pFX->SetFieldType(CFieldExchange::outputColumn);
	// RFX_Text() 和 RFX_Int() 这类宏依赖的是
	// 成员变量的类型，而不是数据库字段的类型。
	// ODBC 尝试自动将列值转换为所请求的类型
	RFX_Long(pFX, _T("[ID]"), m_ID);
	RFX_Text(pFX, _T("[Code]"), m_Code);
	RFX_Text(pFX, _T("[Name]"), m_Name);
	RFX_Text(pFX, _T("[Mic]"), m_Mic);
	RFX_Text(pFX, _T("[TimeZone]"), m_TimeZone);
	RFX_Text(pFX, _T("[Hour]"), m_Hour);
	RFX_Text(pFX, _T("[CloseDate]"), m_CloseDate);
	RFX_Text(pFX, _T("[Country]"), m_Country);
	RFX_Text(pFX, _T("[Source]"), m_Source);
}
/////////////////////////////////////////////////////////////////////////////
// CSetWorldStock 诊断

#ifdef _DEBUG
void CVirtualSetExchange::AssertValid() const {
	CVirtualRecordset::AssertValid();
}

void CVirtualSetExchange::Dump(CDumpContext& dc) const {
	CVirtualRecordset::Dump(dc);
}
#endif //_DEBUG