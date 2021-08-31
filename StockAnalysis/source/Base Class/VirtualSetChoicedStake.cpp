// VirtualSetChoicedStake.h : VirtualSetChoicedStake ���ʵ��

// VirtualSetChoicedStake ʵ��

#include"pch.h"
#include"globedef.h"
#include"Accessory.h"

#include "VirtualSetChoicedStake.h"

CString CVirtualSetChoicedStake::sm_TableName = _T("");

IMPLEMENT_DYNAMIC(CVirtualSetChoicedStake, CRecordset)

CVirtualSetChoicedStake::CVirtualSetChoicedStake(CDatabase* pdb) : CRecordset(pdb) {
	m_ID = 0;
	m_Symbol = _T("");
	m_nFields = 2;
}

CString CVirtualSetChoicedStake::GetDefaultConnect() {
	return GetWorldMarketSchemaConnect();
}

CString CVirtualSetChoicedStake::GetDefaultSQL() {
	return _T("[") + sm_TableName + _T("]");
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
	CRecordset::AssertValid();
}

void CVirtualSetChoicedStake::Dump(CDumpContext& dc) const {
	CRecordset::Dump(dc);
}
#endif //_DEBUG