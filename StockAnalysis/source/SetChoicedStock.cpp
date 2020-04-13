// SetChoicedStock.h : CSetChoicedStock ���ʵ��

// CSetChoicedStock ʵ��

// ���������� 2019��5��26��, 8:15

#include "stdafx.h"
#include"globedef.h"
#include"Accessory.h"

#include "SetChoicedStock.h"
IMPLEMENT_DYNAMIC(CSetChoicedStock, CRecordset)

CSetChoicedStock::CSetChoicedStock(CDatabase* pdb) : CRecordset(pdb) {
  m_Market = 0;
  m_StockCode = "";
  m_nFields = 2;
}

CString CSetChoicedStock::GetDefaultConnect() {
  return GetDefaultSchemaConnect();
}

CString CSetChoicedStock::GetDefaultSQL() {
  return _T("[ChoicedStocks]");
}

void CSetChoicedStock::DoFieldExchange(CFieldExchange* pFX) {
  pFX->SetFieldType(CFieldExchange::outputColumn);
  // RFX_Text() �� RFX_Int() �������������
  // ��Ա���������ͣ����������ݿ��ֶε����͡�
  // ODBC �����Զ�����ֵת��Ϊ�����������
  RFX_Long(pFX, _T("[Market]"), m_Market);
  RFX_Text(pFX, _T("[StockCode]"), m_StockCode);
}
/////////////////////////////////////////////////////////////////////////////
// CSetChoicedStock ���

#ifdef _DEBUG
void CSetChoicedStock::AssertValid() const {
  CRecordset::AssertValid();
}

void CSetChoicedStock::Dump(CDumpContext& dc) const {
  CRecordset::Dump(dc);
}
#endif //_DEBUG