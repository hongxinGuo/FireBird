#include"pch.h"

#include "SetRSStrong1Stock.h"

IMPLEMENT_DYNAMIC(CSetRSStrong1Stock, CVirtualRecordset)

CSetRSStrong1Stock::CSetRSStrong1Stock(CString strSchema, CString strTable, CDatabase* pdb)
	: CVirtualRecordset(strSchema, strTable, pdb) {
	m_Symbol = _T("");
	m_nFields = 1;
}

void CSetRSStrong1Stock::DoFieldExchange(CFieldExchange* pFX) {
	pFX->SetFieldType(CFieldExchange::outputColumn);
	// RFX_Text() �� RFX_Int() �������������
	// ��Ա���������ͣ����������ݿ��ֶε����͡�
	// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Text(pFX, _T("[Symbol]"), m_Symbol);
}
/////////////////////////////////////////////////////////////////////////////
// CSetRSStrong1Stock ���

#ifdef _DEBUG
void CSetRSStrong1Stock::AssertValid() const {
	CVirtualRecordset::AssertValid();
}

void CSetRSStrong1Stock::Dump(CDumpContext& dc) const {
	CVirtualRecordset::Dump(dc);
}
#endif //_DEBUG