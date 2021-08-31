// CVirtualSetExchange.cpp : CVirtualSetExchange ���ʵ��

#include"pch.h"
#include"globedef.h"
#include"accessory.h"

#include "VirtualSetExchange.h"

CString CVirtualSetExchange::sm_TableName = _T("");

IMPLEMENT_DYNAMIC(CVirtualSetExchange, CRecordset)

CVirtualSetExchange::CVirtualSetExchange(CDatabase* pdb)
	: CRecordset(pdb) {
	m_Exchange = _T(" ");

	m_nFields = 2;
}

CString CVirtualSetExchange::GetDefaultConnect() {
	return GetWorldMarketSchemaConnect();
}

CString CVirtualSetExchange::GetDefaultSQL() {
	return sm_TableName;
}

void CVirtualSetExchange::DoFieldExchange(CFieldExchange* pFX) {
	pFX->SetFieldType(CFieldExchange::outputColumn);
	// RFX_Text() �� RFX_Int() �������������
	// ��Ա���������ͣ����������ݿ��ֶε����͡�
	// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[ID]"), m_ID);
	RFX_Text(pFX, _T("[Exchange]"), m_Exchange);
}
/////////////////////////////////////////////////////////////////////////////
// CSetWorldStock ���

#ifdef _DEBUG
void CVirtualSetExchange::AssertValid() const {
	CRecordset::AssertValid();
}

void CVirtualSetExchange::Dump(CDumpContext& dc) const {
	CRecordset::Dump(dc);
}
#endif //_DEBUG