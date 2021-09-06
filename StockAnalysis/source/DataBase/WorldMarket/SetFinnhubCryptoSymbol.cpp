// SetFinnhubCryptoSymbol.cpp : CSetFinnhubCryptoSymbol ���ʵ��

// ���������� 2019��5��26��, 8:14

#include"pch.h"
#include"globedef.h"
#include"accessory.h"

#include "SetFinnhubCryptoSymbol.h"

IMPLEMENT_DYNAMIC(CSetFinnhubCryptoSymbol, CRecordset)

CSetFinnhubCryptoSymbol::CSetFinnhubCryptoSymbol(CDatabase* pdb)
	: CRecordset(pdb) {
	m_Description = _T(" ");
	m_DisplaySymbol = _T(" ");
	m_Exchange = _T(" ");
	m_Symbol = _T(" ");
	m_DayLineStartDate = 29900101;
	m_DayLineEndDate = 19800101;
	m_IPOStatus = __STOCK_NOT_CHECKED__;

	m_nFields = 8;
}

CString CSetFinnhubCryptoSymbol::GetDefaultConnect() {
	return GetWorldMarketSchemaConnect();
}

CString CSetFinnhubCryptoSymbol::GetDefaultSQL() {
	return _T("[finnhub_crypto_symbol]");
}

void CSetFinnhubCryptoSymbol::DoFieldExchange(CFieldExchange* pFX) {
	pFX->SetFieldType(CFieldExchange::outputColumn);
	// RFX_Text() �� RFX_Int() �������������
	// ��Ա���������ͣ����������ݿ��ֶε����͡�
	// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[ID]"), m_ID);
	RFX_Text(pFX, _T("[Description]"), m_Description);
	RFX_Text(pFX, _T("[DisplaySymbol]"), m_DisplaySymbol);
	RFX_Text(pFX, _T("[Exchange]"), m_Exchange);
	RFX_Text(pFX, _T("[Symbol]"), m_Symbol);
	RFX_Long(pFX, _T("[DayLineStartDate]"), m_DayLineStartDate);
	RFX_Long(pFX, _T("[DayLineEndDate]"), m_DayLineEndDate);
	RFX_Long(pFX, _T("[IPOStatus]"), m_IPOStatus);
}
/////////////////////////////////////////////////////////////////////////////
// CSetWorldStock ���

#ifdef _DEBUG
void CSetFinnhubCryptoSymbol::AssertValid() const {
	CRecordset::AssertValid();
}

void CSetFinnhubCryptoSymbol::Dump(CDumpContext& dc) const {
	CRecordset::Dump(dc);
}
#endif //_DEBUG