#include"pch.h"

// Note 需要预先消除max的定义
#undef max

#include<benchmark/benchmark.h>

class CTiingoStockDayLineBenchmark : public benchmark::Fixture {
public:
	void SetUp(const ::benchmark::State& state) override {
		stock.SetSymbol("AAPL");
		stock.LoadDayLineDB();
		stock.SetDayLineEndDate(chrono::local_days(1980y / 01 / 01));
		stock.SetUpdateDayLineDB(true);
	}

	void TearDown(const ::benchmark::State& state) override {
	}

	CTiingoStock stock;
};

//Note: 由于数据库操作的不可控性，以下测试代码仅用于性能测试，实际执行时请根据需要取消注释。
BENCHMARK_F(CTiingoStockDayLineBenchmark, Delete_And_Save)(benchmark::State& state) {
	for (auto _ : state) {
		//stock.DeleteDuplicatedDayLine();
		//stock.SaveDayLineDB();
	}
}

BENCHMARK_F(CTiingoStockDayLineBenchmark, Update)(benchmark::State& state) {
	for (auto _ : state) {
		//stock.UpdateDayLineDB();
	}
}
