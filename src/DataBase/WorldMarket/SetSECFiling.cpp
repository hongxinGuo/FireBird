// CSetSECFilings.cpp : CSetSECFilings ���ʵ��

// ���������� 2019��6��2��, 5:10

#include"pch.h"

#include "SetSECFilings.h"

CSetSECFilings::CSetSECFilings(const CString& strSchema, const CString& strTable, CDatabase* pdb)
	: CVirtualRecordset(strSchema, strTable, pdb) {
	m_nFields = 9;
}

void CSetSECFilings::DoFieldExchange(CFieldExchange* pFX) {
	pFX->SetFieldType(CFieldExchange::outputColumn);
	// RFX_Text() �� RFX_Int() �������������
	// ��Ա���������ͣ����������ݿ��ֶε����͡�
	// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[ID]"), m_ID);
	RFX_Text(pFX, _T("[accessNumber]"), m_AccessNumber);
	RFX_Text(pFX, _T("symbol"), m_Symbol);
	RFX_Long(pFX, _T("cik"), m_CIK);
	RFX_Text(pFX, _T("form"), m_Form);
	RFX_Long(pFX, _T("[filedDate]"), m_FiledDate);
	RFX_Long(pFX, _T("[acceptedDate]"), m_AcceptedDate);
	RFX_Text(pFX, _T("reportURL"), m_ReportURL);
	RFX_Text(pFX, _T("filingURL"), m_FilingURL);
}
