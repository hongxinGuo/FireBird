// SetChoicedStock.h : CSetChoicedStock ���ʵ��



// CSetChoicedStock ʵ��

// ���������� 2019��5��26��, 8:15

#include "stdafx.h"
#include"globedef.h"

#include "SetChoicedStock.h"
IMPLEMENT_DYNAMIC(CSetChoicedStock, CRecordset)

CSetChoicedStock::CSetChoicedStock(CDatabase* pdb) : CRecordset(pdb)
{
	m_Market = 0;
	m_StockCode = "";
	m_StockName = "";
	m_nFields = 3;
}

CString CSetChoicedStock::GetDefaultConnect()
{
#ifdef __ANSI_CODESET__
  return _T("DSN=mysqlA;UID=guo;PASSWORD=guo1426hx;charset=utf8");
#else
  return _T("DSN=mysql;UID=guo;PASSWORD=guo1426hx;charset=utf8");
#endif
}

CString CSetChoicedStock::GetDefaultSQL()
{
	return _T("[ChoicedStocks]");
}

void CSetChoicedStock::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() �� RFX_Int() �������������
// ��Ա���������ͣ����������ݿ��ֶε����͡�
// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[Market]"), m_Market);
	RFX_Text(pFX, _T("[StockCode]"), m_StockCode);
	RFX_Text(pFX, _T("[StockName]"), m_StockName);

}
/////////////////////////////////////////////////////////////////////////////
// CSetChoicedStock ���

#ifdef _DEBUG
void CSetChoicedStock::AssertValid() const
{
	CRecordset::AssertValid();
}

void CSetChoicedStock::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


