// SetWorldChoicedForex.h : CSetWorldChoicedForex ���ʵ��

// SetWorldChoicedForex ʵ��

// ���������� 2019��5��26��, 8:15

#include"pch.h"
#include"globedef.h"
#include"Accessory.h"

#include "SetWorldChoicedForex.h"

IMPLEMENT_DYNAMIC(CSetWorldChoicedForex, CRecordset)

CSetWorldChoicedForex::CSetWorldChoicedForex(CDatabase* pdb) : CRecordset(pdb) {
	m_ID = 0;
	m_Symbol = _T("");
	m_nFields = 2;
}

CString CSetWorldChoicedForex::GetDefaultConnect() {
	return GetWorldMarketSchemaConnect();
}

CString CSetWorldChoicedForex::GetDefaultSQL() {
	return _T("[choiced_forex]");
}

void CSetWorldChoicedForex::DoFieldExchange(CFieldExchange* pFX) {
	pFX->SetFieldType(CFieldExchange::outputColumn);
	// RFX_Text() �� RFX_Int() �������������
	// ��Ա���������ͣ����������ݿ��ֶε����͡�
	// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[ID]"), m_ID);
	RFX_Text(pFX, _T("[Symbol]"), m_Symbol);
}
/////////////////////////////////////////////////////////////////////////////
// CSetChoicedStock ���

#ifdef _DEBUG
void CSetWorldChoicedForex::AssertValid() const {
	CRecordset::AssertValid();
}

void CSetWorldChoicedForex::Dump(CDumpContext& dc) const {
	CRecordset::Dump(dc);
}
#endif //_DEBUG