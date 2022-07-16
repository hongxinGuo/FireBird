// CSetInsiderSentiment.cpp : CSetInsiderSentiment ���ʵ��

// ���������� 2019��6��2��, 5:10

#include"pch.h"

#include "SetInsiderSentiment.h"

IMPLEMENT_DYNAMIC(CSetInsiderSentiment, CVirtualRecordset)

CSetInsiderSentiment::CSetInsiderSentiment(CString strSchema, CString strTable, CDatabase* pdb)
	: CVirtualRecordset(strSchema, strTable, pdb) {
	m_ID = 0;
	m_Symbol = _T(" ");
	m_Date = 19800101;
	m_Change = 0;
	m_mspr = 0.0;
	m_nFields = 5;
}

void CSetInsiderSentiment::DoFieldExchange(CFieldExchange* pFX) {
	pFX->SetFieldType(CFieldExchange::outputColumn);
	// RFX_Text() �� RFX_Int() �������������
	// ��Ա���������ͣ����������ݿ��ֶε����͡�
	// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[ID]"), m_ID);
	RFX_Text(pFX, _T("[Symbol]"), m_Symbol);
	RFX_Long(pFX, _T("[Date]"), m_Date);
	RFX_Long(pFX, _T("[Change]"), m_Change);
	RFX_Double(pFX, _T("[mspr]"), m_mspr);
}
/////////////////////////////////////////////////////////////////////////////
// CSetInsiderTransaction ���

#ifdef _DEBUG
void CSetInsiderSentiment::AssertValid() const {
	CVirtualRecordset::AssertValid();
}

void CSetInsiderSentiment::Dump(CDumpContext& dc) const {
	CVirtualRecordset::Dump(dc);
}
#endif //_DEBUG