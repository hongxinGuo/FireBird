// CSetCompanyNews.cpp : CSetCompanyNews 类的实现

// 代码生成在 2019年6月2日, 5:10

#include"pch.h"

#include "SetFinnhubCompanyNews.h"

CSetCompanyNews::CSetCompanyNews(CString strSchema, CString strTable, CDatabase* pdb)
	: CVirtualRecordset(strSchema, strTable, pdb) {
	m_ID = 0;
	m_CompanySymbol = _T("");
	m_Category = _T("");
	m_DateTime = _T("");
	m_HeadLine = _T("");
	m_NewsID = 0;
	m_Image = _T("");
	m_RelatedSymbol = _T("");
	m_Source = _T("");
	m_Summary = _T("");
	m_URL = _T("");
	m_nFields = 11;
}

void CSetCompanyNews::DoFieldExchange(CFieldExchange* pFX) {
	pFX->SetFieldType(CFieldExchange::outputColumn);
	// RFX_Text() 和 RFX_Int() 这类宏依赖的是
	// 成员变量的类型，而不是数据库字段的类型。
	// ODBC 尝试自动将列值转换为所请求的类型
	RFX_Long(pFX, _T("[ID]"), m_ID);
	RFX_Text(pFX, _T("Symbol"), m_CompanySymbol);
	RFX_Text(pFX, _T("Category"), m_Category, 50000);
	RFX_Text(pFX, _T("DateTime"), m_DateTime);
	RFX_Text(pFX, _T("Headline"), m_HeadLine, 50000);
	RFX_Int(pFX, _T("NewsID"), m_NewsID);
	RFX_Text(pFX, _T("Image"), m_Image, 50000);
	RFX_Text(pFX, _T("RelatedSymbol"), m_RelatedSymbol);
	RFX_Text(pFX, _T("Source"), m_Source, 50000);
	RFX_Text(pFX, _T("Summary"), m_Summary, 50000);
	RFX_Text(pFX, _T("URL"), m_URL, 50000);
}
