/////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"
#include "SetRSOption.h"

CSetRSOption::CSetRSOption(const CString& strSchema, const CString& strTable, CDatabase* pdb)
	: CVirtualRecordset(strSchema, strTable, pdb) {
	m_nFields = 15;
}

void CSetRSOption::DoFieldExchange(CFieldExchange* pFX) {
	pFX->SetFieldType(CFieldExchange::outputColumn);
	// RFX_Text() �� RFX_Int() �������������
	// ��Ա���������ͣ����������ݿ��ֶε����͡�
	// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[ID]"), m_ID);
	RFX_Long(pFX, _T("[Index]"), m_Index);
	RFX_Bool(pFX, _T("[Active]"), m_Active);
	RFX_Long(pFX, _T("[DayLengthFirst]"), m_DayLengthFirst);
	RFX_Long(pFX, _T("[DayLengthSecond]"), m_DayLengthSecond);
	RFX_Long(pFX, _T("[DayLengthThird]"), m_DayLengthThird);
	RFX_Long(pFX, _T("[DayLengthFourth]"), m_DayLengthFourth);
	RFX_Long(pFX, _T("[StrongDayLengthFirst]"), m_StrongDayLengthFirst);
	RFX_Long(pFX, _T("[StrongDayLengthSecond]"), m_StrongDayLengthSecond);
	RFX_Long(pFX, _T("[StrongDayLengthThird]"), m_StrongDayLengthThird);
	RFX_Long(pFX, _T("[StrongDayLengthFourth]"), m_StrongDayLengthFourth);
	RFX_Text(pFX, _T("[RSStrongFirst]"), m_RSStrongFirst);
	RFX_Text(pFX, _T("[RSStrongSecond]"), m_RSStrongSecond);
	RFX_Text(pFX, _T("[RSStrongThird]"), m_RSStrongThird);
	RFX_Text(pFX, _T("[RSStrongFourth]"), m_RSStrongFourth);
}
