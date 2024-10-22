// CSetCountry.cpp : CSetCountry 类的实现

// 代码生成在 2019年6月2日, 5:10

#include"pch.h"

#include "SetFinnhubRecommendationTrends.h"

CSetFinnhubRecommendationTrends::CSetFinnhubRecommendationTrends(const CString& strSchema, const CString& strTable, CDatabase* pdb)
	: CVirtualRecordset(strSchema, strTable, pdb) {
	m_nFields = 8;
}

void CSetFinnhubRecommendationTrends::DoFieldExchange(CFieldExchange* pFX) {
	pFX->SetFieldType(CFieldExchange::outputColumn);
	// RFX_Text() 和 RFX_Int() 这类宏依赖的是
	// 成员变量的类型，而不是数据库字段的类型。
	// ODBC 尝试自动将列值转换为所请求的类型
	RFX_Long(pFX, _T("[ID]"), m_ID);
	RFX_Text(pFX, _T("Symbol"), m_Symbol);
	RFX_Long(pFX, _T("Period"), m_Period);
	RFX_Long(pFX, _T("StrongBuy"), m_StrongBuy);
	RFX_Long(pFX, _T("Buy"), m_Buy);
	RFX_Long(pFX, _T("Hold"), m_Hold);
	RFX_Long(pFX, _T("Sell"), m_Sell);
	RFX_Long(pFX, _T("StrongSell"), m_StrongSell);
}
