#pragma once
#include"VirtualRecordset.h"

class CSetRSOption : public CVirtualRecordset {
public:
	CSetRSOption(const CString& strSchema = _T("ChinaMarket"), const CString& strTable = _T("option_calculatingrs"), CDatabase* pDatabase = nullptr);

	long m_ID{ 0 };
	long m_Index{ 0 };
	BOOL m_Active{ false };
	long m_DayLengthFirst{ 0 };
	long m_DayLengthSecond{ 0 };
	long m_DayLengthThird{ 0 };
	long m_DayLengthFourth{ 0 };
	long m_StrongDayLengthFirst{ 0 };
	long m_StrongDayLengthSecond{ 0 };
	long m_StrongDayLengthThird{ 0 };
	long m_StrongDayLengthFourth{ 0 };
	CString m_RSStrongFirst{ _T("") };
	CString m_RSStrongSecond{ _T("") };
	CString m_RSStrongThird{ _T("") };
	CString m_RSStrongFourth{ _T("") };

public:
	void DoFieldExchange(CFieldExchange* pFX) override;	// RFX Ö§³Ö
};
