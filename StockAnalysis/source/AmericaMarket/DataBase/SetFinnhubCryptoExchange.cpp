// CSetFinnhubCryptoExchange.cpp : CSetFinnhubCryptoExchange ���ʵ��

// ���������� 2019��5��26��, 8:14

#include"pch.h"
#include"globedef.h"
#include"accessory.h"

#include "SetFinnhubCryptoExchange.h"

IMPLEMENT_DYNAMIC(CSetFinnhubCryptoExchange, CRecordset)

CSetFinnhubCryptoExchange::CSetFinnhubCryptoExchange(CDatabase* pdb)
	: CRecordset(pdb) {
	m_Exchange = _T(" ");

	m_nFields = 2;
}

CString CSetFinnhubCryptoExchange::GetDefaultConnect() {
	return GetWorldMarketSchemaConnect();
}

CString CSetFinnhubCryptoExchange::GetDefaultSQL() {
	return _T("[finnhub_crypto_exchange]");
}

void CSetFinnhubCryptoExchange::DoFieldExchange(CFieldExchange* pFX) {
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
void CSetFinnhubCryptoExchange::AssertValid() const {
	CRecordset::AssertValid();
}

void CSetFinnhubCryptoExchange::Dump(CDumpContext& dc) const {
	CRecordset::Dump(dc);
}
#endif //_DEBUG