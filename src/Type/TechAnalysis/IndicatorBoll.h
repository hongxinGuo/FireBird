#pragma once

class CVirtualHistoryCandle; // forward

struct CBoll {
	chrono::local_days Date{ chrono::local_days(chrono::days(0)) };        // candle date
	size_t Index{ 0 };     // index in the input sequence
	double Mid{ 0.0 };     // moving average
	double Upper{ 0.0 };   // upper band
	double Lower{ 0.0 };   // lower band
	double BandWidth{ 0.0 }; // (Upper - Lower) / Mid (if Mid != 0)
	double PercentB{ 0.0 };  // (Price - Lower) / (Upper - Lower) (if denom != 0)
};

namespace FireBird::Indicators {
	class CBollingerBandsCalculator {
	public:
		// Compute Bollinger Bands from a vector of candles.
		// - Uses the candle's GetClose() as the price (preserves same units as stored in the candles).
		// - period: moving average window (commonly 20)
		// - k: multiplier for standard deviation (commonly 2.0)
	};
} // namespace FireBird::Indicators

class CIndicatorBoll {
public:
	CIndicatorBoll() = default;
	CIndicatorBoll(int period) { m_Period = period; }
	CIndicatorBoll(const CIndicatorBoll&) = delete;
	CIndicatorBoll& operator=(const CIndicatorBoll&) = delete;
	CIndicatorBoll(CIndicatorBoll&&) noexcept = delete;
	CIndicatorBoll& operator=(CIndicatorBoll&&) noexcept = delete;
	~CIndicatorBoll() = default;

	void SetCandle(CVirtualDataHistoryCandle* pCandle) { m_pvCandle = pCandle; }
	static std::vector<CBoll> Compute(const std::vector<CVirtualHistoryCandle>& candles, int period = 20, double k = 2.0);

	// Convenience overload for shared pointer to vector (existing codebase uses shared vectors)
	static std::vector<CBoll> Compute(const std::shared_ptr<std::vector<CVirtualHistoryCandle>>& candlesPtr, int period = 20, double k = 2.0);

	void Calculate();

	void ToShow(CDC* pDC, CRect rectDrawArea, int iStepWidth);

protected:
	int m_Period{ 6 }; // 默认九天为一个周期

	CVirtualDataHistoryCandle* m_pvCandle{ nullptr };
	vector<CBoll> m_vBoll;
};
