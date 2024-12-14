#include"pch.h"

#include "SetTiingoStockDelistedSymbol.h"

CSetTiingoStockDelistedSymbol::CSetTiingoStockDelistedSymbol(const CString& strSchema, const CString& strTable, CDatabase* pdb)
	: CVirtualRecordset(strSchema, strTable, pdb) {
	m_nFields = 3;
}

void CSetTiingoStockDelistedSymbol::DoFieldExchange(CFieldExchange* pFX) {
	pFX->SetFieldType(CFieldExchange::outputColumn);
	// RFX_Text() �� RFX_Int() �������������
	// ��Ա���������ͣ����������ݿ��ֶε����͡�
	// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[ID]"), m_ID);
	RFX_Text(pFX, _T("[Symbol]"), m_symbol);
	RFX_Long(pFX, _T("[Date]"), m_date);
}
