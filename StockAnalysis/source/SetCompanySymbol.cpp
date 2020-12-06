// SetCompanySymbol.cpp : CSetStockCode ���ʵ��

// CSetCompanySymbol ʵ��

// ���������� 2019��5��26��, 8:14

#include "stdafx.h"
#include "SetCompanySymbol.h"
#include"globedef.h"
#include"accessory.h"

IMPLEMENT_DYNAMIC(CSetCompanySymbol, CRecordset)

CSetCompanySymbol::CSetCompanySymbol(CDatabase* pdb)
  : CRecordset(pdb) {
  m_Description = _T("");
  m_DisplaySymbol = _T("");
  m_Symbol = _T("");
  m_Type = _T("");
  m_Currency = _T("");
  m_nFields = 6;
}

CString CSetCompanySymbol::GetDefaultConnect() {
  if (!gl_fNormalMode) {
    gl_fTestMode = true;
    return _T("DSN=mysqlTest;UID=Test;PASSWORD=test;charset=utf8mb4"); // mysqlTest��������TestData Schema
  }
  else {
    gl_fTestMode = false;
    return _T("DSN=AmericaMarket;UID=AmericaMarket;PASSWORD=hxguo;charset=utf8mb4"); // mysql��������AmericaStakeMarket Schema
  }
}

CString CSetCompanySymbol::GetDefaultSQL() {
  return _T("[stakesymbol]");
}

void CSetCompanySymbol::DoFieldExchange(CFieldExchange* pFX) {
  pFX->SetFieldType(CFieldExchange::outputColumn);
  // RFX_Text() �� RFX_Int() �������������
  // ��Ա���������ͣ����������ݿ��ֶε����͡�
  // ODBC �����Զ�����ֵת��Ϊ�����������
  RFX_Long(pFX, _T("[ID]"), m_ID);
  RFX_Text(pFX, _T("[Symbol]"), m_Symbol);
  RFX_Text(pFX, _T("[Description]"), m_Description);
  RFX_Text(pFX, _T("[DisplaySymbol]"), m_DisplaySymbol);
  RFX_Text(pFX, _T("[Type]"), m_Type);
  RFX_Text(pFX, _T("[Currency]"), m_Currency);
}
/////////////////////////////////////////////////////////////////////////////
// CSetStockCode ���

#ifdef _DEBUG
void CSetCompanySymbol::AssertValid() const {
  CRecordset::AssertValid();
}

void CSetCompanySymbol::Dump(CDumpContext& dc) const {
  CRecordset::Dump(dc);
}
#endif //_DEBUG