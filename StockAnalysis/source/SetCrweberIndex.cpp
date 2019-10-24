#include "stdafx.h"
#include"accessory.h"

#include "SetCrweberIndex.h"
#include"globedef.h"

IMPLEMENT_DYNAMIC(CSetCrweberIndex, CRecordset)

CSetCrweberIndex::CSetCrweberIndex(CDatabase* pdb)
  : CRecordset(pdb)
{
  m_ID = 0;
  m_Day = 0;
  m_TD1 = m_TD2 = m_TD3C = m_TD5 = m_TD6 = m_TD7 = m_TD8 = m_TD9 = m_TD12 = m_TD15
    = m_TD19 = m_TD20 = m_TD21 = m_VLCC_USGSPORE = m_SUEZMAX_CBSUSG = 0;
  m_TC1 = m_TC2 = m_TC3 = m_TC4 = m_TC5 = m_TC14 = 0;
  m_nFields = 23;
}

CString CSetCrweberIndex::GetDefaultConnect()
{
  return GetDefaultSchemaConnect();
}

CString CSetCrweberIndex::GetDefaultSQL()
{
  return _T("[crweberIndex]");
}

void CSetCrweberIndex::DoFieldExchange(CFieldExchange* pFX)
{
  pFX->SetFieldType(CFieldExchange::outputColumn);
  // RFX_Text() 和 RFX_Int() 这类宏依赖的是
  // 成员变量的类型，而不是数据库字段的类型。
  // ODBC 尝试自动将列值转换为所请求的类型
  RFX_Long(pFX, _T("[ID]"), m_ID);
  RFX_Long(pFX, _T("[Day]"), m_Day);
  RFX_Long(pFX, _T("[VLCC_TD1]"), m_TD1);
  RFX_Long(pFX, _T("[VLCC_TD2]"), m_TD2);
  RFX_Long(pFX, _T("[VLCC_TD3C]"), m_TD3C);
  RFX_Long(pFX, _T("[VLCC_TD5]"), m_TD5);
  RFX_Long(pFX, _T("[VLCC_TD6]"), m_TD6);
  RFX_Long(pFX, _T("[VLCC_TD7]"), m_TD7);
  RFX_Long(pFX, _T("[VLCC_TD8]"), m_TD8);
  RFX_Long(pFX, _T("[VLCC_TD9]"), m_TD9);
  RFX_Long(pFX, _T("[VLCC_TD12]"), m_TD12);
  RFX_Long(pFX, _T("[VLCC_TD15]"), m_TD15);
  RFX_Long(pFX, _T("[VLCC_TD19]"), m_TD19);
  RFX_Long(pFX, _T("[VLCC_TD20]"), m_TD20);
  RFX_Long(pFX, _T("[VLCC_TD21]"), m_TD21);
  RFX_Long(pFX, _T("[VLCC_USGSPORT]"), m_VLCC_USGSPORE);
  RFX_Long(pFX, _T("[SUEZMAX_CBSUSG]"), m_SUEZMAX_CBSUSG);
  RFX_Long(pFX, _T("[CPP_TC1]"), m_TC1);
  RFX_Long(pFX, _T("[CPP_TC2]"), m_TC2);
  RFX_Long(pFX, _T("[CPP_TC3]"), m_TC3);
  RFX_Long(pFX, _T("[CPP_TC4]"), m_TC4);
  RFX_Long(pFX, _T("[CPP_TC5]"), m_TC5);
  RFX_Long(pFX, _T("[CPP_TC14]"), m_TC14);
}
/////////////////////////////////////////////////////////////////////////////
// CSetStockCode 诊断

#ifdef _DEBUG
void CSetCrweberIndex::AssertValid() const
{
  CRecordset::AssertValid();
}

void CSetCrweberIndex::Dump(CDumpContext& dc) const
{
  CRecordset::Dump(dc);
}
#endif //_DEBUG