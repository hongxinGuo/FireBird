#include"stdafx.h"

#include "SetStockSection.h"
#include"accessory.h"

// SetStockCode.h : CSetStockSection ���ʵ��

// CSetStockSection ʵ��

// ���������� 2019��5��26��, 8:14

#include "stdafx.h"

IMPLEMENT_DYNAMIC(CSetStockSection, CRecordset)

CSetStockSection::CSetStockSection(CDatabase* pdb)
  : CRecordset(pdb) {
  m_Active = false;
  m_Market = 0;
  m_IndexNumber = 0;
  m_Comment = _T("");
  m_nFields = 5;
}

CString CSetStockSection::GetDefaultConnect() {
  return GetChinaMarketSchemaConnect();
}

CString CSetStockSection::GetDefaultSQL() {
  return _T("[Stock_Code_Section]");
}

void CSetStockSection::DoFieldExchange(CFieldExchange* pFX) {
  pFX->SetFieldType(CFieldExchange::outputColumn);
  // RFX_Text() �� RFX_Int() �������������
  // ��Ա���������ͣ����������ݿ��ֶε����͡�
  // ODBC �����Զ�����ֵת��Ϊ�����������
  RFX_Long(pFX, _T("[ID]"), m_ID);
  RFX_Bool(pFX, _T("Active"), m_Active);
  RFX_Int(pFX, _T("Market"), m_Market);
  RFX_Long(pFX, _T("IndexNumber"), m_IndexNumber);
  RFX_Text(pFX, _T("Comment"), m_Comment);
}

/////////////////////////////////////////////////////////////////////////////
// CSetStockCode ���

#ifdef _DEBUG
void CSetStockSection::AssertValid() const {
  CRecordset::AssertValid();
}

void CSetStockSection::Dump(CDumpContext& dc) const {
  CRecordset::Dump(dc);
}
#endif //_DEBUG