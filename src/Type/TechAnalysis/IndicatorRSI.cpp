#include"pch.h"

#include "IndicatorRSI.h"
#include <deque>

using namespace FireBird::Indicators;

std::vector<double> CIndicatorRSI::Compute(const std::vector<CVirtualHistoryCandle>& candles, int period) {
	std::vector<double> results;
	if (period <= 0 || candles.empty()) return results;

	results.reserve(candles.size());
	std::deque<double> winGain;
	std::deque<double> winLoss;
	double sumGain = 0.0;
	double sumLoss = 0.0;

	double prevPrice = static_cast<double>(candles[0].GetClose());
	// index 0: no change available
	results.push_back(0.0);

	for (size_t i = 1; i < candles.size(); ++i) {
		double price = static_cast<double>(candles[i].GetClose());
		double change = price - prevPrice;
		double gain = (change > 0.0) ? change : 0.0;
		double loss = (change < 0.0) ? -change : 0.0;

		// push into rolling window
		winGain.push_back(gain);
		winLoss.push_back(loss);
		sumGain += gain;
		sumLoss += loss;

		// keep only 'period' changes
		if ((int)winGain.size() > period) {
			sumGain -= winGain.front();
			winGain.pop_front();
			sumLoss -= winLoss.front();
			winLoss.pop_front();
		}

		double rsi = 0.0;
		if ((int)winGain.size() == period) {
			double avgGain = sumGain / period;
			double avgLoss = sumLoss / period;
			if (avgLoss <= 0.0) {
				// No losses in the window -> RSI = 100
				rsi = 100.0;
			}
			else {
				double rs = avgGain / avgLoss;
				rsi = 100.0 - (100.0 / (1.0 + rs));
			}
		}
		else {
			// not enough data yet -> leave RSI as 0.0 for alignment
			rsi = 0.0;
		}

		results.push_back(rsi);
		prevPrice = price;
	}

	return results;
}

std::vector<double> CIndicatorRSI::Compute(const std::shared_ptr<std::vector<CVirtualHistoryCandle>>& candlesPtr, int period) {
	if (!candlesPtr) return {};
	return Compute(*candlesPtr, period);
}

void CIndicatorRSI::Calculate() {
	std::vector<double> rsiValues = Compute(m_pvCandle->GetDataVector(), m_PeriodFast);
	m_vRSI.reserve(rsiValues.size());
	for (size_t i = 0; i < rsiValues.size(); ++i) {
		m_vRSI.push_back(CRSI{ rsiValues[i], 0.0, 0.0 });
	}
	std::vector<double> rsiValues2 = Compute(m_pvCandle->GetDataVector(), m_PeriodMiddle);
	ASSERT(rsiValues2.size() == rsiValues.size());
	for (size_t i = 0; i < rsiValues.size(); ++i) {
		m_vRSI.at(i).m_RSI12 = rsiValues2[i];
	}
	std::vector<double> rsiValues3 = Compute(m_pvCandle->GetDataVector(), m_PeriodSlow);
	ASSERT(rsiValues3.size() == rsiValues.size());
	for (size_t i = 0; i < rsiValues.size(); ++i) {
		m_vRSI.at(i).m_RSI24 = rsiValues3[i];
	}
}

void CIndicatorRSI::ToShow(CDC* pDC, CRect rectDrawArea, int iStepWidth) {
	constexpr COLORREF crPurple(RGB(128, 0, 128)), crWhite(RGB(255, 255, 255)), crBlue(RGB(0, 0, 255)),
	                   crRed(RGB(255, 0, 0)), crYellow(RGB(255, 255, 0));
	CPen penPurple(PS_SOLID, 1, crPurple), penWhite1(PS_SOLID, 1, crWhite), penRed1(PS_SOLID, 1, crRed);
	CPen penYellow(PS_SOLID, 1, crYellow);
	CPen penBlue(PS_SOLID, 1, crBlue);

	CPen penWhiteDash(PS_DOT, 1, crWhite);
	CPen* pOldPen = pDC->SelectObject(&penWhiteDash);
	int y20 = rectDrawArea.top + rectDrawArea.Height() * 8 / 10;
	int y80 = rectDrawArea.top + rectDrawArea.Height() * 2 / 10;
	pDC->MoveTo(rectDrawArea.left, y20);
	pDC->LineTo(rectDrawArea.right, y20);
	pDC->MoveTo(rectDrawArea.left, y80);
	pDC->LineTo(rectDrawArea.right, y80);
	//pDC->SelectObject(pOldPen);

	long offset = iStepWidth / 2;

	pDC->SelectObject(&penYellow);
	auto it = m_vRSI.end();
	int i = 1;
	--it;
	long y = rectDrawArea.bottom - (it--)->m_RSI6 * rectDrawArea.Height() / 100;
	pDC->MoveTo(rectDrawArea.right - offset, y);
	for (; it != m_vRSI.begin(); --it, i++) {
		y = rectDrawArea.bottom - it->m_RSI6 * rectDrawArea.Height() / 100;
		pDC->LineTo(rectDrawArea.right - offset - iStepWidth * i, y);
		if (i >= m_vRSI.size()) break;
		if (rectDrawArea.right <= iStepWidth * i) break; // 画到窗口左边框为止
	}

	pDC->SelectObject(&penWhite1);
	it = m_vRSI.end();
	i = 1;
	--it;
	y = rectDrawArea.bottom - (it--)->m_RSI12 * rectDrawArea.Height() / 100;
	pDC->MoveTo(rectDrawArea.right - offset, y);
	for (; it != m_vRSI.begin(); --it, i++) {
		y = rectDrawArea.bottom - it->m_RSI12 * rectDrawArea.Height() / 100;
		pDC->LineTo(rectDrawArea.right - offset - iStepWidth * i, y);
		if (i >= m_vRSI.size()) break;
		if (rectDrawArea.right <= iStepWidth * i) break; // 画到窗口左边框为止
	}

	pDC->SelectObject(&penBlue);
	it = m_vRSI.end();
	i = 1;
	--it;
	y = rectDrawArea.bottom - (it--)->m_RSI24 * rectDrawArea.Height() / 100;
	pDC->MoveTo(rectDrawArea.right - offset, y);
	for (; it != m_vRSI.begin(); --it, i++) {
		y = rectDrawArea.bottom - it->m_RSI24 * rectDrawArea.Height() / 100;
		pDC->LineTo(rectDrawArea.right - offset - iStepWidth * i, y);
		if (i >= m_vRSI.size()) break;
		if (rectDrawArea.right <= iStepWidth * i) break; // 画到窗口左边框为止
	}

	pDC->SelectObject(pOldPen);
}
