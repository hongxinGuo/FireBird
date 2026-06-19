#include "pch.h"
#include "GeneralCheck.h"
#include "IndicatorKDJ.h"
#include "ContainerChinaDayLine.h"
#include "DayLine.h"

namespace FireBirdTest {
	class CIndicatorKDJTest : public ::testing::Test {
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
	class TestableIndicatorKDJ : public CIndicatorKDJ {
	public:
		using CIndicatorKDJ::CIndicatorKDJ;
		std::vector<Kdj>& Results() { return m_vKDJ; }
	};

	// Utility to compute expected RSV over a sliding window (mirrors production logic)
	static double ComputeRSV(CContainerChinaDayLine& container, size_t index, int period) {
		auto data = container.GetData(index);
		long long lHigh = 0;
		long long lLow = data->GetLow();
		for (size_t i = index - period + 1; i <= index; ++i) {
			auto d = container.GetData(i);
			lHigh = max(lHigh, d->GetHigh());
			lLow = min(lLow, d->GetLow());
		}
		if (lHigh == 0 || lHigh == lLow) return 50.0;
		return static_cast<double>(data->GetClose() - lLow) * 100.0 / (lHigh - lLow);
	}

	TEST_F(CIndicatorKDJTest, Calculate_SimpleSeries) {
		CContainerChinaDayLine container;

		// Build a short series of daily candles
		std::vector<CDayLine> days;
		struct Raw {
			long h, l, c;
		};
		std::vector<Raw> raws = {
			{ 10, 5, 7 },
			{ 12, 6, 9 },
			{ 14, 7, 10 },
			{ 16, 8, 12 },
			{ 18, 9, 15 }
		};
		for (auto& r : raws) {
			CDayLine d;
			d.SetHigh(r.h);
			d.SetLow(r.l);
			d.SetClose(r.c);
			d.SetRatio(1);
			days.push_back(d);
		}
		// Feed the container
		container.UpdateData(days);

		// Use period = 3 to make hand-calculation manageable
		TestableIndicatorKDJ indicator(3);
		indicator.SetCandle(&container);
		indicator.Calculate();

		auto& res = indicator.Results();
		ASSERT_EQ(res.size(), container.Size());

		// Entries before period-1 should remain the initialized 50
		EXPECT_DOUBLE_EQ(res[0].m_K, 50.0);
		EXPECT_DOUBLE_EQ(res[0].m_D, 50.0);
		EXPECT_DOUBLE_EQ(res[1].m_K, 50.0);
		EXPECT_DOUBLE_EQ(res[1].m_D, 50.0);

		// Compute expected RSV/K/D/J for index = period-1 (2) and index 3 and compare
		const double eps = 1e-6;
		for (size_t idx = 2; idx <= 3; ++idx) {
			double expectedRSV = ComputeRSV(container, idx, 3);

			// previous K/D values used by algorithm:
			double prevK = res[idx - 1].m_K;
			double prevD = res[idx - 1].m_D;
			// For idx==2 prev values are the initialized 50. For idx>2 we use previously computed res entries.

			double expectedK = (prevK * 2.0 + expectedRSV) / 3.0;
			double expectedD = (prevD * 2.0 + expectedK) / 3.0;
			double expectedJ = expectedK * 3.0 - expectedD * 2.0;

			EXPECT_NEAR(res[idx].m_RSV, expectedRSV, eps);
			EXPECT_NEAR(res[idx].m_K, expectedK, 1e-8);
			EXPECT_NEAR(res[idx].m_D, expectedD, 1e-8);
			EXPECT_NEAR(res[idx].m_J, expectedJ, 1e-8);
		}
	}

	TEST_F(CIndicatorKDJTest, HandleFlatRange_RSVBecomes50) {
		CContainerChinaDayLine container;
		std::vector<CDayLine> days;

		// Create a series where high == low over the window -> RSV should be 50
		for (int i = 0; i < 6; ++i) {
			CDayLine d;
			d.SetHigh(100);
			d.SetLow(100);   // high == low
			d.SetClose(100);
			d.SetRatio(1);
			days.push_back(d);
		}
		container.UpdateData(days);

		TestableIndicatorKDJ indicator(3);
		indicator.SetCandle(&container);
		indicator.Calculate();

		auto& res = indicator.Results();
		ASSERT_EQ(res.size(), container.Size());

		// For any computed index RSV must be 50 and K/D/J compute accordingly
		for (size_t idx = 2; idx < res.size(); ++idx) {
			EXPECT_DOUBLE_EQ(res[idx].m_RSV, 50.0);
			// K = (prevK*2 + 50)/3, etc. We'll check values are finite and within reasonable range
			EXPECT_TRUE(std::isfinite(res[idx].m_K));
			EXPECT_TRUE(std::isfinite(res[idx].m_D));
			EXPECT_TRUE(std::isfinite(res[idx].m_J));
		}
	}
}
