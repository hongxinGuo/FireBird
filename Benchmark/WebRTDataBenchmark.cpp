#include"pch.h"

#include<benchmark/benchmark.h>

#include"WebData.h"
#include"WebRTData.h"

CString sSinaWebData = _T("var hq_str_sh600000=\"ÆÖ·¢ÒøÐÐ,11.510,11.490,11.560,11.570,11.440,11.540,11.550,21606007,248901949.000,19900,11.540,54700,11.530,561500,11.520,105600,11.510,172400,11.500,259981,11.550,206108,11.560,325641,11.570,215109,11.580,262900,11.590,2019-07-16,15:00:00,00\";\n");

class CWebRTDataBenchmark1 : public benchmark::Fixture {
public:
	void SetUp(const ::benchmark::State& state) override {
		m_pSinaWebData = make_shared<CWebData>();
		const long lStringLength = sSinaWebData.GetLength();
		m_pSinaWebData->SetData(sSinaWebData.GetBuffer(), lStringLength, 0);
		m_pSinaWebData->Resize(lStringLength);
	}

	void TearDown(const ::benchmark::State& state) override {
	}

	CWebDataPtr m_pSinaWebData;
	CWebRTData m_RTData;
};

BENCHMARK_F(CWebRTDataBenchmark1, ReadSinaData1)(benchmark::State& state) {
	for (auto _ : state) {
		m_pSinaWebData->ResetCurrentPos();
		m_RTData.ReadSinaData(m_pSinaWebData);
	}
}

/*
BENCHMARK_DEFINE_F(CWebRTDataBenchmark, ReadSinaDataBenchmark)(benchmark::State& state) {
	for (auto _ : state) {
	}
}

// BarTest is NOT registered
BENCHMARK_REGISTER_F(CWebRTDataBenchmark, BarTest)->Threads(2);
// BarTest is now registered
*/
