// CSetForexExchange.cpp : CSetForexExchange ���ʵ��

// ���������� 2019��5��26��, 8:14

#include "stdafx.h"
#include"globedef.h"
#include"accessory.h"

#include "SetForexExchange.h"

IMPLEMENT_DYNAMIC(CSetForexExchange, CRecordset)

CSetForexExchange::CSetForexExchange(CDatabase* pdb)
  : CRecordset(pdb) {
  m_Exchange = _T(" ");

  m_nFields = 2;
}

CString CSetForexExchange::GetDefaultConnect() {
  return GetAmericaMarketSchemaConnect();
}

CString CSetForexExchange::GetDefaultSQL() {
  return _T("[forex_exchange]");
}

void CSetForexExchange::DoFieldExchange(CFieldExchange* pFX) {
  pFX->SetFieldType(CFieldExchange::outputColumn);
  // RFX_Text() �� RFX_Int() �������������
  // ��Ա���������ͣ����������ݿ��ֶε����͡�
  // ODBC �����Զ�����ֵת��Ϊ�����������
  RFX_Long(pFX, _T("[ID]"), m_ID);
  RFX_Text(pFX, _T("[Exchange]"), m_Exchange);
}
/////////////////////////////////////////////////////////////////////////////
// CSetAmericaStake ���

#ifdef _DEBUG
void CSetForexExchange::AssertValid() const {
  CRecordset::AssertValid();
}

void CSetForexExchange::Dump(CDumpContext& dc) const {
  CRecordset::Dump(dc);
}
#endif //_DEBUG