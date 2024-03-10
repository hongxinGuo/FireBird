//
// FireBird�бȽϷ�ʱ�ĺ����Ǹ������ݵĽ���������
// Ŀǰ��صĽ�����������US market�������г���Ʊ���룬���СΪ5MB��ʹ��nlohmann json����ʱ��releaseģʽ��ʱ131���룻ʹ��boost PTree
// ����ʱ��releaseģʽ��ʱ320���룻
// ʹ��simdjson������releaseģʽ��parser.iterate()��ʱ1.2���롣
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include<benchmark/benchmark.h>

#include"nlohmannJSonDeclaration.h"

#include"SaveAndLoad.h"
#include"JsonParse.h"
#include "ProductTiingoStockSymbol.h"
#include"WebData.h"

#include"simdjson.h"
using namespace simdjson;

using std::make_shared;

// �����Ŀǰ�ܹ��ҵ�������json���ݣ����ڲ���ParseWithPTree��ParseWithNlohmannJson���ٶ�
// ���Խ����Nlohmann json���ٶȱ�boost��Ptree��50%���ҡ�
// ʹ����������ݣ�nlohmann json��release�汾��ʱ����Ϊ250΢�룻PTree��ʱ����Ϊ330΢�롣
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

static void ParseUsingNlohmannJSon(benchmark::State& state) {
	json j;
	for (auto _ : state) {
		CreateJsonWithNlohmann(j, sData101);
	}
}
BENCHMARK(ParseUsingNlohmannJSon);

static void ParseUsingSimdjson(benchmark::State& state) {
	const padded_string my_data(sData101);
	ondemand::parser parser;
	for (auto _ : state) {
		parser.iterate(my_data);
	}
}

BENCHMARK(ParseUsingSimdjson);

class CJsonParse : public benchmark::Fixture {
public:
	void SetUp(const benchmark::State& state) override {
		CString strFileName = gl_systemConfiguration.GetBenchmarkTestFileDirectory() + _T("StockSymbol.json");
		const string sFileName = (LPCTSTR)strFileName;
		LoadFromFile(strFileName, sUSExchangeStockCode);
		auto bError2 = USExchangedStockCode.load(sFileName);

		strFileName = gl_systemConfiguration.GetBenchmarkTestFileDirectory() + _T("NeteaseRTData.json");
		LoadFromFile(strFileName, sNeteaseRTData);
		sv = sNeteaseRTData.substr(21, sNeteaseRTData.length() - 21 - 2);

		strFileName = gl_systemConfiguration.GetBenchmarkTestFileDirectory() + _T("TengxunDayLine.json");
		LoadFromFile(strFileName, sTengxunDayLine);

		sNeteaseRTDataForPTree = sNeteaseRTData;
		sNeteaseRTDataForPTree.resize(sNeteaseRTDataForPTree.size() - 2);
		sNeteaseRTDataForPTree.erase(sNeteaseRTDataForPTree.begin(), sNeteaseRTDataForPTree.begin() + 21);

		sWorldStockUpdateParameter = _T("{\"Finnhub\":{\"StockFundamentalsCompanyProfileConcise\":20230110,\"StockFundamentalsCompanyNews\":20230205,\"StockFundamentalsBasicFinancials\":20230112,\"StockPriceQuote\":19800104,\"StockFundamentalsPeer\":20230115,\"StockFundamentalsInsiderTransaction\":20230116,\"StockFundamentalsInsiderSentiment\":20230117,\"StockEstimatesEPSSurprise\":19800108},\"Tiingo\":{\"StockFundamentalsCompanyProfile\":20221222,\"StockPriceCandles\":20230210}}");

		strFileName = gl_systemConfiguration.GetBenchmarkTestFileDirectory() + _T("tiingo_fundamentals.json");
		LoadFromFile(strFileName, sTiingoSymbol);
		pWebData = make_shared<CWebData>();
		pWebData->Test_SetBuffer_(sTiingoSymbol.c_str());
	}

	void TearDown(const benchmark::State& state) override {
	}

	string sUSExchangeStockCode;
	string sNeteaseRTData;
	string sv;
	string sNeteaseRTDataForPTree;
	string sTengxunDayLine;
	string sWorldStockUpdateParameter;
	string sTiingoSymbol;
	CWebDataPtr pWebData;
	bool bError{false};

	simdjson::padded_string USExchangedStockCode;
};

// ���ַ�����ʹ��atof() * 1000��һ��
BENCHMARK_F(CJsonParse, StrToDecimal)(benchmark::State& state) {
	constexpr string_view svData{"12345.7654"};
	for (auto _ : state) {
		StrToDecimal(svData, 3);
	}
}

