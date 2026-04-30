#include"pch.h"

#include"MovingAverage.h"

void CMovingAverage::Calculate(const vector<long>& vValue) {
	if (m_period >= vValue.size()) {
		m_vMovingAverage.clear();
		return;
	}
	m_vMovingAverage.resize(vValue.size() - m_period);
	for (size_t index = 0; index < vValue.size() - m_period; index++) {
		long long total = 0;
		for (size_t i = 0; i < m_period; i++) {
			total += vValue.at(index + i);
		}
		m_vMovingAverage[index] = total / m_period;
	}
}

void CMovingAverage::Calculate(const CVirtualDataHistoryCandleBasic* pData) {
	if (m_period >= pData->Size()) {
		m_vMovingAverage.clear();
		return;
	}
	m_vMovingAverage.resize(pData->Size() - m_period);
	for (size_t index = 0; index < pData->Size() - m_period; index++) {
		long long total = 0;
		for (int i = 0; i < m_period; i++) {
			total += pData->GetData(index + i)->GetClose();
		}
		m_vMovingAverage[index] = total / m_period;
	}
}

void CMovingAverage::ToShow(CDC* pDC, CPen* pNewPen, CRect rectClient, int iStepWidth, long lHigh, long lLow) {
	if (m_vMovingAverage.size() < 3) return;
	auto pOldPen = pDC->SelectObject(pNewPen);
	long offset = iStepWidth / 2;
	auto it = m_vMovingAverage.end();
	--it;
	long x = rectClient.right - offset;
	long y = rectClient.top + (1 - static_cast<double>(*it - lLow) / (lHigh - lLow)) * rectClient.Height();
	pDC->MoveTo(x, y);
	--it;
	int i = 1;
	for (; it != m_vMovingAverage.begin(); --it) {
		x = rectClient.right - offset - i * iStepWidth;
		auto value = *it;
		y = rectClient.top + (1 - static_cast<double>(value - lLow) / (lHigh - lLow)) * rectClient.Height();
		pDC->LineTo(x, y);
		i++;
		if (i >= Size()) break;
		if (rectClient.right <= iStepWidth * i) break; // 画到窗口左边框为止
	}
	pDC->SelectObject(pOldPen);
}
