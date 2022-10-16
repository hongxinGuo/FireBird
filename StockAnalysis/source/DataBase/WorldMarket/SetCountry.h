// CSetCountry.h : CSetCountry 的声明

#pragma once

#include"VirtualRecordset.h"

class CSetCountry : public CVirtualRecordset
{
public:
	CSetCountry(CString strSchema = _T("WorldMarket"), CString strTable = _T("country_list"), CDatabase* pDatabase = nullptr);
	DECLARE_DYNAMIC(CSetCountry)

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
	virtual void DoFieldExchange(CFieldExchange* pFX);	// RFX 支持

	// 实现
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};
