#pragma once

#include"SetFinnhubRecommendationTrends.h"

#include<memory>

class CFinnhubRecommendationTrends final {
public:
	CFinnhubRecommendationTrends();
	~CFinnhubRecommendationTrends() = default;

	void Append(CSetFinnhubRecommendationTrends& setCountry) const;
	void Load(const CSetFinnhubRecommendationTrends& setCountry);

public:
	CString m_strSymbol;
	long m_lPeriod; // ∏Ò Ω£∫2020-03-01
	long m_lStrongBuy;
	long m_lBuy;
	long m_lHold;
	long m_lSell;
	long m_lStrongSell;
};

using CFinnhubRecommendationTrendsPtr = shared_ptr<CFinnhubRecommendationTrends>;
using CFinnhubRecommendationTrendsVectorPtr = shared_ptr<vector<CFinnhubRecommendationTrendsPtr>>;
