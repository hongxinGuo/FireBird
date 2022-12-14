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
		for (int i = _WEBSOCKET_TRADES_; i <= _WEBSOCKET_NEWS_; i++) {
			s = gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(i);
			EXPECT_TRUE(!s.empty()) << "确保所有的索引都有对应的字符串";
		}
		for (int i = _SYMBOL_LOOKUP_; i <= _DIVIDENDS_; i++) {
			s = gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(i);
			EXPECT_TRUE(!s.empty()) << "确保所有的索引都有对应的字符串";
		}
		for (int i = _STOCK_ESTIMATES_RECOMMENDATION_TRENDS_; i <= _STOCK_ESTIMATES_EARNING_CALENDAR_; i++) {
			s = gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(i);
			EXPECT_TRUE(!s.empty()) << "确保所有的索引都有对应的字符串";
		}
		for (int i = _STOCK_PRICE_QUOTE_; i <= _STOCK_PRICE_DIVIDEND2_; i++) {
			s = gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(i);
			EXPECT_TRUE(!s.empty()) << "确保所有的索引都有对应的字符串";
		}
	}

	TEST_F(CFinnhubInaccessibleExchangeTest, TestGetFinnhubInquiryString2) {
		// Web Socket
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_WEBSOCKET_TRADES_), _T("WebSocketTrades"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_WEBSOCKET_NEWS_), _T("WebSocketNews")); // Premium

		// Stock Fundamentals
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_SYMBOL_LOOKUP_), _T("StockFundamentalsSymolLookup"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_STOCK_SYMBOLS_), _T("StockFundamentalsSymbols"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_COMPANY_PROFILE_), _T("StockFundamentalsCompanyProfile")); //Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_COMPANY_PROFILE_CONCISE_), _T("StockFundamentalsCompanyProfileConcise"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_COMPANY_EXECTIVE_), _T("StockFundamentalsCompanyExective")); //Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_MARKET_NEWS_), _T("StockFundamentalsMarketNews"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_COMPANY_NEWS_), _T("StockFundamentalsCompanyNews"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_PRESS_RELEASE_), _T("StockFundamentalsPressRelease")); //Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_NEWS_SENTIMENT_), _T("StockFundamentalsNewsSentiment"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_PEERS_), _T("StockFundamentalsPeer"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_BASIC_FINANCIALS_), _T("StockFundamentalsBasicFinancials"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_OWNERSHIP_), _T("StockFundamentalsOwnership")); //Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_FUND_OWNERSHIP_), _T("StockFundamentalsFundOwnership")); //Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_INSIDER_TRANSACTION_), _T("StockFundamentalsInsiderTransaction"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_INSIDER_SENTIMENT_), _T("StockFundamentalsInsiderSentiment"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_FINANCIAL_), _T("StockFundamentalsFiniacial")); //Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_FINAICIAL_AS_REPORTED_), _T("StockFundamentalsFinaicialAsReported"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_REVENUE_BREAKDOWN_), _T("StockFundamentalsRevenueBreakdown"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_SEC_FILINGS_), _T("StockFundamentalsSECFilings"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_INTERNATIONAL_FILINGS_), _T("StockFundamentalsInternationalFilings")); //Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_SEC_SENTIMENT_ANALYSIS_), _T("StockFundamentalsSECSentimentAnalysis")); //Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_SIMILARITY_INDEX_), _T("StockFundamentalsSemilarityIndex")); //Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_IPO_CALENDAR_), _T("StockFundamentalsIPOCanlendar"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_DIVIDENDS_), _T("StockFundamentalsDividends")); //Premium

		// Stock Estimates
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_STOCK_ESTIMATES_RECOMMENDATION_TRENDS_), _T("StockExtimatesRecommendationTrend"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_STOCK_ESTIMATES_PRICE_TARGET_), _T("StockExtimatesPriceTarget"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_STOCK_ESTIMATES_UPGRADE_DOWNGRADE_), _T("StockExtimatesUpgradeDownGrade")); // Primium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_STOCK_ESTIMATES_REVENUE_EXTIMATES_), _T("StockExtimatesRevenueExtimates")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_STOCK_ESTIMATES_EPS_EXTIMATES_), _T("StockExtimatesEPSExtimates")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_STOCK_EXTIMATES_EBITDA_EXTIMATES_), _T("StockExtimatesEBITDAExtimates")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_STOCK_EXTIMATES_EBIT_EXTIMATES_), _T("StockExtimatesEBITExtimates")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_STOCK_ESTIMATES_EPS_SURPRISE_), _T("StockExtimatesEPSSurprise"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_STOCK_ESTIMATES_EARNING_CALENDAR_), _T("StockExtimatesEarningCalendar"));

		// Stock Price
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_STOCK_PRICE_QUOTE_), _T("StockPriceQuote")); // 实时数据优先级最低
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_STOCK_PRICE_CANDLES_), _T("StockPriceCandles")); // 历史数据优先级低
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_STOCK_PRICE_TICK_DATA_), _T("StockPriceTickData")); //Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_STOCK_PRICE_HISTORICAL_NBBO_), _T("StockPriceHistoricalNBBO")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_STOCK_PRICE_LAST_BID_ASK_), _T("StockPriceLastBidAsk")); //Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_STOCK_PRICE_SPLITS_), _T("StockPriceSplits")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_STOCK_PRICE_DIVIDEND2_), _T("StockPriceDividend")); // Premium

		// ETF & Indices
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_ETFS_INDICES_CONSTITUENTS_), _T("ETFSIndicesConstituents"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_ETFS_HISTORY_CONSTITUENTS_), _T("ETFSIndicesHistoryConstitunets")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_ETFS_PROFILE_), _T("ETFSIndicesProfile")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_ETFS_HOLDINGS_), _T("ETFSIndicesHoldings")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_ETFS_SECTOR_), _T("ETFSIndicesSector")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_ETFS_COUNTRY_), _T("ETFSIndicesCountry")); //Premium

		// Mutual funds
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_MUTUAL_FUND_PROFILE_), _T("MutualFundProfile")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_MUTUAL_FUND_HOLDINGS_), _T("MutualFundHoldings")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_MUTUAL_FUND_SECTOR_), _T("MutualFundSector")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_MUTUAL_FUND_COUNTRY_), _T("MutualFundCountry")); // Premium

		// Bonds
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_BOND_PROFILE_), _T("BondProfile")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_BOND_PRICE_), _T("BondPrice")); // Premium

		// Forex
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_FOREX_EXCHANGE_), _T("ForexExchange"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_FOREX_SYMBOLS_), _T("ForexSymbols"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_FOREX_CANDLES_), _T("ForexCandles")); // 历史数据优先级低 Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_FOREX_ALL_RATES_), _T("ForexAllRates")); // Premium

		// Crypto
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_CRYPTO_EXCHANGE_), _T("CryptoExchange"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_CRYPTO_SYMBOLS_), _T("CryptoSymbols"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_CRYPTO_PROFILE_), _T("CryptoProfile")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_CRYPTO_CANDLES_), _T("CryptoCandles"));

		// Technical Analysis
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_TECHNICAL_ANALYSIS_PATTERN_RECOGNITION_), _T("TechnicalAnalysisPatternRecognition")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_TECHNICAL_ANALYSIS_SURPORT_RESISTANCE_), _T("TechnicalAnalysisSurportResistance")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_TECHNICAL_ANALYSIS_AGGREGATE_INDICATORS_), _T("TechnicalAnalysisAggregateIndicators")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_TECHNICAL_ANALYSIS_TECHNICAL_INDICATORS_), _T("TechnicalAnalysisTechnicalIndicators"));

		// Alternative Data
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_ALTERNATIVE_DATA_TRANSCRIPT_LIST_), _T("AlternativeDataTranscriptList")); //Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_ALTERNATIVE_DATA_TRANSCRIPT_), _T("AlternativeDataTranscript")); //Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_ALTERNATIVE_DATA_SOCIAL_SETIMENT_), _T("AlternativeDataSetiment"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_ALTERNATIVE_DATA_INVEST_THEMES_), _T("AlternativeDataInvestThemes")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_ALTERNATIVE_DATA_SUPPLY_CHAIN_), _T("AlternativeDataSupplyChain")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_ALTERNATIVE_DATA_COMPANY_ESG_), _T("AlternativeDataCompanyESG")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_ALTERNATIVE_DATA_EARNING_QUALITY_SCORE_), _T("AlternativeDataQualityScore")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_ALTERNATIVE_DATA_USPTO_PATENTS_), _T("AlternativeDataUSPTOpatents"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_ALTERNATIVE_DATA_VISA_APPLICATION_), _T("AlternativeDataVISAApplication")); //
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_ALTERNATIVE_DATA_COVID_19_), _T("AlternativeDataCOVID_19"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_ALTERNATIVE_DATA_FDA_CALENDAR_), _T("AlternativeDataFDACalendar"));

		// Econimic
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_ECONOMIC_COUNTRY_LIST_), _T("EconomicCountryList"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_ECONOMIC_CALENDAR_), _T("EconomicCalendar")); //Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_ECONOMIC_CODES_), _T("EconomicCodes")); //Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(_ECONOMIC_ECONOMIC_), _T("EconomicEconomic")); //Premium
	}

	TEST_F(CFinnhubInaccessibleExchangeTest, TestGetFinnhubInquiryIndex) {
		// Web Socket
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("WebSocketTrades")), _WEBSOCKET_TRADES_);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("WebSocketNews")), _WEBSOCKET_NEWS_); // Premium

		// Stock Fundamentals
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockFundamentalsSymolLookup")), _SYMBOL_LOOKUP_);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockFundamentalsSymbols")), _STOCK_SYMBOLS_);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockFundamentalsCompanyProfile")), _COMPANY_PROFILE_); //Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockFundamentalsCompanyProfileConcise")), _COMPANY_PROFILE_CONCISE_);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockFundamentalsCompanyExective")), _COMPANY_EXECTIVE_); //Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockFundamentalsMarketNews")), _MARKET_NEWS_);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockFundamentalsCompanyNews")), _COMPANY_NEWS_);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockFundamentalsPressRelease")), _PRESS_RELEASE_); //Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockFundamentalsNewsSentiment")), _NEWS_SENTIMENT_);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockFundamentalsPeer")), _PEERS_);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockFundamentalsBasicFinancials")), _BASIC_FINANCIALS_);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockFundamentalsOwnership")), _OWNERSHIP_); //Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockFundamentalsFundOwnership")), _FUND_OWNERSHIP_); //Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockFundamentalsInsiderTransaction")), _INSIDER_TRANSACTION_);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockFundamentalsInsiderSentiment")), _INSIDER_SENTIMENT_);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockFundamentalsFiniacial")), _FINANCIAL_); //Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockFundamentalsFinaicialAsReported")), _FINAICIAL_AS_REPORTED_);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockFundamentalsRevenueBreakdown")), _REVENUE_BREAKDOWN_);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockFundamentalsSECFilings")), _SEC_FILINGS_);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockFundamentalsInternationalFilings")), _INTERNATIONAL_FILINGS_); //Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockFundamentalsSECSentimentAnalysis")), _SEC_SENTIMENT_ANALYSIS_); //Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockFundamentalsSemilarityIndex")), _SIMILARITY_INDEX_); //Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockFundamentalsIPOCanlendar")), _IPO_CALENDAR_);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockFundamentalsDividends")), _DIVIDENDS_); //Premium

		// Stock Estimates
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockExtimatesRecommendationTrend")), _STOCK_ESTIMATES_RECOMMENDATION_TRENDS_);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockExtimatesPriceTarget")), _STOCK_ESTIMATES_PRICE_TARGET_);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockExtimatesUpgradeDownGrade")), _STOCK_ESTIMATES_UPGRADE_DOWNGRADE_); // Primium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockExtimatesRevenueExtimates")), _STOCK_ESTIMATES_REVENUE_EXTIMATES_); // Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockExtimatesEPSExtimates")), _STOCK_ESTIMATES_EPS_EXTIMATES_); // Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockExtimatesEBITDAExtimates")), _STOCK_EXTIMATES_EBITDA_EXTIMATES_); // Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockExtimatesEBITExtimates")), _STOCK_EXTIMATES_EBIT_EXTIMATES_); // Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockExtimatesEPSSurprise")), _STOCK_ESTIMATES_EPS_SURPRISE_);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockExtimatesEarningCalendar")), _STOCK_ESTIMATES_EARNING_CALENDAR_);

		// Stock Price
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockPriceQuote")), _STOCK_PRICE_QUOTE_); // 实时数据优先级最低
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockPriceCandles")), _STOCK_PRICE_CANDLES_); // 历史数据优先级低
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockPriceTickData")), _STOCK_PRICE_TICK_DATA_); //Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockPriceHistoricalNBBO")), _STOCK_PRICE_HISTORICAL_NBBO_); // Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockPriceLastBidAsk")), _STOCK_PRICE_LAST_BID_ASK_); //Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockPriceSplits")), _STOCK_PRICE_SPLITS_); // Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockPriceDividend")), _STOCK_PRICE_DIVIDEND2_); // Premium

		// ETF & Indices
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("ETFSIndicesConstituents")), _ETFS_INDICES_CONSTITUENTS_);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("ETFSIndicesHistoryConstitunets")), _ETFS_HISTORY_CONSTITUENTS_); // Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("ETFSIndicesProfile")), _ETFS_PROFILE_); // Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("ETFSIndicesHoldings")), _ETFS_HOLDINGS_); // Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("ETFSIndicesSector")), _ETFS_SECTOR_); // Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("ETFSIndicesCountry")), _ETFS_COUNTRY_); //Premium

		// Mutual funds
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("MutualFundProfile")), _MUTUAL_FUND_PROFILE_); // Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("MutualFundHoldings")), _MUTUAL_FUND_HOLDINGS_); // Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("MutualFundSector")), _MUTUAL_FUND_SECTOR_); // Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("MutualFundCountry")), _MUTUAL_FUND_COUNTRY_); // Premium

		// Bonds
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("BondProfile")), _BOND_PROFILE_); // Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("BondPrice")), _BOND_PRICE_); // Premium

		// Forex
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("ForexExchange")), _FOREX_EXCHANGE_);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("ForexSymbols")), _FOREX_SYMBOLS_);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("ForexCandles")), _FOREX_CANDLES_); // 历史数据优先级低 Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("ForexAllRates")), _FOREX_ALL_RATES_); // Premium

		// Crypto
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("CryptoExchange")), _CRYPTO_EXCHANGE_);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("CryptoSymbols")), _CRYPTO_SYMBOLS_);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("CryptoProfile")), _CRYPTO_PROFILE_); // Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("CryptoCandles")), _CRYPTO_CANDLES_);

		// Technical Analysis
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("TechnicalAnalysisPatternRecognition")), _TECHNICAL_ANALYSIS_PATTERN_RECOGNITION_); // Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("TechnicalAnalysisSurportResistance")), _TECHNICAL_ANALYSIS_SURPORT_RESISTANCE_); // Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("TechnicalAnalysisAggregateIndicators")), _TECHNICAL_ANALYSIS_AGGREGATE_INDICATORS_); // Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("TechnicalAnalysisTechnicalIndicators")), _TECHNICAL_ANALYSIS_TECHNICAL_INDICATORS_);

		// Alternative Data
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("AlternativeDataTranscriptList")), _ALTERNATIVE_DATA_TRANSCRIPT_LIST_); //Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("AlternativeDataTranscript")), _ALTERNATIVE_DATA_TRANSCRIPT_); //Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("AlternativeDataSetiment")), _ALTERNATIVE_DATA_SOCIAL_SETIMENT_);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("AlternativeDataInvestThemes")), _ALTERNATIVE_DATA_INVEST_THEMES_); // Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("AlternativeDataSupplyChain")), _ALTERNATIVE_DATA_SUPPLY_CHAIN_); // Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("AlternativeDataCompanyESG")), _ALTERNATIVE_DATA_COMPANY_ESG_); // Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("AlternativeDataQualityScore")), _ALTERNATIVE_DATA_EARNING_QUALITY_SCORE_); // Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("AlternativeDataUSPTOpatents")), _ALTERNATIVE_DATA_USPTO_PATENTS_);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("AlternativeDataVISAApplication")), _ALTERNATIVE_DATA_VISA_APPLICATION_); //
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("AlternativeDataCOVID_19")), _ALTERNATIVE_DATA_COVID_19_);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("AlternativeDataFDACalendar")), _ALTERNATIVE_DATA_FDA_CALENDAR_);

		// Econimic
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("EconomicCountryList")), _ECONOMIC_COUNTRY_LIST_);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("EconomicCalendar")), _ECONOMIC_CALENDAR_); //Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("EconomicCodes")), _ECONOMIC_CODES_); //Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("EconomicEconomic")), _ECONOMIC_ECONOMIC_); //Premium
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
		EXPECT_STREQ(exchange.GetFunctionString(), _T("StockFundamentalsCompanyProfileConcise"));
		EXPECT_STREQ(exchange.GetExchange(0), _T("SS"));
		EXPECT_STREQ(exchange.GetExchange(1), _T("SZ"));
	}

	TEST_F(CFinnhubInaccessibleExchangeTest, TestSaveDB) {
		CInaccessibleExchangesPtr pExchange = make_shared<CInaccessibleExchanges>();
		pExchange->SetFunctionString(_T("WebSocketTrades"));
		pExchange->AddExchange(_T("SS"));
		pExchange->AddExchange(_T("SZ"));

		DeleteFile(gl_systemConfiguration.GetDefaultFileDirectory() + _T("FinnhubInaccessibleExchangeTest.json"));
		gl_finnhubInaccessibleExchange.SetDefaultFileName(_T("FinnhubInaccessibleExchangeTest.json"));

		gl_finnhubInaccessibleExchange.SetInaccessibleExchange(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(pExchange->GetFunctionString()), pExchange);
		gl_finnhubInaccessibleExchange.UpdateJson();
		gl_finnhubInaccessibleExchange.SaveDB();

		gl_finnhubInaccessibleExchange.Clear();
		gl_finnhubInaccessibleExchange.LoadDB();
		gl_finnhubInaccessibleExchange.Update();
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
