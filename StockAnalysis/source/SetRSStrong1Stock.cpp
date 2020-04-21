#include "stdafx.h"
#include"globedef.h"
#include"Accessory.h"

#include "SetRSStrong1Stock.h"

IMPLEMENT_DYNAMIC(CSetRSStrong1Stock, CRecordset)

CSetRSStrong1Stock::CSetRSStrong1Stock(CDatabase* pdb) : CRecordset(pdb) {
  m_Market = 0;
  m_StockCode = "";
  m_nFields = 2;
}

CString CSetRSStrong1Stock::GetDefaultConnect() {
  return GetDefaultSchemaConnect();
}

CString CSetRSStrong1Stock::GetDefaultSQL() {
  return _T("[10rs1above55]");
}

void CSetRSStrong1Stock::DoFieldExchange(CFieldExchange* pFX) {
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
void CSetRSStrong1Stock::AssertValid() const {
  CRecordset::AssertValid();
}

void CSetRSStrong1Stock::Dump(CDumpContext& dc) const {
  CRecordset::Dump(dc);
}
#endif //_DEBUG