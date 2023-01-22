// CSetCompanyNews.cpp : CSetCompanyNews ���ʵ��

// ���������� 2019��6��2��, 5:10

#include"pch.h"

#include "SetFinnhubCompanyNews.h"

CSetCompanyNews::CSetCompanyNews(CString strSchema, CString strTable, CDatabase* pdb)
	: CVirtualRecordset(strSchema, strTable, pdb) {
	m_ID = 0;
	m_CompanySymbol = _T("");
	m_Category = _T("");
	m_DateTime = _T("");
	m_HeadLine = _T("");
	m_NewsID = 0;
	m_Image = _T("");
	m_RelatedSymbol = _T("");
	m_Source = _T("");
	m_Summary = _T("");
	m_URL = _T("");
	m_nFields = 11;
}

void CSetCompanyNews::DoFieldExchange(CFieldExchange* pFX) {
	pFX->SetFieldType(CFieldExchange::outputColumn);
	// RFX_Text() �� RFX_Int() �������������
	// ��Ա���������ͣ����������ݿ��ֶε����͡�
	// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[ID]"), m_ID);
	RFX_Text(pFX, _T("Symbol"), m_CompanySymbol);
	RFX_Text(pFX, _T("Category"), m_Category, 50000);
	RFX_Text(pFX, _T("DateTime"), m_DateTime);
	RFX_Text(pFX, _T("Headline"), m_HeadLine, 50000);
	RFX_Int(pFX, _T("NewsID"), m_NewsID);
	RFX_Text(pFX, _T("Image"), m_Image, 50000);
	RFX_Text(pFX, _T("RelatedSymbol"), m_RelatedSymbol);
	RFX_Text(pFX, _T("Source"), m_Source, 50000);
	RFX_Text(pFX, _T("Summary"), m_Summary, 50000);
	RFX_Text(pFX, _T("URL"), m_URL, 50000);
}
