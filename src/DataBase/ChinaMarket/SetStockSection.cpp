// SetSymbol.h : CSetStockSection ���ʵ��
#include"pch.h"

#include "SetStockSection.h"

CSetStockSection::CSetStockSection(const CString& strSchema, const CString& strTable, CDatabase* pdb)
	: CVirtualRecordset(strSchema, strTable, pdb) {
	m_ID = 0;
	m_Active = false;
	m_Market = 0;
	m_IndexNumber = 0;
	m_Comment = _T("");
	m_nFields = 5;
}

void CSetStockSection::DoFieldExchange(CFieldExchange* pFX) {
	pFX->SetFieldType(CFieldExchange::outputColumn);
	// RFX_Text() �� RFX_Int() �������������
	// ��Ա���������ͣ����������ݿ��ֶε����͡�
	// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[ID]"), m_ID);
	RFX_Bool(pFX, _T("Active"), m_Active);
	RFX_Int(pFX, _T("Market"), m_Market);
	RFX_Long(pFX, _T("IndexNumber"), m_IndexNumber);
	RFX_Text(pFX, _T("Comment"), m_Comment);
}
