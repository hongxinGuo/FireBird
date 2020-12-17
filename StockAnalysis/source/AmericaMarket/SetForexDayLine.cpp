// CSetForexDayLine.cpp : CSetForexDayLine ���ʵ��

// CSetAmericaStakeDayLine ʵ��

// ���������� 2019��6��2��, 5:10

#include "stdafx.h"
#include"afxdb.h"
#include"globedef.h"
#include"accessory.h"

#include "SetForexDayLine.h"

IMPLEMENT_DYNAMIC(CSetForexDayLine, CRecordset)

CSetForexDayLine::CSetForexDayLine(CDatabase* pdb)
  : CRecordset(pdb) {
  m_ID = 0;
  m_Date = 0;
  m_Exchange = _T("");
  m_Symbol = _T("");
  m_LastClose = _T("0");
  m_Open = _T("0");
  m_High = _T("0");
  m_Low = _T("0");
  m_Close = _T("0");
  m_Volume = _T("0");
  m_nFields = 10;
}

CString CSetForexDayLine::GetDefaultConnect() {
  return GetAmericaMarketSchemaConnect();
}

CString CSetForexDayLine::GetDefaultSQL() {
  return _T("[forex_dayline]");
}

void CSetForexDayLine::DoFieldExchange(CFieldExchange* pFX) {
  pFX->SetFieldType(CFieldExchange::outputColumn);
  // RFX_Text() �� RFX_Int() �������������
  // ��Ա���������ͣ����������ݿ��ֶε����͡�
  // ODBC �����Զ�����ֵת��Ϊ�����������
  RFX_Long(pFX, _T("[ID]"), m_ID);
  RFX_Long(pFX, _T("[Date]"), m_Date);
  RFX_Text(pFX, _T("[Exchange]"), m_Exchange);
  RFX_Text(pFX, _T("[Symbol]"), m_Symbol);
  RFX_Text(pFX, _T("[LastClose]"), m_LastClose);
  RFX_Text(pFX, _T("[Open]"), m_Open);
  RFX_Text(pFX, _T("[High]"), m_High);
  RFX_Text(pFX, _T("[Low]"), m_Low);
  RFX_Text(pFX, _T("[Close]"), m_Close);
  RFX_Text(pFX, _T("[Volume]"), m_Volume);
}
/////////////////////////////////////////////////////////////////////////////
// CSetAmericaForexDayLine ���

#ifdef _DEBUG
void CSetForexDayLine::AssertValid() const {
  CRecordset::AssertValid();
}

void CSetForexDayLine::Dump(CDumpContext& dc) const {
  CRecordset::Dump(dc);
}
#endif //_DEBUG