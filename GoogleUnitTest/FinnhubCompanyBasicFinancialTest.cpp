#include"pch.h"

#include"globedef.h"
#include"SystemMessage.h"
#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"ProductFinnhubCompanyBasicFinancial.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CProductFinnhubCompanyBasicFinancialTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		virtual void SetUp(void) override {
		}

		virtual void TearDown(void) override {
			// clearu
			GeneralCheck();
		}

	protected:
		CProductFinnhubCompanyBasicFinancial companyBasicFinancial;
	};

	TEST_F(CProductFinnhubCompanyBasicFinancialTest, TestInitialize) {
		EXPECT_EQ(companyBasicFinancial.GetIndex(), -1);
		EXPECT_STREQ(companyBasicFinancial.GetInquiringStr(), _T("https://finnhub.io/api/v1/stock/metric?symbol="));
	}

	TEST_F(CProductFinnhubCompanyBasicFinancialTest, TestCreatMessage) {
		CWorldStockPtr pStock = gl_pWorldMarket->GetStock(1);
		pStock->SetBasicFinancialUpdated(false);
		companyBasicFinancial.SetMarket(gl_pWorldMarket.get());
		companyBasicFinancial.SetIndex(1);
		EXPECT_STREQ(companyBasicFinancial.CreatMessage(), companyBasicFinancial.GetInquiringStr() + gl_pWorldMarket->GetStock(1)->GetSymbol());
		EXPECT_TRUE(pStock->IsBasicFinancialUpdated());

		gl_pWorldMarket->GetStock(1)->SetBasicFinancialUpdated(false);
	}

	FinnhubWebData finnhubWebData1001(1, _T("AAPL"),
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
}"));

	class ParseFinnhubStockBasicFinancialTest : public::testing::TestWithParam<FinnhubWebData*> {
	protected:
		virtual void SetUp(void) override {
			GeneralCheck();
			FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pStock = gl_pWorldMarket->GetStock(pData->m_strSymbol);
			EXPECT_TRUE(m_pStock != nullptr);
			EXPECT_EQ(m_pStock->GetInsiderTransactionUpdateDate(), 19800101);
			m_pStock->SetInsiderTransactionNeedSave(false);
			EXPECT_FALSE(m_pStock->IsUpdateProfileDB());
			m_pWebData = pData->m_pData;
			m_pWebData->CreatePTree();
			m_pWebData->SetJSonContentType(true);
			m_finnhubCompanyBasicFinancial.SetMarket(gl_pWorldMarket.get());
			long lIndex = gl_pWorldMarket->GetStockIndex(pData->m_strSymbol);
			m_finnhubCompanyBasicFinancial.SetIndex(lIndex);
		}
		virtual void TearDown(void) override {
			// clearup
			while (gl_systemMessage.GetErrorMessageDequeSize() > 0) gl_systemMessage.PopErrorMessage();
			m_pStock->SetUpdateProfileDB(false);
			m_pStock->SetInsiderTransactionNeedSave(false);
			m_pStock->SetInsiderTransactionUpdateDate(19800101);

			GeneralCheck();
		}

	public:
		long m_lIndex;
		CWorldStockPtr m_pStock;
		CWebDataPtr m_pWebData;
		CProductFinnhubCompanyBasicFinancial m_finnhubCompanyBasicFinancial;
	};

	INSTANTIATE_TEST_SUITE_P(TestProcessFinnhubStockBasicFinancial1, ParseFinnhubStockBasicFinancialTest,
												 testing::Values(&finnhubWebData1001));

	TEST_P(ParseFinnhubStockBasicFinancialTest, TestParseFinnhubInsiderTransaction0) {
		CFinnhubStockBasicFinancialPtr pBasicFinancial = m_finnhubCompanyBasicFinancial.ParseFinnhubStockBasicFinancial(m_pWebData);
		switch (m_lIndex) {
		case 1: // ÕýÈ·
			EXPECT_STREQ(pBasicFinancial->m_symbol, _T("AAPL"));
			EXPECT_DOUBLE_EQ(pBasicFinancial->m_10DayAverageTradingVolume, 0.43212);
			EXPECT_DOUBLE_EQ(pBasicFinancial->m_yearToDatePriceReturnDaily, 63.01775);
			break;
		default:
			break;
		}
	}

	class ProcessFinnhubStockBasicFinancialTest : public::testing::TestWithParam<FinnhubWebData*> {
	protected:
		virtual void SetUp(void) override {
			GeneralCheck();
			FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pStock = gl_pWorldMarket->GetStock(pData->m_strSymbol);
			EXPECT_TRUE(m_pStock != nullptr);
			EXPECT_EQ(m_pStock->GetInsiderTransactionUpdateDate(), 19800101);
			m_pStock->SetUpdateBasicFinancialDB(false);
			m_pWebData = pData->m_pData;
			m_pWebData->CreatePTree();
			m_pWebData->SetJSonContentType(true);
			m_finnhubCompanyBasicFinancial.SetMarket(gl_pWorldMarket.get());
			long lIndex = gl_pWorldMarket->GetStockIndex(pData->m_strSymbol);
			m_finnhubCompanyBasicFinancial.SetIndex(lIndex);
		}
		virtual void TearDown(void) override {
			// clearup
			while (gl_systemMessage.GetErrorMessageDequeSize() > 0) gl_systemMessage.PopErrorMessage();
			m_pStock->SetUpdateBasicFinancialDB(false);
			m_pStock->SetBasicFinancialUpdateDate(19800101);

			GeneralCheck();
		}

	public:
		long m_lIndex;
		CWorldStockPtr m_pStock;
		CWebDataPtr m_pWebData;
		CProductFinnhubCompanyBasicFinancial m_finnhubCompanyBasicFinancial;
	};

	INSTANTIATE_TEST_SUITE_P(TestProcessFinnhubStockBasicFinancial1, ProcessFinnhubStockBasicFinancialTest,
													 testing::Values(&finnhubWebData1001));

	TEST_P(ProcessFinnhubStockBasicFinancialTest, TestProcessFinnhubInsiderTransaction0) {
		EXPECT_EQ(m_pStock->GetBasicFinancial(), nullptr);
		m_finnhubCompanyBasicFinancial.ProcessWebData(m_pWebData);
		switch (m_lIndex) {
		case 1:
			EXPECT_TRUE(m_pStock->IsUpdateBasicFinancialDB());
			EXPECT_EQ(m_pStock->GetBasicFinancialUpdateDate(), ((CWorldMarket*)m_finnhubCompanyBasicFinancial.GetMarket())->GetMarketDate());
			EXPECT_THAT(m_pStock->GetBasicFinancial(), NotNull());

			EXPECT_THAT(0, 0);
			break;
		default:
			break;
		}
	}
}