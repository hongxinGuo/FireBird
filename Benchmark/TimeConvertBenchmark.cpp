#include"pch.h"

#include<benchmark/benchmark.h>

#include"TimeConvert.h"

#include<memory>
using std::make_shared;

class CTimeConvertBenchmark : public benchmark::Fixture {
public:
	void SetUp(const ::benchmark::State& state) override { }

	void TearDown(const ::benchmark::State& state) override { }

protected:
};

BENCHMARK_F(CTimeConvertBenchmark, GetNextSecond)(benchmark::State& state) {
	for (auto _ : state) {
		long lTime = GetNextSecond(125959);
	}
}
