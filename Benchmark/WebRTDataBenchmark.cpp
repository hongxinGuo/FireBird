#include"pch.h"

#include<benchmark/benchmark.h>

#include"WebData.h"
#include"WebRTData.h"

using std::make_shared;

CString sSinaWebData = _T("var hq_str_sh600000=\"浦发银行,11.510,11.490,11.560,11.570,11.440,11.540,11.550,21606007,248901949.000,19900,11.540,54700,11.530,561500,11.520,105600,11.510,172400,11.500,259981,11.550,206108,11.560,325641,11.570,215109,11.580,262900,11.590,2019-07-16,15:00:00,00,\";\n");
CString sTengxunWebData = _T("v_sh600000=\"1~浦发银行~600000~12.45~11.96~12.05~920308~515001~405306~12.44~938~12.43~535~12.42~435~12.41~784~12.40~1167~12.45~494~12.46~9397~12.47~5156~12.48~7473~12.49~5513~~20191011155858~0.49~4.10~12.45~12.00~12.45/920308/1131441679~920308~113144~0.33~6.15~~12.45~12.00~3.76~3498.92~3654.33~0.79~13.16~10.76~2.63~-24176~12.29~5.69~6.54~~~0.73~113144.17~0.00~0~~GP-A~31.75~~2.81\";\n");

class CWebRTDataBenchmark1 : public benchmark::Fixture {
public:
	void SetUp(const ::benchmark::State& state) override {
		m_pSinaWebData = make_shared<CWebData>();
		long lStringLength = sSinaWebData.GetLength();
		m_pSinaWebData->Resize(lStringLength);
		m_pSinaWebData->SetData(sSinaWebData.GetBuffer(), lStringLength);

		m_pTengxunWebData = make_shared<CWebData>();
		lStringLength = sTengxunWebData.GetLength();
		m_pTengxunWebData->Resize(lStringLength);
		m_pTengxunWebData->SetData(sTengxunWebData.GetBuffer(), lStringLength);
	}

	void TearDown(const ::benchmark::State& state) override {}

	CWebDataPtr m_pSinaWebData;
	CWebDataPtr m_pTengxunWebData;
	CWebRTData m_RTData;
};

BENCHMARK_F(CWebRTDataBenchmark1, ReadSinaData1)(benchmark::State& state) {
	for (auto _ : state) {
		m_pSinaWebData->ResetCurrentPos();
		m_RTData.ReadSinaData(m_pSinaWebData);
	}
}

BENCHMARK_F(CWebRTDataBenchmark1, ReadTengxunData1)(benchmark::State& state) {
	for (auto _ : state) {
		m_pTengxunWebData->ResetCurrentPos();
		m_RTData.ReadTengxunData(m_pTengxunWebData);
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
