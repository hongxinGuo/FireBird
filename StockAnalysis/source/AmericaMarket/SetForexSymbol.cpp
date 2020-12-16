// SetForexSymbol.cpp : CSetForexSymbol ���ʵ��

// ���������� 2019��5��26��, 8:14

#include "stdafx.h"
#include"globedef.h"
#include"accessory.h"

#include "SetForexSymbol.h"

IMPLEMENT_DYNAMIC(CSetForexSymbol, CRecordset)

CSetForexSymbol::CSetForexSymbol(CDatabase* pdb)
  : CRecordset(pdb) {
  m_Description = _T(" ");
  m_DisplaySymbol = _T(" ");
  m_Exchange = _T(" ");
  m_Symbol = _T(" ");

  m_nFields = 5;
}

CString CSetForexSymbol::GetDefaultConnect() {
  return GetAmericaMarketSchemaConnect();
}

CString CSetForexSymbol::GetDefaultSQL() {
  return _T("[forex_symbol]");
}

void CSetForexSymbol::DoFieldExchange(CFieldExchange* pFX) {
  pFX->SetFieldType(CFieldExchange::outputColumn);
  // RFX_Text() �� RFX_Int() �������������
  // ��Ա���������ͣ����������ݿ��ֶε����͡�
  // ODBC �����Զ�����ֵת��Ϊ�����������
  RFX_Long(pFX, _T("[ID]"), m_ID);
  RFX_Text(pFX, _T("[Description]"), m_Description);
  RFX_Text(pFX, _T("[DisplaySymbol]"), m_DisplaySymbol);
  RFX_Text(pFX, _T("[Exchange]"), m_Exchange);
  RFX_Text(pFX, _T("[Symbol]"), m_Symbol);
}
/////////////////////////////////////////////////////////////////////////////
// CSetAmericaStake ���

#ifdef _DEBUG
void CSetForexSymbol::AssertValid() const {
  CRecordset::AssertValid();
}

void CSetForexSymbol::Dump(CDumpContext& dc) const {
  CRecordset::Dump(dc);
}
#endif //_DEBUG