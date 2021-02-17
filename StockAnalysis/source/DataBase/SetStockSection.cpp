#include"stdafx.h"

#include "SetStockSection.h"
#include"accessory.h"

// SetStockCode.h : CSetStockSection 类的实现

// CSetStockSection 实现

// 代码生成在 2019年5月26日, 8:14

#include "stdafx.h"

IMPLEMENT_DYNAMIC(CSetStockSection, CRecordset)

CSetStockSection::CSetStockSection(CDatabase* pdb)
  : CRecordset(pdb) {
  m_Active = false;
  m_Market = 0;
  m_IndexNumber = 0;
  m_Comment = _T("");
  m_nFields = 5;
}

CString CSetStockSection::GetDefaultConnect() {
  return GetChinaMarketSchemaConnect();
}

CString CSetStockSection::GetDefaultSQL() {
  return _T("[Stock_Code_Section]");
}

void CSetStockSection::DoFieldExchange(CFieldExchange* pFX) {
  pFX->SetFieldType(CFieldExchange::outputColumn);
  // RFX_Text() 和 RFX_Int() 这类宏依赖的是
  // 成员变量的类型，而不是数据库字段的类型。
  // ODBC 尝试自动将列值转换为所请求的类型
  RFX_Long(pFX, _T("[ID]"), m_ID);
  RFX_Bool(pFX, _T("Active"), m_Active);
  RFX_Int(pFX, _T("Market"), m_Market);
  RFX_Long(pFX, _T("IndexNumber"), m_IndexNumber);
  RFX_Text(pFX, _T("Comment"), m_Comment);
}

/////////////////////////////////////////////////////////////////////////////
// CSetStockCode 诊断

#ifdef _DEBUG
void CSetStockSection::AssertValid() const {
  CRecordset::AssertValid();
}

void CSetStockSection::Dump(CDumpContext& dc) const {
  CRecordset::Dump(dc);
}
#endif //_DEBUG