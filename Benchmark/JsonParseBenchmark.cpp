//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// FireBird中比较费时的函数是各个数据的解析工作。
// 目前最繁重的解析工作，时US market的美国市场股票代码，其大小为5MB，使用nlohmann json解析时，release模式用时131毫秒；使用boost PTree
// 解析时，release模式用时320毫秒。
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include<benchmark/benchmark.h>

#include"nlohmannJSonDeclaration.h"

#include"SaveAndLoad.h"
#include"JsonParse.h"
#include"WebData.h"
#include"WebRTData.h"
#include"ChinaMarket.h"

// 这个是目前能够找到的最大的json数据，用于测试ParseWithPTree和ParseWithNlohmannJson的速度
// 测试结果是Nlohmann json的速度比boost的Ptree快50%左右。
// 使用下面的数据，nlohmann json的release版本用时大致为250微秒；PTree用时大致为330微秒。
std::string sData101 = _T("{\
		\"metric\": { \
			\"10DayAverageTradingVolume\": 0.43212,\
			\"13WeekPriceReturnDaily\" : 56.53409,\
			\"26WeekPriceReturnDaily\" : 39.84772,\
			\"3MonthAverageTradingVolume\" : 4.08133,\
			\"52WeekHigh\" : 5.5699,\
			\"52WeekHighDate\" : \"2022-04-08\",\
			\"52WeekLow\" : 3.0699,\
			\"52WeekLowDate\" : \"2021-08-20\",\
			\"52WeekPriceReturnDaily\" : 30.87886,\
			\"5DayPriceReturnDaily\" : 20.04357,\
\
			\"assetTurnoverAnnual\" : 0.26072,\
			\"assetTurnoverTTM\" : 0.26072,\
\
			\"beta\" : 0.60752,\
			\"bookValuePerShareAnnual\" : 9.57393,\
			\"bookValuePerShareQuarterly\" : 9.57393,\
			\"bookValueShareGrowth5Y\" : -4.75925,\
\
			\"capitalSpendingGrowth5Y\" : -56.49411,\
			\"cashFlowPerShareAnnual\" : -0.17905,\
			\"cashFlowPerShareTTM\" : -0.17906,\
			\"cashPerSharePerShareAnnual\" : 1.61358,\
			\"cashPerSharePerShareQuarterly\" : 1.61358,\
			\"currentDividendYieldTTM\" : 0,\
			\"currentEv/freeCashFlowAnnual\" : 1.2,\
			\"currentEv/freeCashFlowTTM\" : null,\
			\"currentRatioAnnual\" : 1.61055,\
			\"currentRatioQuarterly\" : 1.61055,\
\
			\"dividendGrowthRate5Y\" : null,\
			\"dividendPerShare5Y\" : null,\
			\"dividendPerShareAnnual\" : null,\
			\"dividendYield5Y\" : null,\
			\"dividendYieldIndicatedAnnual\" : null,\
			\"dividendsPerShareTTM\" : 0,\
\
			\"ebitdPerShareTTM\" : 0.34761,\
			\"ebitdaCagr5Y\" : -26.28051,\
			\"ebitdaInterimCagr5Y\" : -26.28051,\
			\"epsBasicExclExtraItemsAnnual\" : -1.12407,\
			\"epsBasicExclExtraItemsTTM\" : -1.12246,\
			\"epsExclExtraItemsAnnual\" : -1.12407,\
			\"epsExclExtraItemsTTM\" : -1.12246,\
			\"epsGrowth3Y\" : null,\
			\"epsGrowth5Y\" : null,\
			\"epsGrowthQuarterlyYoy\" : 57.6156,\
			\"epsGrowthTTMYoy\" : -511.5282,\
			\"epsInclExtraItemsAnnual\" : -1.12407,\
			\"epsInclExtraItemsTTM\" : -1.12246,\
			\"epsNormalizedAnnual\" : -1.12407,\
\
			\"focfCagr5Y\" : null,\
			\"freeCashFlowAnnual\" : -6.40426,\
			\"freeCashFlowPerShareTTM\" : -0.18903,\
			\"freeCashFlowTTM\" : -6.40426,\
			\"freeOperatingCashFlow/revenue5Y\" : 4.2309,\
			\"freeOperatingCashFlow/revenueTTM\" : -3.32716,\
\
			\"grossMargin5Y\" : 28.23462,\
			\"grossMarginAnnual\" : 22.37743,\
			\"grossMarginTTM\" : 22.37744,\
\
			\"inventoryTurnoverAnnual\" : 13.98368,\
			\"inventoryTurnoverTTM\" : 13.98368,\
\
			\"longTermDebt/equityAnnual\" : 101.9366,\
			\"longTermDebt/equityQuarterly\" : 101.9366,\
\
			\"marketCapitalization\" : 189.345,\
			\"monthToDatePriceReturnDaily\" : 22.44444,\
\
			\"netDebtAnnual\" : 316.1072,\
			\"netDebtInterim\" : 316.1072,\
			\"netIncomeEmployeeAnnual\" : -776933,\
			\"netIncomeEmployeeTTM\" : -776933.2,\
			\"netInterestCoverageAnnual\" : -1.21208,\
			\"netInterestCoverageTTM\" : -1.21208,\
			\"netMarginGrowth5Y\" : null,\
			\"netProfitMargin5Y\" : -11.52406,\
			\"netProfitMarginAnnual\" : -18.97082,\
			\"netProfitMarginTTM\" : -18.97083,\
\
			\"operatingMargin5Y\" : 1.32556,\
			\"operatingMarginAnnual\" : -10.2087,\
			\"operatingMarginTTM\" : -10.2087,\
\
			\"payoutRatioAnnual\" : null,\
			\"payoutRatioTTM\" : null,\
			\"pbAnnual\" : 0.64854,\
			\"pbQuarterly\" : 0.64854,\
			\"pcfShareTTM\" : null,\
			\"peBasicExclExtraTTM\" : null,\
			\"peExclExtraAnnual\" : null,\
			\"peExclExtraHighTTM\" : 8.87272,\
			\"peExclExtraTTM\" : null,\
			\"peExclLowTTM\" : 7.69796,\
			\"peInclExtraTTM\" : null,\
			\"peNormalizedAnnual\" : null,\
			\"pfcfShareAnnual\" : null,\
			\"pfcfShareTTM\" : null,\
			\"pretaxMargin5Y\" : -11.46396,\
			\"pretaxMarginAnnual\" : -18.8931,\
			\"pretaxMarginTTM\" : -18.89311,\
			\"priceRelativeToS&P50013Week\" : null,\
			\"priceRelativeToS&P50026Week\" : null,\
			\"priceRelativeToS&P5004Week\" : null,\
			\"priceRelativeToS&P50052Week\" : null,\
			\"priceRelativeToS&P500Ytd\" : null,\
			\"psAnnual\" : 0.98369,\
			\"psTTM\" : 0.98369,\
			\"ptbvAnnual\" : 0.57552,\
			\"ptbvQuarterly\" : 0.57552,\
\
			\"quickRatioAnnual\" : 1.42089,\
			\"quickRatioQuarterly\" : 1.42089,\
\
			\"receivablesTurnoverAnnual\" : 10.10098,\
			\"receivablesTurnoverTTM\" : 10.10098,\
			\"revenueEmployeeAnnual\" : 4095411,\
			\"revenueEmployeeTTM\" : 4095411,\
			\"revenueGrowth3Y\" : -2.88897,\
			\"revenueGrowth5Y\" : 3.20402,\
			\"revenueGrowthQuarterlyYoy\" : 25.72476,\
			\"revenueGrowthTTMYoy\" : -12.53004,\
			\"revenuePerShareAnnual\" : 5.68086,\
			\"revenuePerShareTTM\" : 5.68136,\
			\"revenueShareGrowth5Y\" : 0.81717,\
			\"roaRfy\" : -4.94614,\
			\"roaa5Y\" : -3.00716,\
			\"roae5Y\" : -7.10913,\
			\"roaeTTM\" : -12.44075,\
			\"roeRfy\" : -12.44075,\
			\"roeTTM\" : -4.94614,\
			\"roi5Y\" : -3.26368,\
			\"roiAnnual\" : -5.38924,\
			\"roiTTM\" : -5.38924,\
\
			\"tangibleBookValuePerShareAnnual\" : 9.57393,\
			\"tangibleBookValuePerShareQuarterly\" : 9.57393,\
			\"tbvCagr5Y\" : -4.0369,\
			\"totalDebt/totalEquityAnnual\" : 112.9358,\
			\"totalDebt/totalEquityQuarterly\" : 112.9358,\
			\"totalDebtCagr5Y\" : -4.27798,\
\
			\"yearToDatePriceReturnDaily\" : 63.01775\
			},\
		\"series\": {\
				\"annual\": {\
	\"cashRatio\": [{\"period\":\"2020-12-31\",\"v\":0.7634660421545667},{\"period\":\"2019-12-31\",\"v\":0.11672683513838748}],\
	\"currentRatio\":[{\"period\":\"2020-12-31\",\"v\":2.7471},{\"period\":\"2019-12-31\",\"v\":1.2804},{\"period\":\"2018-12-31\",\"v\":2.4764}],\
	\"ebitPerShare\":[{\"period\":\"2020-12-31\",\"v\":-57.636},{\"period\":\"2019-12-31\",\"v\":-8.9067}],\
	\"eps\":[{\"period\":\"2020-12-31\",\"v\":-64.74},{\"period\":\"2019-12-31\",\"v\":-2.64}],\
	\"grossMargin\":[],\
	\"longtermDebtTotalAsset\":[{\"period\":\"2019-12-31\",\"v\":0.3499},{\"period\":\"2018-12-31\",\"v\":0.3572}],\
	\"longtermDebtTotalCapital\":[{\"period\":\"2019-12-31\",\"v\":0.3857},{\"period\":\"2018-12-31\",\"v\":0.3824}],\
	\"longtermDebtTotalEquity\":[{\"period\":\"2019-12-31\",\"v\":0.3499},{\"period\":\"2018-12-31\",\"v\":0.3572}],\
	\"netDebtToTotalCapital\":[{\"period\":\"2019-12-31\",\"v\":0.3499},{\"period\":\"2018-12-31\",\"v\":0.3572}],\
	\"netDebtToTotalEquity\":[{\"period\":\"2019-12-31\",\"v\":0.3499},{\"period\":\"2018-12-31\",\"v\":0.3572}],\
	\"netMargin\":[{\"period\":\"2019-12-31\",\"v\":0.3499},{\"period\":\"2018-12-31\",\"v\":0.3572}],\
	\"operatingMargin\":[{\"period\":\"2019-12-31\",\"v\":0.3499},{\"period\":\"2018-12-31\",\"v\":0.3572}],\
	\"pretaxMargin\":[{\"period\":\"2019-12-31\",\"v\":0.3499},{\"period\":\"2018-12-31\",\"v\":0.3572}],\
	\"salesPerShare\":[{\"period\":\"2019-12-31\",\"v\":0.3499},{\"period\":\"2018-12-31\",\"v\":0.3572}],\
	\"sgaToSale\":[],\
	\"totalDebtToEquity\":[{\"period\":\"2019-12-31\",\"v\":0.3499},{\"period\":\"2018-12-31\",\"v\":0.3572}],\
	\"totalDebtToTotalAsset\":[{\"period\":\"2019-12-31\",\"v\":0.3499},{\"period\":\"2018-12-31\",\"v\":0.3572}],\
	\"totalDebtToTotalCapital\":[{\"period\":\"2019-12-31\",\"v\":0.3499},{\"period\":\"2018-12-31\",\"v\":0.3572}],\
	\"totalRatio\":[{\"period\":\"2019-12-31\",\"v\":0.3499},{\"period\":\"2018-12-31\",\"v\":0.3572}]\
				},\
				\"quarterly\":{\
	\"cashRatio\":[{\"period\":\"2021-03-31\",\"v\":0.6252676659528907},{\"period\":\"2020-12-31\",\"v\":0.7634660421545667}],\
	\"currentRatio\":[{\"period\":\"2021-03-31\",\"v\":0.6252676659528907},{\"period\":\"2020-12-31\",\"v\":0.7634660421545667}],\
	\"ebitPerShare\":[{\"period\":\"2021-03-31\",\"v\":0.6252676659528907},{\"period\":\"2020-12-31\",\"v\":0.7634660421545667}],\
	\"eps\":[{\"period\":\"2021-03-31\",\"v\":0.6252676659528907},{\"period\":\"2020-12-31\",\"v\":0.7634660421545667}],\
	\"grossMargin\":[],\
	\"longtermDebtTotalAsset\":[{\"period\":\"2021-03-31\",\"v\":0.6252676659528907},{\"period\":\"2020-12-31\",\"v\":0.7634660421545667}],\
	\"longtermDebtTotalCapital\":[{\"period\":\"2021-03-31\",\"v\":0.6252676659528907},{\"period\":\"2020-12-31\",\"v\":0.7634660421545667}],\
	\"longtermDebtTotalEquity\":[{\"period\":\"2021-03-31\",\"v\":0.6252676659528907},{\"period\":\"2020-12-31\",\"v\":0.7634660421545667}],\
	\"netDebtToTotalCapital\":[{\"period\":\"2021-03-31\",\"v\":0.6252676659528907},{\"period\":\"2020-12-31\",\"v\":0.7634660421545667}],\
	\"netDebtToTotalEquity\":[{\"period\":\"2021-03-31\",\"v\":0.6252676659528907},{\"period\":\"2020-12-31\",\"v\":0.7634660421545667}],\
	\"netMargin\":[{\"period\":\"2021-03-31\",\"v\":0.6252676659528907},{\"period\":\"2020-12-31\",\"v\":0.7634660421545667}],\
	\"operatingMargin\":[{\"period\":\"2021-03-31\",\"v\":0.6252676659528907},{\"period\":\"2020-12-31\",\"v\":0.7634660421545667}],\
	\"pretaxMargin\":[{\"period\":\"2021-03-31\",\"v\":0.6252676659528907},{\"period\":\"2020-12-31\",\"v\":0.7634660421545667}],\
	\"salesPerShare\":[{\"period\":\"2021-03-31\",\"v\":0.6252676659528907},{\"period\":\"2020-12-31\",\"v\":0.7634660421545667}],\
	\"sgaToSale\":[],\
	\"totalDebtToEquity\":[{\"period\":\"2021-03-31\",\"v\":0.6252676659528907},{\"period\":\"2020-12-31\",\"v\":0.7634660421545667}],\
	\"totalDebtToTotalAsset\":[{\"period\":\"2021-03-31\",\"v\":0.6252676659528907},{\"period\":\"2020-12-31\",\"v\":0.7634660421545667}],\
	\"totalDebtToTotalCapital\":[{\"period\":\"2021-03-31\",\"v\":0.6252676659528907},{\"period\":\"2020-12-31\",\"v\":0.7634660421545667}],\
	\"totalRatio\":[{\"period\":\"2021-03-31\",\"v\":0.6252676659528907},{\"period\":\"2020-12-31\",\"v\":0.7634660421545667}]\
				}\
		},\
		\"metricType\":\"all\",\
		\"symbol\":\"AAPL\"\
}");

