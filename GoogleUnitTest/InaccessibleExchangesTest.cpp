#include"pch.h"

#include"WorldMarket.h"
#include"ChinaMarket.h"

#include"GeneralCheck.h"

#include"FinnhubInquiryType.h"

#include"FinnhubInaccessibleExchange.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CInaccessibleExchangesTest : public ::testing::Test {
		virtual void SetUp(void) override {
			GeneralCheck();

			vExchanges.push_back(_T("US"));
			vExchanges.push_back(_T("SS"));
			vExchanges.push_back(_T("SZ"));
			inaccessibleExchange.Assign(_T("COMPANY_NEWS"), 1, vExchanges);
		}
		virtual void TearDown(void) override {
			gl_systemStatus.SetWorkingMode(false);

			GeneralCheck();
		}
	protected:
		vector<string> vExchanges;
		CInaccessibleExchanges inaccessibleExchange;
	};

	TEST_F(CInaccessibleExchangesTest, TestAssign) {
		EXPECT_TRUE(inaccessibleExchange.HaveExchange(_T("US"))) << "默认包括US";
		EXPECT_TRUE(inaccessibleExchange.HaveExchange(_T("SZ"))) << "默认包括US";
		EXPECT_TRUE(inaccessibleExchange.HaveExchange(_T("SS"))) << "默认包括US";
		EXPECT_FALSE(inaccessibleExchange.HaveExchange(_T("FA"))) << "默认不包括FA";
		vector<string> v{ _T("US2"), _T("SZ2"), _T("SS2") };
		inaccessibleExchange.Assign(_T("Another"), 2, v);

		EXPECT_TRUE(inaccessibleExchange.HaveExchange(_T("US2")));
		EXPECT_TRUE(inaccessibleExchange.HaveExchange(_T("SZ2")));
		EXPECT_TRUE(inaccessibleExchange.HaveExchange(_T("SS2")));
		EXPECT_FALSE(inaccessibleExchange.HaveExchange(_T("US")));
		EXPECT_FALSE(inaccessibleExchange.HaveExchange(_T("SZ")));
		EXPECT_FALSE(inaccessibleExchange.HaveExchange(_T("SS")));
	}

	TEST_F(CInaccessibleExchangesTest, TestAddExchange) {
		EXPECT_FALSE(inaccessibleExchange.HaveExchange(_T("FA"))) << "默认不包括FA";
		inaccessibleExchange.AddExchange(_T("FA"));
		EXPECT_TRUE(inaccessibleExchange.HaveExchange(_T("FA"))) << "添加后就包括FA了";
	}

	TEST_F(CInaccessibleExchangesTest, TestDeleteExchange) {
		EXPECT_TRUE(inaccessibleExchange.HaveExchange(_T("US"))) << "默认包括US";
		inaccessibleExchange.DeleteExchange(_T("US"));
		EXPECT_FALSE(inaccessibleExchange.HaveExchange(_T("US"))) << "删除后就不包括US了";
	}

	TEST_F(CInaccessibleExchangesTest, TestHaveExchange) {
		EXPECT_TRUE(inaccessibleExchange.HaveExchange(_T("US"))) << "默认包括US";
		EXPECT_TRUE(inaccessibleExchange.HaveExchange(_T("SZ"))) << "默认包括US";
		EXPECT_TRUE(inaccessibleExchange.HaveExchange(_T("SS"))) << "默认包括US";
		EXPECT_FALSE(inaccessibleExchange.HaveExchange(_T("FA"))) << "默认不包括FA";
	}
}

namespace StockAnalysisTest {
	class CFinnhubInaccessibleExchangeTest : public ::testing::Test {
		virtual void SetUp(void) override {
			GeneralCheck();
		}
		virtual void TearDown(void) override {
			gl_systemStatus.SetWorkingMode(false);

			GeneralCheck();
		}
	protected:
	};

	TEST_F(CFinnhubInaccessibleExchangeTest, TestGetFinnhubInquiryString1) {
		string s;
		for (int i = __WEBSOCKET__TRADES__; i <= __WEBSOCKET__NEWS__; i++) {
			s = gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(i);
			EXPECT_TRUE(s.size() > 0) << "确保所有的索引都有对应的字符串";
		}
		for (int i = __SYMBOL_LOOKUP__; i <= __DIVIDENDS__; i++) {
			s = gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(i);
			EXPECT_TRUE(s.size() > 0) << "确保所有的索引都有对应的字符串";
		}
		for (int i = __STOCK_ESTIMATES_RECOMMENDATION_TRENDS__; i <= __STOCK_ESTIMATES_EARNING_CALENDAR__; i++) {
			s = gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(i);
			EXPECT_TRUE(s.size() > 0) << "确保所有的索引都有对应的字符串";
		}
		for (int i = __STOCK_PRICE_QUOTE__; i <= __STOCK_PRICE_DIVIDEND2__; i++) {
			s = gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(i);
			EXPECT_TRUE(s.size() > 0) << "确保所有的索引都有对应的字符串";
		}
	}

