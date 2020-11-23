#include"stdafx.h"

#include "SetSectionIndex.h"
#include"accessory.h"

// SetStakeCode.h : CSetStakeCode ���ʵ��

// CSetStakeCode ʵ��

// ���������� 2019��5��26��, 8:14

#include "stdafx.h"

IMPLEMENT_DYNAMIC(CSetSectionIndex, CRecordset)

CSetSectionIndex::CSetSectionIndex(CDatabase* pdb)
  : CRecordset(pdb) {
  m_Active = false;
  m_Market = 0;
  m_IndexNumber = 0;
  m_Comment = _T("");
  m_nFields = 5;
}

CString CSetSectionIndex::GetDefaultConnect() {
  return GetDefaultSchemaConnect();
}

CString CSetSectionIndex::GetDefaultSQL() {
  return _T("[StakeCodeSection]");
}

void CSetSectionIndex::DoFieldExchange(CFieldExchange* pFX) {
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
// CSetStakeCode ���

#ifdef _DEBUG
void CSetSectionIndex::AssertValid() const {
  CRecordset::AssertValid();
}

void CSetSectionIndex::Dump(CDumpContext& dc) const {
  CRecordset::Dump(dc);
}
#endif //_DEBUG