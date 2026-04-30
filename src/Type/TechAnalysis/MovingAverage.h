#pragma once
#include <VirtualDataHistoryCandleBasic.h>

class CMovingAverage {
public:
	CMovingAverage(int period) { m_period = period; }
	CMovingAverage(const CMovingAverage&) = delete;
	CMovingAverage& operator=(const CMovingAverage&) = delete;
	CMovingAverage(CMovingAverage&&) noexcept = delete;
	CMovingAverage& operator=(CMovingAverage&&) noexcept = delete;
	~CMovingAverage() = default;

	void SetPeriod(int period) noexcept { m_period = period; }
	size_t Size() const { return m_vMovingAverage.size(); }
	void Calculate(const vector<long>& vValue);
	void Calculate(const CVirtualDataHistoryCandleBasic* pData);

	void ToShow(CDC* pDC, CPen* pNewPen, CRect rectClient, int iStepWidth, long lHigh, long lLow);

protected:
	int m_period;
	vector<long> m_vMovingAverage;
};