	TEST_F(CFinnhubInaccessibleExchangeTest, TestGetFinnhubInquiryString2) {
		// Web Socket
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__WEBSOCKET__TRADES__).c_str(), _T("WebSocketTrades"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__WEBSOCKET__NEWS__).c_str(), _T("WebSocketNews"));// Premium

		// Stock Fundamentals
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__SYMBOL_LOOKUP__).c_str(), _T("StockFundamentalsSymolLookup"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__STOCK_SYMBOLS__).c_str(), _T("StockFundamentalsSymbols"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__COMPANY_PROFILE__).c_str(), _T("StockFundamentalsCompanyProfile")); //Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__COMPANY_PROFILE_CONCISE__).c_str(), _T("StockFundamentalsCompanyProfileConcise"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__COMPANY_EXECTIVE__).c_str(), _T("StockFundamentalsCompanyExective")); //Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__MARKET_NEWS__).c_str(), _T("StockFundamentalsMarketNews"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__COMPANY_NEWS__).c_str(), _T("StockFundamentalsCompanyNews"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__PRESS_RELEASE__).c_str(), _T("StockFundamentalsPressRelease")); //Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__NEWS_SENTIMENT__).c_str(), _T("StockFundamentalsNewsSentiment"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__PEERS__).c_str(), _T("StockFundamentalsPeer"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__BASIC_FINANCIALS__).c_str(), _T("StockFundamentalsBasicFinancials"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__OWNERSHIP__).c_str(), _T("StockFundamentalsOwnership")); //Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__FUND_OWNERSHIP__).c_str(), _T("StockFundamentalsFundOwnership")); //Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__INSIDER_TRANSACTION__).c_str(), _T("StockFundamentalsInsiderTransaction"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__INSIDER_SENTIMENT__).c_str(), _T("StockFundamentalsInsiderSentiment"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__FINANCIAL__).c_str(), _T("StockFundamentalsFiniacial")); //Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__FINAICIAL_AS_REPORTED__).c_str(), _T("StockFundamentalsFinaicialAsReported"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__REVENUE_BREAKDOWN__).c_str(), _T("StockFundamentalsRevenueBreakdown"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__SEC_FILINGS__).c_str(), _T("StockFundamentalsSECFilings"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__INTERNATIONAL_FILINGS__).c_str(), _T("StockFundamentalsInternationalFilings")); //Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__SEC_SENTIMENT_ANALYSIS__).c_str(), _T("StockFundamentalsSECSentimentAnalysis")); //Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__SIMILARITY_INDEX__).c_str(), _T("StockFundamentalsSemilarityIndex")); //Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__IPO_CALENDAR__).c_str(), _T("StockFundamentalsIPOCanlendar"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__DIVIDENDS__).c_str(), _T("StockFundamentalsDividends")); //Premium

		// Stock Estimates
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__STOCK_ESTIMATES_RECOMMENDATION_TRENDS__).c_str(), _T("StockExtimatesRecommendationTrend"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__STOCK_ESTIMATES_PRICE_TARGET__).c_str(), _T("StockExtimatesPriceTarget"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__STOCK_ESTIMATES_UPGRADE_DOWNGRADE__).c_str(), _T("StockExtimatesUpgradeDownGrade")); // Primium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__STOCK_ESTIMATES_REVENUE_EXTIMATES__).c_str(), _T("StockExtimatesRevenueExtimates")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__STOCK_ESTIMATES_EPS_EXTIMATES__).c_str(), _T("StockExtimatesEPSExtimates")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__STOCK_EXTIMATES_EBITDA_EXTIMATES__).c_str(), _T("StockExtimatesEBITDAExtimates")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__STOCK_EXTIMATES_EBIT_EXTIMATES__).c_str(), _T("StockExtimatesEBITExtimates")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__STOCK_ESTIMATES_EPS_SURPRISE__).c_str(), _T("StockExtimatesEPSSurprise"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__STOCK_ESTIMATES_EARNING_CALENDAR__).c_str(), _T("StockExtimatesEarningCalendar"));

		// Stock Price
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__STOCK_PRICE_QUOTE__).c_str(), _T("StockPriceQuote")); // 实时数据优先级最低
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__STOCK_PRICE_CANDLES__).c_str(), _T("StockPriceCandles")); // 历史数据优先级低
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__STOCK_PRICE_TICK_DATA__).c_str(), _T("StockPriceTickData")); //Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__STOCK_PRICE_HISTORICAL_NBBO__).c_str(), _T("StockPriceHistoricalNBBO")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__STOCK_PRICE_LAST_BID_ASK__).c_str(), _T("StockPriceLastBidAsk")); //Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__STOCK_PRICE_SPLITS__).c_str(), _T("StockPriceSplits")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__STOCK_PRICE_DIVIDEND2__).c_str(), _T("StockPriceDividend")); // Premium

		// ETF & Indices
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__ETFS_INDICES_CONSTITUENTS__).c_str(), _T("ETFSIndicesConstituents"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__ETFS_HISTORY_CONSTITUENTS__).c_str(), _T("ETFSIndicesHistoryConstitunets")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__ETFS_PROFILE__).c_str(), _T("ETFSIndicesProfile")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__ETFS_HOLDINGS__).c_str(), _T("ETFSIndicesHoldings")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__ETFS_SECTOR__).c_str(), _T("ETFSIndicesSector")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__ETFS_COUNTRY__).c_str(), _T("ETFSIndicesCountry")); //Premium

		// Mutual funds
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__MUTUAL_FUND_PROFILE__).c_str(), _T("MutualFundProfile")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__MUTUAL_FUND_HOLDINGS__).c_str(), _T("MutualFundHoldings"));// Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__MUTUAL_FUND_SECTOR__).c_str(), _T("MutualFundSector")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__MUTUAL_FUND_COUNTRY__).c_str(), _T("MutualFundCountry")); // Premium

		// Bonds
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__BOND_PROFILE__).c_str(), _T("BondProfile")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__BOND_PRICE__).c_str(), _T("BondPrice")); // Premium

		// Forex
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__FOREX_EXCHANGE__).c_str(), _T("ForexExchange"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__FOREX_SYMBOLS__).c_str(), _T("ForexSymbols"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__FOREX_CANDLES__).c_str(), _T("ForexCandles")); // 历史数据优先级低 Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__FOREX_ALL_RATES__).c_str(), _T("ForexAllRates"));  // Premium

		// Crypto
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__CRYPTO_EXCHANGE__).c_str(), _T("CryptoExchange"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__CRYPTO_SYMBOLS__).c_str(), _T("CryptoSymbols"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__CRYPTO_PROFILE__).c_str(), _T("CryptoProfile")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__CRYPTO_CANDLES__).c_str(), _T("CryptoCandles"));

		// Technical Analysis
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__TECHNICAL_ANALYSIS_PATTERN_RECOGNITION__).c_str(), _T("TechnicalAnalysisPatternRecognition")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__TECHNICAL_ANALYSIS_SURPORT_RESISTANCE__).c_str(), _T("TechnicalAnalysisSurportResistance")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__TECHNICAL_ANALYSIS_AGGREGATE_INDICATORS__).c_str(), _T("TechnicalAnalysisAggregateIndicators")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__TECHNICAL_ANALYSIS_TECHNICAL_INDICATORS__).c_str(), _T("TechnicalAnalysisTechnicalIndicators"));

		// Alternative Data
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__ALTERNATIVE_DATA_TRANSCRIPT_LIST__).c_str(), _T("AlternativeDataTranscriptList")); //Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__ALTERNATIVE_DATA_TRANSCRIPT__).c_str(), _T("AlternativeDataTranscript")); //Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__ALTERNATIVE_DATA_SOCIAL_SETIMENT__).c_str(), _T("AlternativeDataSetiment"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__ALTERNATIVE_DATA_INVEST_THEMES__).c_str(), _T("AlternativeDataInvestThemes")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__ALTERNATIVE_DATA_SUPPLY_CHAIN__).c_str(), _T("AlternativeDataSupplyChain"));  // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__ALTERNATIVE_DATA_COMPANY_ESG__).c_str(), _T("AlternativeDataCompanyESG")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__ALTERNATIVE_DATA_EARNING_QUALITY_SCORE__).c_str(), _T("AlternativeDataQualityScore")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__ALTERNATIVE_DATA_USPTO_PATENTS__).c_str(), _T("AlternativeDataUSPTOpatents"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__ALTERNATIVE_DATA_VISA_APPLICATION__).c_str(), _T("AlternativeDataVISAApplication")); //
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__ALTERNATIVE_DATA_COVID_19__).c_str(), _T("AlternativeDataCOVID_19"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__ALTERNATIVE_DATA_FDA_CALENDAR__).c_str(), _T("AlternativeDataFDACalendar"));

		// Econimic
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__ECONOMIC_COUNTRY_LIST__).c_str(), _T("EconomicCountryList"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__ECONOMIC_CALENDAR__).c_str(), _T("EconomicCalendar")); //Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__ECONOMIC_CODES__).c_str(), _T("EconomicCodes")); //Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__ECONOMIC_ECONOMIC__).c_str(), _T("EconomicEconomic")); //Premium
	}
}