#include"pch.h"

#include"GeneralCheck.h"

#include"WorldMarket.h"
import FireBird.Stock.Finnhub;
#include"ProductFinnhubCompanyBasicFinancial.h"
import FireBird.System.Message;

using namespace testing;

namespace FireBirdTest {
	class CProductFinnhubCompanyBasicFinancialTest : public Test {
	protected:
		static void SetUpTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		static void TearDownTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void TearDown() override {
			// clearUp
			SCOPED_TRACE("");
			GeneralCheck();
		}

	protected:
		CProductFinnhubCompanyBasicFinancial companyBasicFinancial;
	};

	TEST_F(CProductFinnhubCompanyBasicFinancialTest, TestInitialize) {
		EXPECT_EQ(companyBasicFinancial.GetIndex(), 0);
		EXPECT_STREQ(companyBasicFinancial.GetInquiryFunction(), _T("https://finnhub.io/api/v1/stock/metric?symbol="));
	}

	TEST_F(CProductFinnhubCompanyBasicFinancialTest, TestCreatMessage) {
		const CFinnhubStockPtr pStock = gl_dataContainerFinnhubStock.GetStock(1);
		pStock->SetUpdateBasicFinancial(true);
		companyBasicFinancial.SetMarket(gl_pWorldMarket);
		companyBasicFinancial.SetIndex(1);
		EXPECT_STREQ(companyBasicFinancial.CreateMessage(), companyBasicFinancial.GetInquiryFunction() + gl_dataContainerFinnhubStock.GetStock(1)->GetSymbol() + _T("&metric=all"));
		EXPECT_TRUE(pStock->IsUpdateBasicFinancial()) << "处理接收到的数据后方设置此标识";

		gl_dataContainerFinnhubStock.GetStock(1)->SetUpdateBasicFinancial(true);
	}

	Test_FinnhubWebData finnhubWebData1002(2, _T("AAPL"),
	                                       _T("{\
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
			\"currentEv/freeCashFlowTTM\" : 1.23,\
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
}"));

	// BVDRF是美股ADR，其本土代码为MBWS.PA
	Test_FinnhubWebData finnhubWebData1003(3, _T("BVDRF"),
	                                       _T("{\
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
			\"currentEv/freeCashFlowTTM\" : 1.23,\
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
		\"metricType\":\"perShare\",\
		\"symbol\":\"MBWS.PA\"\
}"));

	// 有些股票只有部分数据
	Test_FinnhubWebData finnhubWebData1004(4, _T("AAPL"), _T("{\"metric\":{\"52WeekHigh\":1.18,\"52WeekLow\":1},\"metricType\":\"marketCapitalization\",\"series\":{},\"symbol\":\"OTSCS\"}"));
	// Metric out of range
	Test_FinnhubWebData finnhubWebData1005(5, _T("AAPL"), _T("{\"metric\":{\"52WeekHigh\":1.18,\"52WeekLow\":1},\"metricType\":\"out of range\",\"series\":{},\"symbol\":\"OTSCS\"}"));
	// 有些股票只有部分数据， 测试metric
	Test_FinnhubWebData finnhubWebData1006(6, _T("AAPL"), _T("{\"metric\":{\"52WeekHigh\":1.18,\"52WeekLow\":1},\"metricType\":\"metric\",\"series\":{},\"symbol\":\"OTSCS\"}"));
	// 有些股票只有部分数据, 测试eps
	Test_FinnhubWebData finnhubWebData1007(7, _T("AAPL"), _T("{\"metric\":{\"52WeekHigh\":1.18,\"52WeekLow\":1},\"metricType\":\"eps\",\"series\":{},\"symbol\":\"OTSCS\"}"));

	class ParseFinnhubStockBasicFinancialTest : public TestWithParam<Test_FinnhubWebData*> {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			const Test_FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pStock = gl_dataContainerFinnhubStock.GetStock(pData->m_strSymbol);
			EXPECT_TRUE(m_pStock != nullptr);
			EXPECT_EQ(m_pStock->GetInsiderTransactionUpdateDate(), 19800101);
			m_pStock->SetUpdateBasicFinancial(true);
			EXPECT_FALSE(m_pStock->IsUpdateProfileDB());
			m_pWebData = pData->m_pData;
			m_finnhubCompanyBasicFinancial.__Test_checkAccessRight(m_pWebData);

			m_finnhubCompanyBasicFinancial.SetMarket(gl_pWorldMarket);
			const auto lIndex = gl_dataContainerFinnhubStock.GetOffset(pData->m_strSymbol);
			m_finnhubCompanyBasicFinancial.SetIndex(lIndex);
		}

		void TearDown() override {
			// clearUp
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
			m_pStock->SetUpdateProfileDB(false);
			m_pStock->SetBasicFinancialUpdateDate(19800101);
			m_pStock->SetUpdateBasicFinancial(true);
			m_pStock->UpdateBasicFinancial(nullptr);

			SCOPED_TRACE("");
			GeneralCheck();
		}

	public:
		long m_lIndex;
		CFinnhubStockPtr m_pStock;
		CWebDataPtr m_pWebData;
		CProductFinnhubCompanyBasicFinancial m_finnhubCompanyBasicFinancial;
	};

