#include "pch.h"
#include "RelativeStrengthIndex.h"

void CIndicatorRSI::reset() {
		m_vClose.clear();
		m_vRSI.clear();
		currentHigh_ = std::numeric_limits<double>::quiet_NaN();
		currentLow_ = std::numeric_limits<double>::quiet_NaN();
		currentDif_ = std::numeric_limits<double>::quiet_NaN();
}

void CIndicatorRSI::Calculate(int period) {
		auto candleSize = m_pvCandle->Size();
		if (m_pvCandle->Empty()) return;

		m_vClose.clear();
		m_vClose.reserve(candleSize);
		for (std::size_t i = 0; i < candleSize; ++i)
				m_vClose.push_back(static_cast<double>(m_pvCandle->GetData(i)->GetClose()) / m_pvCandle->GetRatio());

		std::vector<double> gains(candleSize, std::numeric_limits<double>::quiet_NaN());
		std::vector<double> losses(candleSize, std::numeric_limits<double>::quiet_NaN());

		for (std::size_t i = 1; i < candleSize; ++i) {
				double diff = m_vClose[i] - m_vClose[i - 1];
				gains[i] = diff > 0 ? diff : 0.0;
				losses[i] = diff < 0 ? -diff : 0.0;
		}

		std::vector<double> avgGain(candleSize, std::numeric_limits<double>::quiet_NaN());
		std::vector<double> avgLoss(candleSize, std::numeric_limits<double>::quiet_NaN());

		// first average is simple moving average over first 'period' values (starting at index 1)
		if (candleSize > static_cast<std::size_t>(period)) {
				double sumG = 0.0, sumL = 0.0;
				for (int i = 1; i <= period; ++i) {
						sumG += gains[i];
						sumL += losses[i];
				}
				avgGain[period] = sumG / static_cast<double>(period);
				avgLoss[period] = sumL / static_cast<double>(period);
				for (std::size_t i = period + 1; i < candleSize; ++i) {
						avgGain[i] = (avgGain[i - 1] * (period - 1) + gains[i]) / period;
						avgLoss[i] = (avgLoss[i - 1] * (period - 1) + losses[i]) / period;
				}
		}
		else {
				// fallback: calculate running averages
				double sumG = 0.0, sumL = 0.0;
				for (std::size_t i = 1; i < candleSize; ++i) {
						sumG += gains[i];
						sumL += losses[i];
						avgGain[i] = sumG / static_cast<double>(i);
						avgLoss[i] = sumL / static_cast<double>(i);
				}
		}

		m_vRSI.clear();
		m_vRSI.reserve(candleSize);
		for (std::size_t i = 0; i < candleSize; ++i) {
				RSIResult r;
				r.rsi = std::numeric_limits<double>::quiet_NaN();
				if (!std::isnan(avgGain[i]) && !std::isnan(avgLoss[i])) {
						if (avgLoss[i] == 0.0) r.rsi = 100.0;
						else {
								double rs = avgGain[i] / avgLoss[i];
								r.rsi = 100.0 - (100.0 / (1.0 + rs));
						}
				}
				m_vRSI.push_back(r);
				r.Reset();
		}
}

