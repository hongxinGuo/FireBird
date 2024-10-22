// CSetCountry.h : CSetCountry 的声明

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

	// 重写
	// 向导生成的虚函数重写
public:
	void DoFieldExchange(CFieldExchange* pFX) override;	// RFX 支持
};