	INSTANTIATE_TEST_SUITE_P(TestProcessFinnhubStockBasicFinancial1, ParseFinnhubStockBasicFinancialTest,
	                         testing::Values(&finnhubWebData0, &finnhubWebData1, &finnhubWebData1004, &finnhubWebData1002, &finnhubWebData1003,
		                         &finnhubWebData1005,&finnhubWebData1006, &finnhubWebData1007));

	TEST_P(ParseFinnhubStockBasicFinancialTest, TestParseFinnhubInsiderTransaction0) {
		CFinnhubStockBasicFinancialPtr pBasicFinancial = m_finnhubCompanyBasicFinancial.ParseFinnhubStockBasicFinancial(m_pWebData);
		switch (m_lIndex) {
		case 0: // 空数据
			EXPECT_TRUE(pBasicFinancial == nullptr);
			break;
		case 1: // 无权利访问的数据
			EXPECT_TRUE(pBasicFinancial == nullptr);
			EXPECT_THAT(gl_systemMessage.InnerSystemInfoSize(), 0) << gl_systemMessage.PopInnerSystemInformationMessage();
			break;
		case 2: // 正确
			EXPECT_FALSE(pBasicFinancial == nullptr);
			EXPECT_STREQ(pBasicFinancial->m_symbol, _T("AAPL"));
			EXPECT_DOUBLE_EQ(pBasicFinancial->m_10DayAverageTradingVolume, 0.43212);
			EXPECT_DOUBLE_EQ(pBasicFinancial->m_yearToDatePriceReturnDaily, 63.01775);
			EXPECT_DOUBLE_EQ(pBasicFinancial->m_currentEV_freeCashFlowAnnual, 1.2);
			EXPECT_EQ(pBasicFinancial->m_annual.m_cashRatio.size(), 2);
			EXPECT_EQ(pBasicFinancial->m_annual.m_cashRatio.at(0).m_period, 20201231);
			EXPECT_DOUBLE_EQ(pBasicFinancial->m_annual.m_cashRatio.at(0).m_value, 0.7634660421545667);
			EXPECT_THAT(gl_systemMessage.InnerSystemInfoSize(), 0) << gl_systemMessage.PopInnerSystemInformationMessage();
			break;
		case 3: //
			EXPECT_FALSE(pBasicFinancial == nullptr);
			EXPECT_STREQ(pBasicFinancial->m_symbol, _T("MBWS.PA")) << "BVDRF的本土代码名称为MBWS.PA";
			EXPECT_DOUBLE_EQ(pBasicFinancial->m_10DayAverageTradingVolume, 0.43212);
			EXPECT_THAT(gl_systemMessage.InnerSystemInfoSize(), 0) << gl_systemMessage.PopInnerSystemInformationMessage();
			break;
		case 4:
			EXPECT_FALSE(pBasicFinancial == nullptr);
			EXPECT_STREQ(pBasicFinancial->m_symbol, _T("OTSCS"));
			EXPECT_DOUBLE_EQ(pBasicFinancial->m_52WeekHigh, 1.18);
			EXPECT_DOUBLE_EQ(pBasicFinancial->m_52WeekLow, 1.0);
			EXPECT_DOUBLE_EQ(pBasicFinancial->m_10DayAverageTradingVolume, 0.0) << "没有此项数据，故而其值为初始值";
			break;
		case 5:
			EXPECT_THAT(gl_systemMessage.InnerSystemInfoSize(), 1) << "Metric out of range";
			gl_systemMessage.PopInnerSystemInformationMessage();
		case 6:
			EXPECT_THAT(gl_systemMessage.InnerSystemInfoSize(), 0) << gl_systemMessage.PopInnerSystemInformationMessage();
			break;
		case 7:
			EXPECT_THAT(gl_systemMessage.InnerSystemInfoSize(), 0) << gl_systemMessage.PopInnerSystemInformationMessage();
			break;
		default:
			break;
		}
	}

