#include"pch.h"
#include"FinnhubRecommendationTrends.h"

CFinnhubRecommendationTrends::CFinnhubRecommendationTrends() {
	m_strSymbol = "";
	m_lPeriod = 19700101; // 格式：2020-03-01
	m_lStrongBuy = 0;
	m_lBuy = 0;
	m_lHold = 0;
	m_lSell = 0;
	m_lStrongSell = 0;
}
