// CSetFinnhubForexExchange.cpp : CSetFinnhubForexExchange 类的实现

// 代码生成在 2019年5月26日, 8:14

#include "stdafx.h"
#include"globedef.h"
#include"accessory.h"

#include "SetFinnhubForexExchange.h"

IMPLEMENT_DYNAMIC(CSetFinnhubForexExchange, CRecordset)

CSetFinnhubForexExchange::CSetFinnhubForexExchange(CDatabase* pdb)
  : CRecordset(pdb) {
  m_Exchange = _T(" ");

  m_nFields = 2;
}

CString CSetFinnhubForexExchange::GetDefaultConnect() {
  return GetAmericaMarketSchemaConnect();
}

CString CSetFinnhubForexExchange::GetDefaultSQL() {
  return _T("[finnhub_forex_exchange]");
}

void CSetFinnhubForexExchange::DoFieldExchange(CFieldExchange* pFX) {
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
void CSetFinnhubForexExchange::AssertValid() const {
  CRecordset::AssertValid();
}

void CSetFinnhubForexExchange::Dump(CDumpContext& dc) const {
  CRecordset::Dump(dc);
}
#endif //_DEBUG