// ����US�������Ĺ�Ʊ�������ݣ�5MB��ʱ��Releaseģʽ��nlohmann json��ʱ130���룬PTree��ʱ310���룻
BENCHMARK_F(CJsonParse, StockSymbolParseUsingNlohmannJSon)(benchmark::State& state) {
	json j;
	for (auto _ : state) {
		CreateJsonWithNlohmann(j, sUSExchangeStockCode);
	}
}

BENCHMARK_F(CJsonParse, StockSymbolParseUsingSimdjson)(benchmark::State& state) {
	const CString strFileName1 = gl_systemConfiguration.GetBenchmarkTestFileDirectory() + _T("StockSymbol.json");
	const string sFileName = (LPCTSTR)strFileName1;
	const auto j = padded_string::load(sFileName);
	ondemand::parser parser;
	for (auto _ : state) {
		parser.iterate(j);
	}
}

// ����Neteaseʵʱ����ʱ��nlohmann json��ʱ16���룬PTree��ʱ32���롣
BENCHMARK_F(CJsonParse, NeteaseRTDataCreateJsonUsingNlohmannJson)(benchmark::State& state) {
	json j;
	for (auto _ : state) {
		CreateJsonWithNlohmann(j, sNeteaseRTData, 21, 2);
	}
}

BENCHMARK_F(CJsonParse, NeteaseRTDataCreateJsonUsingSimdjson)(benchmark::State& state) {
	const CString strFileName2 = gl_systemConfiguration.GetBenchmarkTestFileDirectory() + _T("NeteaseRTData.json");
	const string sFileName = (LPCTSTR)strFileName2;
	const auto j = padded_string::load(sFileName);
	ondemand::parser parser;
	for (auto _ : state) {
		parser.iterate(j);
	}
}

// ����WorldStock update parameter��nlohmann json��ʱ50΢�루debug), 7΢�루release)��
BENCHMARK_F(CJsonParse, WorldStockUpdateParameterCreateJsonUsingNlohmannJson)(benchmark::State& state) {
	json j;
	for (auto _ : state) {
		CreateJsonWithNlohmann(j, sWorldStockUpdateParameter);
	}
}

// ����������neteaseʵʱ���ݡ�
BENCHMARK_F(CJsonParse, NeteaseRTDataParseUsingNlohmannJson)(benchmark::State& state) {
	static json j; // �˱�����������Ϊ�ֲ�������������ܵ���ջ�����ԭ�����
	for (auto _ : state) {
		CreateJsonWithNlohmann(j, sNeteaseRTData, 21, 2);
		shared_ptr<vector<CWebRTDataPtr>> pvWebRTData = ParseNeteaseRTData(&j);
	}
}

BENCHMARK_F(CJsonParse, NeteaseRTDataParseUsingSimdjson1)(benchmark::State& state) {
	for (auto _ : state) {
		shared_ptr<vector<CWebRTDataPtr>> pvWebRTData = ParseNeteaseRTDataWithSimdjson(sv);
	}
}

BENCHMARK_F(CJsonParse, NeteaseRTDataParseUsingSimdjson2)(benchmark::State& state) {
	for (auto _ : state) {
		shared_ptr<vector<CWebRTDataPtr>> pvWebRTData = ParseNeteaseRTDataWithSimdjson2(sv);
	}
}

// ����������tengxun�������ݡ�
BENCHMARK_F(CJsonParse, ParseTengxunDayLineUsingSimdjson)(benchmark::State& state) {
	const string_view svData = sTengxunDayLine;
	for (auto _ : state) {
		auto vData = ParseTengxunDayLine(svData, _T("sh000001")); // Ĭ�ϲ����ļ��еĹ�Ʊ����Ϊsh000001.
	}
}

// ����������tengxun�������ݡ�
BENCHMARK_F(CJsonParse, ParseTiingoFundamentalsUsingSimdjson)(benchmark::State& state) {
	CProductTiingoStockSymbol p;
	for (auto _ : state) {
		auto vData = p.ParseTiingoStockSymbol2(pWebData); // Ĭ�ϲ����ļ��еĹ�Ʊ����Ϊsh000001.
	}
}

BENCHMARK_F(CJsonParse, ParseTiingoFundamentalsUsingNlohmannjson)(benchmark::State& state) {
	CProductTiingoStockSymbol p;
	for (auto _ : state) {
		auto vData = p.ParseTiingoStockSymbol(pWebData); // Ĭ�ϲ����ļ��еĹ�Ʊ����Ϊsh000001.
	}
}

