#pragma once
#include"VirtualRecordset.h"

class CSetRSOption : public CVirtualRecordset {
public:
	CSetRSOption(const CString& strSchema = "ChinaMarket", const CString& strTable = "option_calculatingrs", CDatabase* pDatabase = nullptr);

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
	CString m_RSStrongFirst{ "" };
	CString m_RSStrongSecond{ "" };
	CString m_RSStrongThird{ "" };
	CString m_RSStrongFourth{ "" };

public:
	void DoFieldExchange(CFieldExchange* pFX) override;	// RFX 支持
};
