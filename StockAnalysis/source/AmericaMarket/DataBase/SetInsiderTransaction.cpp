// CSetInsiderTransaction.cpp : CSetInsiderTransaction ���ʵ��

// ���������� 2019��6��2��, 5:10

#include"pch.h"
#include"afxdb.h"
#include"globedef.h"
#include"accessory.h"

#include "SetInsiderTransaction.h"

IMPLEMENT_DYNAMIC(CSetInsiderTransaction, CRecordset)

CSetInsiderTransaction::CSetInsiderTransaction(CDatabase* pdb)
  : CRecordset(pdb) {
  m_ID = 0;
  m_Symbol = _T(" ");
  m_PersonName = _T(" ");
  m_Share = 0;
  m_Change = 0;
  m_FilingDate = 19700101;
  m_TransactionDate = 19700101;
  m_TransactionCode = _T(" ");
  m_TransactionPrice = 0.0;
  m_nFields = 9;
}

CString CSetInsiderTransaction::GetDefaultConnect() {
  return GetWorldMarketSchemaConnect();
}

CString CSetInsiderTransaction::GetDefaultSQL() {
  return _T("[insider_transaction]");
}

void CSetInsiderTransaction::DoFieldExchange(CFieldExchange* pFX) {
  pFX->SetFieldType(CFieldExchange::outputColumn);
  // RFX_Text() �� RFX_Int() �������������
  // ��Ա���������ͣ����������ݿ��ֶε����͡�
  // ODBC �����Զ�����ֵת��Ϊ�����������
  RFX_Long(pFX, _T("[ID]"), m_ID);
  RFX_Text(pFX, _T("[Symbol]"), m_Symbol);
  RFX_Text(pFX, _T("[PersonName]"), m_PersonName);
  RFX_Long(pFX, _T("[Share]"), m_Share);
  RFX_Long(pFX, _T("[Change]"), m_Change);
  RFX_Long(pFX, _T("[FilingDate]"), m_FilingDate);
  RFX_Long(pFX, _T("[TransactionDate]"), m_TransactionDate);
  RFX_Text(pFX, _T("[TransactionCode]"), m_TransactionCode);
  RFX_Double(pFX, _T("[TransactionPrice]"), m_TransactionPrice);
}
/////////////////////////////////////////////////////////////////////////////
// CSetInsiderTransaction ���

#ifdef _DEBUG
void CSetInsiderTransaction::AssertValid() const {
  CRecordset::AssertValid();
}

void CSetInsiderTransaction::Dump(CDumpContext& dc) const {
  CRecordset::Dump(dc);
}
#endif //_DEBUG