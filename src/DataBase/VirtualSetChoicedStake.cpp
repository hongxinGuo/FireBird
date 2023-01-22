// VirtualSetChosenStake.h : VirtualSetChosenStake ���ʵ��

#include"pch.h"

#include "VirtualSetChosenStake.h"

CVirtualSetChosenStake::CVirtualSetChosenStake(CString strSchema, CString strTable, CDatabase* pDatabase)
	: CVirtualRecordset(strSchema, strTable, pDatabase) {
	m_ID = 0;
	m_Symbol = _T("");
	m_nFields = 2;
}

void CVirtualSetChosenStake::DoFieldExchange(CFieldExchange* pFX) {
	pFX->SetFieldType(CFieldExchange::outputColumn);
	// RFX_Text() �� RFX_Int() �������������
	// ��Ա���������ͣ����������ݿ��ֶε����͡�
	// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[ID]"), m_ID);
	RFX_Text(pFX, _T("[Symbol]"), m_Symbol);
}
