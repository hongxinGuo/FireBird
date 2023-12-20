// CSetCountry.h : CSetCountry 的声明

#pragma once

#include"VirtualRecordset.h"

class CSetCountry : public CVirtualRecordset {
public:
	explicit CSetCountry(const CString& strSchema = _T("WorldMarket"), const CString& strTable = _T("country_list"), CDatabase* pDatabase = nullptr);

	long m_ID;
	CString m_Code2;
	CString m_Code3;
	CString m_CodeNo;
	CString m_Country;
	CString m_Currency;
	CString m_CurrencyCode;

	// 重写
	// 向导生成的虚函数重写
public:
	void DoFieldExchange(CFieldExchange* pFX) override;	// RFX 支持
};
