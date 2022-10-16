// CSetCompanyNews.cpp : CSetCompanyNews 类的实现

// 代码生成在 2019年6月2日, 5:10

#include"pch.h"

#include "SetFinnhubCompanyNews.h"

IMPLEMENT_DYNAMIC(CSetCompanyNews, CVirtualRecordset)

CSetCompanyNews::CSetCompanyNews(CString strSchema, CString strTable, CDatabase* pdb)
	: CVirtualRecordset(strSchema, strTable, pdb) {
	m_ID = 0;
	m_CompanySymbol = _T("");
	m_nFields = 11;
}

void CSetCompanyNews::DoFieldExchange(CFieldExchange* pFX) {
	pFX->SetFieldType(CFieldExchange::outputColumn);
	// RFX_Text() 和 RFX_Int() 这类宏依赖的是
	// 成员变量的类型，而不是数据库字段的类型。
	// ODBC 尝试自动将列值转换为所请求的类型
	RFX_Long(pFX, _T("[ID]"), m_ID);
	RFX_Text(pFX, _T("symbol"), m_CompanySymbol);
	RFX_Text(pFX, _T("category"), m_Category);
	RFX_Text(pFX, _T("date_time"), m_DateTime);
	RFX_Text(pFX, _T("headline"), m_HeadLine);
	RFX_Int(pFX, _T("news_id"), m_NewsID);
	RFX_Text(pFX, _T("image"), m_Image);
	RFX_Text(pFX, _T("related_symbol"), m_RelatedSymbol);
	RFX_Text(pFX, _T("source"), m_Source);
	RFX_Text(pFX, _T("summary"), m_Summary);
	RFX_Text(pFX, _T("url"), m_URL);
}
/////////////////////////////////////////////////////////////////////////////
// CSetCompanyNews 诊断

#ifdef _DEBUG
void CSetCompanyNews::AssertValid() const {
	CVirtualRecordset::AssertValid();
}

void CSetCompanyNews::Dump(CDumpContext& dc) const {
	CVirtualRecordset::Dump(dc);
}
#endif //_DEBUG