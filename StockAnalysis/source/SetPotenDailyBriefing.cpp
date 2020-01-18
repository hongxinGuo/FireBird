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
  // RFX_Text() �� RFX_Int() �������������
  // ��Ա���������ͣ����������ݿ��ֶε����͡�
  // ODBC �����Զ�����ֵת��Ϊ�����������
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
}
/////////////////////////////////////////////////////////////////////////////
// CSetStockCode ���

#ifdef _DEBUG
void CSetPotenDailyBriefing::AssertValid() const {
  CRecordset::AssertValid();
}

void CSetPotenDailyBriefing::Dump(CDumpContext& dc) const {
  CRecordset::Dump(dc);
}
#endif //_DEBUG