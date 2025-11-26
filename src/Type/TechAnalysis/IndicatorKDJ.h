#pragma once

class CKDJ {
public:
	double m_RSV{ 0.0 };
	double m_K{ 0.0 };
	double m_D{ 0.0 };
	double m_J{ 0.0 };
};

class CIndicatorKDJ {
public:
	CIndicatorKDJ() {}
	CIndicatorKDJ(int period) { m_Period = period; }
	~CIndicatorKDJ() = default;

	void SetCandle(CVirtualDataHistoryCandleExtend* pCandle) { m_pvCandle = pCandle; }
	void Calculate(); //计算KDJ指标

	void ToShow(CDC* pDC, CRect rectDrawArea, int iStepWidth);

protected:
	int m_Period{ 9 }; // 默认九天为一个周期

	CVirtualDataHistoryCandleExtend* m_pvCandle{ nullptr };
	vector<CKDJ> m_vKDJ;
};
