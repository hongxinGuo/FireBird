// CSetEconomicCalendar.cpp : CSetEconomicCalendar ���ʵ��

// CSetWorldStockDayLine ʵ��

// ���������� 2019��6��2��, 5:10

#include"pch.h"

#include "SetEconomicCalendar.h"

CSetEconomicCalendar::CSetEconomicCalendar(const CString& strSchema, const CString& strTable, CDatabase* pdb)
	: CVirtualRecordset(strSchema, strTable, pdb) {
	m_ID = 0;
	m_Country = _T(" ");
	m_Event = _T(" ");
	m_Impact = _T(" ");
	m_Actual = _T(" ");
	m_Estimate = _T(" ");
	m_Prev = _T(" ");
	m_Unit = _T(" ");
	m_nFields = 9;
}

void CSetEconomicCalendar::DoFieldExchange(CFieldExchange* pFX) {
	pFX->SetFieldType(CFieldExchange::outputColumn);
	// RFX_Text() �� RFX_Int() �������������
	// ��Ա���������ͣ����������ݿ��ֶε����͡�
	// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[ID]"), m_ID);
	RFX_Text(pFX, _T("[Time]"), m_Time);
	RFX_Text(pFX, _T("Country"), m_Country);
	RFX_Text(pFX, _T("Event"), m_Event);
	RFX_Text(pFX, _T("Impact"), m_Impact);
	RFX_Text(pFX, _T("Actual"), m_Actual);
	RFX_Text(pFX, _T("Estimate"), m_Estimate);
	RFX_Text(pFX, _T("Prev"), m_Prev);
	RFX_Text(pFX, _T("Unit"), m_Unit);
}
