#include"pch.h"

#include "IndicatorBoll.h"
#include "VirtualHistoryCandle.h" // CVirtualHistoryCandle
#include <deque>

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
void CIndicatorBoll::ToShow(CDC* pDC, CRect rectDrawArea, int iStepWidth) {
}
