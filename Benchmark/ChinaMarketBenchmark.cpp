#include"pch.h"

#include<benchmark/benchmark.h>
import FireBird.System.Globedef;

import FireBird.Market.China;

class CChinaMarketBenchmark : public benchmark::Fixture {
public:
	void SetUp(const ::benchmark::State& state) override {
		if (gl_pChinaMarket == nullptr) gl_pChinaMarket = make_shared<CChinaMarket>();
	}

	void TearDown(const ::benchmark::State& state) override {
	}
};