static void ParseWithNlohmannJSon(benchmark::State& state) {
	json j;
	for (auto _ : state) { auto f = NlohmannCreateJson(&j, sData101); }
}

BENCHMARK(ParseWithNlohmannJSon);

class CJsonParse : public benchmark::Fixture {
public:
	void SetUp(const ::benchmark::State& state) override {
		CString strFileName = gl_systemConfiguration.GetBenchmarkTestFileDirectory() + _T("StockSymbol.json");
		LoadFromFile(strFileName, sUSExchangeStockCode);

		strFileName = gl_systemConfiguration.GetBenchmarkTestFileDirectory() + _T("NeteaseRTData.json");
		LoadFromFile(strFileName, sNeteaseRTData);

		sNeteaseRTDataForPTree = sNeteaseRTData;
		sNeteaseRTDataForPTree.resize(sNeteaseRTDataForPTree.size() - 2);
		sNeteaseRTDataForPTree.erase(sNeteaseRTDataForPTree.begin(), sNeteaseRTDataForPTree.begin() + 21);
	}

	void TearDown(const ::benchmark::State& state) override { }

	string sUSExchangeStockCode;
	string sNeteaseRTData;
	string sNeteaseRTDataForPTree;
};

// 解析US交易所的股票代码数据（5MB）时，Release模式，nlohmann json用时130毫秒，PTree用时310毫秒；
BENCHMARK_F(CJsonParse, StockSymbolParseWithNlohmannJSon)(benchmark::State& state) {
	json j;
	for (auto _ : state) { auto f = NlohmannCreateJson(&j, sUSExchangeStockCode); }
}

