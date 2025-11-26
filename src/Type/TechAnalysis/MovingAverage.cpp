#include"pch.h"

#include"MovingAverage.h"

void CMovingAverage::Calculate(const vector<long>& vValue) {
	m_vMovingAverage.resize(vValue.size() - m_period);
	for (size_t index = 0; index < vValue.size() - m_period; index++) {
		long long total = 0;
		for (size_t i = 0; i < m_period; i++) {
			total += vValue.at(index + i);
		}
		m_vMovingAverage[index] = total / m_period;
	}
}

void CMovingAverage::Calculate(const CVirtualDataHistoryCandleExtend* pData) {
	m_vMovingAverage.resize(pData->Size() - m_period);
	for (size_t index = 0; index < pData->Size() - m_period; index++) {
		long long total = 0;
		for (size_t i = 0; i < m_period; i++) {
			total += pData->GetData(index + i)->GetClose();
		}
		m_vMovingAverage[index] = total / m_period;
	}
}

void CMovingAverage::ToShow(CDC* pDC, CPen* pNewPen, CRect rectClient, int iStepWidth, long lHigh, long lLow) {
	auto pOldPen = pDC->SelectObject(pNewPen);
	auto it = m_vMovingAverage.end();
	--it;
	int i = 0;
	long x = rectClient.right;
	long y = (1 - static_cast<double>(*it - lLow) / (lHigh - lLow)) * rectClient.Height();
	pDC->MoveTo(x, y);
	--it;
	for (; it != m_vMovingAverage.begin(); --it) {
		x = rectClient.right - i * iStepWidth;
		auto value = *it;
		y = (1 - static_cast<double>(value - lLow) / (lHigh - lLow)) * rectClient.Height();
		pDC->LineTo(x, y);
		i++;
		if (i >= Size()) break;
		if (rectClient.right <= iStepWidth * i) break; // 画到窗口左边框为止
	}
	pDC->SelectObject(pOldPen);
}
