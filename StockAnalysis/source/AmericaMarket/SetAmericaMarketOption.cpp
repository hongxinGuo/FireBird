// CSetAmericaMarketOption.cpp : CSetAmericaMarketOption ���ʵ��

// CSetAmericaMarketOption ʵ��

// ���������� 2019��6��2��, 5:10

#include "stdafx.h"
#include"afxdb.h"
#include"globedef.h"
#include"accessory.h"

#include "SetAmericaMarketOption.h"

IMPLEMENT_DYNAMIC(CSetAmericaMarketOption, CRecordset)

CSetAmericaMarketOption::CSetAmericaMarketOption(CDatabase* pdb)
  : CRecordset(pdb) {
  m_ID = 0;
  m_FinnhubToken = _T(" ");
  m_TiingoToken = _T(" ");
  m_QuandlToken = _T(" ");
  m_nFields = 4;
}

CString CSetAmericaMarketOption::GetDefaultConnect() {
  return GetAmericaMarketSchemaConnect();
}

CString CSetAmericaMarketOption::GetDefaultSQL() {
  return _T("[america_market_option]");
}

void CSetAmericaMarketOption::DoFieldExchange(CFieldExchange* pFX) {
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
// CSetAmericaForexDayLine ���

#ifdef _DEBUG
void CSetAmericaMarketOption::AssertValid() const {
  CRecordset::AssertValid();
}

void CSetAmericaMarketOption::Dump(CDumpContext& dc) const {
  CRecordset::Dump(dc);
}
#endif //_DEBUG