#pragma once
#include <VirtualDataHistoryCandleExtend.h>

class CMovingAverage {
public:
	CMovingAverage(int period) { m_period = period; }
	~CMovingAverage() = default;

	void SetPeriod(int period) noexcept { m_period = period; }
	size_t Size() const { return m_vMovingAverage.size(); }
	void Calculate(const vector<long>& vValue);
	void Calculate(const CVirtualDataHistoryCandleExtend* pData);

	void ToShow(CDC* pDC, CPen* pNewPen, CRect rectClient, long lHigh, long lLow);

protected:
	int m_period;
	vector<long> m_vMovingAverage;
};
