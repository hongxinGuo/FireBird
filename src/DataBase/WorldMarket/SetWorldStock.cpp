// CSetWorldStock.cpp : CSetWorldStock 类的实现

#include"pch.h"

#include "SetWorldStock.h"

CSetWorldStock::CSetWorldStock(const CString& strSchema, const CString& strTable, CDatabase* pdb)
	: CVirtualRecordset(strSchema, strTable, pdb) {
	m_ID = 0;
	m_Description = _T(" ");
	m_DisplaySymbol = _T(" ");
	m_IPODate = _T(" ");
	m_Type = _T(" ");
	m_Mic = _T(" ");
	m_Figi = _T(" ");
	m_Currency = _T(" ");
	m_Country = _T(" ");
	m_ListedExchange = _T(" ");
	m_FinnhubIndustry = _T(" ");
	m_Peer = _T("{}"); // 这个是json制式数据
	m_Name = _T(" ");
	m_Phone = _T(" ");
	m_Symbol = _T(" ");
	m_EmployeeTotal = 0;
	m_ExchangeCode = _T(" ");
	m_Ticker = _T(" ");
	m_WebURL = _T(" ");
	m_Cusip = _T(" ");
	m_Sedol = _T(" ");
	m_Ggroup = _T(" ");
	m_Gind = _T(" ");
	m_Gsector = _T(" ");
	m_Gsubind = _T(" ");
	m_Isin = _T(" ");
	m_Naics = _T(" ");
	m_NaicsNationalIndustry = _T(" ");
	m_NaicsSector = _T(" ");
	m_NaicsSubsector = _T(" ");
	m_Logo = _T(" ");
	m_State = _T(" ");

	m_IPOStatus = _STOCK_NOT_CHECKED_;

	// Tiingo Symbol信息
	m_TiingoPermaTicker = _T("");
	m_IsActive = true;
	m_IsADR = false;
	m_TiingoIndustry = _T("");
	m_TiingoSector = _T("");
	m_SICCode = 0;
	m_SICIndustry = _T("");
	m_SICSector = _T("");
	m_CompanyWebSite = _T("");
	m_SECFilingWebSite = _T("");

	m_UpdateDate = _T("{}"); // 这个用于存储各更新日期（json制式）

	m_nFields = 48;
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
	RFX_Text(pFX, _T("[ListedExchange]"), m_ListedExchange);
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
	RFX_Text(pFX, _T("[TiingoPermaTicker]"), m_TiingoPermaTicker);
	RFX_Bool(pFX, _T("[IsActive]"), m_IsActive);
	RFX_Bool(pFX, _T("[IsADR]"), m_IsADR);
	RFX_Long(pFX, _T("[SICCode]"), m_SICCode);
	RFX_Text(pFX, _T("[SICIndustry]"), m_SICIndustry);
	RFX_Text(pFX, _T("[SICSector]"), m_SICSector);
	RFX_Text(pFX, _T("[TiingoIndustry]"), m_TiingoIndustry);
	RFX_Text(pFX, _T("[TiingoSector]"), m_TiingoSector);
	RFX_Text(pFX, _T("[CompanyWebSite]"), m_CompanyWebSite);
	RFX_Text(pFX, _T("[SECFilingWebSite]"), m_SECFilingWebSite);
	RFX_Text(pFX, _T("UpdateDate"), m_UpdateDate, 10000); // Note RFX_Text默认最大长度为256.
}
