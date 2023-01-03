// CSetCountry.h : CSetCountry ������

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

	// ��д
		// �����ɵ��麯����д
public:
	virtual void DoFieldExchange(CFieldExchange* pFX);	// RFX ֧��

	// ʵ��
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};
