// CSetFinnhubForexExchange.cpp : CSetFinnhubForexExchange ���ʵ��

// ���������� 2019��5��26��, 8:14

#include "stdafx.h"
#include"globedef.h"
#include"accessory.h"

#include "SetFinnhubForexExchange.h"

IMPLEMENT_DYNAMIC(CSetFinnhubForexExchange, CRecordset)

CSetFinnhubForexExchange::CSetFinnhubForexExchange(CDatabase* pdb)
  : CRecordset(pdb) {
  m_Exchange = _T(" ");

  m_nFields = 2;
}

CString CSetFinnhubForexExchange::GetDefaultConnect() {
  return GetAmericaMarketSchemaConnect();
}

CString CSetFinnhubForexExchange::GetDefaultSQL() {
  return _T("[finnhub_forex_exchange]");
}

void CSetFinnhubForexExchange::DoFieldExchange(CFieldExchange* pFX) {
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
void CSetFinnhubForexExchange::AssertValid() const {
  CRecordset::AssertValid();
}

void CSetFinnhubForexExchange::Dump(CDumpContext& dc) const {
  CRecordset::Dump(dc);
}
#endif //_DEBUG