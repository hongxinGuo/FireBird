// SetWorldChoicedStock.h : CSetWorldChoicedStock ���ʵ��

// CSetWorldChoicedStock ʵ��

// ���������� 2019��5��26��, 8:15

#include"pch.h"
#include"globedef.h"
#include"Accessory.h"

#include "SetWorldChoicedStock.h"
IMPLEMENT_DYNAMIC(CSetWorldChoicedStock, CRecordset)

CSetWorldChoicedStock::CSetWorldChoicedStock(CDatabase* pdb) : CRecordset(pdb) {
  m_ID = 0;
  m_Symbol = _T("");
  m_nFields = 2;
}

CString CSetWorldChoicedStock::GetDefaultConnect() {
  return GetWorldMarketSchemaConnect();
}

CString CSetWorldChoicedStock::GetDefaultSQL() {
  return _T("[choicedstock]");
}

void CSetWorldChoicedStock::DoFieldExchange(CFieldExchange* pFX) {
  pFX->SetFieldType(CFieldExchange::outputColumn);
  // RFX_Text() �� RFX_Int() �������������
  // ��Ա���������ͣ����������ݿ��ֶε����͡�
  // ODBC �����Զ�����ֵת��Ϊ�����������
  RFX_Long(pFX, _T("[ID]"), m_ID);
  RFX_Text(pFX, _T("[Symbol]"), m_Symbol);
}
/////////////////////////////////////////////////////////////////////////////
// CSetChoicedStock ���

#ifdef _DEBUG
void CSetWorldChoicedStock::AssertValid() const {
  CRecordset::AssertValid();
}

void CSetWorldChoicedStock::Dump(CDumpContext& dc) const {
  CRecordset::Dump(dc);
}
#endif //_DEBUG