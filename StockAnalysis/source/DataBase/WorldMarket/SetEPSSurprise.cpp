// CSetEPSSurprise.cpp : CSetEPSSurprise ���ʵ��

// CSetWorldStockDayLine ʵ��

// ���������� 2019��6��2��, 5:10

#include"pch.h"
#include"afxdb.h"
#include"globedef.h"
#include"accessory.h"

#include "SetEPSSurprise.h"

IMPLEMENT_DYNAMIC(CSetEPSSurprise, CRecordset)

CSetEPSSurprise::CSetEPSSurprise(CDatabase* pdb)
  : CRecordset(pdb) {
  m_ID = 0;
  m_Symbol = _T(" ");
  m_Date = 0;
  m_Actual = 0;
  m_Estimate = 0;
  m_nFields = 5;
}

CString CSetEPSSurprise::GetDefaultConnect() {
  return GetWorldMarketSchemaConnect();
}

CString CSetEPSSurprise::GetDefaultSQL() {
  return _T("[eps_surprise]");
}

void CSetEPSSurprise::DoFieldExchange(CFieldExchange* pFX) {
  pFX->SetFieldType(CFieldExchange::outputColumn);
  // RFX_Text() �� RFX_Int() �������������
  // ��Ա���������ͣ����������ݿ��ֶε����͡�
  // ODBC �����Զ�����ֵת��Ϊ�����������
  RFX_Long(pFX, _T("[ID]"), m_ID);
  RFX_Text(pFX, _T("[Symbol]"), m_Symbol);
  RFX_Long(pFX, _T("[Date]"), m_Date);
  RFX_Double(pFX, _T("[Actual]"), m_Actual);
  RFX_Double(pFX, _T("[Estimate]"), m_Estimate);
}
/////////////////////////////////////////////////////////////////////////////
// CSetEPSSurprise ���

#ifdef _DEBUG
void CSetEPSSurprise::AssertValid() const {
  CRecordset::AssertValid();
}

void CSetEPSSurprise::Dump(CDumpContext& dc) const {
  CRecordset::Dump(dc);
}
#endif //_DEBUG