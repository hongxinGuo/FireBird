#include"pch.h"

#include "IndicatorBoll.h"
#include "VirtualHistoryCandle.h" // CVirtualHistoryCandle

using namespace FireBird::Indicators;

std::vector<CBoll> CIndicatorBoll::Compute(const std::vector<CVirtualHistoryCandle>& candles, int period, double k) {
	std::vector<CBoll> results;
	if (period <= 0 || candles.empty()) return results;

	results.reserve(candles.size());
	std::deque<double> window;
	window.clear();

	double sum = 0.0;
	double sumsq = 0.0;

	for (size_t i = 0; i < candles.size(); ++i) {
		// Use GetClose() as-is. Caller is responsible for any scaling (ratio).
		double price = static_cast<double>(candles[i].GetClose());

		// add new
		window.push_back(price);
		sum += price;
		sumsq += price * price;

		// remove old if over window
		if ((int)window.size() > period) {
			double old = window.front();
			window.pop_front();
			sum -= old;
			sumsq -= old * old;
		}

		// only compute when we have enough data
		if ((int)window.size() == period) {
			double mean = sum / period;
			// population variance (divide by period). Use max to avoid small negative due to FP.
			double var = (sumsq - (sum * sum) / period) / period;
			if (var < 0 && var > -1e-12) var = 0;
			double sd = std::sqrt(max(0.0, var));

			double upper = mean + k * sd;
			double lower = mean - k * sd;

			CBoll r;
			r.Date = candles[i].GetDate();
			r.Index = i;
			r.Mid = mean;
			r.Upper = upper;
			r.Lower = lower;
			r.BandWidth = (mean != 0.0) ? ((upper - lower) / mean) : 0.0;
			double denom = (upper - lower);
			r.PercentB = (denom != 0.0) ? ((price - lower) / denom) : 0.0;

			results.push_back(r);
		}
		else {
			// keep alignment by pushing an empty result for indices before first full window
			results.push_back(CBoll{ candles[i].GetDate(), i, 0.0, 0.0, 0.0, 0.0, 0.0 });
		}
	}

	return results;
}

std::vector<CBoll> CIndicatorBoll::Compute(const std::shared_ptr<std::vector<CVirtualHistoryCandle>>& candlesPtr, int period, double k) {
	if (!candlesPtr) return {};
	return Compute(*candlesPtr, period, k);
}

void CIndicatorBoll::Calculate() {
	vector<CBoll> vBoll = Compute(m_pvCandle->GetDataVector());
	m_vBoll.reserve(vBoll.size());
	for (size_t i = 0; i < vBoll.size(); i++) {
		m_vBoll.push_back(vBoll.at(i));
	}
}

void CIndicatorBoll::ToShow(CDC* pDC, CRect rectDrawArea, int iStepWidth) {
	if (!m_pvCandle) return;
	//auto& data = m_pvCandle->GetDataVector();
	//if (data.empty() || rectDrawArea.IsRectEmpty() || iStepWidth <= 0) return;

	// Recompute Boll values for current candle data
	//m_vBoll = Compute(data, m_Period, 2.0);
	if (m_vBoll.empty()) return;

	// How many candles fit in the drawing area
	const int maxPoints = rectDrawArea.Width() / iStepWidth;
	if (maxPoints <= 0) return;
	const size_t pointsToDraw = static_cast<size_t>(min((int)m_vBoll.size(), maxPoints));
	if (pointsToDraw == 0) return;

	// Determine vertical range from last `pointsToDraw` boll values
	double dHigh = std::numeric_limits<double>::lowest();
	double dLow = std::numeric_limits<double>::max();
	const size_t startIndex = m_vBoll.size() - pointsToDraw;
	for (size_t i = startIndex; i < m_vBoll.size(); ++i) {
		const auto& b = m_vBoll[i];
		// Skip zero/invalid entries when computing bounds
		if (b.Upper != 0.0) dHigh = std::max(dHigh, b.Upper);
		if (b.Lower != 0.0) dLow = std::min(dLow, b.Lower);
		if (b.Mid != 0.0) {
			dHigh = std::max(dHigh, b.Mid);
			dLow = std::min(dLow, b.Mid);
		}
	}
	if (dHigh <= dLow) {
		// avoid division by zero; provide a small range
		dHigh = dLow + 1.0;
	}

	const int offset = iStepWidth / 2;
	auto toY = [&](double value) -> int {
		// clamp and map value to pixel Y (top..bottom)
		const double ratio = (value - dLow) / (dHigh - dLow);
		const double clamped = std::clamp(ratio, 0.0, 1.0);
		return rectDrawArea.top + static_cast<int>((1.0 - clamped) * rectDrawArea.Height());
	};

	// Helper to draw a series (Upper / Mid / Lower). Draw from newest (right) to oldest (left).
	auto drawSeries = [&](COLORREF color, auto accessor) {
		CPen pen(PS_SOLID, 1, color);
		CPen* pOldPen = (CPen*)pDC->SelectObject(&pen);
		bool started = false;
		for (int j = 0; j < static_cast<int>(pointsToDraw); ++j) {
			const size_t idx = m_vBoll.size() - 1 - j;
			const double v = accessor(m_vBoll[idx]);
			if (v == 0.0) {
				started = false;
				continue;
			}
			const int x = rectDrawArea.right - offset - j * iStepWidth;
			const int y = toY(v);
			if (!started) {
				pDC->MoveTo(x, y);
				started = true;
			}
			else {
				pDC->LineTo(x, y);
			}
			// stop if we've reached left edge
			if (rectDrawArea.right <= (j + 1) * iStepWidth) break;
		}
		pDC->SelectObject(pOldPen);
	};

	// Draw Upper (blue), Mid (yellow), Lower (green)
	drawSeries(RGB(0, 0, 255), [](const CBoll& b) { return b.Upper; });
	drawSeries(RGB(255, 255, 0), [](const CBoll& b) { return b.Mid; });
	drawSeries(RGB(0, 255, 0), [](const CBoll& b) { return b.Lower; });
}
