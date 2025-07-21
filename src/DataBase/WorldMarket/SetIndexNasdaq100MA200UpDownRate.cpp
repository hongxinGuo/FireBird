// CSetIndexNasdaq100.h : CSetIndexNasdaq100 ���ʵ��

#include"pch.h"

#include "SetIndexNasdaq100MA200UpDownRate.h"

CSetIndexNasdaq100MA200UpDownRate::CSetIndexNasdaq100MA200UpDownRate(const CString& strSchema, const CString& strTable, CDatabase* pDatabase)
	: CVirtualRecordset(strSchema, strTable, pDatabase) {
	m_nFields = 2;
}

void CSetIndexNasdaq100MA200UpDownRate::DoFieldExchange(CFieldExchange* pFX) {
	pFX->SetFieldType(CFieldExchange::outputColumn);
	// RFX_Text() �� RFX_Int() �������������
	// ��Ա���������ͣ����������ݿ��ֶε����͡�
	// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[Date]"), m_Date);
	RFX_Long(pFX, _T("[Rate]"), m_Rate);
}
