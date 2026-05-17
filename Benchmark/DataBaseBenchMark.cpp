#include"pch.h"

// Note 需要预先消除max的定义
#undef max

#include<benchmark/benchmark.h>

#include "dataBaseConnector.h"

#include"ContainerTiingoStock.h"

class CDataBaseBenchmark : public benchmark::Fixture {
public:
	void SetUp(const ::benchmark::State& state) override {
		InitSqlppConnectionPool("FireBird", "firebird", "stock_market", "localhost", 5, true);
	}

	void TearDown(const ::benchmark::State& state) override {
	}

	CContainerTiingoStock container;
};

BENCHMARK_F(CDataBaseBenchmark, LoadProfileDB_using_mfc_record)(benchmark::State& state) {
	for (auto _ : state) {
		container.LoadProfileDB2();
		container.Reset();
	}
}

BENCHMARK_F(CDataBaseBenchmark, LoadProfileDB_using_sqlpp11)(benchmark::State& state) {
	for (auto _ : state) {
		container.LoadProfileDB();
	}
}
