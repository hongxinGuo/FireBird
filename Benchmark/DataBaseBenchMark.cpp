#include"pch.h"

// Note 需要预先消除max的定义
#undef max

#include<benchmark/benchmark.h>

#include "ChinaMarket.h"
#include "dataBaseConnector.h"

#include"ContainerTiingoStock.h"
#include "WorldMarket.h"

class CDataBaseBenchmark : public benchmark::Fixture {
public:
	void SetUp(const ::benchmark::State& state) override {
		//Todo: benchmark目前使用工作数据库，考虑改用测试数据库，避免对正式环境的数据库造成影响。
		InitSqlppMySQLConnectionPool("Test", "test", "stock_market_test", "localhost", 3306, 20, false);
		if (gl_pChinaMarket == nullptr) gl_pChinaMarket = make_shared<CChinaMarket>();
		if (gl_pWorldMarket == nullptr) gl_pWorldMarket = make_shared<CWorldMarket>();
	}

	void TearDown(const ::benchmark::State& state) override {
	}

	CContainerTiingoStock tiingoContainer;
	CContainerChinaStock chinaContainer;
	CContainerTiingoStockDayLine tiingoDayLineContainer;
	CContainerFinnhubStock finnhubStockContainer;
};

BENCHMARK_F(CDataBaseBenchmark, LoadChinaStockProfileDB_using_mfc_record)(benchmark::State& state) {
	for (auto _ : state) {
		chinaContainer.LoadProfileDB2();
	}
}

BENCHMARK_F(CDataBaseBenchmark, LoadChinaStockProfileDB_using_sqlpp11)(benchmark::State& state) {
	for (auto _ : state) {
		chinaContainer.LoadProfileDB();
	}
}

BENCHMARK_F(CDataBaseBenchmark, LoadFinnhubStockDayLineDB_using_mfc_record)(benchmark::State& state) {
	for (auto _ : state) {
		finnhubStockContainer.LoadProfileDB2();
	}
}

BENCHMARK_F(CDataBaseBenchmark, LoadFinnhubStockDayLineDB_using_sqlpp11)(benchmark::State& state) {
	for (auto _ : state) {
		finnhubStockContainer.LoadProfileDB();
	}
}
