#include "stdafx.h"
#include"Accessory.h"

#include "SetPotenDailyBriefing.h"
#include"globedef.h"

IMPLEMENT_DYNAMIC(CSetPotenDailyBriefing, CRecordset)

CSetPotenDailyBriefing::CSetPotenDailyBriefing(CDatabase* pdb)
  : CRecordset(pdb) {
  m_ID = 0;
  m_Day = 0;
  m_TD3C = m_TD9 = m_TD20 = m_TD21 = _T("0");
  m_TC1 = m_TC2 = m_TC5 = m_TC14 = _T("0");
  m_VLCC_TC_1YEAR = m_SUEZMAX_TC_1YEAR = m_AFRAMAX_TC_1YEAR = m_LR2_TC_1YEAR = m_LR1_TC_1YEAR = m_MR_TC_1YEAR = m_HANDYMAX_TC_1YEAR = _T("0");
  m_nFields = 31;
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
  RFX_Text(pFX, _T("[TD3C]"), m_TD3C);
  RFX_Text(pFX, _T("[TD20]"), m_TD20);
  RFX_Text(pFX, _T("[TD9]"), m_TD9);
  RFX_Text(pFX, _T("[TD21]"), m_TD21);
  RFX_Text(pFX, _T("[TC1]"), m_TC1);
  RFX_Text(pFX, _T("[TC2]"), m_TC2);
  RFX_Text(pFX, _T("[TC5]"), m_TC5);
  RFX_Text(pFX, _T("[TC14]"), m_TC14);
  RFX_Text(pFX, _T("[VLCC_TC1YEAR]"), m_VLCC_TC_1YEAR);
  RFX_Text(pFX, _T("[SUEZMAX_TC1YEAR]"), m_SUEZMAX_TC_1YEAR);
  RFX_Text(pFX, _T("[AFRAMAX_TC1YEAR]"), m_AFRAMAX_TC_1YEAR);
  RFX_Text(pFX, _T("[LR2_TC1YEAR]"), m_LR2_TC_1YEAR);
  RFX_Text(pFX, _T("[LR1_TC1YEAR]"), m_LR1_TC_1YEAR);
  RFX_Text(pFX, _T("[MR_TC1YEAR]"), m_MR_TC_1YEAR);
  RFX_Text(pFX, _T("[HANDYMAX_TC1YEAR]"), m_HANDYMAX_TC_1YEAR);
  RFX_Text(pFX, _T("[VLCC_NEWBUILD]"), m_VLCC_NEW_BUILD);
  RFX_Text(pFX, _T("[SUEZMAX_NEWBUILD]"), m_SUEZMAX_NEW_BUILD);
  RFX_Text(pFX, _T("[AFRAMAX_NEWBUILD]"), m_AFRAMAX_NEW_BUILD);
  RFX_Text(pFX, _T("[LR2_NEWBUILD]"), m_LR2_NEW_BUILD);
  RFX_Text(pFX, _T("[LR1_NEWBUILD]"), m_LR1_NEW_BUILD);
  RFX_Text(pFX, _T("[MR_NEWBUILD]"), m_MR_NEW_BUILD);
  RFX_Text(pFX, _T("[HANDYMAX_NEWBUILD]"), m_HANDYMAX_NEW_BUILD);
  RFX_Text(pFX, _T("[VLCC_5YEAROLD]"), m_VLCC_5YEAR_OLD);
  RFX_Text(pFX, _T("[SUEZMAX_5YEAROLD]"), m_SUEZMAX_5YEAR_OLD);
  RFX_Text(pFX, _T("[AFRAMAX_5YEAROLD]"), m_AFRAMAX_5YEAR_OLD);
  RFX_Text(pFX, _T("[LR2_5YEAROLD]"), m_LR2_5YEAR_OLD);
  RFX_Text(pFX, _T("[LR1_5YEAROLD]"), m_LR1_5YEAR_OLD);
  RFX_Text(pFX, _T("[MR_5YEAROLD]"), m_MR_5YEAR_OLD);
  RFX_Text(pFX, _T("[HANDYMAX_5YEAROLD]"), m_HANDYMAX_5YEAR_OLD);
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