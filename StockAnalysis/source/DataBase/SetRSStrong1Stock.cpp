#include "stdafx.h"
#include"globedef.h"
#include"Accessory.h"

#include "SetRSStrong1Stock.h"

IMPLEMENT_DYNAMIC(CSetRSStrong1Stock, CRecordset)

CSetRSStrong1Stock::CSetRSStrong1Stock(CDatabase* pdb) : CRecordset(pdb) {
  m_Symbol = _T("");
  m_nFields = 1;
}

CString CSetRSStrong1Stock::GetDefaultConnect() {
  return GetChinaMarketSchemaConnect();
}

CString CSetRSStrong1Stock::GetDefaultSQL() {
  return _T("[10rs1above55]");
}

void CSetRSStrong1Stock::DoFieldExchange(CFieldExchange* pFX) {
  pFX->SetFieldType(CFieldExchange::outputColumn);
  // RFX_Text() 和 RFX_Int() 这类宏依赖的是
  // 成员变量的类型，而不是数据库字段的类型。
  // ODBC 尝试自动将列值转换为所请求的类型
  RFX_Text(pFX, _T("[Symbol]"), m_Symbol);
}
/////////////////////////////////////////////////////////////////////////////
// CSetRSStrong1Stock 诊断

#ifdef _DEBUG
void CSetRSStrong1Stock::AssertValid() const {
  CRecordset::AssertValid();
}

void CSetRSStrong1Stock::Dump(CDumpContext& dc) const {
  CRecordset::Dump(dc);
}
#endif //_DEBUG