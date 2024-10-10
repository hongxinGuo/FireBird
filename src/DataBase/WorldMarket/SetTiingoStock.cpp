// CSetTiingoStock.cpp : CSetTiingoStock 类的实现

#include"pch.h"

#include "SetTiingoStock.h"

CSetTiingoStock::CSetTiingoStock(const CString& strSchema, const CString& strTable, CDatabase* pdb)
	: CVirtualRecordset(strSchema, strTable, pdb) {
	m_ID = 0;
	m_TiingoPermaTicker = _T("");
	m_Ticker = _T("");
	m_Name = _T("");
	m_IsActive = true;
	m_IsADR = false;
	m_SicCode = 0;
	m_SicIndustry = _T("");
	m_SicSector = _T("");
	m_TiingoIndustry = _T("");
	m_TiingoSector = _T("");
	m_ReportingCurrency = _T("");
	m_Location = _T("");
	m_CompanyWebSite = _T("");
	m_SECFilingWebSite = _T("");
	m_UpdateDate = _T("");

	m_nFields = 16;
}

void CSetTiingoStock::DoFieldExchange(CFieldExchange* pFX) {
	pFX->SetFieldType(CFieldExchange::outputColumn);
	// RFX_Text() 和 RFX_Int() 这类宏依赖的是
	// 成员变量的类型，而不是数据库字段的类型。
	// ODBC 尝试自动将列值转换为所请求的类型
	RFX_Long(pFX, _T("[ID]"), m_ID);
	RFX_Text(pFX, _T("[TiingoPermaTicker]"), m_TiingoPermaTicker);
	RFX_Text(pFX, _T("[Ticker]"), m_Ticker);
	RFX_Text(pFX, _T("[Name]"), m_Name);
	RFX_Bool(pFX, _T("[IsActive]"), m_IsActive);
	RFX_Bool(pFX, _T("[IsADR]"), m_IsADR);
	RFX_Long(pFX, _T("[SICCode]"), m_SicCode);
	RFX_Text(pFX, _T("[SICIndustry]"), m_SicIndustry);
	RFX_Text(pFX, _T("[SICSector]"), m_SicSector);
	RFX_Text(pFX, _T("[TiingoIndustry]"), m_TiingoIndustry);
	RFX_Text(pFX, _T("[TiingoSector]"), m_TiingoSector);
	RFX_Text(pFX, _T("[ReportingCurrency]"), m_ReportingCurrency);
	RFX_Text(pFX, _T("[Location]"), m_Location);
	RFX_Text(pFX, _T("[CompanyWebSite]"), m_CompanyWebSite);
	RFX_Text(pFX, _T("[SECFilingWebSite]"), m_SECFilingWebSite);
	RFX_Text(pFX, _T("UpdateDate"), m_UpdateDate, 10000); // Note RFX_Text默认最大长度为256.
}
