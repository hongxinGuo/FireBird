#include "pch.h"
#include "GeneralCheck.h"
#include "IndicatorMACD.h"
#include "ContainerChinaDayLine.h"
#include "DayLine.h"

namespace FireBirdTest {
	class CIndicatorMACDTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		static void TearDownTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void TearDown() override {
			SCOPED_TRACE("");
			GeneralCheck();
		}
	};

	// Test helper: subclass to expose results for assertions
	class TestableIndicatorMACD : public CIndicatorMACD {
	public:
		using CIndicatorMACD::CIndicatorMACD;
		std::vector<MACDResult>& Results() { return m_vMACD; }
	};

	// Helper: compute EMA series using the same initialization logic as production.
	static std::vector<double> ComputeEMAMatchingImpl(const std::vector<double>& closes, int period) {
		std::size_t n = closes.size();
		std::vector<double> ema(n, std::numeric_limits<double>::quiet_NaN());
		if (n == 0) return ema;
		double alpha = 2.0 / (period + 1.0);

		if (n >= static_cast<std::size_t>(period)) {
			// set SMA at index period-1
			double s = 0.0;
			for (int i = 0; i < period; ++i) s += closes[i];
			ema[period - 1] = s / static_cast<double>(period);
			for (std::size_t i = period; i < n; ++i)
				ema[i] = alpha * closes[i] + (1.0 - alpha) * ema[i - 1];
		}
		else {
			// fallback: initialize first point then iterate
			ema[0] = closes[0];
			for (std::size_t i = 1; i < n; ++i)
				ema[i] = alpha * closes[i] + (1.0 - alpha) * ema[i - 1];
		}
		return ema;
	}

	TEST_F(CIndicatorMACDTest, Calculate_SimpleSeries) {
		CContainerChinaDayLine container;

		// Build a simple increasing close series
		std::vector<CDayLine> days;
		std::vector<long> closesLong = { 10, 11, 12, 13, 14, 15, 16, 17, 18 };
		std::vector<double> closes;
		for (auto c : closesLong) {
			CDayLine d;
			d.SetHigh(c + 1);
			d.SetLow(c - 1);
			d.SetClose(c);
			d.SetRatio(container.GetRatio());
			days.push_back(d);
			closes.push_back(static_cast<double>(c) / container.GetRatio());
		}
		container.UpdateData(days);

		int fastPeriod = 3;
		int slowPeriod = 6;
		int signalPeriod = 4;

		TestableIndicatorMACD indicator(fastPeriod, slowPeriod, signalPeriod);
		indicator.SetCandle(&container);
		indicator.Calculate(fastPeriod, slowPeriod, signalPeriod);

		auto& res = indicator.Results();
		ASSERT_EQ(res.size(), container.Size());

		// Compute expected fast/slow EMAs with production initialization rules
		auto emaFast = ComputeEMAMatchingImpl(closes, fastPeriod);
		auto emaSlow = ComputeEMAMatchingImpl(closes, slowPeriod);

		// compute macdLine: valid only where both fast & slow EMA are not NaN
		std::size_t n = closes.size();
		std::vector<double> macdLine(n, std::numeric_limits<double>::quiet_NaN());
		for (std::size_t i = 0; i < n; ++i) {
			if (!std::isnan(emaFast[i]) && !std::isnan(emaSlow[i]))
				macdLine[i] = emaFast[i] - emaSlow[i];
		}

		// compute expected signalEMA following the same logic as implementation
		std::vector<double> signal(n, std::numeric_limits<double>::quiet_NaN());
		double alphaSignal = 2.0 / (signalPeriod + 1.0);

		// find first valid macd index
		std::size_t firstMacdIdx = n;
		for (std::size_t i = 0; i < n; ++i) {
			if (!std::isnan(macdLine[i])) {
				firstMacdIdx = i;
				break;
			}
		}

		if (firstMacdIdx != n) {
			if (n - firstMacdIdx >= static_cast<std::size_t>(signalPeriod)) {
				double s = 0.0;
				for (std::size_t i = 0; i < static_cast<std::size_t>(signalPeriod); ++i) s += macdLine[firstMacdIdx + i];
				signal[firstMacdIdx + signalPeriod - 1] = s / static_cast<double>(signalPeriod);
				for (std::size_t i = firstMacdIdx + signalPeriod; i < n; ++i)
					if (!std::isnan(macdLine[i]))
						signal[i] = alphaSignal * macdLine[i] + (1.0 - alphaSignal) * signal[i - 1];
			}
			else {
				signal[firstMacdIdx] = macdLine[firstMacdIdx];
				for (std::size_t i = firstMacdIdx + 1; i < n; ++i)
					if (!std::isnan(macdLine[i]))
						signal[i] = alphaSignal * macdLine[i] + (1.0 - alphaSignal) * signal[i - 1];
			}
		}

		const double eps = 1e-5;

		// Validate values according to production behavior:
		// - indices before slow EMA available -> macd NaN
		// - indices where macd exists but signal not yet defined -> macd finite, signal NaN, hist NaN
		// - indices where both defined -> compare numeric values; hist in implementation is 2*(macd - signal)
		// Determine slow EMA start index:
		std::size_t slowStart = (closes.size() >= static_cast<std::size_t>(slowPeriod)) ? (slowPeriod - 1) : 0;
		std::size_t firstSignalDefined = n; // index where signal becomes non-NaN

		for (std::size_t i = 0; i < n; ++i) {
			if (!std::isnan(signal[i])) {
				firstSignalDefined = i;
				break;
			}
		}

		for (std::size_t i = 0; i < n; ++i) {
			if (i < slowStart) {
				EXPECT_TRUE(std::isnan(res[i].m_macd));
				EXPECT_TRUE(std::isnan(res[i].m_signal));
				EXPECT_TRUE(std::isnan(res[i].m_hist));
			}
			else {
				// macd should be finite where both EMAs are available
				if (!std::isnan(macdLine[i])) {
					EXPECT_TRUE(std::isfinite(res[i].m_macd));
				}
				else {
					EXPECT_TRUE(std::isnan(res[i].m_macd));
				}

				if (i < firstSignalDefined) {
					// signal and hist not yet computed
					EXPECT_TRUE(std::isnan(res[i].m_signal));
					EXPECT_TRUE(std::isnan(res[i].m_hist));
				}
				else {
					// compare numeric values at and after firstSignalDefined
					double expectedMACD = macdLine[i];
					double expectedSignal = signal[i];
					double expectedHist = 2.0 * (expectedMACD - expectedSignal);

					EXPECT_NEAR(res[i].m_macd, expectedMACD, eps);
					EXPECT_NEAR(res[i].m_signal, expectedSignal, eps);
					EXPECT_NEAR(res[i].m_hist, expectedHist, eps);
				}
			}
		}
	}

	TEST_F(CIndicatorMACDTest, HandleFlatClose_MACDZero) {
		CContainerChinaDayLine container;
		std::vector<CDayLine> days;
		std::vector<double> closes;

		for (int i = 0; i < 10; ++i) {
			CDayLine d;
			d.SetHigh(100);
			d.SetLow(100);
			d.SetClose(100);
			d.SetRatio(1);
			days.push_back(d);
			closes.push_back(100.0);
		}
		container.UpdateData(days);

		int fastPeriod = 3, slowPeriod = 6, signalPeriod = 4;
		TestableIndicatorMACD indicator(fastPeriod, slowPeriod, signalPeriod);
		indicator.SetCandle(&container);
		indicator.Calculate(fastPeriod, slowPeriod, signalPeriod);

		auto& res = indicator.Results();
		ASSERT_EQ(res.size(), container.Size());

		// Compute expected fast/slow EMAs and macdLine
		auto emaFast = ComputeEMAMatchingImpl(closes, fastPeriod);
		auto emaSlow = ComputeEMAMatchingImpl(closes, slowPeriod);

		std::size_t n = closes.size();
		std::vector<double> macdLine(n, std::numeric_limits<double>::quiet_NaN());
		for (std::size_t i = 0; i < n; ++i) {
			if (!std::isnan(emaFast[i]) && !std::isnan(emaSlow[i]))
				macdLine[i] = emaFast[i] - emaSlow[i];
		}

		// macdLine should be (approximately) zero where defined
		for (std::size_t i = 0; i < n; ++i) {
			if (!std::isnan(macdLine[i])) {
				EXPECT_NEAR(macdLine[i], 0.0, 1e-12);
				EXPECT_TRUE(std::isfinite(res[i].m_macd));
				EXPECT_NEAR(res[i].m_macd, 0.0, 1e-8);
			}
			else {
				EXPECT_TRUE(std::isnan(res[i].m_macd));
			}
		}

		// Signal + histogram will only be defined at/after the index computed by the implementation.
		// Find first signal index in result
		std::size_t firstSignalIdx = n;
		for (std::size_t i = 0; i < n; ++i)
			if (!std::isnan(res[i].m_signal)) {
				firstSignalIdx = i;
				break;
			}

		if (firstSignalIdx < n) {
			// At indices where signal is defined, it should be ~0 and histogram ~0
			for (std::size_t i = firstSignalIdx; i < n; ++i) {
				if (!std::isnan(res[i].m_signal)) {
					EXPECT_NEAR(res[i].m_signal, 0.0, 1e-8);
				}
				if (!std::isnan(res[i].m_hist)) {
					EXPECT_NEAR(res[i].m_hist, 0.0, 1e-8);
				}
			}
		}
	}
}
