// CSetWorldStock.cpp : CSetWorldStock 类的实现

#include"pch.h"

#include "SetWorldStock.h"

CSetWorldStock::CSetWorldStock(const CString& strSchema, const CString& strTable, CDatabase* pdb)
	: CVirtualRecordset(strSchema, strTable, pdb) {
	m_nFields = 37;
}

void CSetWorldStock::DoFieldExchange(CFieldExchange* pFX) {
	pFX->SetFieldType(CFieldExchange::outputColumn);
	// RFX_Text() 和 RFX_Int() 这类宏依赖的是
	// 成员变量的类型，而不是数据库字段的类型。
	// ODBC 尝试自动将列值转换为所请求的类型
	RFX_Long(pFX, _T("[ID]"), m_ID);
	RFX_Text(pFX, _T("[Symbol]"), m_Symbol);
	RFX_Text(pFX, _T("[ExchangeCode]"), m_ExchangeCode);
	RFX_Text(pFX, _T("[Description]"), m_Description);
	RFX_Text(pFX, _T("[DisplaySymbol]"), m_DisplaySymbol);
	RFX_Text(pFX, _T("[Type]"), m_Type);
	RFX_Text(pFX, _T("[Mic]"), m_Mic);
	RFX_Text(pFX, _T("[Figi]"), m_Figi);
	RFX_Text(pFX, _T("[Currency]"), m_Currency);

	RFX_Text(pFX, _T("[Address]"), m_Address);
	RFX_Text(pFX, _T("[City]"), m_City);
	RFX_Text(pFX, _T("[Country]"), m_Country);
	RFX_Text(pFX, _T("[cusip]"), m_Cusip);
	RFX_Text(pFX, _T("[sedol]"), m_Sedol);
	RFX_Long(pFX, _T("[EmployeeTotal]"), m_EmployeeTotal);
	RFX_Text(pFX, _T("[ggroup]"), m_Ggroup);
	RFX_Text(pFX, _T("[gind]"), m_Gind);
	RFX_Text(pFX, _T("[gsector]"), m_Gsector);
	RFX_Text(pFX, _T("[gsubind]"), m_Gsubind);
	RFX_Text(pFX, _T("[IPODate]"), m_IPODate);
	RFX_Text(pFX, _T("[isin]"), m_Isin);
	RFX_Text(pFX, _T("[MarketCapitalization]"), m_MarketCapitalization);
	RFX_Text(pFX, _T("[naics]"), m_Naics);
	RFX_Text(pFX, _T("[naicsNationalIndustry]"), m_NaicsNationalIndustry);
	RFX_Text(pFX, _T("[naicsSector]"), m_NaicsSector);
	RFX_Text(pFX, _T("[naicsSubsector]"), m_NaicsSubsector);
	RFX_Text(pFX, _T("[Name]"), m_Name);
	RFX_Text(pFX, _T("[Phone]"), m_Phone);
	RFX_Text(pFX, _T("[ShareOutstanding]"), m_ShareOutstanding);
	RFX_Text(pFX, _T("[state]"), m_State);
	RFX_Text(pFX, _T("[Ticker]"), m_Ticker);
	RFX_Text(pFX, _T("[WebURL]"), m_WebURL);
	RFX_Text(pFX, _T("[Logo]"), m_Logo);
	RFX_Text(pFX, _T("[FinnhubIndustry]"), m_FinnhubIndustry);
	RFX_Text(pFX, _T("[Peer]"), m_Peer, 2000);
	RFX_Long(pFX, _T("[IPOStatus]"), m_IPOStatus);
	RFX_Text(pFX, _T("UpdateDate"), m_UpdateDate, 10000); // Note RFX_Text默认最大长度为256.
}
