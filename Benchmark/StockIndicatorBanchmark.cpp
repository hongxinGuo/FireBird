#include"pch.h"

// Note 需要预先消除max的定义
#undef max

#include<benchmark/benchmark.h>

#include"IndicatorMACD.h"

class CIndicatorMACDBenchmark : public benchmark::Fixture {
public:
	void SetUp(const ::benchmark::State& state) override {
		stock.SetSymbol("AAPL");
		stock.LoadDayLineDB();
		macd.SetCandle(stock.DayLine());
	}

	void TearDown(const ::benchmark::State& state) override {
	}

	CTiingoStock stock;
	CIndicatorMACD macd;
};

BENCHMARK_F(CIndicatorMACDBenchmark, CalculateMACD1)(benchmark::State& state) {
	for (auto _ : state) {
		macd.Calculate();
	}
}

BENCHMARK_F(CIndicatorMACDBenchmark, CalculateMACD2)(benchmark::State& state) {
	for (auto _ : state) {
		macd.Calculate2();
	}
}
