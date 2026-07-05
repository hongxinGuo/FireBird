#include"pch.h"

// Note 需要预先消除max的定义
#undef max

#include<benchmark/benchmark.h>

#include"IndicatorMACD.h"

class CIndicatorMACDBenchmark : public benchmark::Fixture {
public:
	void SetUp(const ::benchmark::State& state) override {
		pContainer = make_shared<CContainerTiingoStockDayLine>();
		pContainer->LoadDB("AAPL");
		pContainer->SplitAdjust();
		macd.SetCandle(pContainer);
	}

	void TearDown(const ::benchmark::State& state) override {
	}

	CContainerTiingoStockDayLinePtr pContainer;
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
