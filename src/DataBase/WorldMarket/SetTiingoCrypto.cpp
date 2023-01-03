// CSetTiingoCrypto.cpp : CSetTiingoCrypto ���ʵ��

#include"pch.h"

#include "SetTiingoCrypto.h"

IMPLEMENT_DYNAMIC(CSetTiingoCrypto, CVirtualRecordset)

CSetTiingoCrypto::CSetTiingoCrypto(CString strSchema, CString strTable, CDatabase* pdb)
	: CVirtualRecordset(strSchema, strTable, pdb) {
	m_Ticker = _T("");
	m_Name = _T("");
	m_BaseCurrency = _T("");
	m_QuoteCurrency = _T("");
	m_Description = _T("");

	m_nFields = 6;
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
}
/////////////////////////////////////////////////////////////////////////////
// CSetTiingoCrypto ���

#ifdef _DEBUG
void CSetTiingoCrypto::AssertValid() const {
	CVirtualRecordset::AssertValid();
}

void CSetTiingoCrypto::Dump(CDumpContext& dc) const {
	CVirtualRecordset::Dump(dc);
}
#endif //_DEBUG