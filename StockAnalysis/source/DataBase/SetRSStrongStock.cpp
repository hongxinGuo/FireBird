#include "stdafx.h"
#include"globedef.h"
#include"Accessory.h"

#include "SetRSStrongStock.h"

IMPLEMENT_DYNAMIC(CSetRSStrongStock, CRecordset)

CSetRSStrongStock::CSetRSStrongStock(long lIndex, CDatabase* pdb) : CRecordset(pdb) {
  ASSERT((m_lIndex >= 0) && (m_lIndex < 10));
  m_lIndex = lIndex;
  m_Market = 0;
  m_StockCode = _T("");
  m_nFields = 2;
}

CString CSetRSStrongStock::GetDefaultConnect() {
  return GetDefaultSchemaConnect();
}

CString CSetRSStrongStock::GetDefaultSQL() {
  CString str = _T("[selected_rs_");
  char buffer[10];

  ASSERT((m_lIndex >= 0) && (m_lIndex < 10));
  sprintf_s(buffer, _T("%1d"), m_lIndex);
  str += buffer;
  str += _T("]");

  return str;
}

void CSetRSStrongStock::DoFieldExchange(CFieldExchange* pFX) {
  pFX->SetFieldType(CFieldExchange::outputColumn);
  // RFX_Text() �� RFX_Int() �������������
  // ��Ա���������ͣ����������ݿ��ֶε����͡�
  // ODBC �����Զ�����ֵת��Ϊ�����������
  RFX_Long(pFX, _T("[Market]"), m_Market);
  RFX_Text(pFX, _T("[StockCode]"), m_StockCode);
}
/////////////////////////////////////////////////////////////////////////////
// CSetRSStrong1Stock ���

#ifdef _DEBUG
void CSetRSStrongStock::AssertValid() const {
  CRecordset::AssertValid();
}

void CSetRSStrongStock::Dump(CDumpContext& dc) const {
  CRecordset::Dump(dc);
}
#endif //_DEBUG