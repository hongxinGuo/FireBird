// CSetWorldMarketNews.cpp : CSetWorldMarketNews 类的实现

#include"pch.h"

import FireBird.Set.TiingoMarketNews;

CSetTiingoMarketNews::CSetTiingoMarketNews(const CString& strSchema, const CString& strTable, CDatabase* pdb)
	: CVirtualRecordset(strSchema, strTable, pdb) {
	m_ID = 0;
	m_ids = 0;
	m_tickers = _T("");
	m_title = _T("");
	m_tag = _T("");
	m_source = _T("");
	m_description = _T(" ");
	m_url = _T("");
	m_crawlDate = _T("19700101000000");
	m_publishedDate = _T("19700101000000");

	m_nFields = 10;
}

void CSetTiingoMarketNews::DoFieldExchange(CFieldExchange* pFX) {
	pFX->SetFieldType(CFieldExchange::outputColumn);
	// RFX_Text() 和 RFX_Int() 这类宏依赖的是
	// 成员变量的类型，而不是数据库字段的类型。
	// ODBC 尝试自动将列值转换为所请求的类型
	RFX_Long(pFX, _T("[ID]"), m_ID);
	RFX_Long(pFX, _T("ids"), m_ids);
	RFX_Text(pFX, _T("tickers"), m_tickers);
	RFX_Text(pFX, _T("title"), m_title);
	RFX_Text(pFX, _T("tags"), m_tag);
	RFX_Text(pFX, _T("source"), m_source);
	RFX_Text(pFX, _T("description"), m_description);
	RFX_Text(pFX, _T("url"), m_url);
	RFX_Text(pFX, _T("crawlDate"), m_crawlDate);
	RFX_Text(pFX, _T("publishedDate"), m_publishedDate);
}
