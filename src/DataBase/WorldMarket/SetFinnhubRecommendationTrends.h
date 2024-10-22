// CSetCountry.h : CSetCountry 的声明

#pragma once

#include"VirtualRecordset.h"

class CSetFinnhubRecommendationTrends : public CVirtualRecordset {
public:
	CSetFinnhubRecommendationTrends(const CString& strSchema = _T("WorldMarket"), const CString& strTable = _T("finnhub_recommendation_trends"), CDatabase* pDatabase = nullptr);

	long m_ID{ 0 };
	CString m_Symbol{ _T("") };
	long m_Period{ 0 }; // 格式：2020-03-01
	long m_StrongBuy{ 0 };
	long m_Buy{ 0 };
	long m_Hold{ 0 };
	long m_Sell{ 0 };
	long m_StrongSell{ 0 };

	// 重写
	// 向导生成的虚函数重写
public:
	void DoFieldExchange(CFieldExchange* pFX) override;	// RFX 支持
};
