#include"pch.h"

#include<benchmark/benchmark.h>

#include "ChinaMarket.h"
#include "dataBaseConnector.h"

#include "WorldMarket.h"

using namespace std;

class CDataBaseBenchmark : public benchmark::Fixture {
public:
	void SetUp(const ::benchmark::State& state) override {
		InitSqlppMySQLConnectionPool("Test", "test", "stock_market_test");
		if (gl_pChinaMarket == nullptr) gl_pChinaMarket = make_shared<CChinaMarket>();
		if (gl_pWorldMarket == nullptr) gl_pWorldMarket = make_shared<CWorldMarket>();
	}

	void TearDown(const ::benchmark::State& state) override {
	}
};
