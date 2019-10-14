// SetStockCode.h : CSetStockCode ���ʵ��

// CSetStockCode ʵ��

// ���������� 2019��5��26��, 8:14

#include "stdafx.h"
#include "SetStockCode.h"
#include"globedef.h"
#include"accessory.h"

IMPLEMENT_DYNAMIC(CSetStockCode, CRecordset)

CSetStockCode::CSetStockCode(CDatabase* pdb)
  : CRecordset(pdb)
{
  m_Counter = 0;
  m_StockType = 0;
  m_StockCode = "";
  m_StockName = "";
  m_DayLineStartDay = 19900101;
  m_DayLineEndDay = 19900101;
  m_IPOed = 0;
  m_nFields = 8;
}

CString CSetStockCode::GetDefaultConnect()
{
  return GetDefaultSchemaConnect();
}

CString CSetStockCode::GetDefaultSQL()
{
  return _T("[StockCode]");
}

void CSetStockCode::DoFieldExchange(CFieldExchange* pFX)
{
  pFX->SetFieldType(CFieldExchange::outputColumn);
  // RFX_Text() �� RFX_Int() �������������
  // ��Ա���������ͣ����������ݿ��ֶε����͡�
  // ODBC �����Զ�����ֵת��Ϊ�����������
  RFX_Long(pFX, _T("[ID]"), m_ID);
  RFX_Long(pFX, _T("[Counter]"), m_Counter);
  RFX_Int(pFX, _T("[StockType]"), m_StockType);
  RFX_Text(pFX, _T("[StockCode]"), m_StockCode);
  RFX_Text(pFX, _T("[StockName]"), m_StockName);
  RFX_Long(pFX, _T("[DayLineStartDay]"), m_DayLineStartDay);
  RFX_Long(pFX, _T("[DayLineEndDay]"), m_DayLineEndDay);
  RFX_Long(pFX, _T("[IPOed]"), m_IPOed);
}
/////////////////////////////////////////////////////////////////////////////
// CSetStockCode ���

#ifdef _DEBUG
void CSetStockCode::AssertValid() const
{
  CRecordset::AssertValid();
}

void CSetStockCode::Dump(CDumpContext& dc) const
{
  CRecordset::Dump(dc);
}
#endif //_DEBUG