	class ProcessFinnhubStockBasicFinancialTest : public TestWithParam<Test_FinnhubWebData*> {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			const Test_FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pStock = gl_dataContainerFinnhubStock.GetStock(pData->m_strSymbol);
			EXPECT_TRUE(m_pStock != nullptr);
			EXPECT_EQ(m_pStock->GetInsiderTransactionUpdateDate(), 19800101);
			m_pStock->SetUpdateBasicFinancialDB(false);
			m_pWebData = pData->m_pData;
			m_finnhubCompanyBasicFinancial.__Test_checkAccessRight(m_pWebData);

			m_finnhubCompanyBasicFinancial.SetMarket(gl_pWorldMarket);
			const auto lIndex = gl_dataContainerFinnhubStock.GetOffset(pData->m_strSymbol);
			m_finnhubCompanyBasicFinancial.SetIndex(lIndex);
		}

		void TearDown() override {
			// clearUp
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
			m_pStock->SetUpdateBasicFinancialDB(false);
			m_pStock->SetBasicFinancialUpdateDate(19800101);
			m_pStock->UpdateBasicFinancial(nullptr);

			SCOPED_TRACE("");
			GeneralCheck();
		}

	public:
		long m_lIndex = 0;
		CFinnhubStockPtr m_pStock = nullptr;
		CWebDataPtr m_pWebData = nullptr;
		CProductFinnhubCompanyBasicFinancial m_finnhubCompanyBasicFinancial;
	};

	INSTANTIATE_TEST_SUITE_P(TestProcessFinnhubStockBasicFinancial1, ProcessFinnhubStockBasicFinancialTest,
	                         testing::Values(&finnhubWebData1004, &finnhubWebData1002, &finnhubWebData1003,
		                         &finnhubWebData1005, &finnhubWebData1006, &finnhubWebData1007));

	TEST_P(ProcessFinnhubStockBasicFinancialTest, TestProcessFinnhubInsiderTransaction0) {
		EXPECT_EQ(m_pStock->GetBasicFinancial(), nullptr);
		m_finnhubCompanyBasicFinancial.ParseAndStoreWebData(m_pWebData);
		switch (m_lIndex) {
		case 0: // 空数据
			EXPECT_FALSE(m_pStock->IsUpdateBasicFinancial());
			EXPECT_FALSE(m_pStock->IsUpdateBasicFinancialDB());
			EXPECT_TRUE(m_pStock->IsUpdateProfileDB());
			EXPECT_EQ(m_pStock->GetBasicFinancialUpdateDate(), m_finnhubCompanyBasicFinancial.GetMarket()->GetMarketDate());
			EXPECT_THAT(gl_systemMessage.InnerSystemInfoSize(), 0) << gl_systemMessage.PopInnerSystemInformationMessage();
			break;
		case 1: // 无权利访问的数据
			EXPECT_FALSE(m_pStock->IsUpdateBasicFinancial());
			EXPECT_FALSE(m_pStock->IsUpdateBasicFinancialDB());
			EXPECT_TRUE(m_pStock->IsUpdateProfileDB());
			EXPECT_EQ(m_pStock->GetBasicFinancialUpdateDate(), m_finnhubCompanyBasicFinancial.GetMarket()->GetMarketDate());
			EXPECT_THAT(gl_systemMessage.InnerSystemInfoSize(), 0) << gl_systemMessage.PopInnerSystemInformationMessage();
			break;
		case 2:
			EXPECT_FALSE(m_pStock->IsUpdateBasicFinancial());
			EXPECT_TRUE(m_pStock->IsUpdateBasicFinancialDB());
			EXPECT_TRUE(m_pStock->IsUpdateProfileDB());
			EXPECT_EQ(m_pStock->GetBasicFinancialUpdateDate(), m_finnhubCompanyBasicFinancial.GetMarket()->GetMarketDate());
			EXPECT_THAT(m_pStock->GetBasicFinancial(), NotNull());
			EXPECT_THAT(gl_systemMessage.InnerSystemInfoSize(), 0) << gl_systemMessage.PopInnerSystemInformationMessage();
			break;
		case 3: // ADR
			EXPECT_FALSE(m_pStock->IsUpdateBasicFinancial());
			EXPECT_TRUE(m_pStock->IsUpdateBasicFinancialDB());
			EXPECT_TRUE(m_pStock->IsUpdateProfileDB());
			EXPECT_EQ(m_pStock->GetBasicFinancialUpdateDate(), m_finnhubCompanyBasicFinancial.GetMarket()->GetMarketDate());
			EXPECT_THAT(m_pStock->GetBasicFinancial(), NotNull());
			EXPECT_THAT(gl_systemMessage.ErrorMessageSize(), 0) << "BVDRF ADR的本土代码名称为MBWS.PA，是合理的，不是错误代码，不用报错";
			EXPECT_THAT(gl_systemMessage.InnerSystemInfoSize(), 0) << gl_systemMessage.PopInnerSystemInformationMessage();
			break;
		case 4: // 部分数据
			EXPECT_FALSE(m_pStock->IsUpdateBasicFinancial());
			EXPECT_TRUE(m_pStock->IsUpdateBasicFinancialDB());
			EXPECT_TRUE(m_pStock->IsUpdateProfileDB());
			EXPECT_EQ(m_pStock->GetBasicFinancialUpdateDate(), m_finnhubCompanyBasicFinancial.GetMarket()->GetMarketDate());
			EXPECT_THAT(m_pStock->GetBasicFinancial(), NotNull());
			EXPECT_THAT(gl_systemMessage.ErrorMessageSize(), 0) << "BVDRF ADR的本土代码名称为MBWS.PA，是合理的，不是错误代码，不用报错";
			EXPECT_THAT(gl_systemMessage.InnerSystemInfoSize(), 0) << gl_systemMessage.PopInnerSystemInformationMessage();
			break;
		case 5: // Metric out of range
			EXPECT_FALSE(m_pStock->IsUpdateBasicFinancial());
			EXPECT_TRUE(m_pStock->IsUpdateBasicFinancialDB());
			EXPECT_TRUE(m_pStock->IsUpdateProfileDB());
			EXPECT_EQ(m_pStock->GetBasicFinancialUpdateDate(), m_finnhubCompanyBasicFinancial.GetMarket()->GetMarketDate());
			EXPECT_THAT(m_pStock->GetBasicFinancial(), NotNull());
			EXPECT_THAT(gl_systemMessage.InnerSystemInfoSize(), 1) << "Metric out of range";
			gl_systemMessage.PopInnerSystemInformationMessage();
			break;
		case 6:
			EXPECT_THAT(gl_systemMessage.InnerSystemInfoSize(), 0) << gl_systemMessage.PopInnerSystemInformationMessage();
			break;
		case 7:
			EXPECT_THAT(gl_systemMessage.InnerSystemInfoSize(), 0) << gl_systemMessage.PopInnerSystemInformationMessage();
			break;
		default:
			break;
		}
		// 恢复原状
		m_pStock->SetUpdateBasicFinancial(true);
		m_pStock->SetUpdateProfileDB(false);
	}
}
