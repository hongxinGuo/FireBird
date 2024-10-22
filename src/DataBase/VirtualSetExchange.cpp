// CVirtualSetExchange.cpp : CVirtualSetExchange ���ʵ��

#include"pch.h"

#include "VirtualSetExchange.h"

CVirtualSetExchange::CVirtualSetExchange(const CString& strSchema, const CString& strTable, CDatabase* pdb)
	: CVirtualRecordset(strSchema, strTable, pdb) {
	m_nFields = 13;
}

void CVirtualSetExchange::DoFieldExchange(CFieldExchange* pFX) {
	pFX->SetFieldType(CFieldExchange::outputColumn);
	// RFX_Text() �� RFX_Int() �������������
	// ��Ա���������ͣ����������ݿ��ֶε����͡�
	// ODBC �����Զ�����ֵת��Ϊ�����������
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
