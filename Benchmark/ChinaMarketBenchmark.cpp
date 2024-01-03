#include"pch.h"

#include<benchmark/benchmark.h>

#include"ChinaMarket.h"

using std::make_shared;

class CChinaMarketBenchmark : public benchmark::Fixture {
public:
	void SetUp(const ::benchmark::State& state) override {}

	void TearDown(const ::benchmark::State& state) override {}
};

BENCHMARK_F(CChinaMarketBenchmark, CalculateTime)(benchmark::State& state) {
	for (auto _ : state) {
		gl_pChinaMarket->CalculateTime();
	}
}
