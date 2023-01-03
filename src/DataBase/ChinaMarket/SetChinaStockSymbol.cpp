// SetStockCode.cpp : CSetChinaStockSymbol 类的实现

#include"pch.h"
#include "SetChinaStockSymbol.h"
#include"globedef.h"

IMPLEMENT_DYNAMIC(CSetChinaStockSymbol, CVirtualRecordset)

CSetChinaStockSymbol::CSetChinaStockSymbol(CString strSchema, CString strTable, CDatabase* pdb)
	: CVirtualRecordset(strSchema, strTable, pdb) {
	m_Description = _T("");
	m_Exchange = _T("");
	m_Symbol = _T("");
	m_DisplaySymbol = _T("");
	m_DayLineStartDate = __CHINA_MARKET_BEGIN_DATE__;
	m_DayLineEndDate = __CHINA_MARKET_BEGIN_DATE__;
	m_IPOStatus = 0;
	m_nFields = 8;
}

void CSetChinaStockSymbol::DoFieldExchange(CFieldExchange* pFX) {
	pFX->SetFieldType(CFieldExchange::outputColumn);
	// RFX_Text() 和 RFX_Int() 这类宏依赖的是
	// 成员变量的类型，而不是数据库字段的类型。
	// ODBC 尝试自动将列值转换为所请求的类型
	RFX_Long(pFX, _T("[ID]"), m_ID);
	RFX_Text(pFX, _T("[Description]"), m_Description);
	RFX_Text(pFX, _T("[DisplaySymbol]"), m_DisplaySymbol);
	RFX_Text(pFX, _T("[Exchange]"), m_Exchange);
	RFX_Text(pFX, _T("[Symbol]"), m_Symbol);
	RFX_Long(pFX, _T("[DayLineStartDate]"), m_DayLineStartDate);
	RFX_Long(pFX, _T("[DayLineEndDate]"), m_DayLineEndDate);
	RFX_Long(pFX, _T("[IPOStatus]"), m_IPOStatus);
}
/////////////////////////////////////////////////////////////////////////////
// CSetChinaStockSymbol 诊断

#ifdef _DEBUG
void CSetChinaStockSymbol::AssertValid() const {
	CVirtualRecordset::AssertValid();
	ASSERT(m_DayLineStartDate >= 19700101);
	ASSERT(m_DayLineEndDate >= 19700101);
}

void CSetChinaStockSymbol::Dump(CDumpContext& dc) const {
	CVirtualRecordset::Dump(dc);
}
#endif //_DEBUG