/////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "SetRSOption.h"
#include"globedef.h"
#include"accessory.h"

IMPLEMENT_DYNAMIC(CSetRSOption, CRecordset)

CSetRSOption::CSetRSOption(CDatabase* pdb)
  : CRecordset(pdb) {
  m_ID = 0;
  m_Index = 0;
  m_Active = false;
  m_DayLengthFirst = 0;
  m_DayLengthSecond = 0;
  m_DayLengthThird = 0;
  m_DayLengthFourth = 0;
  m_StrongDayLengthFirst = 0;
  m_StrongDayLengthSecond = 0;
  m_StrongDayLengthThird = 0;
  m_StrongDayLengthFourth = 0;
  m_RSStrongFirst = _T("50.000");
  m_RSStrongSecond = _T("50.000");
  m_RSStrongThird = _T("50.000");
  m_RSStrongFourth = _T("50.000");
  m_nFields = 15;
}

CString CSetRSOption::GetDefaultConnect() {
  return GetChinaMarketSchemaVonnect();
}

CString CSetRSOption::GetDefaultSQL() {
  return _T("[option_calculatingrs]");
}

void CSetRSOption::DoFieldExchange(CFieldExchange* pFX) {
  pFX->SetFieldType(CFieldExchange::outputColumn);
  // RFX_Text() 和 RFX_Int() 这类宏依赖的是
  // 成员变量的类型，而不是数据库字段的类型。
  // ODBC 尝试自动将列值转换为所请求的类型
  RFX_Long(pFX, _T("[ID]"), m_ID);
  RFX_Long(pFX, _T("[Index]"), m_Index);
  RFX_Bool(pFX, _T("[Active]"), m_Active);
  RFX_Long(pFX, _T("[DayLengthFirst]"), m_DayLengthFirst);
  RFX_Long(pFX, _T("[DayLengthSecond]"), m_DayLengthSecond);
  RFX_Long(pFX, _T("[DayLengthThird]"), m_DayLengthThird);
  RFX_Long(pFX, _T("[DayLengthFourth]"), m_DayLengthFourth);
  RFX_Long(pFX, _T("[StrongDayLengthFirst]"), m_StrongDayLengthFirst);
  RFX_Long(pFX, _T("[StrongDayLengthSecond]"), m_StrongDayLengthSecond);
  RFX_Long(pFX, _T("[StrongDayLengthThird]"), m_StrongDayLengthThird);
  RFX_Long(pFX, _T("[StrongDayLengthFourth]"), m_StrongDayLengthFourth);
  RFX_Text(pFX, _T("[RSStrongFirst]"), m_RSStrongFirst);
  RFX_Text(pFX, _T("[RSStrongSecond]"), m_RSStrongSecond);
  RFX_Text(pFX, _T("[RSStrongThird]"), m_RSStrongThird);
  RFX_Text(pFX, _T("[RSStrongFourth]"), m_RSStrongFourth);
}
/////////////////////////////////////////////////////////////////////////////
// CSetStockCode 诊断

#ifdef _DEBUG
void CSetRSOption::AssertValid() const {
  CRecordset::AssertValid();
}

void CSetRSOption::Dump(CDumpContext& dc) const {
  CRecordset::Dump(dc);
}
#endif //_DEBUG