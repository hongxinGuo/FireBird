#include "stdafx.h"
#include"Accessory.h"

#include "SetPotenDailyBriefing.h"
#include"globedef.h"

IMPLEMENT_DYNAMIC(CSetPotenDailyBriefing, CRecordset)

CSetPotenDailyBriefing::CSetPotenDailyBriefing(CDatabase* pdb)
  : CRecordset(pdb) {
  m_ID = 0;
  m_Day = 0;
  m_VLCC_TD3C = m_AFRAMAX_TD9 = m_SUEZMAX_TD20 = m_PANAMAX_TD21 = _T("0");
  m_LR2_TC1 = m_MR_TC2 = m_LR1_TC5 = m_MR_TC14 = _T("0");
  m_VLCC_TC_1YEAR = m_SUEZMAX_TC_1YEAR = m_AFRAMAX_TC_1YEAR = m_LR2_TC_1YEAR = m_LR1_TC_1YEAR = m_MR_TC_1YEAR = m_HANDYMAX_TC_1YEAR = _T("0");
  m_HandyUSFlag_USG_USAC = _T("0");
  m_LNG_138M3EastMonthly = m_LNG_138M3EastWeekly = m_LNG_138M3WestMonthly = m_LNG_138M3WestWeekly = _T("0");
  m_LNG_160M3EastMonthly = m_LNG_160M3EastWeekly = m_LNG_160M3WestMonthly = m_LNG_160M3WestWeekly = _T("0");
  m_LPG_VLGC44LastSpotRate = m_LPG_VLGC44SpotTCERate = _T("0");
  m_Bunker_Rotterdam_IFO380 = m_Bunker_Rotterdam_MDO = m_Bunker_Singapore_IFO380 = m_Bunker_Singapore_MDO = m_Bunker_USGolf_IFO380 = m_Bunker_USGolf_MDO = _T("0");
  m_nFields = 48;
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
  RFX_Text(pFX, _T("[VLCC_TD3C]"), m_VLCC_TD3C);
  RFX_Text(pFX, _T("[SUEZMAX_TD20]"), m_SUEZMAX_TD20);
  RFX_Text(pFX, _T("[AFRAMAX_TD9]"), m_AFRAMAX_TD9);
  RFX_Text(pFX, _T("[PANAMAX_TD21]"), m_PANAMAX_TD21);
  RFX_Text(pFX, _T("[LR2_TC1]"), m_LR2_TC1);
  RFX_Text(pFX, _T("[LR1_TC5]"), m_LR1_TC5);
  RFX_Text(pFX, _T("[MR_TC2]"), m_MR_TC2);
  RFX_Text(pFX, _T("[MR_TC14]"), m_MR_TC14);

  RFX_Text(pFX, _T("[VLCC_TC1YEAR]"), m_VLCC_TC_1YEAR);
  RFX_Text(pFX, _T("[SUEZMAX_TC1YEAR]"), m_SUEZMAX_TC_1YEAR);
  RFX_Text(pFX, _T("[AFRAMAX_TC1YEAR]"), m_AFRAMAX_TC_1YEAR);
  RFX_Text(pFX, _T("[LR2_TC1YEAR]"), m_LR2_TC_1YEAR);
  RFX_Text(pFX, _T("[LR1_TC1YEAR]"), m_LR1_TC_1YEAR);
  RFX_Text(pFX, _T("[MR_TC1YEAR]"), m_MR_TC_1YEAR);
  RFX_Text(pFX, _T("[HANDYMAX_TC1YEAR]"), m_HANDYMAX_TC_1YEAR);
  RFX_Text(pFX, _T("[VLCC_NEWBUILD]"), m_VLCC_NewBuild);
  RFX_Text(pFX, _T("[SUEZMAX_NEWBUILD]"), m_SUEZMAX_NewBuild);
  RFX_Text(pFX, _T("[AFRAMAX_NEWBUILD]"), m_AFRAMAX_NewBuild);
  RFX_Text(pFX, _T("[LR2_NEWBUILD]"), m_LR2_NewBuild);
  RFX_Text(pFX, _T("[LR1_NEWBUILD]"), m_LR1_NewBuild);
  RFX_Text(pFX, _T("[MR_NEWBUILD]"), m_MR_NewBuild);
  RFX_Text(pFX, _T("[HANDYMAX_NEWBUILD]"), m_HANDYMAX_NewBuild);
  RFX_Text(pFX, _T("[VLCC_5YEAROLD]"), m_VLCC_5YearOld);
  RFX_Text(pFX, _T("[SUEZMAX_5YEAROLD]"), m_SUEZMAX_5YearOld);
  RFX_Text(pFX, _T("[AFRAMAX_5YEAROLD]"), m_AFRAMAX_5YearOld);
  RFX_Text(pFX, _T("[LR2_5YEAROLD]"), m_LR2_5YearOld);
  RFX_Text(pFX, _T("[LR1_5YEAROLD]"), m_LR1_5YearOld);
  RFX_Text(pFX, _T("[MR_5YEAROLD]"), m_MR_5YearOld);
  RFX_Text(pFX, _T("[HANDYMAX_5YEAROLD]"), m_HANDYMAX_5YearOld);

  RFX_Text(pFX, _T("[HANDYUSFLAG_USG_USAC]"), m_HandyUSFlag_USG_USAC);

  RFX_Text(pFX, _T("[LNG_160M3WESTWEEKLY]"), m_LNG_160M3WestWeekly);
  RFX_Text(pFX, _T("[LNG_160M3WESTMONTHLY]"), m_LNG_160M3WestMonthly);
  RFX_Text(pFX, _T("[LNG_138M3WESTWEEKLY]"), m_LNG_138M3WestWeekly);
  RFX_Text(pFX, _T("[LNG_138M3WESTMONTHLY]"), m_LNG_138M3WestMonthly);
  RFX_Text(pFX, _T("[LNG_160M3EASTWEEKLY]"), m_LNG_160M3EastWeekly);
  RFX_Text(pFX, _T("[LNG_160M3EASTMONTHLY]"), m_LNG_160M3EastMonthly);
  RFX_Text(pFX, _T("[LNG_138M3EASTWEEKLY]"), m_LNG_138M3EastWeekly);
  RFX_Text(pFX, _T("[LNG_138M3EASTMONTHLY]"), m_LNG_138M3EastMonthly);

  RFX_Text(pFX, _T("[LPG_VLGC44LASTSPOTRATE]"), m_LPG_VLGC44LastSpotRate);
  RFX_Text(pFX, _T("[LPG_VLGC44SPOTTCERATE]"), m_LPG_VLGC44SpotTCERate);

  RFX_Text(pFX, _T("[BUNKER_USGULF_IFO380]"), m_Bunker_USGolf_IFO380);
  RFX_Text(pFX, _T("[BUNKER_USGULF_MDO]"), m_Bunker_USGolf_MDO);
  RFX_Text(pFX, _T("[BUNKER_ROTTERDAM_IFO380]"), m_Bunker_Rotterdam_IFO380);
  RFX_Text(pFX, _T("[BUNKER_ROTTERDAM_MDO]"), m_Bunker_Rotterdam_MDO);
  RFX_Text(pFX, _T("[BUNKER_SINGAPORE_IFO380]"), m_Bunker_Singapore_IFO380);
  RFX_Text(pFX, _T("[BUNKER_SINGAPORE_MDO]"), m_Bunker_Singapore_MDO);
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