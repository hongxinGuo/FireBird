#pragma once

class CKDJ {
	double m_K{ 0.0 };
	double m_D{ 0.0 };
	double m_J{ 0.0 };
};

class CIndicatorKDJ {
public:
	CIndicatorKDJ();
	~CIndicatorKDJ();

	void SetCandle(CVirtualDataHistoryCandleExtend* pDayLine) { m_pvDayLine = pDayLine; }
	void CalculateIndicator(); //计算KDJ指标

protected:
	int m_Period{ 9 }; // 默认九天为一个周期

	CVirtualDataHistoryCandleExtend* m_pvDayLine{ nullptr };
	vector<CKDJ> m_vKDJ;
};
