#include"pch.h"

#include"WorldMarket.h"
#include"ChinaMarket.h"

#include"GeneralCheck.h"

#include"FinnhubInquiryType.h"

#include"FinnhubInaccessibleExchange.h"
#include"nlohmannJsonDeclaration.h" // 按照顺序输出json，必须使用此ordered_json,以保证解析后的数据与解析前的顺序一致。

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CInaccessibleExchangesTest : public ::testing::Test {
		void SetUp(void) override {
			GeneralCheck();

			vExchanges.push_back(_T("US"));
			vExchanges.push_back(_T("SS"));
			vExchanges.push_back(_T("SZ"));
			inaccessibleExchange.Assign(_T("COMPANY_NEWS"), 1, vExchanges);
		}

		void TearDown(void) override {
			gl_systemStatus.SetWorkingMode(false);

			GeneralCheck();
		}

	protected:
		vector<CString> vExchanges;
		CInaccessibleExchanges inaccessibleExchange;
	};

	TEST_F(CInaccessibleExchangesTest, TestAssign) {
		EXPECT_TRUE(inaccessibleExchange.HaveExchange(_T("US"))) << "默认包括US";
		EXPECT_TRUE(inaccessibleExchange.HaveExchange(_T("SZ"))) << "默认包括US";
		EXPECT_TRUE(inaccessibleExchange.HaveExchange(_T("SS"))) << "默认包括US";
		EXPECT_FALSE(inaccessibleExchange.HaveExchange(_T("FA"))) << "默认不包括FA";
		vector<CString> v{_T("US2"), _T("SZ2"), _T("SS2")};
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
		void SetUp(void) override { GeneralCheck(); }

		void TearDown(void) override {
			gl_systemStatus.SetWorkingMode(false);

			GeneralCheck();
		}

	protected:
	};

	TEST_F(CFinnhubInaccessibleExchangeTest, TestGetFinnhubInquiryString1) {
		string s;
		for (int i = WEBSOCKET_TRADES_; i <= WEBSOCKET_NEWS_; i++) {
			s = gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(i);
			EXPECT_TRUE(!s.empty()) << "确保所有的索引都有对应的字符串";
		}
		for (int i = SYMBOL_LOOKUP_; i <= DIVIDENDS_; i++) {
			s = gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(i);
			EXPECT_TRUE(!s.empty()) << "确保所有的索引都有对应的字符串";
		}
		for (int i = STOCK_ESTIMATES_RECOMMENDATION_TRENDS_; i <= STOCK_ESTIMATES_EARNING_CALENDAR_; i++) {
			s = gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(i);
			EXPECT_TRUE(!s.empty()) << "确保所有的索引都有对应的字符串";
		}
		for (int i = STOCK_PRICE_QUOTE_; i <= STOCK_PRICE_DIVIDEND2_; i++) {
			s = gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(i);
			EXPECT_TRUE(!s.empty()) << "确保所有的索引都有对应的字符串";
		}
	}

	TEST_F(CFinnhubInaccessibleExchangeTest, TestGetFinnhubInquiryString2) {
		// Web Socket
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(WEBSOCKET_TRADES_), _T("WebSocketTrades"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(WEBSOCKET_NEWS_), _T("WebSocketNews")); // Premium

		// Stock Fundamentals
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(SYMBOL_LOOKUP_), _T("StockFundamentalsSymolLookup"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(STOCK_SYMBOLS_), _T("StockFundamentalsSymbols"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(COMPANY_PROFILE_), _T("StockFundamentalsCompanyProfile")); //Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(COMPANY_PROFILE_CONCISE_), _T("StockFundamentalsCompanyProfileConcise"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(COMPANY_EXECTIVE_), _T("StockFundamentalsCompanyExective")); //Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(MARKET_NEWS_), _T("StockFundamentalsMarketNews"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(COMPANY_NEWS_), _T("StockFundamentalsCompanyNews"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(PRESS_RELEASE_), _T("StockFundamentalsPressRelease")); //Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(NEWS_SENTIMENT_), _T("StockFundamentalsNewsSentiment"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(PEERS_), _T("StockFundamentalsPeer"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(BASIC_FINANCIALS_), _T("StockFundamentalsBasicFinancials"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(OWNERSHIP_), _T("StockFundamentalsOwnership")); //Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(FUND_OWNERSHIP_), _T("StockFundamentalsFundOwnership")); //Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(INSIDER_TRANSACTION_), _T("StockFundamentalsInsiderTransaction"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(INSIDER_SENTIMENT_), _T("StockFundamentalsInsiderSentiment"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(FINANCIAL_), _T("StockFundamentalsFiniacial")); //Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(FINANCIAL_AS_REPORTED_), _T("StockFundamentalsFinaicialAsReported"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(REVENUE_BREAKDOWN_), _T("StockFundamentalsRevenueBreakdown"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(SEC_FILINGS_), _T("StockFundamentalsSECFilings"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(INTERNATIONAL_FILINGS_), _T("StockFundamentalsInternationalFilings")); //Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(SEC_SENTIMENT_ANALYSIS_), _T("StockFundamentalsSECSentimentAnalysis")); //Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(SIMILARITY_INDEX_), _T("StockFundamentalsSemilarityIndex")); //Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(IPO_CALENDAR_), _T("StockFundamentalsIPOCanlendar"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(DIVIDENDS_), _T("StockFundamentalsDividends")); //Premium

		// Stock Estimates
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(STOCK_ESTIMATES_RECOMMENDATION_TRENDS_), _T("StockExtimatesRecommendationTrend"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(STOCK_ESTIMATES_PRICE_TARGET_), _T("StockExtimatesPriceTarget"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(STOCK_ESTIMATES_UPGRADE_DOWNGRADE_), _T("StockExtimatesUpgradeDownGrade")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(STOCK_ESTIMATES_REVENUE_ESTIMATES_), _T("StockExtimatesRevenueExtimates")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(STOCK_ESTIMATES_EPS_ESTIMATES_), _T("StockExtimatesEPSExtimates")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(STOCK_ESTIMATES_EBITDA_ESTIMATES_), _T("StockExtimatesEBITDAExtimates")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(STOCK_ESTIMATES_EBIT_ESTIMATES_), _T("StockExtimatesEBITExtimates")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(STOCK_ESTIMATES_EPS_SURPRISE_), _T("StockExtimatesEPSSurprise"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(STOCK_ESTIMATES_EARNING_CALENDAR_), _T("StockExtimatesEarningCalendar"));

		// Stock Price
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(STOCK_PRICE_QUOTE_), _T("StockPriceQuote")); // 实时数据优先级最低
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(STOCK_PRICE_CANDLES_), _T("StockPriceCandles")); // 历史数据优先级低
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(STOCK_PRICE_TICK_DATA_), _T("StockPriceTickData")); //Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(STOCK_PRICE_HISTORICAL_NBBO_), _T("StockPriceHistoricalNBBO")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(STOCK_PRICE_LAST_BID_ASK_), _T("StockPriceLastBidAsk")); //Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(STOCK_PRICE_SPLITS_), _T("StockPriceSplits")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(STOCK_PRICE_DIVIDEND2_), _T("StockPriceDividend")); // Premium

		// ETF & Indices
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(ETFS_INDICES_CONSTITUENTS_), _T("ETFSIndicesConstituents"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(ETFS_HISTORY_CONSTITUENTS_), _T("ETFSIndicesHistoryConstitunets")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(ETFS_PROFILE_), _T("ETFSIndicesProfile")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(ETFS_HOLDINGS_), _T("ETFSIndicesHoldings")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(ETFS_SECTOR_), _T("ETFSIndicesSector")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(ETFS_COUNTRY_), _T("ETFSIndicesCountry")); //Premium

		// Mutual funds
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(MUTUAL_FUND_PROFILE_), _T("MutualFundProfile")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(MUTUAL_FUND_HOLDINGS_), _T("MutualFundHoldings")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(MUTUAL_FUND_SECTOR_), _T("MutualFundSector")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(MUTUAL_FUND_COUNTRY_), _T("MutualFundCountry")); // Premium

		// Bonds
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(BOND_PROFILE_), _T("BondProfile")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(BOND_PRICE_), _T("BondPrice")); // Premium

		// Forex
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(FOREX_EXCHANGE_), _T("ForexExchange"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(FOREX_SYMBOLS_), _T("ForexSymbols"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(FOREX_CANDLES_), _T("ForexCandles")); // 历史数据优先级低 Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(FOREX_ALL_RATES_), _T("ForexAllRates")); // Premium

		// Crypto
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(CRYPTO_EXCHANGE_), _T("CryptoExchange"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(CRYPTO_SYMBOLS_), _T("CryptoSymbols"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(CRYPTO_PROFILE_), _T("CryptoProfile")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(CRYPTO_CANDLES_), _T("CryptoCandles"));

		// Technical Analysis
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(TECHNICAL_ANALYSIS_PATTERN_RECOGNITION_), _T("TechnicalAnalysisPatternRecognition")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(TECHNICAL_ANALYSIS_SUPPORT_RESISTANCE_), _T("TechnicalAnalysisSurportResistance")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(TECHNICAL_ANALYSIS_AGGREGATE_INDICATORS_), _T("TechnicalAnalysisAggregateIndicators")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(TECHNICAL_ANALYSIS_TECHNICAL_INDICATORS_), _T("TechnicalAnalysisTechnicalIndicators"));

		// Alternative Data
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(ALTERNATIVE_DATA_TRANSCRIPT_LIST_), _T("AlternativeDataTranscriptList")); //Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(ALTERNATIVE_DATA_TRANSCRIPT_), _T("AlternativeDataTranscript")); //Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(ALTERNATIVE_DATA_SOCIAL_SENTIMENT_), _T("AlternativeDataSentiment"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(ALTERNATIVE_DATA_INVEST_THEMES_), _T("AlternativeDataInvestThemes")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(ALTERNATIVE_DATA_SUPPLY_CHAIN_), _T("AlternativeDataSupplyChain")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(ALTERNATIVE_DATA_COMPANY_ESG_), _T("AlternativeDataCompanyESG")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(ALTERNATIVE_DATA_EARNING_QUALITY_SCORE_), _T("AlternativeDataQualityScore")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(ALTERNATIVE_DATA_USPTO_PATENTS_), _T("AlternativeDataUSPTOpatents"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(ALTERNATIVE_DATA_VISA_APPLICATION_), _T("AlternativeDataVISAApplication")); //
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(ALTERNATIVE_DATA_COVID_19_), _T("AlternativeDataCOVID_19"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(ALTERNATIVE_DATA_FDA_CALENDAR_), _T("AlternativeDataFDACalendar"));

		// Economic
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(ECONOMIC_COUNTRY_LIST_), _T("EconomicCountryList"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(ECONOMIC_CALENDAR_), _T("EconomicCalendar")); //Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(ECONOMIC_CODES_), _T("EconomicCodes")); //Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(ECONOMIC_ECONOMIC_), _T("EconomicEconomic")); //Premium
	}

	TEST_F(CFinnhubInaccessibleExchangeTest, TestGetFinnhubInquiryIndex) {
		// Web Socket
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("WebSocketTrades")), WEBSOCKET_TRADES_);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("WebSocketNews")), WEBSOCKET_NEWS_); // Premium

		// Stock Fundamentals
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockFundamentalsSymolLookup")), SYMBOL_LOOKUP_);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockFundamentalsSymbols")), STOCK_SYMBOLS_);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockFundamentalsCompanyProfile")), COMPANY_PROFILE_); //Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockFundamentalsCompanyProfileConcise")), COMPANY_PROFILE_CONCISE_);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockFundamentalsCompanyExective")), COMPANY_EXECTIVE_); //Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockFundamentalsMarketNews")), MARKET_NEWS_);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockFundamentalsCompanyNews")), COMPANY_NEWS_);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockFundamentalsPressRelease")), PRESS_RELEASE_); //Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockFundamentalsNewsSentiment")), NEWS_SENTIMENT_);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockFundamentalsPeer")), PEERS_);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockFundamentalsBasicFinancials")), BASIC_FINANCIALS_);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockFundamentalsOwnership")), OWNERSHIP_); //Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockFundamentalsFundOwnership")), FUND_OWNERSHIP_); //Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockFundamentalsInsiderTransaction")), INSIDER_TRANSACTION_);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockFundamentalsInsiderSentiment")), INSIDER_SENTIMENT_);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockFundamentalsFiniacial")), FINANCIAL_); //Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockFundamentalsFinaicialAsReported")), FINANCIAL_AS_REPORTED_);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockFundamentalsRevenueBreakdown")), REVENUE_BREAKDOWN_);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockFundamentalsSECFilings")), SEC_FILINGS_);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockFundamentalsInternationalFilings")), INTERNATIONAL_FILINGS_); //Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockFundamentalsSECSentimentAnalysis")), SEC_SENTIMENT_ANALYSIS_); //Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockFundamentalsSemilarityIndex")), SIMILARITY_INDEX_); //Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockFundamentalsIPOCanlendar")), IPO_CALENDAR_);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockFundamentalsDividends")), DIVIDENDS_); //Premium

		// Stock Estimates
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockExtimatesRecommendationTrend")), STOCK_ESTIMATES_RECOMMENDATION_TRENDS_);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockExtimatesPriceTarget")), STOCK_ESTIMATES_PRICE_TARGET_);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockExtimatesUpgradeDownGrade")), STOCK_ESTIMATES_UPGRADE_DOWNGRADE_); // Primium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockExtimatesRevenueExtimates")), STOCK_ESTIMATES_REVENUE_ESTIMATES_); // Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockExtimatesEPSExtimates")), STOCK_ESTIMATES_EPS_ESTIMATES_); // Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockExtimatesEBITDAExtimates")), STOCK_ESTIMATES_EBITDA_ESTIMATES_); // Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockExtimatesEBITExtimates")), STOCK_ESTIMATES_EBIT_ESTIMATES_); // Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockExtimatesEPSSurprise")), STOCK_ESTIMATES_EPS_SURPRISE_);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockExtimatesEarningCalendar")), STOCK_ESTIMATES_EARNING_CALENDAR_);

		// Stock Price
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockPriceQuote")), STOCK_PRICE_QUOTE_); // 实时数据优先级最低
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockPriceCandles")), STOCK_PRICE_CANDLES_); // 历史数据优先级低
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockPriceTickData")), STOCK_PRICE_TICK_DATA_); //Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockPriceHistoricalNBBO")), STOCK_PRICE_HISTORICAL_NBBO_); // Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockPriceLastBidAsk")), STOCK_PRICE_LAST_BID_ASK_); //Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockPriceSplits")), STOCK_PRICE_SPLITS_); // Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockPriceDividend")), STOCK_PRICE_DIVIDEND2_); // Premium

		// ETF & Indices
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("ETFSIndicesConstituents")), ETFS_INDICES_CONSTITUENTS_);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("ETFSIndicesHistoryConstitunets")), ETFS_HISTORY_CONSTITUENTS_); // Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("ETFSIndicesProfile")), ETFS_PROFILE_); // Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("ETFSIndicesHoldings")), ETFS_HOLDINGS_); // Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("ETFSIndicesSector")), ETFS_SECTOR_); // Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("ETFSIndicesCountry")), ETFS_COUNTRY_); //Premium

		// Mutual funds
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("MutualFundProfile")), MUTUAL_FUND_PROFILE_); // Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("MutualFundHoldings")), MUTUAL_FUND_HOLDINGS_); // Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("MutualFundSector")), MUTUAL_FUND_SECTOR_); // Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("MutualFundCountry")), MUTUAL_FUND_COUNTRY_); // Premium

		// Bonds
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("BondProfile")), BOND_PROFILE_); // Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("BondPrice")), BOND_PRICE_); // Premium

		// Forex
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("ForexExchange")), FOREX_EXCHANGE_);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("ForexSymbols")), FOREX_SYMBOLS_);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("ForexCandles")), FOREX_CANDLES_); // 历史数据优先级低 Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("ForexAllRates")), FOREX_ALL_RATES_); // Premium

		// Crypto
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("CryptoExchange")), CRYPTO_EXCHANGE_);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("CryptoSymbols")), CRYPTO_SYMBOLS_);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("CryptoProfile")), CRYPTO_PROFILE_); // Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("CryptoCandles")), CRYPTO_CANDLES_);

		// Technical Analysis
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("TechnicalAnalysisPatternRecognition")), TECHNICAL_ANALYSIS_PATTERN_RECOGNITION_); // Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("TechnicalAnalysisSurportResistance")), TECHNICAL_ANALYSIS_SUPPORT_RESISTANCE_); // Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("TechnicalAnalysisAggregateIndicators")), TECHNICAL_ANALYSIS_AGGREGATE_INDICATORS_); // Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("TechnicalAnalysisTechnicalIndicators")), TECHNICAL_ANALYSIS_TECHNICAL_INDICATORS_);

		// Alternative Data
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("AlternativeDataTranscriptList")), ALTERNATIVE_DATA_TRANSCRIPT_LIST_); //Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("AlternativeDataTranscript")), ALTERNATIVE_DATA_TRANSCRIPT_); //Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("AlternativeDataSentiment")), ALTERNATIVE_DATA_SOCIAL_SENTIMENT_);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("AlternativeDataInvestThemes")), ALTERNATIVE_DATA_INVEST_THEMES_); // Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("AlternativeDataSupplyChain")), ALTERNATIVE_DATA_SUPPLY_CHAIN_); // Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("AlternativeDataCompanyESG")), ALTERNATIVE_DATA_COMPANY_ESG_); // Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("AlternativeDataQualityScore")), ALTERNATIVE_DATA_EARNING_QUALITY_SCORE_); // Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("AlternativeDataUSPTOpatents")), ALTERNATIVE_DATA_USPTO_PATENTS_);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("AlternativeDataVISAApplication")), ALTERNATIVE_DATA_VISA_APPLICATION_); //
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("AlternativeDataCOVID_19")), ALTERNATIVE_DATA_COVID_19_);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("AlternativeDataFDACalendar")), ALTERNATIVE_DATA_FDA_CALENDAR_);

		// Economic
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("EconomicCountryList")), ECONOMIC_COUNTRY_LIST_);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("EconomicCalendar")), ECONOMIC_CALENDAR_); //Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("EconomicCodes")), ECONOMIC_CODES_); //Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("EconomicEconomic")), ECONOMIC_ECONOMIC_); //Premium
	}

	TEST_F(CFinnhubInaccessibleExchangeTest, TestGlobeVariable) {
		json jsFinnhubInaccessibleExchange = json::parse(gl_sFinnhubInaccessibleExchange);
		CInaccessibleExchanges exchange;
		string s2 = jsFinnhubInaccessibleExchange[_T("InaccessibleExchange")][0][_T("Function")];
		exchange.SetFunctionString(s2.c_str());
		for (int i = 0; i < jsFinnhubInaccessibleExchange[_T("InaccessibleExchange")][0][_T("Exchange")].size(); i++) {
			string s = jsFinnhubInaccessibleExchange[_T("InaccessibleExchange")][0][_T("Exchange")][i];
			exchange.AddExchange(s.c_str());
		}
		EXPECT_EQ(jsFinnhubInaccessibleExchange["UpdateDate"], 20221205);
		EXPECT_STREQ(exchange.GetFunctionString(), _T("StockFundamentalsCompanyProfileConcise"));
		EXPECT_STREQ(exchange.GetExchange(0), _T("SS"));
		EXPECT_STREQ(exchange.GetExchange(1), _T("SZ"));
	}

	TEST_F(CFinnhubInaccessibleExchangeTest, TestSaveDB) {
		auto pExchange = make_shared<CInaccessibleExchanges>();
		pExchange->SetFunctionString(_T("WebSocketTrades"));
		pExchange->AddExchange(_T("SS"));
		pExchange->AddExchange(_T("SZ"));

		DeleteFile(gl_systemConfiguration.GetDefaultFileDirectory() + _T("FinnhubInaccessibleExchangeTest.json"));
		gl_finnhubInaccessibleExchange.SetDefaultFileName(_T("FinnhubInaccessibleExchangeTest.json"));

		gl_finnhubInaccessibleExchange.SetUpdateDate(20230101);
		gl_finnhubInaccessibleExchange.SetInaccessibleExchange(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(pExchange->GetFunctionString()), pExchange);
		gl_finnhubInaccessibleExchange.UpdateJson();
		gl_finnhubInaccessibleExchange.SaveDB();

		gl_finnhubInaccessibleExchange.Clear();
		gl_finnhubInaccessibleExchange.LoadDB();
		gl_finnhubInaccessibleExchange.Update();
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetUpdateDate(), 20230101);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetInaccessibleExchangeSize(), 1);
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetInaccessibleExchange(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(pExchange->GetFunctionString()))->GetFunctionString(),
		             _T("WebSocketTrades"));
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetInaccessibleExchange(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(pExchange->GetFunctionString()))->ExchangeSize(), 2);
		CString str;
		str = gl_finnhubInaccessibleExchange.GetInaccessibleExchange(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(pExchange->GetFunctionString()))->GetExchange(0);
		EXPECT_STREQ(str, _T("SS"));

		// 恢复原状
		DeleteFile(gl_systemConfiguration.GetDefaultFileDirectory() + _T("FinnhubInaccessibleExchangeTest.json"));
		gl_finnhubInaccessibleExchange.SetDefaultFileName(_T("FinnhubInaccessibleExchange.json"));
	}
}
