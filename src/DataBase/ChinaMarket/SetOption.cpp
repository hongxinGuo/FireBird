#include"pch.h"

#include "SetOption.h"

CSetOption::CSetOption(const CString& strSchema, const CString& strTable, CDatabase* pdb)
	: CVirtualRecordset(strSchema, strTable, pdb) {
	m_nFields = 7;
}

void CSetOption::DoFieldExchange(CFieldExchange* pFX) {
	pFX->SetFieldType(CFieldExchange::outputColumn);
	// RFX_Text() �� RFX_Int() �������������
	// ��Ա���������ͣ����������ݿ��ֶε����͡�
	// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[RelativeStrongStartDate]"), m_RSStartDate);
	RFX_Long(pFX, _T("[RelativeStrongEndDate]"), m_RSEndDate);
	RFX_Long(pFX, _T("[LastLoginDate]"), m_LastLoginDate);
	RFX_Long(pFX, _T("[LastLoginTime]"), m_LastLoginTime);
	RFX_Long(pFX, _T("[UpdatedDateFor10DaysRS1]"), m_UpdatedDateFor10DaysRS1);
	RFX_Long(pFX, _T("[UpdatedDateFor10DaysRS2]"), m_UpdatedDateFor10DaysRS2);
	RFX_Long(pFX, _T("[UpdatedDateFor10DaysRS]"), m_UpdatedDateFor10DaysRS);
}
