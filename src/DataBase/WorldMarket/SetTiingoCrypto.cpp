// CSetTiingoCrypto.cpp : CSetTiingoCrypto ���ʵ��

#include"pch.h"

#include "SetTiingoCrypto.h"

CSetTiingoCrypto::CSetTiingoCrypto(const CString& strSchema, const CString& strTable, CDatabase* pdb)
	: CVirtualRecordset(strSchema, strTable, pdb) {
	m_nFields = 7;
}

void CSetTiingoCrypto::DoFieldExchange(CFieldExchange* pFX) {
	pFX->SetFieldType(CFieldExchange::outputColumn);
	// RFX_Text() �� RFX_Int() �������������
	// ��Ա���������ͣ����������ݿ��ֶε����͡�
	// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[ID]"), m_ID);
	RFX_Text(pFX, _T("[Ticker]"), m_Ticker);
	RFX_Text(pFX, _T("[Name]"), m_Name);
	RFX_Text(pFX, _T("[BaseCurrency]"), m_BaseCurrency);
	RFX_Text(pFX, _T("[QuoteCurrency]"), m_QuoteCurrency);
	RFX_Text(pFX, _T("[Description]"), m_Description);
	RFX_Text(pFX, _T("[UpdateDate]"), m_UpdateDate, 10000);
}
