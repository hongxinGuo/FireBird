// SetSymbol.cpp : CSetStockCode 类的实现

// CSetStockCode 实现

// 代码生成在 2019年5月26日, 8:14

#include"pch.h"
#include "SetStockCode.h"
#include"globedef.h"
#include"accessory.h"

IMPLEMENT_DYNAMIC(CSetStockCode, CRecordset)

CSetStockCode::CSetStockCode(CDatabase* pdb)
  : CRecordset(pdb) {
  m_Symbol = _T("");
  m_StockName = _T("");
  m_DayLineStartDate = __CHINA_MARKET_BEGIN_DATE__;
  m_DayLineEndDate = __CHINA_MARKET_BEGIN_DATE__;
  m_IPOStatus = 0;
  m_nFields = 6;
}

CString CSetStockCode::GetDefaultConnect() {
  return GetChinaMarketSchemaConnect();
}

CString CSetStockCode::GetDefaultSQL() {
  return _T("[stockcode]");
}

void CSetStockCode::DoFieldExchange(CFieldExchange* pFX) {
  pFX->SetFieldType(CFieldExchange::outputColumn);
  // RFX_Text() 和 RFX_Int() 这类宏依赖的是
  // 成员变量的类型，而不是数据库字段的类型。
  // ODBC 尝试自动将列值转换为所请求的类型
  RFX_Long(pFX, _T("[ID]"), m_ID);
  RFX_Text(pFX, _T("[Symbol]"), m_Symbol);
  RFX_Text(pFX, _T("[StockName]"), m_StockName);
  RFX_Long(pFX, _T("[DayLineStartDate]"), m_DayLineStartDate);
  RFX_Long(pFX, _T("[DayLineEndDate]"), m_DayLineEndDate);
  RFX_Long(pFX, _T("[IPOed]"), m_IPOStatus);
}
/////////////////////////////////////////////////////////////////////////////
// CSetStockCode 诊断

#ifdef _DEBUG
void CSetStockCode::AssertValid() const {
  CRecordset::AssertValid();
}

void CSetStockCode::Dump(CDumpContext& dc) const {
  CRecordset::Dump(dc);
}
#endif //_DEBUG