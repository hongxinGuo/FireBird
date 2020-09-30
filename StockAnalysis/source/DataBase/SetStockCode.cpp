// SetStockCode.h : CSetStockCode 类的实现

// CSetStockCode 实现

// 代码生成在 2019年5月26日, 8:14

#include "stdafx.h"
#include "SetStockCode.h"
#include"globedef.h"
#include"accessory.h"

IMPLEMENT_DYNAMIC(CSetStockCode, CRecordset)

CSetStockCode::CSetStockCode(CDatabase* pdb)
  : CRecordset(pdb) {
  m_Counter = 0;
  m_StockType = 0;
  m_StockCode = _T("");
  m_StockName = _T("");
  m_DLStartDate = __CHINA_MARKET_BEGIN_DAY__;
  m_DLEndDate = __CHINA_MARKET_BEGIN_DAY__;
  m_IPOStatus = 0;
  m_nFields = 8;
}

CString CSetStockCode::GetDefaultConnect() {
  return GetDefaultSchemaConnect();
}

CString CSetStockCode::GetDefaultSQL() {
  return _T("[StockCode]");
}

void CSetStockCode::DoFieldExchange(CFieldExchange* pFX) {
  pFX->SetFieldType(CFieldExchange::outputColumn);
  // RFX_Text() 和 RFX_Int() 这类宏依赖的是
  // 成员变量的类型，而不是数据库字段的类型。
  // ODBC 尝试自动将列值转换为所请求的类型
  RFX_Long(pFX, _T("[ID]"), m_ID);
  RFX_Long(pFX, _T("[Counter]"), m_Counter);
  RFX_Int(pFX, _T("[StockType]"), m_StockType);
  RFX_Text(pFX, _T("[StockCode]"), m_StockCode);
  RFX_Text(pFX, _T("[StockName]"), m_StockName);
  RFX_Long(pFX, _T("[DayLineStartDate]"), m_DLStartDate);
  RFX_Long(pFX, _T("[DayLineEndDate]"), m_DLEndDate);
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