// SetFinnhubStockBasicFinancialAnnual.cpp : CSetCountry ���ʵ��

// ���������� 2019��6��2��, 5:10

#include"pch.h"
#include"globedef.h"

#include "SetFinnhubStockBasicFinancialAnnual.h"

IMPLEMENT_DYNAMIC(CSetFinnhubStockBasicFinancialAnnual, CVirtualRecordset)

CSetFinnhubStockBasicFinancialAnnual::CSetFinnhubStockBasicFinancialAnnual(CString strSchema, CString strTable, CDatabase* pdb)
	: CVirtualRecordset(strSchema, strTable, pdb) {
	m_ID = 0;
	m_symbol = _T("");
	m_type = _T("");
	m_date = 19800101;
	m_value = 0;

	m_nFields = 5;
}

void CSetFinnhubStockBasicFinancialAnnual::DoFieldExchange(CFieldExchange* pFX) {
	pFX->SetFieldType(CFieldExchange::outputColumn);
	// RFX_Text() �� RFX_Int() �������������
	// ��Ա���������ͣ����������ݿ��ֶε����͡�
	// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[ID]"), m_ID);
	RFX_Text(pFX, _T("Symbol"), m_symbol);
	RFX_Text(pFX, _T("Type"), m_type);
	RFX_Int(pFX, _T("Date"), m_date);
	RFX_Double(pFX, _T("Value"), m_value);
}
/////////////////////////////////////////////////////////////////////////////
// CSetCountry ���

#ifdef _DEBUG
void CSetFinnhubStockBasicFinancialAnnual::AssertValid() const {
	CVirtualRecordset::AssertValid();
}

void CSetFinnhubStockBasicFinancialAnnual::Dump(CDumpContext& dc) const {
	CVirtualRecordset::Dump(dc);
}
#endif //_DEBUG