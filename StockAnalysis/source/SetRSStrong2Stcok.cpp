#include "stdafx.h"
#include"globedef.h"
#include"Accessory.h"

#include "SetRSStrong2Stock.h"

IMPLEMENT_DYNAMIC(CSetRSStrong2Stock, CRecordset)

CSetRSStrong2Stock::CSetRSStrong2Stock(CDatabase* pdb) : CRecordset(pdb) {
  m_Market = 0;
  m_StockCode = "";
  m_nFields = 2;
}

CString CSetRSStrong2Stock::GetDefaultConnect() {
  return GetDefaultSchemaConnect();
}

CString CSetRSStrong2Stock::GetDefaultSQL() {
  return _T("[10rsabove55]");
}

void CSetRSStrong2Stock::DoFieldExchange(CFieldExchange* pFX) {
  pFX->SetFieldType(CFieldExchange::outputColumn);
  // RFX_Text() 和 RFX_Int() 这类宏依赖的是
  // 成员变量的类型，而不是数据库字段的类型。
  // ODBC 尝试自动将列值转换为所请求的类型
  RFX_Long(pFX, _T("[Market]"), m_Market);
  RFX_Text(pFX, _T("[StockCode]"), m_StockCode);
}
/////////////////////////////////////////////////////////////////////////////
// CSetRSStrong2Stock 诊断

#ifdef _DEBUG
void CSetRSStrong2Stock::AssertValid() const {
  CRecordset::AssertValid();
}

void CSetRSStrong2Stock::Dump(CDumpContext& dc) const {
  CRecordset::Dump(dc);
}
#endif //_DEBUG