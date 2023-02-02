#include"pch.h"

#include<benchmark/benchmark.h>

#include"VirtualDataSource.h"

#include<memory>
using std::make_shared;

class CDataSourceBenchmark : public benchmark::Fixture {
public:
	void SetUp(const ::benchmark::State& state) override { }

	void TearDown(const ::benchmark::State& state) override { }

protected:
	CVirtualDataSource dataSource;
};

BENCHMARK_F(CDataSourceBenchmark, XferReadingToBuffer)(benchmark::State& state) {
	for (auto _ : state) {
		dataSource.XferReadingToBuffer(0, 1024 * 16);
	}
}
