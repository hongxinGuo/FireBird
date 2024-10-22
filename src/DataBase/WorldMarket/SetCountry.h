// CSetCountry.h : CSetCountry ������

#pragma once

#include"VirtualRecordset.h"

class CSetCountry : public CVirtualRecordset {
public:
	explicit CSetCountry(const CString& strSchema = _T("WorldMarket"), const CString& strTable = _T("finnhub_country_list"), CDatabase* pDatabase = nullptr);

	long m_ID{ 0 };
	CString m_Code2{ _T("") };
	CString m_Code3{ _T("") };
	CString m_CodeNo{ _T("") };
	CString m_Country{ _T("") };
	CString m_Currency{ _T("") };
	CString m_CurrencyCode{ _T("") };

	// ��д
	// �����ɵ��麯����д
public:
	void DoFieldExchange(CFieldExchange* pFX) override;	// RFX ֧��
};
