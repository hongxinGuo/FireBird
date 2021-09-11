#pragma once
#include"VirtualRecordset.h"

class CSetRSOption : public CVirtualRecordset
{
public:
	CSetRSOption(CString strSchema = _T("ChinaMarket"), CString strTable = _T("option_calculatingrs"), CDatabase* pDatabase = nullptr);
	DECLARE_DYNAMIC(CSetRSOption)

	long m_ID;
	long m_Index;
	BOOL m_Active;
	long m_DayLengthFirst;
	long m_DayLengthSecond;
	long m_DayLengthThird;
	long m_DayLengthFourth;
	long m_StrongDayLengthFirst;
	long m_StrongDayLengthSecond;
	long m_StrongDayLengthThird;
	long m_StrongDayLengthFourth;
	CString m_RSStrongFirst;
	CString m_RSStrongSecond;
	CString m_RSStrongThird;
	CString m_RSStrongFourth;
public:
	virtual void DoFieldExchange(CFieldExchange* pFX);	// RFX 支持

// 实现
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};
