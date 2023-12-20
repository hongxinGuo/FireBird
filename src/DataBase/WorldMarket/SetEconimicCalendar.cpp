// CSetEconomicCalendar.cpp : CSetEconomicCalendar 类的实现

// CSetWorldStockDayLine 实现

// 代码生成在 2019年6月2日, 5:10

#include"pch.h"

#include "SetEconomicCalendar.h"

CSetEconomicCalendar::CSetEconomicCalendar(const CString& strSchema, const CString& strTable, CDatabase* pdb)
	: CVirtualRecordset(strSchema, strTable, pdb) {
	m_ID = 0;
	m_Country = _T(" ");
	m_Event = _T(" ");
	m_Impact = _T(" ");
	m_Actual = _T(" ");
	m_Estimate = _T(" ");
	m_Prev = _T(" ");
	m_Unit = _T(" ");
	m_nFields = 9;
}

void CSetEconomicCalendar::DoFieldExchange(CFieldExchange* pFX) {
	pFX->SetFieldType(CFieldExchange::outputColumn);
	// RFX_Text() 和 RFX_Int() 这类宏依赖的是
	// 成员变量的类型，而不是数据库字段的类型。
	// ODBC 尝试自动将列值转换为所请求的类型
	RFX_Long(pFX, _T("[ID]"), m_ID);
	RFX_Text(pFX, _T("[Time]"), m_Time);
	RFX_Text(pFX, _T("Country"), m_Country);
	RFX_Text(pFX, _T("Event"), m_Event);
	RFX_Text(pFX, _T("Impact"), m_Impact);
	RFX_Text(pFX, _T("Actual"), m_Actual);
	RFX_Text(pFX, _T("Estimate"), m_Estimate);
	RFX_Text(pFX, _T("Prev"), m_Prev);
	RFX_Text(pFX, _T("Unit"), m_Unit);
}
