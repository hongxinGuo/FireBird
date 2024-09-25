// CSetWorldFundamentalDefinition.cpp : CSetWorldFundamentalDefinition ���ʵ��

#include"pch.h"

#include "SetTiingoFundamentalDefinition.h"

CSetTiingoFundamentalDefinition::CSetTiingoFundamentalDefinition(const CString& strSchema, const CString& strTable, CDatabase* pdb)
	: CVirtualRecordset(strSchema, strTable, pdb) {
	m_ID = 0;
	m_dataCode = _T("");
	m_name = _T("");
	m_description = _T("");
	m_units = _T("");
	m_statementType = _T("");

	m_nFields = 6;
}

void CSetTiingoFundamentalDefinition::DoFieldExchange(CFieldExchange* pFX) {
	pFX->SetFieldType(CFieldExchange::outputColumn);
	// RFX_Text() �� RFX_Int() �������������
	// ��Ա���������ͣ����������ݿ��ֶε����͡�
	// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[ID]"), m_ID);
	RFX_Text(pFX, _T("dataCode"), m_dataCode);
	RFX_Text(pFX, _T("name"), m_name);
	RFX_Text(pFX, _T("description"), m_description, 1000); // Note Ĭ���ַ�����Ϊ255�������Ļ���Ҫ�趨
	RFX_Text(pFX, _T("statementType"), m_statementType);
	RFX_Text(pFX, _T("units"), m_units);
}
