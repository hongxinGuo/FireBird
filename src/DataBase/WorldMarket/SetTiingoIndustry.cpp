// CSetTiingoIndustry.cpp : CSetTiingoIndustry ���ʵ��

#include"pch.h"

#include "SetTiingoIndustry.h"

CSetTiingoIndustry::CSetTiingoIndustry(CString strSchema, CString strTable, CDatabase* pdb)
	: CVirtualRecordset(strSchema, strTable, pdb) {
	m_ID = 0;
	m_Industry = _T("");
	m_Sector = _T("");
	m_nFields = 3;
}

void CSetTiingoIndustry::DoFieldExchange(CFieldExchange* pFX) {
	pFX->SetFieldType(CFieldExchange::outputColumn);
	// RFX_Text() �� RFX_Int() �������������
	// ��Ա���������ͣ����������ݿ��ֶε����͡�
	// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[ID]"), m_ID);
	RFX_Text(pFX, _T("[Industry]"), m_Industry);
	RFX_Text(pFX, _T("[Sector]"), m_Sector);
}
