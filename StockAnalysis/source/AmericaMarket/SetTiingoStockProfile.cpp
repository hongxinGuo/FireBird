// CSetTiingoStockProfile.cpp : CSetStockCode 类的实现

// CSetTiingoStockProfile 实现

// 代码生成在 2019年5月26日, 8:14

#include"pch.h"
#include"globedef.h"
#include"accessory.h"

#include "SetTiingoStockProfile.h"

IMPLEMENT_DYNAMIC(CSetTiingoStockProfile, CRecordset)

CSetTiingoStockProfile::CSetTiingoStockProfile(CDatabase* pdb)
  : CRecordset(pdb) {
  m_TiingoPermaTicker = _T("");
  m_Ticker = _T("");
  m_Name = _T("");
  m_IsActive = true;
  m_IsADR = false;
  m_SICCode = 0;
  m_SICIndustry = _T("");
  m_SICSector = _T("");
  m_TiingoIndustry = _T("");
  m_TiingoSector = _T("");
  m_ReportingCurrency = _T("");
  m_Location = _T("");
  m_CompanyWebSite = _T("");
  m_SECFilingWebSite = _T("");
  m_DailyDataUpdateDate = 19800101;
  m_StatementUpdateDate = 19800101;

  m_nFields = 17;
}

CString CSetTiingoStockProfile::GetDefaultConnect() {
  return GetWorldMarketSchemaConnect();
}

CString CSetTiingoStockProfile::GetDefaultSQL() {
  return _T("[tiingo_stock_fundanmental]");
}

void CSetTiingoStockProfile::DoFieldExchange(CFieldExchange* pFX) {
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
  RFX_Long(pFX, _T("[SICCode]"), m_SICCode);
  RFX_Text(pFX, _T("[SICIndustry]"), m_SICIndustry);
  RFX_Text(pFX, _T("[SICSector]"), m_SICSector);
  RFX_Text(pFX, _T("[TiingoIndustry]"), m_TiingoIndustry);
  RFX_Text(pFX, _T("[TiingoSector]"), m_TiingoSector);
  RFX_Text(pFX, _T("[ReportingCurrency]"), m_ReportingCurrency);
  RFX_Text(pFX, _T("[Location]"), m_Location);
  RFX_Text(pFX, _T("[CompanyWebSite]"), m_CompanyWebSite);
  RFX_Text(pFX, _T("[SECFilingWebSite]"), m_SECFilingWebSite);
  RFX_Long(pFX, _T("[StatementUpdateDate]"), m_StatementUpdateDate);
  RFX_Long(pFX, _T("[DailyDataUpdateDate]"), m_DailyDataUpdateDate);
}
/////////////////////////////////////////////////////////////////////////////
// CSetTiingoStockProfile 诊断

#ifdef _DEBUG
void CSetTiingoStockProfile::AssertValid() const {
  CRecordset::AssertValid();
}

void CSetTiingoStockProfile::Dump(CDumpContext& dc) const {
  CRecordset::Dump(dc);
}
#endif //_DEBUG