// CSetTiingoIndustry.cpp : CSetTiingoIndustry 类的实现

// CSetTiingoIndustry 实现

// 代码生成在 2021年3月19日, 5:10

#include"pch.h"
#include"afxdb.h"
#include"globedef.h"
#include"accessory.h"

#include "SetTiingoIndustry.h"

IMPLEMENT_DYNAMIC(CSetTiingoIndustry, CRecordset)

CSetTiingoIndustry::CSetTiingoIndustry(CDatabase* pdb)
  : CRecordset(pdb) {
  m_ID = 0;
  m_Industry = _T("");
  m_Sector = _T("");
  m_nFields = 3;
}

CString CSetTiingoIndustry::GetDefaultConnect() {
  return GetWorldMarketSchemaConnect();
}

CString CSetTiingoIndustry::GetDefaultSQL() {
  return _T("[tiingo_industry]");
}

void CSetTiingoIndustry::DoFieldExchange(CFieldExchange* pFX) {
  pFX->SetFieldType(CFieldExchange::outputColumn);
  // RFX_Text() 和 RFX_Int() 这类宏依赖的是
  // 成员变量的类型，而不是数据库字段的类型。
  // ODBC 尝试自动将列值转换为所请求的类型
  RFX_Long(pFX, _T("[ID]"), m_ID);
  RFX_Text(pFX, _T("[Industry]"), m_Industry);
  RFX_Text(pFX, _T("[Sector]"), m_Sector);
}
/////////////////////////////////////////////////////////////////////////////
// CSetTiingoIndustry 诊断

#ifdef _DEBUG
void CSetTiingoIndustry::AssertValid() const {
  CRecordset::AssertValid();
}

void CSetTiingoIndustry::Dump(CDumpContext& dc) const {
  CRecordset::Dump(dc);
}
#endif //_DEBUG