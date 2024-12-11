#include "pch.h"
module FireBird.Set.TiingoStock52WeekHigh;

CSetTiingoStock52WeekHigh::CSetTiingoStock52WeekHigh(const CString& strSchema, const CString& strTable, CDatabase* pDatabase)
	: CVirtualRecordset(strSchema, strTable, pDatabase) {
	m_nFields = 4;
}

void CSetTiingoStock52WeekHigh::DoFieldExchange(CFieldExchange* pFX) {
	pFX->SetFieldType(CFieldExchange::outputColumn);
	// RFX_Text() �� RFX_Int() �������������
	// ��Ա���������ͣ����������ݿ��ֶε����͡�
	// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[ID]"), m_ID);
	RFX_Text(pFX, _T("[Symbol]"), m_Symbol);
	RFX_Text(pFX, _T("[Exchange]"), m_Exchange);
	RFX_Long(pFX, _T("[Date]"), m_Date);
}
