#include"pch.h"
#include"FinnhubRecommendationTrends.h"

CFinnhubRecommendationTrends::CFinnhubRecommendationTrends() {
	m_strSymbol = _T("");
	m_lPeriod = 19700101; // ∏Ò Ω£∫2020-03-01
	m_lStrongBuy = 0;
	m_lBuy = 0;
	m_lHold = 0;
	m_lSell = 0;
	m_lStrongSell = 0;
}

void CFinnhubRecommendationTrends::Append(CSetFinnhubRecommendationTrends& setFinnhubRecommendationTrends) const {
	setFinnhubRecommendationTrends.AddNew();
	setFinnhubRecommendationTrends.m_Symbol = m_strSymbol;
	setFinnhubRecommendationTrends.m_Period = m_lPeriod;
	setFinnhubRecommendationTrends.m_StrongBuy = m_lStrongBuy;
	setFinnhubRecommendationTrends.m_Buy = m_lBuy;
	setFinnhubRecommendationTrends.m_Hold = m_lHold;
	setFinnhubRecommendationTrends.m_Sell = m_lSell;
	setFinnhubRecommendationTrends.m_StrongSell = m_lStrongSell;
	setFinnhubRecommendationTrends.Update();
}

void CFinnhubRecommendationTrends::Load(const CSetFinnhubRecommendationTrends& setFinnhubRecommendationTrends) {
	m_strSymbol = setFinnhubRecommendationTrends.m_Symbol;
	m_lPeriod = setFinnhubRecommendationTrends.m_Period;
	m_lStrongBuy = setFinnhubRecommendationTrends.m_StrongBuy;
	m_lBuy = setFinnhubRecommendationTrends.m_Buy;
	m_lHold = setFinnhubRecommendationTrends.m_Hold;
	m_lSell = setFinnhubRecommendationTrends.m_Sell;
	m_lStrongSell = setFinnhubRecommendationTrends.m_StrongSell;
}
