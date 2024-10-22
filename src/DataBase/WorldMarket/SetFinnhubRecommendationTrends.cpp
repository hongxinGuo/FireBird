// CSetCountry.cpp : CSetCountry ���ʵ��

// ���������� 2019��6��2��, 5:10

#include"pch.h"

#include "SetFinnhubRecommendationTrends.h"

CSetFinnhubRecommendationTrends::CSetFinnhubRecommendationTrends(const CString& strSchema, const CString& strTable, CDatabase* pdb)
	: CVirtualRecordset(strSchema, strTable, pdb) {
	m_nFields = 8;
}

void CSetFinnhubRecommendationTrends::DoFieldExchange(CFieldExchange* pFX) {
	pFX->SetFieldType(CFieldExchange::outputColumn);
	// RFX_Text() �� RFX_Int() �������������
	// ��Ա���������ͣ����������ݿ��ֶε����͡�
	// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[ID]"), m_ID);
	RFX_Text(pFX, _T("Symbol"), m_Symbol);
	RFX_Long(pFX, _T("Period"), m_Period);
	RFX_Long(pFX, _T("StrongBuy"), m_StrongBuy);
	RFX_Long(pFX, _T("Buy"), m_Buy);
	RFX_Long(pFX, _T("Hold"), m_Hold);
	RFX_Long(pFX, _T("Sell"), m_Sell);
	RFX_Long(pFX, _T("StrongSell"), m_StrongSell);
}
