// CSetAmericaStake.cpp : CSetStockCode 类的实现

// CSetAmericaStake 实现

// 代码生成在 2019年5月26日, 8:14

#include "stdafx.h"
#include"globedef.h"
#include"accessory.h"

#include "SetAmericaStake.h"

IMPLEMENT_DYNAMIC(CSetAmericaStake, CRecordset)

CSetAmericaStake::CSetAmericaStake(CDatabase* pdb)
  : CRecordset(pdb) {
  m_IPODate = _T(" ");
  m_Currency = _T(" ");
  m_Type = _T(" ");
  m_Country = _T(" ");
  m_Description = _T(" ");
  m_DisplaySymbol = _T(" ");
  m_Exchange = _T(" ");
  m_FinnhubIndustry = _T(" ");
  m_Name = _T(" ");
  m_Phone = _T(" ");
  m_Symbol = _T(" ");
  m_Ticker = _T(" ");
  m_WebURL = _T(" ");
  m_AmericaStakeUpdateDate = 19900101;
  m_DayLineStartDate = 29900101;
  m_DayLineEndDate = 19900101;

  m_nFields = 19;
}

CString CSetAmericaStake::GetDefaultConnect() {
  return GetAmericaMarketSchemaConnect();
}

CString CSetAmericaStake::GetDefaultSQL() {
  return _T("[companyprofile]");
}

void CSetAmericaStake::DoFieldExchange(CFieldExchange* pFX) {
  pFX->SetFieldType(CFieldExchange::outputColumn);
  // RFX_Text() 和 RFX_Int() 这类宏依赖的是
  // 成员变量的类型，而不是数据库字段的类型。
  // ODBC 尝试自动将列值转换为所请求的类型
  RFX_Long(pFX, _T("[ID]"), m_ID);
  RFX_Text(pFX, _T("[Symbol]"), m_Symbol);
  RFX_Text(pFX, _T("[Description]"), m_Description);
  RFX_Text(pFX, _T("[DisplaySymbol]"), m_DisplaySymbol);
  RFX_Text(pFX, _T("[Type]"), m_Type);
  RFX_Text(pFX, _T("[Currency]"), m_Currency);
  RFX_Text(pFX, _T("[Country]"), m_Country);
  RFX_Text(pFX, _T("[Exchange]"), m_Exchange);
  RFX_Text(pFX, _T("[Ticker]"), m_Ticker);
  RFX_Text(pFX, _T("[IPODate]"), m_IPODate);
  RFX_Text(pFX, _T("[Phone]"), m_Phone);
  RFX_Text(pFX, _T("[ShareOutstanding]"), m_ShareOutstanding);
  RFX_Text(pFX, _T("[MarketCapitalization]"), m_MarketCapitalization);
  RFX_Text(pFX, _T("[Name]"), m_Name);
  RFX_Text(pFX, _T("[WebURL]"), m_WebURL);
  RFX_Text(pFX, _T("[FinnHubIndustry]"), m_FinnhubIndustry);
  RFX_Long(pFX, _T("[AmericaStakeUpdateDate]"), m_AmericaStakeUpdateDate);
  RFX_Long(pFX, _T("[DayLineStartDate]"), m_DayLineStartDate);
  RFX_Long(pFX, _T("[DayLineEndDate]"), m_DayLineEndDate);
}
/////////////////////////////////////////////////////////////////////////////
// CSetAmericaStake 诊断

#ifdef _DEBUG
void CSetAmericaStake::AssertValid() const {
  CRecordset::AssertValid();
}

void CSetAmericaStake::Dump(CDumpContext& dc) const {
  CRecordset::Dump(dc);
}
#endif //_DEBUG