// SetStockCode.cpp : CSetStockCode ���ʵ��

#include"pch.h"
#include "SetStockCode.h"
#include"globedef.h"

IMPLEMENT_DYNAMIC(CSetStockCode, CVirtualRecordset)

CSetStockCode::CSetStockCode(CString strSchema, CString strTable, CDatabase* pdb)
	: CVirtualRecordset(strSchema, strTable, pdb) {
	m_Symbol = _T("");
	m_StockName = _T("");
	m_DayLineStartDate = __CHINA_MARKET_BEGIN_DATE__;
	m_DayLineEndDate = __CHINA_MARKET_BEGIN_DATE__;
	m_IPOStatus = 0;
	m_nFields = 6;
}

void CSetStockCode::DoFieldExchange(CFieldExchange* pFX) {
	pFX->SetFieldType(CFieldExchange::outputColumn);
	// RFX_Text() �� RFX_Int() �������������
	// ��Ա���������ͣ����������ݿ��ֶε����͡�
	// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[ID]"), m_ID);
	RFX_Text(pFX, _T("[Symbol]"), m_Symbol);
	RFX_Text(pFX, _T("[StockName]"), m_StockName);
	RFX_Long(pFX, _T("[DayLineStartDate]"), m_DayLineStartDate);
	RFX_Long(pFX, _T("[DayLineEndDate]"), m_DayLineEndDate);
	RFX_Long(pFX, _T("[IPOed]"), m_IPOStatus);
}
/////////////////////////////////////////////////////////////////////////////
// CSetStockCode ���

#ifdef _DEBUG
void CSetStockCode::AssertValid() const {
	CVirtualRecordset::AssertValid();
}

void CSetStockCode::Dump(CDumpContext& dc) const {
	CVirtualRecordset::Dump(dc);
}
#endif //_DEBUG