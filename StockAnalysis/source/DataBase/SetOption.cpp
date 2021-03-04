#include"pch.h"
#include"accessory.h"

#include "SetOption.h"
#include"globedef.h"

IMPLEMENT_DYNAMIC(CSetOption, CRecordset)

CSetOption::CSetOption(CDatabase* pdb)
  : CRecordset(pdb) {
  m_ID = 0;
  m_RSStartDate = __CHINA_MARKET_BEGIN_DATE__;
  m_RSEndDate = __CHINA_MARKET_BEGIN_DATE__;
  m_LastLoginDate = __CHINA_MARKET_BEGIN_DATE__;
  m_UpdatedDateFor10DaysRS1 = __CHINA_MARKET_BEGIN_DATE__;
  m_UpdatedDateFor10DaysRS2 = __CHINA_MARKET_BEGIN_DATE__;
  m_UpdatedDateFor10DaysRS = __CHINA_MARKET_BEGIN_DATE__;
  m_nFields = 6;
}
// 此连接字符串中可能包含明文密码和/或其他重要
// 信息。请在查看完此连接字符串并找到所有与安全
// 有关的问题后移除 #error。可能需要将此密码存
// 储为其他格式或使用其他的用户身份验证。
CString CSetOption::GetDefaultConnect() {
  return GetChinaMarketSchemaConnect();
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
  RFX_Long(pFX, _T("[LastLoginDate]"), m_LastLoginDate);
  RFX_Long(pFX, _T("[UpdatedDateFor10DaysRS1]"), m_UpdatedDateFor10DaysRS1);
  RFX_Long(pFX, _T("[UpdatedDateFor10DaysRS2]"), m_UpdatedDateFor10DaysRS2);
  RFX_Long(pFX, _T("[UpdatedDateFor10DaysRS]"), m_UpdatedDateFor10DaysRS);
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