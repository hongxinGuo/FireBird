// CSetSECFilings.h : CSetSECFilings ������

#pragma once

#include"VirtualRecordset.h"

class CSetSECFilings : public CVirtualRecordset {
public:
	explicit CSetSECFilings(const CString& strSchema = _T("WorldMarket"), const CString& strTable = _T("finnhub_stock_sec_filings"), CDatabase* pDatabase = nullptr);

	long m_ID;
	CString m_Symbol;
	long m_CIK;
	CString m_AccessNumber;
	CString m_Form;
	long m_FiledDate;
	long m_AcceptedDate;
	CString m_ReportURL;
	CString m_FilingURL;

	// ��д
	// �����ɵ��麯����д
public:
	void DoFieldExchange(CFieldExchange* pFX) override;	// RFX ֧��
};
