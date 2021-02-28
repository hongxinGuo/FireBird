// CSetWorldMarketOption.cpp : CSetWorldMarketOption ���ʵ��

// CSetWorldMarketOption ʵ��

// ���������� 2019��6��2��, 5:10

#include "stdafx.h"
#include"afxdb.h"
#include"globedef.h"
#include"accessory.h"

#include "SetWorldMarketOption.h"

IMPLEMENT_DYNAMIC(CSetWorldMarketOption, CRecordset)

CSetWorldMarketOption::CSetWorldMarketOption(CDatabase* pdb)
  : CRecordset(pdb) {
  m_ID = 0;
  m_FinnhubToken = _T(" ");
  m_TiingoToken = _T(" ");
  m_QuandlToken = _T(" ");
  m_nFields = 4;
}

CString CSetWorldMarketOption::GetDefaultConnect() {
  return GetWorldMarketSchemaConnect();
}

CString CSetWorldMarketOption::GetDefaultSQL() {
  return _T("[world_market_option]");
}

void CSetWorldMarketOption::DoFieldExchange(CFieldExchange* pFX) {
  pFX->SetFieldType(CFieldExchange::outputColumn);
  // RFX_Text() �� RFX_Int() �������������
  // ��Ա���������ͣ����������ݿ��ֶε����͡�
  // ODBC �����Զ�����ֵת��Ϊ�����������
  RFX_Long(pFX, _T("[ID]"), m_ID);
  RFX_Text(pFX, _T("FinnhubToken"), m_FinnhubToken);
  RFX_Text(pFX, _T("TiingoToken"), m_TiingoToken);
  RFX_Text(pFX, _T("QuandlToken"), m_QuandlToken);
}
/////////////////////////////////////////////////////////////////////////////
// CSetWorldForexDayLine ���

#ifdef _DEBUG
void CSetWorldMarketOption::AssertValid() const {
  CRecordset::AssertValid();
}

void CSetWorldMarketOption::Dump(CDumpContext& dc) const {
  CRecordset::Dump(dc);
}
#endif //_DEBUG