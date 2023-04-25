// CVirtualSetExchange.cpp : CVirtualSetExchange ���ʵ��

#include"pch.h"

#include "VirtualSetExchange.h"

CVirtualSetExchange::CVirtualSetExchange(const CString& strSchema, const CString& strTable, CDatabase* pdb)
	: CVirtualRecordset(strSchema, strTable, pdb) {
	m_ID = 0;
	m_Code = _T("");
	m_Name = _T("");
	m_Mic = _T("");
	m_TimeZone = _T("");
	m_Hour = _T("");
	m_CloseDate = _T("");
	m_Country = _T("");
	m_Source = _T("");
	m_nFields = 9;
}

void CVirtualSetExchange::DoFieldExchange(CFieldExchange* pFX) {
	pFX->SetFieldType(CFieldExchange::outputColumn);
	// RFX_Text() �� RFX_Int() �������������
	// ��Ա���������ͣ����������ݿ��ֶε����͡�
	// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[ID]"), m_ID);
	RFX_Text(pFX, _T("[Code]"), m_Code);
	RFX_Text(pFX, _T("[Name]"), m_Name);
	RFX_Text(pFX, _T("[Mic]"), m_Mic);
	RFX_Text(pFX, _T("[TimeZone]"), m_TimeZone);
	RFX_Text(pFX, _T("[Hour]"), m_Hour);
	RFX_Text(pFX, _T("[CloseDate]"), m_CloseDate);
	RFX_Text(pFX, _T("[Country]"), m_Country);
	RFX_Text(pFX, _T("[Source]"), m_Source);
}
