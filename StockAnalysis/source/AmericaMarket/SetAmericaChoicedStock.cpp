// SetAmericaChoicedStock.h : CSetAmericaChoicedStock 类的实现

// CSetAmericaChoicedStock 实现

// 代码生成在 2019年5月26日, 8:15

#include "stdafx.h"
#include"globedef.h"
#include"Accessory.h"

#include "SetAmericaChoicedStock.h"
IMPLEMENT_DYNAMIC(CSetAmericaChoicedStock, CRecordset)

CSetAmericaChoicedStock::CSetAmericaChoicedStock(CDatabase* pdb) : CRecordset(pdb) {
  m_ID = 0;
  m_Symbol = _T("");
  m_nFields = 2;
}

CString CSetAmericaChoicedStock::GetDefaultConnect() {
  return GetAmericaMarketSchemaConnect();
}

CString CSetAmericaChoicedStock::GetDefaultSQL() {
  return _T("[choicedstock]");
}

void CSetAmericaChoicedStock::DoFieldExchange(CFieldExchange* pFX) {
  pFX->SetFieldType(CFieldExchange::outputColumn);
  // RFX_Text() 和 RFX_Int() 这类宏依赖的是
  // 成员变量的类型，而不是数据库字段的类型。
  // ODBC 尝试自动将列值转换为所请求的类型
  RFX_Long(pFX, _T("[ID]"), m_ID);
  RFX_Text(pFX, _T("[Symbol]"), m_Symbol);
}
/////////////////////////////////////////////////////////////////////////////
// CSetChoicedStock 诊断

#ifdef _DEBUG
void CSetAmericaChoicedStock::AssertValid() const {
  CRecordset::AssertValid();
}

void CSetAmericaChoicedStock::Dump(CDumpContext& dc) const {
  CRecordset::Dump(dc);
}
#endif //_DEBUG