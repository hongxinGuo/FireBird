// SetAmericaChoicedStock.h : CSetAmericaChoicedStock ���ʵ��

// CSetAmericaChoicedStock ʵ��

// ���������� 2019��5��26��, 8:15

#include "stdafx.h"
#include"globedef.h"
#include"Accessory.h"

#include "SetAmericaChoicedStock.h"
IMPLEMENT_DYNAMIC(CSetAmericaChoicedStock, CRecordset)

CSetAmericaChoicedStock::CSetAmericaChoicedStock(CDatabase* pdb) : CRecordset(pdb) {
  m_ID = 0;
  m_Symbol = _T("");
  m_nFields = 2;
}

CString CSetAmericaChoicedStock::GetDefaultConnect() {
  return GetAmericaMarketSchemaConnect();
}

CString CSetAmericaChoicedStock::GetDefaultSQL() {
  return _T("[choicedstock]");
}

void CSetAmericaChoicedStock::DoFieldExchange(CFieldExchange* pFX) {
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
void CSetAmericaChoicedStock::AssertValid() const {
  CRecordset::AssertValid();
}

void CSetAmericaChoicedStock::Dump(CDumpContext& dc) const {
  CRecordset::Dump(dc);
}
#endif //_DEBUG