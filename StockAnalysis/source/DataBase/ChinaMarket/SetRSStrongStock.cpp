#include"pch.h"

#include "SetRSStrongStock.h"

IMPLEMENT_DYNAMIC(CSetRSStrongStock, CVirtualRecordset)

CSetRSStrongStock::CSetRSStrongStock(long lIndex, CString strSchema, CString strTable, CDatabase* pdb)
	: CVirtualRecordset(strSchema, strTable, pdb) {
	ASSERT((m_lIndex >= 0) && (m_lIndex < 10));
	m_lIndex = lIndex;
	m_Symbol = _T("");
	m_nFields = 1;
}

CString CSetRSStrongStock::GetDefaultSQL() {
	CString str = _T("[") + m_Table;
	char buffer[10];

	ASSERT((m_lIndex >= 0) && (m_lIndex < 10));
	sprintf_s(buffer, _T("%1d"), m_lIndex);
	str += buffer;
	str += _T("]");

	return str;
}

void CSetRSStrongStock::DoFieldExchange(CFieldExchange* pFX) {
	pFX->SetFieldType(CFieldExchange::outputColumn);
	// RFX_Text() �� RFX_Int() �������������
	// ��Ա���������ͣ����������ݿ��ֶε����͡�
	// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Text(pFX, _T("[Symbol]"), m_Symbol);
}
/////////////////////////////////////////////////////////////////////////////
// CSetRSStrong1Stock ���

#ifdef _DEBUG
void CSetRSStrongStock::AssertValid() const {
	CVirtualRecordset::AssertValid();
}

void CSetRSStrongStock::Dump(CDumpContext& dc) const {
	CVirtualRecordset::Dump(dc);
}
#endif //_DEBUG