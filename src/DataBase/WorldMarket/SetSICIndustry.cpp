// CSetSICIndustry.cpp : CSetSICIndustry ���ʵ��

// CSetSICIndustry ʵ��

// ���������� 2021��3��19��, 5:10

#include"pch.h"

#include "SetSICIndustry.h"

IMPLEMENT_DYNAMIC(CSetSICIndustry, CVirtualRecordset)

CSetSICIndustry::CSetSICIndustry(CString strSchema, CString strTable, CDatabase* pdb)
	: CVirtualRecordset(strSchema, strTable, pdb) {
	m_ID = 0;
	m_Code = 0;
	m_Industry = _T("");
	m_Sector = _T("");
	m_nFields = 4;
}

void CSetSICIndustry::DoFieldExchange(CFieldExchange* pFX) {
	pFX->SetFieldType(CFieldExchange::outputColumn);
	// RFX_Text() �� RFX_Int() �������������
	// ��Ա���������ͣ����������ݿ��ֶε����͡�
	// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[ID]"), m_ID);
	RFX_Long(pFX, _T("[Code]"), m_Code);
	RFX_Text(pFX, _T("[Industry]"), m_Industry);
	RFX_Text(pFX, _T("[Sector]"), m_Sector);
}
/////////////////////////////////////////////////////////////////////////////
// CSetSICIndustry ���

#ifdef _DEBUG
void CSetSICIndustry::AssertValid() const {
	CVirtualRecordset::AssertValid();
}

void CSetSICIndustry::Dump(CDumpContext& dc) const {
	CVirtualRecordset::Dump(dc);
}
#endif //_DEBUG