// 解析Netease实时数据时，nlohmann json用时16毫秒，PTree用时32毫秒。
BENCHMARK_F(CJsonParse, NeteaseRTDataCreateJsonWithNlohmannJson)(benchmark::State& state) {
	json j;

	for (auto _ : state) { [[maybe_unused]] auto f = NlohmannCreateJson(&j, sNeteaseRTData, 21, 2); }
}

// 解析并处理netease实时数据。
json j; // 此变量不能声明为局部变量，否则可能导致栈溢出。原因待查
BENCHMARK_F(CJsonParse, NeteaseRTDataParseWithNlohmannJson)(benchmark::State& state) {
	for (auto _ : state) {
		auto f = NlohmannCreateJson(&j, sNeteaseRTData, 21, 2);
		shared_ptr<vector<CWebRTDataPtr>> pvWebRTData = ParseNeteaseRTData(&j);
	}
}

class CWithNlohmannJson : public benchmark::Fixture {
public:
	void SetUp(const ::benchmark::State& state) override {
		const CString strFileName = gl_systemConfiguration.GetBenchmarkTestFileDirectory() + _T("NeteaseRTData.json");
		LoadFromFile(strFileName, s);
		try { js = json::parse(s.begin() + 21, s.end() - 2); }
		catch (json::parse_error&) { fDone = false; }
	}

