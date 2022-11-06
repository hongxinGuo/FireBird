#include"pch.h"

#include<benchmark/benchmark.h>

#include"SaveAndLoad.h"

#include"Thread.h"
#include"JsonParse.h"

#include"WebData.h"
#include"WebRTData.h"

using namespace std;
#include<memory>
#include<string>

class CParseNeteaseRTDataWithNlohmannJsonBenchmark : public benchmark::Fixture {
public:
	void SetUp(const ::benchmark::State& state) {
		LoadFromFile(_T("C:\\StockAnalysis\\Benchmark Test Data\\NeteaseRTData.json"), s);
		try {
			js = json::parse(s.begin() + 21, s.end() - 2);
		}
		catch (json::parse_error& e) {
			fDone = false;
		}
	}

	void TearDown(const ::benchmark::State& state) {
	}
	string s;
	json js; // �˴�����ʹ������ָ�룬��������������⣬ԭ������
	vector<CWebRTDataPtr> vWebRTDataReceived;
	bool fDone;
};

// ����nlohmann json����NeteaseRTData���ٶ�
BENCHMARK_F(CParseNeteaseRTDataWithNlohmannJsonBenchmark, ParseNeteaseRTDataBenchmark1)(benchmark::State& state) {
	for (auto _ : state) {
		ParseNeteaseRTData(&js, vWebRTDataReceived);
	}
}

class CParseNeteaseRTDataWithPTreeBenchmark : public benchmark::Fixture {
public:
	void SetUp(const ::benchmark::State& state) {
		LoadFromFile(_T("C:\\StockAnalysis\\Benchmark Test Data\\NeteaseRTData.json"), s);
		s.resize(s.size() - 2);
		s.erase(s.begin(), s.begin() + 21);
		fDone = ParseWithPTree(pt, s);
	}

	void TearDown(const ::benchmark::State& state) {
	}
	string s;
	ptree pt; // �˴�����ʹ������ָ�룬��������������⣬ԭ������
	vector<CWebRTDataPtr> vWebRTDataReceived;
	bool fDone;
};

// ����boost ptree����NeteaseRTData���ٶ�
BENCHMARK_F(CParseNeteaseRTDataWithPTreeBenchmark, ParseNeteaseRTDataBenchmark2)(benchmark::State& state) {
	for (auto _ : state) {
		ParseNeteaseRTData(&pt, vWebRTDataReceived);
	}
}