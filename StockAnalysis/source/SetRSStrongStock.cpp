#include "stdafx.h"
#include"globedef.h"
#include"Accessory.h"

#include "SetRSStrongStock.h"

IMPLEMENT_DYNAMIC(CSetRSStrongStock, CRecordset)

CSetRSStrongStock::CSetRSStrongStock(CDatabase* pdb) : CRecordset(pdb) {
  m_Market = 0;
  m_StockCode = "";
  m_nFields = 2;
}

CString CSetRSStrongStock::GetDefaultConnect() {
  return GetDefaultSchemaConnect();
}

CString CSetRSStrongStock::GetDefaultSQL() {
  return gl_pChinaStockMarket->GetCurrentRSStrongSQL();
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