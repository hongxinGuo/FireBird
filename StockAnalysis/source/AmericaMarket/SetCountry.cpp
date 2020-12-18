// CSetCountry.cpp : CSetCountry 类的实现

// CSetAmericaStakeDayLine 实现

// 代码生成在 2019年6月2日, 5:10

#include "stdafx.h"
#include"afxdb.h"
#include"globedef.h"
#include"accessory.h"

#include "SetCountry.h"

IMPLEMENT_DYNAMIC(CSetCountry, CRecordset)

CSetCountry::CSetCountry(CDatabase* pdb)
  : CRecordset(pdb) {
  m_ID = 0;
  m_Code2 = _T(" ");
  m_Code3 = _T(" ");
  m_CodeNo = _T(" ");
  m_Country = _T(" ");
  m_Currency = _T(" ");
  m_CurrencyCode = _T(" ");
  m_nFields = 7;
}

CString CSetCountry::GetDefaultConnect() {
  return GetAmericaMarketSchemaConnect();
}

CString CSetCountry::GetDefaultSQL() {
  return _T("[country_list]");
}

void CSetCountry::DoFieldExchange(CFieldExchange* pFX) {
  pFX->SetFieldType(CFieldExchange::outputColumn);
  // RFX_Text() 和 RFX_Int() 这类宏依赖的是
  // 成员变量的类型，而不是数据库字段的类型。
  // ODBC 尝试自动将列值转换为所请求的类型
  RFX_Long(pFX, _T("[ID]"), m_ID);
  RFX_Text(pFX, _T("Code2"), m_Code2);
  RFX_Text(pFX, _T("Code3"), m_Code3);
  RFX_Text(pFX, _T("CodeNo"), m_CodeNo);
  RFX_Text(pFX, _T("Country"), m_Country);
  RFX_Text(pFX, _T("Currency"), m_Currency);
  RFX_Text(pFX, _T("CurrencyCode"), m_CurrencyCode);
}
/////////////////////////////////////////////////////////////////////////////
// CSetAmericaForexDayLine 诊断

#ifdef _DEBUG
void CSetCountry::AssertValid() const {
  CRecordset::AssertValid();
}

void CSetCountry::Dump(CDumpContext& dc) const {
  CRecordset::Dump(dc);
}
#endif //_DEBUG