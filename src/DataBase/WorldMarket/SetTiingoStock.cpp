// CSetTiingoStock.cpp : CSetTiingoStock ���ʵ��

#include"pch.h"

#include "SetTiingoStock.h"

CSetTiingoStock::CSetTiingoStock(const CString& strSchema, const CString& strTable, CDatabase* pdb)
	: CVirtualRecordset(strSchema, strTable, pdb) {
	m_nFields = 17;
}

void CSetTiingoStock::DoFieldExchange(CFieldExchange* pFX) {
	pFX->SetFieldType(CFieldExchange::outputColumn);
	// RFX_Text() �� RFX_Int() �������������
	// ��Ա���������ͣ����������ݿ��ֶε����͡�
	// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[ID]"), m_ID);
	RFX_Text(pFX, _T("[TiingoPermaTicker]"), m_TiingoPermaTicker);
	RFX_Text(pFX, _T("[Ticker]"), m_Ticker);
	RFX_Text(pFX, _T("[Name]"), m_Name);
	RFX_Bool(pFX, _T("[IsActive]"), m_IsActive);
	RFX_Bool(pFX, _T("[IsADR]"), m_IsADR);
	RFX_Long(pFX, _T("[SICCode]"), m_SicCode);
	RFX_Text(pFX, _T("[SICIndustry]"), m_SicIndustry);
	RFX_Text(pFX, _T("[SICSector]"), m_SicSector);
	RFX_Text(pFX, _T("[TiingoIndustry]"), m_TiingoIndustry);
	RFX_Text(pFX, _T("[TiingoSector]"), m_TiingoSector);
	RFX_Text(pFX, _T("[ReportingCurrency]"), m_ReportingCurrency);
	RFX_Text(pFX, _T("[Location]"), m_Location);
	RFX_Text(pFX, _T("[CompanyWebSite]"), m_CompanyWebSite);
	RFX_Text(pFX, _T("[SECFilingWebSite]"), m_SECFilingWebSite);
	RFX_Long(pFX, _T("[IPOStatus]"), m_IPOStatus);
	RFX_Text(pFX, _T("UpdateDate"), m_UpdateDate, 10000); // Note RFX_TextĬ����󳤶�Ϊ256.
}
