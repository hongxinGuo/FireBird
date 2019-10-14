// SetDayLine.h : CSetDayLine ���ʵ��

// CSetDayLine ʵ��

// ���������� 2019��6��2��, 5:10

#include "stdafx.h"
#include"afxdb.h"
#include"globedef.h"
#include"accessory.h"

#include "SetDayLine.h"
IMPLEMENT_DYNAMIC(CSetDayLine, CRecordset)

CSetDayLine::CSetDayLine(CDatabase* pdb)
  : CRecordset(pdb)
{
  m_ID = 0;
  m_Time = 0;
  m_Market = 0;
  m_StockCode = "";
  m_StockName = "";
  m_LastClose = 0;
  m_Open = 0;
  m_High = 0;
  m_Low = 0;
  m_Close = 0;
  m_Volume = 0;
  m_Amount = 0;
  m_UpAndDown = 0;
  m_UpDownRate = 0;
  m_ChangeHandRate = 0;
  m_TotalValue = 0.0;
  m_CurrentValue = 0.0;
  m_RelativeStrong = 0;	//��������г���ǿ��
  m_nFields = 18;
}

CString CSetDayLine::GetDefaultConnect()
{
  return GetDefaultSchemaConnect();
}

CString CSetDayLine::GetDefaultSQL()
{
  return _T("[DayLine]");
}

void CSetDayLine::DoFieldExchange(CFieldExchange* pFX)
{
  pFX->SetFieldType(CFieldExchange::outputColumn);
  // RFX_Text() �� RFX_Int() �������������
  // ��Ա���������ͣ����������ݿ��ֶε����͡�
  // ODBC �����Զ�����ֵת��Ϊ�����������
  RFX_Long(pFX, _T("[ID]"), m_ID);
  RFX_Long(pFX, _T("[Time]"), m_Time);
  RFX_Long(pFX, _T("[Market]"), m_Market);
  RFX_Text(pFX, _T("[StockCode]"), m_StockCode);
  RFX_Text(pFX, _T("[StockName]"), m_StockName);
  RFX_Double(pFX, _T("[LastClose]"), m_LastClose);
  RFX_Double(pFX, _T("[Open]"), m_Open);
  RFX_Double(pFX, _T("[High]"), m_High);
  RFX_Double(pFX, _T("[Low]"), m_Low);
  RFX_Double(pFX, _T("[Close]"), m_Close);
  RFX_Double(pFX, _T("[Volume]"), m_Volume);
  RFX_Double(pFX, _T("[Amount]"), m_Amount);
  RFX_Double(pFX, _T("[UpAndDown]"), m_UpAndDown);
  RFX_Double(pFX, _T("[UpDownRate]"), m_UpDownRate);
  RFX_Double(pFX, _T("[ChangeHandRate]"), m_ChangeHandRate);
  RFX_Double(pFX, _T("[TotalValue]"), m_TotalValue);
  RFX_Double(pFX, _T("[CurrentValue]"), m_CurrentValue);
  RFX_Double(pFX, _T("[RelativeStrong]"), m_RelativeStrong);	//��������г���ǿ��
}
/////////////////////////////////////////////////////////////////////////////
// CSetDayLine ���

#ifdef _DEBUG
void CSetDayLine::AssertValid() const
{
  CRecordset::AssertValid();
}

void CSetDayLine::Dump(CDumpContext& dc) const
{
  CRecordset::Dump(dc);
}
#endif //_DEBUG