#include "stdafx.h"
#include"accessory.h"

#include "SetOption.h"
#include"globedef.h"

IMPLEMENT_DYNAMIC(CSetOption, CRecordset)

CSetOption::CSetOption(CDatabase* pdb)
  : CRecordset(pdb) {
  m_ID = 0;
  m_RSStartDate = __CHINA_MARKET_BEGIN_DAY__;
  m_RSEndDate = __CHINA_MARKET_BEGIN_DAY__;
  m_LastLoginDay = __CHINA_MARKET_BEGIN_DAY__;
  m_UpdatedDayFor10DayRS1 = __CHINA_MARKET_BEGIN_DAY__;
  m_UpdatedDayFor10DayRS2 = __CHINA_MARKET_BEGIN_DAY__;
  m_UpdatedDayFor10DayRS = __CHINA_MARKET_BEGIN_DAY__;
  m_nFields = 6;
}
// 此连接字符串中可能包含明文密码和/或其他重要
// 信息。请在查看完此连接字符串并找到所有与安全
// 有关的问题后移除 #error。可能需要将此密码存
// 储为其他格式或使用其他的用户身份验证。
CString CSetOption::GetDefaultConnect() {
  return GetDefaultSchemaConnect();
}

CString CSetOption::GetDefaultSQL() {
  return _T("[options]");
}

void CSetOption::DoFieldExchange(CFieldExchange* pFX) {
  pFX->SetFieldType(CFieldExchange::outputColumn);
  // RFX_Text() 和 RFX_Int() 这类宏依赖的是
  // 成员变量的类型，而不是数据库字段的类型。
  // ODBC 尝试自动将列值转换为所请求的类型
  RFX_Long(pFX, _T("[RelativeStrongStartDate]"), m_RSStartDate);
  RFX_Long(pFX, _T("[RelativeStrongEndDate]"), m_RSEndDate);
  RFX_Long(pFX, _T("[LastLoginDay]"), m_LastLoginDay);
  RFX_Long(pFX, _T("[UpdatedDayFor10DayRS1]"), m_UpdatedDayFor10DayRS1);
  RFX_Long(pFX, _T("[UpdatedDayFor10DayRS2]"), m_UpdatedDayFor10DayRS2);
  RFX_Long(pFX, _T("[UpdatedDayFor10DayRS]"), m_UpdatedDayFor10DayRS);
}
/////////////////////////////////////////////////////////////////////////////
// CSetStockCode 诊断

#ifdef _DEBUG
void CSetOption::AssertValid() const {
  CRecordset::AssertValid();
}

void CSetOption::Dump(CDumpContext& dc) const {
  CRecordset::Dump(dc);
}
#endif //_DEBUG