// CSetIndexNasdaq100.h : CSetIndexNasdaq100 ���ʵ��

#include"pch.h"

#include "SetIndexNasdaq100.h"

CSetIndexNasdaq100::CSetIndexNasdaq100(const CString& strSchema, const CString& strTable, CDatabase* pDatabase)
	: CVirtualRecordset(strSchema, strTable, pDatabase) {
	m_nFields = 3;
}

void CSetIndexNasdaq100::DoFieldExchange(CFieldExchange* pFX) {
	pFX->SetFieldType(CFieldExchange::outputColumn);
	// RFX_Text() �� RFX_Int() �������������
	// ��Ա���������ͣ����������ݿ��ֶε����͡�
	// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[ID]"), m_ID);
	RFX_Text(pFX, _T("[Symbol]"), m_Symbol);
	RFX_Long(pFX, _T("[ExpiredDate]"), m_ExpiredDate);
}
