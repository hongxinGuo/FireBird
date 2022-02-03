// CSetWorldMarketOption.cpp : CSetWorldMarketOption ���ʵ��

#include"pch.h"

#include "SetWorldMarketOption.h"

IMPLEMENT_DYNAMIC(CSetWorldMarketOption, CVirtualRecordset)

CSetWorldMarketOption::CSetWorldMarketOption(CString strSchema, CString strTable, CDatabase* pdb)
	: CVirtualRecordset(strSchema, strTable, pdb) {
	m_ID = 0;
	m_FinnhubToken = _T(" ");
	m_FinnhubNumberPerHour = 3000; // ÿ���Ӳ�ѯ50�Σ�ÿ�μ��1.2��
	m_TiingoToken = _T(" ");
	m_TiingoNumberPerHour = 450; // ÿ���Ӳ�ѯ7.5�Ρ�ÿ�μ��8�롣
	m_QuandlToken = _T(" ");
	m_QuandlNumberPerHour = 0;
	m_nFields = 4;
}

void CSetWorldMarketOption::DoFieldExchange(CFieldExchange* pFX) {
	pFX->SetFieldType(CFieldExchange::outputColumn);
	// RFX_Text() �� RFX_Int() �������������
	// ��Ա���������ͣ����������ݿ��ֶε����͡�
	// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[ID]"), m_ID);
	RFX_Text(pFX, _T("FinnhubToken"), m_FinnhubToken);
	RFX_Text(pFX, _T("TiingoToken"), m_TiingoToken);
	RFX_Text(pFX, _T("QuandlToken"), m_QuandlToken);
}
/////////////////////////////////////////////////////////////////////////////
// CSetWorldForexDayLine ���

#ifdef _DEBUG
void CSetWorldMarketOption::AssertValid() const {
	CVirtualRecordset::AssertValid();
}

void CSetWorldMarketOption::Dump(CDumpContext& dc) const {
	CVirtualRecordset::Dump(dc);
}
#endif //_DEBUG