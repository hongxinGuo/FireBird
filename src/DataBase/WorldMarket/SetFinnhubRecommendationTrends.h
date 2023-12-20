// CSetCountry.h : CSetCountry ������

#pragma once

#include"VirtualRecordset.h"

class CSetFinnhubRecommendationTrends : public CVirtualRecordset {
public:
	CSetFinnhubRecommendationTrends(const CString& strSchema = _T("WorldMarket"), const CString& strTable = _T("finnhub_recommendation_trends"), CDatabase* pDatabase = nullptr);

	long m_ID;
	CString m_Symbol;
	long m_Period; // ��ʽ��2020-03-01
	long m_StrongBuy;
	long m_Buy;
	long m_Hold;
	long m_Sell;
	long m_StrongSell;

	// ��д
	// �����ɵ��麯����д
public:
	void DoFieldExchange(CFieldExchange* pFX) override;	// RFX ֧��
};
