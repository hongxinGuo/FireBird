// CSetCountry.cpp : CSetCountry ���ʵ��

// ���������� 2019��6��2��, 5:10

#include"pch.h"

import FireBird.Set.FinnhubCountry;

CSetCountry::CSetCountry(const CString& strSchema, const CString& strTable, CDatabase* pdb)
	: CVirtualRecordset(strSchema, strTable, pdb) {
	m_nFields = 7;
}

void CSetCountry::DoFieldExchange(CFieldExchange* pFX) {
	pFX->SetFieldType(CFieldExchange::outputColumn);
	// RFX_Text() �� RFX_Int() �������������
	// ��Ա���������ͣ����������ݿ��ֶε����͡�
	// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[ID]"), m_ID);
	RFX_Text(pFX, _T("Code2"), m_Code2);
	RFX_Text(pFX, _T("Code3"), m_Code3);
	RFX_Text(pFX, _T("CodeNo"), m_CodeNo);
	RFX_Text(pFX, _T("Country"), m_Country);
	RFX_Text(pFX, _T("Currency"), m_Currency);
	RFX_Text(pFX, _T("CurrencyCode"), m_CurrencyCode);
}
