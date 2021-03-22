// CSetWorldStock.cpp : CSetStockCode 类的实现

// CSetWorldStock 实现

// 代码生成在 2019年5月26日, 8:14

#include"pch.h"
#include"globedef.h"
#include"accessory.h"

#include "SetWorldStock.h"

IMPLEMENT_DYNAMIC(CSetWorldStock, CRecordset)

CSetWorldStock::CSetWorldStock(CDatabase* pdb)
  : CRecordset(pdb) {
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
  m_Peer = _T(" ");
  m_Name = _T(" ");
  m_Phone = _T(" ");
  m_Symbol = _T(" ");
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

  m_ProfileUpdateDate = 19800101;
  m_DayLineStartDate = 29900101;
  m_DayLineEndDate = 19800101;
  m_PeerUpdateDate = 19800101;
  m_LastRTDataUpdateDate = 19800101;
  m_LastEPSSurpriseUpdateDate = 19800101;
  m_IPOStatus = __STOCK_NOT_CHECKED__;

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
  m_DailyDataUpdateDate = 19800101;
  m_StatementUpdateDate = 19800101;

  m_nFields = 55;
}

CString CSetWorldStock::GetDefaultConnect() {
  return GetWorldMarketSchemaConnect();
}

CString CSetWorldStock::GetDefaultSQL() {
  return _T("[stock_profile]");
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
  RFX_Text(pFX, _T("[Peer]"), m_Peer);
  RFX_Long(pFX, _T("[ProfileUpdateDate]"), m_ProfileUpdateDate);
  RFX_Long(pFX, _T("[DayLineStartDate]"), m_DayLineStartDate);
  RFX_Long(pFX, _T("[DayLineEndDate]"), m_DayLineEndDate);
  RFX_Long(pFX, _T("[LastRTDataUpdateDate]"), m_LastRTDataUpdateDate);
  RFX_Long(pFX, _T("[PeerUpdateDate]"), m_PeerUpdateDate);
  RFX_Long(pFX, _T("[LastEPSSurpriseUpdateDate]"), m_LastEPSSurpriseUpdateDate);
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
  RFX_Long(pFX, _T("[StatementUpdateDate]"), m_StatementUpdateDate);
  RFX_Long(pFX, _T("[DailyDataUpdateDate]"), m_DailyDataUpdateDate);
}
/////////////////////////////////////////////////////////////////////////////
// CSetWorldStock 诊断

#ifdef _DEBUG
void CSetWorldStock::AssertValid() const {
  CRecordset::AssertValid();
}

void CSetWorldStock::Dump(CDumpContext& dc) const {
  CRecordset::Dump(dc);
}
#endif //_DEBUG