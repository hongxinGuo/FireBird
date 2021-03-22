// CSetEconomicCalendar.cpp : CSetEconomicCalendar ���ʵ��

// CSetWorldStockDayLine ʵ��

// ���������� 2019��6��2��, 5:10

#include"pch.h"
#include"afxdb.h"
#include"globedef.h"
#include"accessory.h"

#include "SetEconomicCalendar.h"

IMPLEMENT_DYNAMIC(CSetEconomicCalendar, CRecordset)

CSetEconomicCalendar::CSetEconomicCalendar(CDatabase* pdb)
  : CRecordset(pdb) {
  m_ID = 0;
  m_Country = _T(" ");
  m_Event = _T(" ");
  m_Impact = _T(" ");
  m_Actual = _T(" ");
  m_Estimate = _T(" ");
  m_Prev = _T(" ");
  m_Unit = _T(" ");
  m_nFields = 9;
}

CString CSetEconomicCalendar::GetDefaultConnect() {
  return GetWorldMarketSchemaConnect();
}

CString CSetEconomicCalendar::GetDefaultSQL() {
  return _T("[economic_calendar]");
}

void CSetEconomicCalendar::DoFieldExchange(CFieldExchange* pFX) {
  pFX->SetFieldType(CFieldExchange::outputColumn);
  // RFX_Text() �� RFX_Int() �������������
  // ��Ա���������ͣ����������ݿ��ֶε����͡�
  // ODBC �����Զ�����ֵת��Ϊ�����������
  RFX_Long(pFX, _T("[ID]"), m_ID);
  RFX_Text(pFX, _T("[Time]"), m_Time);
  RFX_Text(pFX, _T("Country"), m_Country);
  RFX_Text(pFX, _T("Event"), m_Event);
  RFX_Text(pFX, _T("Impact"), m_Impact);
  RFX_Text(pFX, _T("Actual"), m_Actual);
  RFX_Text(pFX, _T("Estimate"), m_Estimate);
  RFX_Text(pFX, _T("Prev"), m_Prev);
  RFX_Text(pFX, _T("Unit"), m_Unit);
}
/////////////////////////////////////////////////////////////////////////////
// CSetEconomicCalendar ���

#ifdef _DEBUG
void CSetEconomicCalendar::AssertValid() const {
  CRecordset::AssertValid();
}

void CSetEconomicCalendar::Dump(CDumpContext& dc) const {
  CRecordset::Dump(dc);
}
#endif //_DEBUG