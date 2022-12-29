#include"pch.h"

#include<benchmark/benchmark.h>

#include"FinnhubInaccessibleExchange.h"
#include"FinnhubInquiryType.h"

#include<memory>
using std::make_shared;

class CFinnhubInaccessibleExchangeBenchmark : public benchmark::Fixture {
public:
	void SetUp(const ::benchmark::State& state) override { }

	void TearDown(const ::benchmark::State& state) override { }

	CFinnhubInaccessibleExchangePtr m_pInaccessibleExchange;
};

BENCHMARK_F(CFinnhubInaccessibleExchangeBenchmark, IsInaccessible1)(benchmark::State& state) {
	for (auto _ : state) {
		gl_finnhubInaccessibleExchange.IsInaccessible(BASIC_FINANCIALS_, _T("SS"));
	}
}

BENCHMARK_F(CFinnhubInaccessibleExchangeBenchmark, IsInaccessible2)(benchmark::State& state) {
	for (auto _ : state) {
		gl_finnhubInaccessibleExchange.IsInaccessible(BASIC_FINANCIALS_, _T("A"));
	}
}

BENCHMARK_F(CFinnhubInaccessibleExchangeBenchmark, IsInaccessible3)(benchmark::State& state) {
	for (auto _ : state) {
		gl_finnhubInaccessibleExchange.IsInaccessible(PEERS_, _T("SS"));
	}
}
