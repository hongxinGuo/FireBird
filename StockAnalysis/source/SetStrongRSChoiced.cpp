#include "stdafx.h"
#include"globedef.h"
#include"Accessory.h"

#include "SetStrongRSChoiced.h"

IMPLEMENT_DYNAMIC(CSetStrongRSChoiced, CRecordset)

CSetStrongRSChoiced::CSetStrongRSChoiced(CDatabase* pdb) : CRecordset(pdb) {
  m_Market = 0;
  m_StockCode = "";
  m_nFields = 2;
}

CString CSetStrongRSChoiced::GetDefaultConnect() {
  return GetDefaultSchemaConnect();
}

CString CSetStrongRSChoiced::GetDefaultSQL() {
  return _T("[10rsabove55]");
}

void CSetStrongRSChoiced::DoFieldExchange(CFieldExchange* pFX) {
  pFX->SetFieldType(CFieldExchange::outputColumn);
  // RFX_Text() �� RFX_Int() �������������
  // ��Ա���������ͣ����������ݿ��ֶε����͡�
  // ODBC �����Զ�����ֵת��Ϊ�����������
  RFX_Long(pFX, _T("[Market]"), m_Market);
  RFX_Text(pFX, _T("[StockCode]"), m_StockCode);
}
/////////////////////////////////////////////////////////////////////////////
// CSetStrongRSChoiced ���

#ifdef _DEBUG
void CSetStrongRSChoiced::AssertValid() const {
  CRecordset::AssertValid();
}

void CSetStrongRSChoiced::Dump(CDumpContext& dc) const {
  CRecordset::Dump(dc);
}
#endif //_DEBUG