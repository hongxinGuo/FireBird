#include"pch.h"

#include "SetRSStrong2Stock.h"

IMPLEMENT_DYNAMIC(CSetRSStrong2Stock, CVirtualRecordset)

CSetRSStrong2Stock::CSetRSStrong2Stock(CString strSchema, CString strTable, CDatabase* pdb)
	: CVirtualRecordset(strSchema, strTable, pdb) {
	m_Symbol = _T("");
	m_nFields = 1;
}

void CSetRSStrong2Stock::DoFieldExchange(CFieldExchange* pFX) {
	pFX->SetFieldType(CFieldExchange::outputColumn);
	// RFX_Text() �� RFX_Int() �������������
	// ��Ա���������ͣ����������ݿ��ֶε����͡�
	// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Text(pFX, _T("[Symbol]"), m_Symbol);
}
/////////////////////////////////////////////////////////////////////////////
// CSetRSStrong2Stock ���

#ifdef _DEBUG
void CSetRSStrong2Stock::AssertValid() const {
	CVirtualRecordset::AssertValid();
}

void CSetRSStrong2Stock::Dump(CDumpContext& dc) const {
	CVirtualRecordset::Dump(dc);
}
#endif //_DEBUG