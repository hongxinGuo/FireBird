// SetChoicedStock.h : CSetChoicedStock ���ʵ��

// CSetChoicedStock ʵ��

// ���������� 2019��5��26��, 8:15

#include "stdafx.h"
#include"globedef.h"
#include"Accessory.h"

#include "SetChoicedStock.h"
IMPLEMENT_DYNAMIC(CSetChoicedStock, CRecordset)

CSetChoicedStock::CSetChoicedStock(CDatabase* pdb) : CRecordset(pdb) {
  m_Symbol = _T("");
  m_nFields = 1;
}

CString CSetChoicedStock::GetDefaultConnect() {
  return GetChinaMarketSchemaConnect();
}

CString CSetChoicedStock::GetDefaultSQL() {
  return _T("[ChoicedStocks]");
}

void CSetChoicedStock::DoFieldExchange(CFieldExchange* pFX) {
  pFX->SetFieldType(CFieldExchange::outputColumn);
  // RFX_Text() �� RFX_Int() �������������
  // ��Ա���������ͣ����������ݿ��ֶε����͡�
  // ODBC �����Զ�����ֵת��Ϊ�����������
  RFX_Text(pFX, _T("[StockCode]"), m_Symbol);
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