// CSetInsiderTransaction.cpp : CSetInsiderTransaction ���ʵ��

#include"pch.h"

#include "SetInsiderTransaction.h"

CSetInsiderTransaction::CSetInsiderTransaction(const CString& strSchema, const CString& strTable, CDatabase* pdb)
	: CVirtualRecordset(strSchema, strTable, pdb) {
	m_nFields = 9;
}

void CSetInsiderTransaction::DoFieldExchange(CFieldExchange* pFX) {
	pFX->SetFieldType(CFieldExchange::outputColumn);
	// RFX_Text() �� RFX_Int() �������������
	// ��Ա���������ͣ����������ݿ��ֶε����͡�
	// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[ID]"), m_ID);
	RFX_Text(pFX, _T("[Symbol]"), m_Symbol);
	RFX_Text(pFX, _T("[PersonName]"), m_PersonName);
	RFX_Text(pFX, _T("[Share]"), m_Share);
	RFX_Text(pFX, _T("[Change]"), m_Change);
	RFX_Long(pFX, _T("[FilingDate]"), m_FilingDate);
	RFX_Long(pFX, _T("[TransactionDate]"), m_TransactionDate);
	RFX_Text(pFX, _T("[TransactionCode]"), m_TransactionCode);
	RFX_Double(pFX, _T("[TransactionPrice]"), m_TransactionPrice);
}
