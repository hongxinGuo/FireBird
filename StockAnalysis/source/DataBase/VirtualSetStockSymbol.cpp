// VirtualSetStockSymbol.cpp : CVirtualSetStockSymbol ���ʵ��

#include"pch.h"
#include"globedef.h"
#include"GlobeSymbolDef.h"

#include "VirtualSetStockSymbol.h"

IMPLEMENT_DYNAMIC(CVirtualSetStockSymbol, CVirtualRecordset)

CVirtualSetStockSymbol::CVirtualSetStockSymbol(CString strSchema, CString strTable, CDatabase* pdb)
	: CVirtualRecordset(strSchema, strTable, pdb) {
	m_Description = _T(" ");
	m_Exchange = _T(" ");
	m_Symbol = _T(" ");
	m_DisplaySymbol = _T(" ");
	m_DayLineStartDate = 29900101;
	m_DayLineEndDate = 19800101;
	m_IPOStatus = __STOCK_NOT_CHECKED__;

	m_nFields = 8;
}

void CVirtualSetStockSymbol::DoFieldExchange(CFieldExchange* pFX) {
	pFX->SetFieldType(CFieldExchange::outputColumn);
	// RFX_Text() �� RFX_Int() �������������
	// ��Ա���������ͣ����������ݿ��ֶε����͡�
	// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[ID]"), m_ID);
	RFX_Text(pFX, _T("[Description]"), m_Description);
	RFX_Text(pFX, _T("[Exchange]"), m_Exchange);
	RFX_Text(pFX, _T("[Symbol]"), m_Symbol);
	RFX_Text(pFX, _T("[DisplaySymbol]"), m_DisplaySymbol);
	RFX_Long(pFX, _T("[DayLineStartDate]"), m_DayLineStartDate);
	RFX_Long(pFX, _T("[DayLineEndDate]"), m_DayLineEndDate);
	RFX_Long(pFX, _T("[IPOStatus]"), m_IPOStatus);
}
/////////////////////////////////////////////////////////////////////////////
// CVirtualSetStockSymbol ���

#ifdef _DEBUG
void CVirtualSetStockSymbol::AssertValid() const {
	CVirtualRecordset::AssertValid();
}

void CVirtualSetStockSymbol::Dump(CDumpContext& dc) const {
	CVirtualRecordset::Dump(dc);
}
#endif //_DEBUG