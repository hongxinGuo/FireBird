#include "stdafx.h"
#include"globedef.h"
#include"Accessory.h"

#include "SetRSStrong2Stock.h"

IMPLEMENT_DYNAMIC(CSetRSStrong2Stock, CRecordset)

CSetRSStrong2Stock::CSetRSStrong2Stock(CDatabase* pdb) : CRecordset(pdb) {
  m_Market = 0;
  m_StockCode = "";
  m_nFields = 2;
}

CString CSetRSStrong2Stock::GetDefaultConnect() {
  return GetDefaultSchemaConnect();
}

CString CSetRSStrong2Stock::GetDefaultSQL() {
  return _T("[10rsabove55]");
}

void CSetRSStrong2Stock::DoFieldExchange(CFieldExchange* pFX) {
  pFX->SetFieldType(CFieldExchange::outputColumn);
  // RFX_Text() �� RFX_Int() �������������
  // ��Ա���������ͣ����������ݿ��ֶε����͡�
  // ODBC �����Զ�����ֵת��Ϊ�����������
  RFX_Long(pFX, _T("[Market]"), m_Market);
  RFX_Text(pFX, _T("[StockCode]"), m_StockCode);
}
/////////////////////////////////////////////////////////////////////////////
// CSetRSStrong2Stock ���

#ifdef _DEBUG
void CSetRSStrong2Stock::AssertValid() const {
  CRecordset::AssertValid();
}

void CSetRSStrong2Stock::Dump(CDumpContext& dc) const {
  CRecordset::Dump(dc);
}
#endif //_DEBUG