void CIndicatorRSI::Calculate2(int period) {
		// same as Calculate but reads directly from m_pvCandle
		auto candleSize = m_pvCandle->Size();
		if (m_pvCandle->Empty()) return;

		std::vector<double> gains(candleSize, std::numeric_limits<double>::quiet_NaN());
		std::vector<double> losses(candleSize, std::numeric_limits<double>::quiet_NaN());

		for (std::size_t i = 1; i < candleSize; ++i) {
				double prev = static_cast<double>(m_pvCandle->GetData(i - 1)->GetClose()) / m_pvCandle->GetRatio();
				double cur = static_cast<double>(m_pvCandle->GetData(i)->GetClose()) / m_pvCandle->GetRatio();
				double diff = cur - prev;
				gains[i] = diff > 0 ? diff : 0.0;
				losses[i] = diff < 0 ? -diff : 0.0;
		}

		std::vector<double> avgGain(candleSize, std::numeric_limits<double>::quiet_NaN());
		std::vector<double> avgLoss(candleSize, std::numeric_limits<double>::quiet_NaN());

		if (candleSize > static_cast<std::size_t>(period)) {
				double sumG = 0.0, sumL = 0.0;
				for (int i = 1; i <= period; ++i) {
						sumG += gains[i];
						sumL += losses[i];
				}
				avgGain[period] = sumG / static_cast<double>(period);
				avgLoss[period] = sumL / static_cast<double>(period);
				for (std::size_t i = period + 1; i < candleSize; ++i) {
						avgGain[i] = (avgGain[i - 1] * (period - 1) + gains[i]) / period;
						avgLoss[i] = (avgLoss[i - 1] * (period - 1) + losses[i]) / period;
				}
		}
		else {
				double sumG = 0.0, sumL = 0.0;
				for (std::size_t i = 1; i < candleSize; ++i) {
						sumG += gains[i];
						sumL += losses[i];
						avgGain[i] = sumG / static_cast<double>(i);
						avgLoss[i] = sumL / static_cast<double>(i);
				}
		}

		m_vRSI.clear();
		m_vRSI.reserve(candleSize);
		for (std::size_t i = 0; i < candleSize; ++i) {
				RSIResult r;
				r.rsi = std::numeric_limits<double>::quiet_NaN();
				if (!std::isnan(avgGain[i]) && !std::isnan(avgLoss[i])) {
						if (avgLoss[i] == 0.0) r.rsi = 100.0;
						else {
								double rs = avgGain[i] / avgLoss[i];
								r.rsi = 100.0 - (100.0 / (1.0 + rs));
						}
				}
				m_vRSI.push_back(r);
				r.Reset();
		}
}

void CIndicatorRSI::ToShow(CDC* pDC, CRect rectDrawArea, int iStepWidth) {
		constexpr COLORREF crWhite(RGB(255, 255, 255));
		CPen penWhite1(PS_SOLID, 1, crWhite);

		CalculateCurrentHighLow(rectDrawArea, iStepWidth);

		CPen* pOldPen = pDC->SelectObject(&penWhite1);
		auto it = m_vRSI.end();
		if (m_vRSI.empty()) return;
		int i = 1;
		--it;
		long y = rectDrawArea.bottom - ((it->rsi - currentLow_) / currentDif_) * rectDrawArea.Height();
		--it;
		pDC->MoveTo(rectDrawArea.right - iStepWidth / 2, y);
		for (; it != m_vRSI.begin(); --it, i++) {
				y = rectDrawArea.bottom - ((it->rsi - currentLow_) / currentDif_) * rectDrawArea.Height();
				pDC->LineTo(rectDrawArea.right - iStepWidth / 2 - iStepWidth * i, y);
				if (i >= m_vRSI.size()) break;
				if (rectDrawArea.right <= iStepWidth * i) break;
		}
		pDC->SelectObject(pOldPen);
}

void CIndicatorRSI::CalculateCurrentHighLow(CRect rectDrawArea, int iStepWidth) {
		if (m_vRSI.empty()) return;
		auto it = m_vRSI.end();
		int i = 1;
		--it;
		currentHigh_ = it->rsi;
		currentLow_ = it->rsi;
		--it;
		for (; it != m_vRSI.begin(); --it, i++) {
				currentHigh_ = max(currentHigh_, it->rsi);
				currentLow_ = min(currentLow_, it->rsi);
				if (i >= m_vRSI.size()) break;
				if (rectDrawArea.right <= iStepWidth * i) break;
		}
		currentDif_ = currentHigh_ - currentLow_;
		if (currentDif_ <= 0.0) currentDif_ = 1.0; // avoid division by zero
}
