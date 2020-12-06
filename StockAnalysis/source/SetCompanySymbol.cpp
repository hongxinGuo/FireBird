// SetCompanySymbol.cpp : CSetStockCode 类的实现

// CSetCompanySymbol 实现

// 代码生成在 2019年5月26日, 8:14

#include "stdafx.h"
#include "SetCompanySymbol.h"
#include"globedef.h"
#include"accessory.h"

IMPLEMENT_DYNAMIC(CSetCompanySymbol, CRecordset)

CSetCompanySymbol::CSetCompanySymbol(CDatabase* pdb)
  : CRecordset(pdb) {
  m_Description = _T("");
  m_DisplaySymbol = _T("");
  m_Symbol = _T("");
  m_Type = _T("");
  m_Currency = _T("");
  m_nFields = 6;
}

CString CSetCompanySymbol::GetDefaultConnect() {
  if (!gl_fNormalMode) {
    gl_fTestMode = true;
    return _T("DSN=mysqlTest;UID=Test;PASSWORD=test;charset=utf8mb4"); // mysqlTest操作的是TestData Schema
  }
  else {
    gl_fTestMode = false;
    return _T("DSN=AmericaMarket;UID=AmericaMarket;PASSWORD=hxguo;charset=utf8mb4"); // mysql操作的是AmericaStakeMarket Schema
  }
}

CString CSetCompanySymbol::GetDefaultSQL() {
  return _T("[stakesymbol]");
}

void CSetCompanySymbol::DoFieldExchange(CFieldExchange* pFX) {
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
}
/////////////////////////////////////////////////////////////////////////////
// CSetStockCode 诊断

#ifdef _DEBUG
void CSetCompanySymbol::AssertValid() const {
  CRecordset::AssertValid();
}

void CSetCompanySymbol::Dump(CDumpContext& dc) const {
  CRecordset::Dump(dc);
}
#endif //_DEBUG