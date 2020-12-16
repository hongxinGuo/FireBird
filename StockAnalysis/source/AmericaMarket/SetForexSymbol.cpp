// SetForexSymbol.cpp : CSetForexSymbol 类的实现

// 代码生成在 2019年5月26日, 8:14

#include "stdafx.h"
#include"globedef.h"
#include"accessory.h"

#include "SetForexSymbol.h"

IMPLEMENT_DYNAMIC(CSetForexSymbol, CRecordset)

CSetForexSymbol::CSetForexSymbol(CDatabase* pdb)
  : CRecordset(pdb) {
  m_Description = _T(" ");
  m_DisplaySymbol = _T(" ");
  m_Exchange = _T(" ");
  m_Symbol = _T(" ");

  m_nFields = 5;
}

CString CSetForexSymbol::GetDefaultConnect() {
  return GetAmericaMarketSchemaConnect();
}

CString CSetForexSymbol::GetDefaultSQL() {
  return _T("[forex_symbol]");
}

void CSetForexSymbol::DoFieldExchange(CFieldExchange* pFX) {
  pFX->SetFieldType(CFieldExchange::outputColumn);
  // RFX_Text() 和 RFX_Int() 这类宏依赖的是
  // 成员变量的类型，而不是数据库字段的类型。
  // ODBC 尝试自动将列值转换为所请求的类型
  RFX_Long(pFX, _T("[ID]"), m_ID);
  RFX_Text(pFX, _T("[Description]"), m_Description);
  RFX_Text(pFX, _T("[DisplaySymbol]"), m_DisplaySymbol);
  RFX_Text(pFX, _T("[Exchange]"), m_Exchange);
  RFX_Text(pFX, _T("[Symbol]"), m_Symbol);
}
/////////////////////////////////////////////////////////////////////////////
// CSetAmericaStake 诊断

#ifdef _DEBUG
void CSetForexSymbol::AssertValid() const {
  CRecordset::AssertValid();
}

void CSetForexSymbol::Dump(CDumpContext& dc) const {
  CRecordset::Dump(dc);
}
#endif //_DEBUG