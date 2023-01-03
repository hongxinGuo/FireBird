// SetStockCode.cpp : CSetChinaStockSymbol ���ʵ��

#include"pch.h"
#include "SetChinaStockSymbol.h"
#include"globedef.h"

IMPLEMENT_DYNAMIC(CSetChinaStockSymbol, CVirtualRecordset)

CSetChinaStockSymbol::CSetChinaStockSymbol(CString strSchema, CString strTable, CDatabase* pdb)
	: CVirtualRecordset(strSchema, strTable, pdb) {
	m_Description = _T("");
	m_Exchange = _T("");
	m_Symbol = _T("");
	m_DisplaySymbol = _T("");
	m_DayLineStartDate = __CHINA_MARKET_BEGIN_DATE__;
	m_DayLineEndDate = __CHINA_MARKET_BEGIN_DATE__;
	m_IPOStatus = 0;
	m_nFields = 8;
}

void CSetChinaStockSymbol::DoFieldExchange(CFieldExchange* pFX) {
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
// CSetChinaStockSymbol ���

#ifdef _DEBUG
void CSetChinaStockSymbol::AssertValid() const {
	CVirtualRecordset::AssertValid();
	ASSERT(m_DayLineStartDate >= 19700101);
	ASSERT(m_DayLineEndDate >= 19700101);
}

void CSetChinaStockSymbol::Dump(CDumpContext& dc) const {
	CVirtualRecordset::Dump(dc);
}
#endif //_DEBUG