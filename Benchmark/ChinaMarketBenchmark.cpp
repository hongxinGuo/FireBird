#include"pch.h"

#undef max

#include<benchmark/benchmark.h>

#include"ChinaStockCodeConverter.h"
#include"ChinaMarket.h"

#include"TiingoStock.h"

class CChinaMarketBenchmark : public benchmark::Fixture {
public:
	void SetUp(const ::benchmark::State& state) override {
		if (gl_pChinaMarket == nullptr) gl_pChinaMarket = make_shared<CChinaMarket>();
	}

	void TearDown(const ::benchmark::State& state) override {
	}
};

BENCHMARK_F(CChinaMarketBenchmark, XferSinaToStandard)(benchmark::State& state) {
	const string svData{ "sh600024" };
	for (auto _ : state) {
		string sResult = XferSinaToStandard(svData);
	}
}

class CTiingoStockBenchmark : public benchmark::Fixture {
public:
	void SetUp(const ::benchmark::State& state) override {
		if (gl_pChinaMarket == nullptr) gl_pChinaMarket = make_shared<CChinaMarket>();
	}

	void TearDown(const ::benchmark::State& state) override {
	}
};

BENCHMARK_F(CTiingoStockBenchmark, Get)(benchmark::State& state) {
	CTiingoStock stock;
	stock.SetTiingoPermaTicker("abcdefghijklomop");
	for (auto _ : state) {
		auto s = stock.GetTiingoPermaTicker();
	}
}
BENCHMARK_F(CTiingoStockBenchmark, Get2)(benchmark::State& state) {
	CTiingoStock stock;
	stock.SetTiingoPermaTicker("abcdefghijklomop");
	for (auto _ : state) {
		auto s = stock.GetTiingoPermaTicker2();
	}
}
