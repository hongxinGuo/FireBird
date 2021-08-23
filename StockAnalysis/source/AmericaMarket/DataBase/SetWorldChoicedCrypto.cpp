// SetWorldChoicedCrypto.h : CSetWorldChoicedCrypto ���ʵ��

// CSetWorldChoicedCrypto ʵ��

// ���������� 2019��5��26��, 8:15

#include"pch.h"
#include"globedef.h"
#include"Accessory.h"

#include "SetWorldChoicedCrypto.h"

IMPLEMENT_DYNAMIC(CSetWorldChoicedCrypto, CRecordset)

CSetWorldChoicedCrypto::CSetWorldChoicedCrypto(CDatabase* pdb) : CRecordset(pdb) {
	m_ID = 0;
	m_Symbol = _T("");
	m_nFields = 2;
}

CString CSetWorldChoicedCrypto::GetDefaultConnect() {
	return GetWorldMarketSchemaConnect();
}

CString CSetWorldChoicedCrypto::GetDefaultSQL() {
	return _T("[choiced_crypto]");
}

void CSetWorldChoicedCrypto::DoFieldExchange(CFieldExchange* pFX) {
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
void CSetWorldChoicedCrypto::AssertValid() const {
	CRecordset::AssertValid();
}

void CSetWorldChoicedCrypto::Dump(CDumpContext& dc) const {
	CRecordset::Dump(dc);
}
#endif //_DEBUG