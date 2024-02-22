#include"pch.h"

#include<benchmark/benchmark.h>

#include"VirtualDataSource.h"

using std::make_shared;

class CDataSourceBenchmark : public benchmark::Fixture {
public:
	void SetUp(const ::benchmark::State& state) override {
		dataSource.ResetBuffer(WEB_SOURCE_DATA_BUFFER_SIZE_);
	}

	void TearDown(const ::benchmark::State& state) override {
	}

protected:
	CVirtualDataSource dataSource;
};

BENCHMARK_F(CDataSourceBenchmark, XferReadingToBufferSize16KB)(benchmark::State& state) {
	for (auto _ : state) {
		dataSource.XferReadingToBuffer(WEB_SOURCE_DATA_BUFFER_SIZE_);
	}
}

char destBuffer[1024 * WEB_SOURCE_DATA_BUFFER_SIZE_];
char srcBuffer[1024 * WEB_SOURCE_DATA_BUFFER_SIZE_];
BENCHMARK_F(CDataSourceBenchmark, XferReadingToBufferSize16MBUsingMemcpy)(benchmark::State& state) {
	for (auto _ : state) {
		memcpy(destBuffer, srcBuffer, 1024 * WEB_SOURCE_DATA_BUFFER_SIZE_);
	}
}
