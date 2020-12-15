// CSetForexExchange.cpp : CSetForexExchange 类的实现

// 代码生成在 2019年5月26日, 8:14

#include "stdafx.h"
#include"globedef.h"
#include"accessory.h"

#include "SetForexExchange.h"

IMPLEMENT_DYNAMIC(CSetForexExchange, CRecordset)

CSetForexExchange::CSetForexExchange(CDatabase* pdb)
  : CRecordset(pdb) {
  m_Exchange = _T(" ");

  m_nFields = 2;
}

CString CSetForexExchange::GetDefaultConnect() {
  return GetAmericaMarketSchemaConnect();
}

CString CSetForexExchange::GetDefaultSQL() {
  return _T("[forex_exchange]");
}

void CSetForexExchange::DoFieldExchange(CFieldExchange* pFX) {
  pFX->SetFieldType(CFieldExchange::outputColumn);
  // RFX_Text() 和 RFX_Int() 这类宏依赖的是
  // 成员变量的类型，而不是数据库字段的类型。
  // ODBC 尝试自动将列值转换为所请求的类型
  RFX_Long(pFX, _T("[ID]"), m_ID);
  RFX_Text(pFX, _T("[Exchange]"), m_Exchange);
}
/////////////////////////////////////////////////////////////////////////////
// CSetAmericaStake 诊断

#ifdef _DEBUG
void CSetForexExchange::AssertValid() const {
  CRecordset::AssertValid();
}

void CSetForexExchange::Dump(CDumpContext& dc) const {
  CRecordset::Dump(dc);
}
#endif //_DEBUG