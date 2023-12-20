// CSetCountry.h : CSetCountry 的声明

#pragma once

#include"VirtualRecordset.h"

class CSetFinnhubRecommendationTrends : public CVirtualRecordset {
public:
	CSetFinnhubRecommendationTrends(const CString& strSchema = _T("WorldMarket"), const CString& strTable = _T("finnhub_recommendation_trends"), CDatabase* pDatabase = nullptr);

	long m_ID;
	CString m_Symbol;
	long m_Period; // 格式：2020-03-01
	long m_StrongBuy;
	long m_Buy;
	long m_Hold;
	long m_Sell;
	long m_StrongSell;

	// 重写
	// 向导生成的虚函数重写
public:
	void DoFieldExchange(CFieldExchange* pFX) override;	// RFX 支持
};
