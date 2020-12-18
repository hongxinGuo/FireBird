// SetAmericaStakeDayLine.cpp : CSetDayLineBasicInfo ���ʵ��

// CSetAmericaStakeDayLine ʵ��

// ���������� 2019��6��2��, 5:10

#include "stdafx.h"
#include"afxdb.h"
#include"globedef.h"
#include"accessory.h"

#include "SetAmericaStakeDayLine.h"
IMPLEMENT_DYNAMIC(CSetAmericaStakeDayLine, CRecordset)

CSetAmericaStakeDayLine::CSetAmericaStakeDayLine(CDatabase* pdb)
  : CRecordset(pdb) {
  m_ID = 0;
  m_Date = 0;
  m_Market = _T("");
  m_Symbol = _T("");
  m_Ticker = _T("");
  m_LastClose = _T("0");
  m_Open = _T("0");
  m_High = _T("0");
  m_Low = _T("0");
  m_Close = _T("0");
  m_Volume = _T("0");
  m_Amount = _T("0");
  m_UpAndDown = _T("0");
  m_UpDownRate = _T("0");
  m_ChangeHandRate = _T("0");
  m_TotalValue = _T("0");
  m_CurrentValue = _T("0");
  m_RS = _T("");	//��������г���ǿ��
  m_RSIndex = _T("");	//��������г���ǿ��
  m_nFields = 20;
}

CString CSetAmericaStakeDayLine::GetDefaultConnect() {
  return GetAmericaMarketSchemaConnect();
}

CString CSetAmericaStakeDayLine::GetDefaultSQL() {
  return _T("[dayline]");
}

void CSetAmericaStakeDayLine::DoFieldExchange(CFieldExchange* pFX) {
  pFX->SetFieldType(CFieldExchange::outputColumn);
  // RFX_Text() �� RFX_Int() �������������
  // ��Ա���������ͣ����������ݿ��ֶε����͡�
  // ODBC �����Զ�����ֵת��Ϊ�����������
  RFX_Long(pFX, _T("[ID]"), m_ID);
  RFX_Long(pFX, _T("[Date]"), m_Date);
  RFX_Text(pFX, _T("[Market]"), m_Market);
  RFX_Text(pFX, _T("[Symbol]"), m_Symbol);
  RFX_Text(pFX, _T("[Ticker]"), m_Ticker);
  RFX_Text(pFX, _T("[LastClose]"), m_LastClose);
  RFX_Text(pFX, _T("[Open]"), m_Open);
  RFX_Text(pFX, _T("[High]"), m_High);
  RFX_Text(pFX, _T("[Low]"), m_Low);
  RFX_Text(pFX, _T("[Close]"), m_Close);
  RFX_Text(pFX, _T("[Volume]"), m_Volume);
  RFX_Text(pFX, _T("[Amount]"), m_Amount);
  RFX_Text(pFX, _T("[UpAndDown]"), m_UpAndDown);
  RFX_Text(pFX, _T("[UpDownRate]"), m_UpDownRate);
  RFX_Text(pFX, _T("[ChangeHandRate]"), m_ChangeHandRate);
  RFX_Text(pFX, _T("[TotalValue]"), m_TotalValue);
  RFX_Text(pFX, _T("[CurrentValue]"), m_CurrentValue);
  RFX_Text(pFX, _T("[RelativeStrong]"), m_RS);	//��������г���ǿ��
  RFX_Text(pFX, _T("[RelativeStrongIndex]"), m_RSIndex);	//������г�ָ����ǿ��
  RFX_Text(pFX, _T("[RelativeStrongBackup]"), m_RSBackup);	//������г�ָ����ǿ��
}
/////////////////////////////////////////////////////////////////////////////
// CSetAmericaStakeDayLine ���

#ifdef _DEBUG
void CSetAmericaStakeDayLine::AssertValid() const {
  CRecordset::AssertValid();
}

void CSetAmericaStakeDayLine::Dump(CDumpContext& dc) const {
  CRecordset::Dump(dc);
}
#endif //_DEBUG