// VirtualSetChoicedStake.h : VirtualSetChoicedStake ���ʵ��

// VirtualSetChoicedStake ʵ��

#include"pch.h"

#include "VirtualSetChoicedStake.h"

IMPLEMENT_DYNAMIC(CVirtualSetChoicedStake, CVirtualRecordset)

CVirtualSetChoicedStake::CVirtualSetChoicedStake(CString strSchema, CString strTable, CDatabase* pdb)
	: CVirtualRecordset(strSchema, strTable, pdb) {
	m_ID = 0;
	m_Symbol = _T("");
	m_nFields = 2;
}

void CVirtualSetChoicedStake::DoFieldExchange(CFieldExchange* pFX) {
	pFX->SetFieldType(CFieldExchange::outputColumn);
	// RFX_Text() �� RFX_Int() �������������
	// ��Ա���������ͣ����������ݿ��ֶε����͡�
	// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[ID]"), m_ID);
	RFX_Text(pFX, _T("[Symbol]"), m_Symbol);
}
/////////////////////////////////////////////////////////////////////////////
// CVirtualSetChoicedStake ���

#ifdef _DEBUG
void CVirtualSetChoicedStake::AssertValid() const {
	CVirtualRecordset::AssertValid();
}

void CVirtualSetChoicedStake::Dump(CDumpContext& dc) const {
	CVirtualRecordset::Dump(dc);
}
#endif //_DEBUG