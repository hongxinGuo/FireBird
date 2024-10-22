// CSetSECFilings.cpp : CSetSECFilings 类的实现

// 代码生成在 2019年6月2日, 5:10

#include"pch.h"

#include "SetSECFilings.h"

CSetSECFilings::CSetSECFilings(const CString& strSchema, const CString& strTable, CDatabase* pdb)
	: CVirtualRecordset(strSchema, strTable, pdb) {
	m_nFields = 9;
}

void CSetSECFilings::DoFieldExchange(CFieldExchange* pFX) {
	pFX->SetFieldType(CFieldExchange::outputColumn);
	// RFX_Text() 和 RFX_Int() 这类宏依赖的是
	// 成员变量的类型，而不是数据库字段的类型。
	// ODBC 尝试自动将列值转换为所请求的类型
	RFX_Long(pFX, _T("[ID]"), m_ID);
	RFX_Text(pFX, _T("[accessNumber]"), m_AccessNumber);
	RFX_Text(pFX, _T("symbol"), m_Symbol);
	RFX_Long(pFX, _T("cik"), m_CIK);
	RFX_Text(pFX, _T("form"), m_Form);
	RFX_Long(pFX, _T("[filedDate]"), m_FiledDate);
	RFX_Long(pFX, _T("[acceptedDate]"), m_AcceptedDate);
	RFX_Text(pFX, _T("reportURL"), m_ReportURL);
	RFX_Text(pFX, _T("filingURL"), m_FilingURL);
}
