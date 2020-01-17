#include "stdafx.h"
#include"accessory.h"

#include "SetPotenDailyBriefing.h"
#include"globedef.h"

IMPLEMENT_DYNAMIC(CSetPotenDailyBriefing, CRecordset)

CSetPotenDailyBriefing::CSetPotenDailyBriefing(CDatabase* pdb)
  : CRecordset(pdb) {
  m_ID = 0;
  m_Day = 0;
  m_TD3C = m_TD9 = m_TD20 = m_TD21 = _T("0");
  m_TC1 = m_TC2 = m_TC5 = m_TC14 = _T("0");
  m_VLCC_TC_1YEAR = m_SUEZMAX_TC_1YEAR = m_AFRAMAX_TC_1YEAR = m_PANAMAX_TC_1YEAR = m_MR_TC_1YEAR = m_HANDY_TC_1YEAR = _T("0");
  m_nFields = 30;
}

CString CSetPotenDailyBriefing::GetDefaultConnect() {
  return GetDefaultSchemaConnect();
}

CString CSetPotenDailyBriefing::GetDefaultSQL() {
  return _T("[potendailybriefing]");
}

void CSetPotenDailyBriefing::DoFieldExchange(CFieldExchange* pFX) {
  pFX->SetFieldType(CFieldExchange::outputColumn);
  // RFX_Text() 和 RFX_Int() 这类宏依赖的是
  // 成员变量的类型，而不是数据库字段的类型。
  // ODBC 尝试自动将列值转换为所请求的类型
  RFX_Long(pFX, _T("[ID]"), m_ID);
  RFX_Long(pFX, _T("[Day]"), m_Day);
  RFX_Text(pFX, _T("[VLCC_TD3C]"), m_TD3C);
  RFX_Text(pFX, _T("[SUEZMAX_TD20]"), m_TD20);
  RFX_Text(pFX, _T("[AFRAMAX_TD9]"), m_TD9);
  RFX_Text(pFX, _T("[PANAMAX_TD21]"), m_TD21);
  RFX_Text(pFX, _T("[CPP_TC1]"), m_TC1);
  RFX_Text(pFX, _T("[CPP_TC2]"), m_TC2);
  RFX_Text(pFX, _T("[CPP_TC5]"), m_TC5);
  RFX_Text(pFX, _T("[CPP_TC14]"), m_TC14);
  RFX_Text(pFX, _T("[VLCC_TC_1YEAR]"), m_VLCC_TC_1YEAR);
  RFX_Text(pFX, _T("[SUEZMAX_TC_1YEAR]"), m_SUEZMAX_TC_1YEAR);
  RFX_Text(pFX, _T("[AFRAMAX_TC_1YEAR]"), m_AFRAMAX_TC_1YEAR);
  RFX_Text(pFX, _T("[PANAMAX_TC_1YEAR]"), m_PANAMAX_TC_1YEAR);
  RFX_Text(pFX, _T("[MR_TC_1YEAR]"), m_MR_TC_1YEAR);
  RFX_Text(pFX, _T("[HANDY_TC_1YEAR]"), m_HANDY_TC_1YEAR);
  RFX_Text(pFX, _T("[VLCC_NEWBUILD]"), m_VLCC_NEW_BUILD);
  RFX_Text(pFX, _T("[SUEZMAX_NEWBUILD]"), m_SUEZMAX_NEW_BUILD);
  RFX_Text(pFX, _T("[AFRXMAX_NEWBUILD]"), m_AFRAMAX_NEW_BUILD);
  RFX_Text(pFX, _T("[LR2_NEWBUILD]"), m_LR2_NEW_BUILD);
  RFX_Text(pFX, _T("[LR1_NEWBUILD]"), m_LR1_NEW_BUILD);
  RFX_Text(pFX, _T("[MR_NEWBUILD]"), m_MR_NEW_BUILD);
  RFX_Text(pFX, _T("[HANDYMAX_NEWBUILD]"), m_HANDYMAX_NEW_BUILD);
  RFX_Text(pFX, _T("[VLCC_5YEARSOLD]"), m_VLCC_5YEARS_OLD);
  RFX_Text(pFX, _T("[SUEZMAX_5YEARSOLD]"), m_SUEZMAX_5YEARS_OLD);
  RFX_Text(pFX, _T("[AFRXMAX_5YEARSOLD]"), m_AFRAMAX_5YEARS_OLD);
  RFX_Text(pFX, _T("[LR2_5YEARSOLD]"), m_LR2_5YEARS_OLD);
  RFX_Text(pFX, _T("[LR1_5YEARSOLD]"), m_LR1_5YEARS_OLD);
  RFX_Text(pFX, _T("[MR_5YEARSOLD]"), m_MR_5YEARS_OLD);
  RFX_Text(pFX, _T("[HANDYMAX_5YEARSOLD]"), m_HANDYMAX_5YEARS_OLD);
}
/////////////////////////////////////////////////////////////////////////////
// CSetStockCode 诊断

#ifdef _DEBUG
void CSetPotenDailyBriefing::AssertValid() const {
  CRecordset::AssertValid();
}

void CSetPotenDailyBriefing::Dump(CDumpContext& dc) const {
  CRecordset::Dump(dc);
}
#endif //_DEBUG