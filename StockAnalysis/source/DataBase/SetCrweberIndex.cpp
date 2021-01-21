#include "stdafx.h"
#include"accessory.h"

#include "SetCrweberIndex.h"
#include"globedef.h"

IMPLEMENT_DYNAMIC(CSetCrweberIndex, CRecordset)

CSetCrweberIndex::CSetCrweberIndex(CDatabase* pdb)
  : CRecordset(pdb) {
  m_ID = 0;
  m_Date = 0;
  m_TD1 = m_TD2 = m_TD3C = m_TD5 = m_TD6 = m_TD7 = m_TD8 = m_TD9 = m_TD12 = m_TD15
    = m_TD19 = m_TD20 = m_TD21 = m_VLCC_USGSPORE = m_SUEZMAX_CBSUSG = _T("0");
  m_TC1 = m_TC2 = m_TC3 = m_TC4 = m_TC5 = m_TC14 = m_CPP_USGCBS = _T("0");
  m_VLCC_TC_1YEAR = m_SUEZMAX_TC_1YEAR = m_AFRAMAX_TC_1YEAR = m_PANAMAX_TC_1YEAR = m_MR_TC_1YEAR = m_HANDY_TC_1YEAR = _T("0");
  m_VLCC_TC_3YEAR = m_SUEZMAX_TC_3YEAR = m_AFRAMAX_TC_3YEAR = m_PANAMAX_TC_3YEAR = m_MR_TC_3YEAR = m_HANDY_TC_3YEAR = _T("0");
  m_nFields = 36;
}

CString CSetCrweberIndex::GetDefaultConnect() {
  return GetChinaMarketSchemaConnect();
}

CString CSetCrweberIndex::GetDefaultSQL() {
  return _T("[crweberIndex]");
}

void CSetCrweberIndex::DoFieldExchange(CFieldExchange* pFX) {
  pFX->SetFieldType(CFieldExchange::outputColumn);
  // RFX_Text() 和 RFX_Int() 这类宏依赖的是
  // 成员变量的类型，而不是数据库字段的类型。
  // ODBC 尝试自动将列值转换为所请求的类型
  RFX_Long(pFX, _T("[ID]"), m_ID);
  RFX_Long(pFX, _T("[Date]"), m_Date);
  RFX_Text(pFX, _T("[VLCC_TD1]"), m_TD1);
  RFX_Text(pFX, _T("[VLCC_TD2]"), m_TD2);
  RFX_Text(pFX, _T("[VLCC_TD3C]"), m_TD3C);
  RFX_Text(pFX, _T("[SUEZMAX_TD5]"), m_TD5);
  RFX_Text(pFX, _T("[SUEZMAX_TD6]"), m_TD6);
  RFX_Text(pFX, _T("[AFRAMAX_TD7]"), m_TD7);
  RFX_Text(pFX, _T("[AFRAMAX_TD8]"), m_TD8);
  RFX_Text(pFX, _T("[AFRAMAX_TD9]"), m_TD9);
  RFX_Text(pFX, _T("[PANAMAX_TD12]"), m_TD12);
  RFX_Text(pFX, _T("[VLCC_TD15]"), m_TD15);
  RFX_Text(pFX, _T("[AFRAMAX_TD19]"), m_TD19);
  RFX_Text(pFX, _T("[SUEZMAX_TD20]"), m_TD20);
  RFX_Text(pFX, _T("[PANAMAX_TD21]"), m_TD21);
  RFX_Text(pFX, _T("[VLCC_USGSPORE]"), m_VLCC_USGSPORE);
  RFX_Text(pFX, _T("[SUEZMAX_CBSUSG]"), m_SUEZMAX_CBSUSG);
  RFX_Text(pFX, _T("[CPP_TC1]"), m_TC1);
  RFX_Text(pFX, _T("[CPP_TC2]"), m_TC2);
  RFX_Text(pFX, _T("[CPP_TC3]"), m_TC3);
  RFX_Text(pFX, _T("[CPP_TC4]"), m_TC4);
  RFX_Text(pFX, _T("[CPP_TC5]"), m_TC5);
  RFX_Text(pFX, _T("[CPP_TC14]"), m_TC14);
  RFX_Text(pFX, _T("[CPP_USGCBS]"), m_CPP_USGCBS);
  RFX_Text(pFX, _T("[VLCC_TC_1YEAR]"), m_VLCC_TC_1YEAR);
  RFX_Text(pFX, _T("[SUEZMAX_TC_1YEAR]"), m_SUEZMAX_TC_1YEAR);
  RFX_Text(pFX, _T("[AFRAMAX_TC_1YEAR]"), m_AFRAMAX_TC_1YEAR);
  RFX_Text(pFX, _T("[PANAMAX_TC_1YEAR]"), m_PANAMAX_TC_1YEAR);
  RFX_Text(pFX, _T("[MR_TC_1YEAR]"), m_MR_TC_1YEAR);
  RFX_Text(pFX, _T("[HANDY_TC_1YEAR]"), m_HANDY_TC_1YEAR);
  RFX_Text(pFX, _T("[VLCC_TC_3YEAR]"), m_VLCC_TC_3YEAR);
  RFX_Text(pFX, _T("[SUEZMAX_TC_3YEAR]"), m_SUEZMAX_TC_3YEAR);
  RFX_Text(pFX, _T("[AFRAMAX_TC_3YEAR]"), m_AFRAMAX_TC_3YEAR);
  RFX_Text(pFX, _T("[PANAMAX_TC_3YEAR]"), m_PANAMAX_TC_3YEAR);
  RFX_Text(pFX, _T("[MR_TC_3YEAR]"), m_MR_TC_3YEAR);
  RFX_Text(pFX, _T("[HANDY_TC_3YEAR]"), m_HANDY_TC_3YEAR);
}
/////////////////////////////////////////////////////////////////////////////
// CSetStockCode 诊断

#ifdef _DEBUG
void CSetCrweberIndex::AssertValid() const {
  CRecordset::AssertValid();
}

void CSetCrweberIndex::Dump(CDumpContext& dc) const {
  CRecordset::Dump(dc);
}
#endif //_DEBUG