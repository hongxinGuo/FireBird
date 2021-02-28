#include "stdafx.h"
#include"globedef.h"
#include"Accessory.h"

#include "SetRSStrong2Stock.h"

IMPLEMENT_DYNAMIC(CSetRSStrong2Stock, CRecordset)

CSetRSStrong2Stock::CSetRSStrong2Stock(CDatabase* pdb) : CRecordset(pdb) {
  m_Symbol = _T("");
  m_nFields = 1;
}

CString CSetRSStrong2Stock::GetDefaultConnect() {
  return GetChinaMarketSchemaConnect();
}

CString CSetRSStrong2Stock::GetDefaultSQL() {
  return _T("[10rs2above55]");
}

void CSetRSStrong2Stock::DoFieldExchange(CFieldExchange* pFX) {
  pFX->SetFieldType(CFieldExchange::outputColumn);
  // RFX_Text() �� RFX_Int() �������������
  // ��Ա���������ͣ����������ݿ��ֶε����͡�
  // ODBC �����Զ�����ֵת��Ϊ�����������
  RFX_Text(pFX, _T("[StockCode]"), m_Symbol);
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