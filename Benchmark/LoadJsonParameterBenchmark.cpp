#include"pch.h"

#include<benchmark/benchmark.h>

#include"VirtualDataSource.h"

using std::make_shared;

class CLoadJsonParameterBenchmark : public benchmark::Fixture {
public:
	void SetUp(const ::benchmark::State& state) override {}

	void TearDown(const ::benchmark::State& state) override {}

protected:
	CVirtualDataSource dataSource;
};
