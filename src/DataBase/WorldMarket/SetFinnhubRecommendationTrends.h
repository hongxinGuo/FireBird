// CSetCountry.h : CSetCountry ������

#pragma once

#include"VirtualRecordset.h"

class CSetFinnhubRecommendationTrends : public CVirtualRecordset {
public:
	CSetFinnhubRecommendationTrends(const CString& strSchema = _T("WorldMarket"), const CString& strTable = _T("finnhub_recommendation_trends"), CDatabase* pDatabase = nullptr);

	long m_ID{ 0 };
	CString m_Symbol{ _T("") };
	long m_Period{ 0 }; // ��ʽ��2020-03-01
	long m_StrongBuy{ 0 };
	long m_Buy{ 0 };
	long m_Hold{ 0 };
	long m_Sell{ 0 };
	long m_StrongSell{ 0 };

	// ��д
	// �����ɵ��麯����д
public:
	void DoFieldExchange(CFieldExchange* pFX) override;	// RFX ֧��
};
