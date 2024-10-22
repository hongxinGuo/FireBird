// CSetSECFilings.h : CSetSECFilings ������

#pragma once

#include"VirtualRecordset.h"

class CSetSECFilings : public CVirtualRecordset {
public:
	explicit CSetSECFilings(const CString& strSchema = _T("WorldMarket"), const CString& strTable = _T("finnhub_stock_sec_filings"), CDatabase* pDatabase = nullptr);

	long m_ID{ 0 };
	CString m_Symbol{ _T("") };
	long m_CIK{ 0 };
	CString m_AccessNumber{ _T("") };
	CString m_Form{ _T("") };
	long m_FiledDate{ 0 };
	long m_AcceptedDate{ 0 };
	CString m_ReportURL{ _T("") };
	CString m_FilingURL{ _T("") };

	// ��д
	// �����ɵ��麯����д
public:
	void DoFieldExchange(CFieldExchange* pFX) override;	// RFX ֧��
};
