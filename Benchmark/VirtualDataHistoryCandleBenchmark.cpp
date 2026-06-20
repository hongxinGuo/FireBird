#include "pch.h"

// Note 需要预先消除max的定义
#undef max

#include <benchmark/benchmark.h>

#include "VirtualDataHistoryCandle.h"
#include "VirtualHistoryCandle.h"
#include "TimeConvert.h"

using namespace std;

class CVirtualDataHistoryCandleBenchmark : public benchmark::Fixture {
public:
	void SetUp(const ::benchmark::State& state) override {
		using namespace StockMarket;
		const auto& t = TiingoStockDayline{};

		pStock = make_shared<CTiingoStock>();
		pStock->SetSymbol("AAPL");
		pStock->LoadDayLineDB();
		m_pContainer = pStock->DayLine();

		m_existingDate = toLocalDays(20151009);
		m_missingDate = toLocalDays(20151010);
	}

	void TearDown(const ::benchmark::State& /*state*/) override {
	}

	CTiingoStockPtr pStock;
	CVirtualDataHistoryCandle* m_pContainer;
	chrono::local_days m_existingDate;
	chrono::local_days m_missingDate;
};

// Benchmark lookup for an existing date
BENCHMARK_F(CVirtualDataHistoryCandleBenchmark, GetCandle_Existing)(benchmark::State& state) {
	for (auto _ : state) {
		auto ptr = m_pContainer->GetCandle(m_existingDate);
		benchmark::DoNotOptimize(ptr);
	}
}

// Benchmark lookup for a missing date
BENCHMARK_F(CVirtualDataHistoryCandleBenchmark, GetCandle_Missing)(benchmark::State& state) {
	for (auto _ : state) {
		auto ptr = m_pContainer->GetCandle(m_missingDate);
		benchmark::DoNotOptimize(ptr);
	}
}

BENCHMARK_F(CVirtualDataHistoryCandleBenchmark, GetCandle_Existing2)(benchmark::State& state) {
	for (auto _ : state) {
		auto ptr = m_pContainer->GetCandle2(m_existingDate);
		benchmark::DoNotOptimize(ptr);
	}
}

// Benchmark lookup for a missing date
BENCHMARK_F(CVirtualDataHistoryCandleBenchmark, GetCandle_Missing2)(benchmark::State& state) {
	for (auto _ : state) {
		auto ptr = m_pContainer->GetCandle2(m_missingDate);
		benchmark::DoNotOptimize(ptr);
	}
}