	void TearDown(const ::benchmark::State& state) override { }

	string s;
	json js; // 此处不能使用智能指针，否则出现重入问题，原因不明。
	vector<CWebRTDataPtr> vWebRTDataReceived;
	bool fDone;
};

// 测试nlohmann json解析NeteaseRTData的速度
BENCHMARK_F(CWithNlohmannJson, ParseNeteaseRTData1)(benchmark::State& state) { for (auto _ : state) { shared_ptr<vector<CWebRTDataPtr>> pvWebRTData = ParseNeteaseRTData(&js); } }

class CTengxunRTData : public benchmark::Fixture {
public:
	void SetUp(const ::benchmark::State& state) override {
		const CString strFileName = gl_systemConfiguration.GetBenchmarkTestFileDirectory() + _T("TengxunRTData.dat");
		LoadFromFile(strFileName, s);
		CString str = s.c_str();
		pWebData = make_shared<CWebData>();
		const long lStringLength = str.GetLength();
		pWebData->ResetCurrentPos(); // 每次要重置开始的位置
		pWebData->SetBufferLength(lStringLength);
		pWebData->Resize(lStringLength);
		pWebData->SetData(str.GetBuffer(), lStringLength, 0);
	}

	void TearDown(const ::benchmark::State& state) override { }

