// CVirtualSetExchange.cpp : CVirtualSetExchange 类的实现

#include"pch.h"

#include "VirtualSetExchange.h"

CVirtualSetExchange::CVirtualSetExchange(const CString& strSchema, const CString& strTable, CDatabase* pdb)
	: CVirtualRecordset(strSchema, strTable, pdb) {
	m_nFields = 13;
}

void CVirtualSetExchange::DoFieldExchange(CFieldExchange* pFX) {
	pFX->SetFieldType(CFieldExchange::outputColumn);
	// RFX_Text() 和 RFX_Int() 这类宏依赖的是
	// 成员变量的类型，而不是数据库字段的类型。
	// ODBC 尝试自动将列值转换为所请求的类型
	RFX_Long(pFX, _T("[ID]"), m_ID);
	RFX_Text(pFX, _T("[code]"), m_Code);
	RFX_Text(pFX, _T("[name]"), m_Name);
	RFX_Text(pFX, _T("[mic]"), m_Mic);
	RFX_Text(pFX, _T("[timezone]"), m_TimeZone);
	RFX_Text(pFX, _T("[pre_market]"), m_PreMarket);
	RFX_Text(pFX, _T("[hour]"), m_Hour);
	RFX_Text(pFX, _T("[post_market]"), m_PostMarket);
	RFX_Text(pFX, _T("[close_date]"), m_CloseDate);
	RFX_Text(pFX, _T("[country]"), m_Country);
	RFX_Text(pFX, _T("[country_name]"), m_CountryName);
	RFX_Text(pFX, _T("[source]"), m_Source);
	RFX_Text(pFX, _T("[MyUnknownColumn]"), m_MyUnknownColumn);
}
