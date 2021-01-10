// SetFinnhubForexSymbol.cpp : CSetFinnhubForexSymbol 类的实现

// 代码生成在 2019年5月26日, 8:14

#include "stdafx.h"
#include"globedef.h"
#include"accessory.h"

#include "SetFinnhubForexSymbol.h"

IMPLEMENT_DYNAMIC(CSetFinnhubForexSymbol, CRecordset)

CSetFinnhubForexSymbol::CSetFinnhubForexSymbol(CDatabase* pdb)
  : CRecordset(pdb) {
  m_Description = _T(" ");
  m_DisplaySymbol = _T(" ");
  m_Exchange = _T(" ");
  m_Symbol = _T(" ");
  m_DayLineStartDate = 29900101;
  m_DayLineEndDate = 19800101;
  m_IPOStatus = __STAKE_NOT_CHECKED__;

  m_nFields = 8;
}

CString CSetFinnhubForexSymbol::GetDefaultConnect() {
  return GetAmericaMarketSchemaConnect();
}

CString CSetFinnhubForexSymbol::GetDefaultSQL() {
  return _T("[finnhub_forex_symbol]");
}

void CSetFinnhubForexSymbol::DoFieldExchange(CFieldExchange* pFX) {
  pFX->SetFieldType(CFieldExchange::outputColumn);
  // RFX_Text() 和 RFX_Int() 这类宏依赖的是
  // 成员变量的类型，而不是数据库字段的类型。
  // ODBC 尝试自动将列值转换为所请求的类型
  RFX_Long(pFX, _T("[ID]"), m_ID);
  RFX_Text(pFX, _T("[Description]"), m_Description);
  RFX_Text(pFX, _T("[DisplaySymbol]"), m_DisplaySymbol);
  RFX_Text(pFX, _T("[Exchange]"), m_Exchange);
  RFX_Text(pFX, _T("[Symbol]"), m_Symbol);
  RFX_Long(pFX, _T("[DayLineStartDate]"), m_DayLineStartDate);
  RFX_Long(pFX, _T("[DayLineEndDate]"), m_DayLineEndDate);
  RFX_Long(pFX, _T("[IPOStatus]"), m_IPOStatus);
}
/////////////////////////////////////////////////////////////////////////////
// CSetAmericaStake 诊断

#ifdef _DEBUG
void CSetFinnhubForexSymbol::AssertValid() const {
  CRecordset::AssertValid();
}

void CSetFinnhubForexSymbol::Dump(CDumpContext& dc) const {
  CRecordset::Dump(dc);
}
#endif //_DEBUG