#include"pch.h"

#include "SetRSStrongStock.h"

CSetRSStrongStock::CSetRSStrongStock(long lIndex, const CString& strSchema, const CString& strTable, CDatabase* pdb)
	: CVirtualRecordset(strSchema, strTable, pdb) {
	ASSERT((m_lIndex >= 0) && (m_lIndex < 10));
	m_lIndex = lIndex;
	m_Symbol = _T("");
	m_nFields = 1;
}

CString CSetRSStrongStock::GetDefaultSQL() {
	string s = _T("[");
	s += m_Table;

	ASSERT((m_lIndex >= 0) && (m_lIndex < 10));
	s += fmt::format("{:Ld}]", m_lIndex);

	return s.c_str();
}

void CSetRSStrongStock::DoFieldExchange(CFieldExchange* pFX) {
	pFX->SetFieldType(CFieldExchange::outputColumn);
	// RFX_Text() �� RFX_Int() �������������
	// ��Ա���������ͣ����������ݿ��ֶε����͡�
	// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Text(pFX, _T("[Symbol]"), m_Symbol);
}