class CWithNlohmannJson : public benchmark::Fixture {
public:
	void SetUp(const benchmark::State& state) override {
		const CString strFileName = gl_systemConfiguration.GetBenchmarkTestFileDirectory() + _T("NeteaseRTData.json");
		LoadFromFile(strFileName, s);
		sv = s;
		sv = sv.substr(21, sv.length() - 21 - 2);
		try {
			js = json::parse(s.begin() + 21, s.end() - 2);
		}
		catch (json::parse_error&) {
			fDone = false;
		}
		pWebData = make_shared<CWebData>();
		pWebData->Test_SetBuffer_(s.c_str());
	}

	void TearDown(const benchmark::State& state) override {
	}

	CWebDataPtr pWebData;
	string s;
	string_view sv;
	json js; // �˴�����ʹ������ָ�룬��������������⣬ԭ������
	vector<CWebRTDataPtr> vWebRTDataReceived;
	bool fDone;
};

// ����nlohmann json��ȡNeteaseRTData���ٶȣ�������Ԥ�Ƚ����ˣ�
BENCHMARK_F(CWithNlohmannJson, ParseNeteaseRTDataUsingNlohmannJson1)(benchmark::State& state) {
	for (auto _ : state) {
		shared_ptr<vector<CWebRTDataPtr>> pvWebRTData = ParseNeteaseRTData(&js);
	}
}

// ����nlohmann json��ȡNeteaseRTData���ٶȣ�����δ������
BENCHMARK_F(CWithNlohmannJson, ParseNeteaseRTDataUsingNlohmannJson2)(benchmark::State& state) {
	for (auto _ : state) {
		shared_ptr<vector<CWebRTDataPtr>> pvWebRTData = ParseNeteaseRTDataWithNlohmannJSon(pWebData);
	}
}

//simdjson��������ȡNeteaseRTData���ٶ�
BENCHMARK_F(CWithNlohmannJson, ParseNeteaseRTDataUsingSimdjson1)(benchmark::State& state) {
	for (auto _ : state) {
		shared_ptr<vector<CWebRTDataPtr>> pvWebRTData = ParseNeteaseRTDataWithSimdjson(sv);
	}
}

BENCHMARK_F(CWithNlohmannJson, ParseNeteaseRTDataUsingSimdjson2)(benchmark::State& state) {
	for (auto _ : state) {
		shared_ptr<vector<CWebRTDataPtr>> pvWebRTData = ParseNeteaseRTDataWithSimdjson2(sv);
	}
}

class CSinaRTData : public benchmark::Fixture {
public:
	void SetUp(const benchmark::State& state) override {
		const CString strFileName = gl_systemConfiguration.GetBenchmarkTestFileDirectory() + _T("SinaRTData.dat");
		LoadFromFile(strFileName, s);
		CString str = s.c_str();
		pWebData = make_shared<CWebData>();
		const long lStringLength = str.GetLength();
		pWebData->ResetCurrentPos(); // ÿ��Ҫ���ÿ�ʼ��λ��
		pWebData->Resize(lStringLength);
		pWebData->SetData(str.GetBuffer(), lStringLength);
	}

	void TearDown(const benchmark::State& state) override {
	}

	string s;
	CWebDataPtr pWebData;
};

BENCHMARK_F(CSinaRTData, ParseSinaRTData)(benchmark::State& state) {
	for (auto _ : state) {
		ParseSinaRTData(pWebData);
	}
}

BENCHMARK_F(CSinaRTData, ParseSinaRTDataUsingThreadPool)(benchmark::State& state) {
	for (auto _ : state) {
		ParseSinaRTDataUsingWorkingThread(pWebData); // Note �˺�������ʱ������������ڴ棬�ڲ�����ɺ��ͷŵú�����
	}
}

class CTengxunRTData : public benchmark::Fixture {
public:
	void SetUp(const benchmark::State& state) override {
		const CString strFileName = gl_systemConfiguration.GetBenchmarkTestFileDirectory() + _T("TengxunRTData.dat");
		LoadFromFile(strFileName, s);
		CString str = s.c_str();
		pWebData = make_shared<CWebData>();
		const long lStringLength = str.GetLength();
		pWebData->ResetCurrentPos(); // ÿ��Ҫ���ÿ�ʼ��λ��
		pWebData->Resize(lStringLength);
		pWebData->SetData(str.GetBuffer(), lStringLength);
	}

	void TearDown(const benchmark::State& state) override {
	}

	string s;
	CWebDataPtr pWebData;
};

BENCHMARK_F(CTengxunRTData, ParseTengxunRTData)(benchmark::State& state) {
	for (auto _ : state) {
		ParseTengxunRTData(pWebData);
	}
}

BENCHMARK_F(CTengxunRTData, ParseTengxunRTDataUsingThreadPool)(benchmark::State& state) {
	for (auto _ : state) {
		ParseTengxunRTDataUsingWorkingThread(pWebData); // Note �˺�������ʱ������������ڴ棬�ڲ�����ɺ��ͷŵú�����
	}
}