	string s;
	CWebDataPtr pWebData;
};

// 测试nlohmann json解析NeteaseRTData的速度
BENCHMARK_F(CTengxunRTData, ParseTengxunRTData1)(benchmark::State& state) {
	for (auto _ : state) {
		//pWebData->ResetCurrentPos(); // 每次要重置开始的位置
		shared_ptr<vector<CWebRTDataPtr>> pvWebRTData = ParseTengxunRTData(pWebData);
	}
}

class CSinaRTData : public benchmark::Fixture {
public:
	void SetUp(const ::benchmark::State& state) override {
		const CString strFileName = gl_systemConfiguration.GetBenchmarkTestFileDirectory() + _T("SinaRTData.dat");
		LoadFromFile(strFileName, s);
		CString str = s.c_str();
		pWebData = make_shared<CWebData>();
		const long lStringLength = str.GetLength();
		pWebData->ResetCurrentPos(); // 每次要重置开始的位置
		pWebData->SetBufferLength(lStringLength);
		pWebData->Resize(lStringLength);
		pWebData->SetData(str.GetBuffer(), lStringLength, 0);
	}

	void TearDown(const ::benchmark::State& state) override { }

	string s;
	CWebDataPtr pWebData;
};

// 测试nlohmann json解析NeteaseRTData的速度
BENCHMARK_F(CSinaRTData, ParseSinaRTData)(benchmark::State& state) {
	for (auto _ : state) {
		pWebData->ResetCurrentPos(); // 每次要重置开始的位置
		shared_ptr<vector<CWebRTDataPtr>> pvWebData = ParseSinaRTData(pWebData);
	}
}
