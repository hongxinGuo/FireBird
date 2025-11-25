#include"pch.h"

#include"WorldMarket.h"
#include"GeneralCheck.h"
#include"FinnhubInquiryType.h"

namespace FireBirdTest {
	class CXferFinnhubInquiryTest : public ::testing::Test {
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void TearDown() override {
			gl_systemConfiguration.SetWorkingMode(false);

			SCOPED_TRACE("");
			GeneralCheck();
		}
	};

	TEST_F(CXferFinnhubInquiryTest, TestGetFinnhubInquiryString1) {
		string s;
		s.clear();
		for (int i = WEBSOCKET_TRADES_; i <= WEBSOCKET_NEWS_; i++) {
			s = gl_FinnhubInquiryType.GetInquiryString(i);
			EXPECT_TRUE(!s.empty()) << "确保所有的索引都有对应的字符串";
			s.clear();
		}
		for (int i = SYMBOL_LOOKUP_; i <= ISIN_CHANGE_; i++) {
			s = gl_FinnhubInquiryType.GetInquiryString(i);
			EXPECT_TRUE(!s.empty()) << "确保所有的索引都有对应的字符串";
			s.clear();
		}
		for (int i = STOCK_ESTIMATES_RECOMMENDATION_TRENDS_; i <= STOCK_ESTIMATES_EARNING_CALENDAR_; i++) {
			s = gl_FinnhubInquiryType.GetInquiryString(i);
			EXPECT_TRUE(!s.empty()) << "确保所有的索引都有对应的字符串";
			s.clear();
		}
		for (int i = STOCK_PRICE_QUOTE_; i <= STOCK_PRICE_DIVIDEND2_; i++) {
			s = gl_FinnhubInquiryType.GetInquiryString(i);
			EXPECT_TRUE(!s.empty()) << "确保所有的索引都有对应的字符串";
			s.clear();
		}
		for (int i = ETFS_INDICES_CONSTITUENTS_; i <= ETFS_COUNTRY_; i++) {
			s = gl_FinnhubInquiryType.GetInquiryString(i);
			EXPECT_TRUE(!s.empty()) << "确保所有的索引都有对应的字符串";
			s.clear();
		}
		for (int i = MUTUAL_FUND_PROFILE_; i <= MUTUAL_FUND_COUNTRY_; i++) {
			s = gl_FinnhubInquiryType.GetInquiryString(i);
			EXPECT_TRUE(!s.empty()) << "确保所有的索引都有对应的字符串";
			s.clear();
		}
		for (int i = BOND_PROFILE_; i <= BOND_PRICE_; i++) {
			s = gl_FinnhubInquiryType.GetInquiryString(i);
			EXPECT_TRUE(!s.empty()) << "确保所有的索引都有对应的字符串";
			s.clear();
		}
		for (int i = FOREX_EXCHANGE_; i <= FOREX_ALL_RATES_; i++) {
			s = gl_FinnhubInquiryType.GetInquiryString(i);
			EXPECT_TRUE(!s.empty()) << "确保所有的索引都有对应的字符串";
			s.clear();
		}
		for (int i = CRYPTO_EXCHANGE_; i <= CRYPTO_CANDLES_; i++) {
			s = gl_FinnhubInquiryType.GetInquiryString(i);
			EXPECT_TRUE(!s.empty()) << "确保所有的索引都有对应的字符串";
			s.clear();
		}
		for (int i = TECHNICAL_ANALYSIS_PATTERN_RECOGNITION_; i <= TECHNICAL_ANALYSIS_TECHNICAL_INDICATORS_; i++) {
			s = gl_FinnhubInquiryType.GetInquiryString(i);
			EXPECT_TRUE(!s.empty()) << "确保所有的索引都有对应的字符串";
			s.clear();
		}
		for (int i = ALTERNATIVE_DATA_TRANSCRIPT_LIST_; i <= ALTERNATIVE_DATA_FDA_CALENDAR_; i++) {
			s = gl_FinnhubInquiryType.GetInquiryString(i);
			EXPECT_TRUE(!s.empty()) << "确保所有的索引都有对应的字符串";
		}
		for (int i = ECONOMIC_COUNTRY_LIST_; i <= ECONOMIC_ECONOMIC_; i++) {
			s = gl_FinnhubInquiryType.GetInquiryString(i);
			EXPECT_TRUE(!s.empty()) << "确保所有的索引都有对应的字符串";
			s.clear();
		}
	}

	TEST_F(CXferFinnhubInquiryTest, TestGetFinnhubInquiryString2) {
		// Web Socket
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(WEBSOCKET_TRADES_).compare("WebSocketTrades") == 0);
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(WEBSOCKET_NEWS_).compare("WebSocketNews")== 0); // Premium

		// Stock Fundamentals
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(SYMBOL_LOOKUP_).compare("StockFundamentalsSymbolLookup")== 0);
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(STOCK_SYMBOLS_).compare("StockFundamentalsSymbols")== 0);
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(MARKET_STATUS_).compare("StockFundamentalsMarketStatus")== 0);
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(MARKET_HOLIDAY_).compare("StockFundamentalsMarketHoliday")== 0);
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(COMPANY_PROFILE_).compare("StockFundamentalsCompanyProfile")== 0); //Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(COMPANY_PROFILE_CONCISE_).compare("StockFundamentalsCompanyProfileConcise")== 0);
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(COMPANY_EXECUTIVE_).compare("StockFundamentalsCompanyExecutive")== 0); //Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(MARKET_NEWS_).compare("StockFundamentalsMarketNews")== 0);
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(COMPANY_NEWS_).compare("StockFundamentalsCompanyNews")== 0);
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(PRESS_RELEASE_).compare("StockFundamentalsPressRelease")== 0); //Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(NEWS_SENTIMENT_).compare("StockFundamentalsNewsSentiment")== 0);
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(PEERS_).compare("StockFundamentalsPeer")== 0);
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(BASIC_FINANCIALS_).compare("StockFundamentalsBasicFinancials")== 0);
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(OWNERSHIP_).compare("StockFundamentalsOwnership")== 0); //Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(FUND_OWNERSHIP_).compare("StockFundamentalsFundOwnership")== 0); //Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(INSIDER_TRANSACTION_).compare("StockFundamentalsInsiderTransaction")== 0);
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(INSIDER_SENTIMENT_).compare("StockFundamentalsInsiderSentiment")== 0);
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(FINANCIAL_).compare("StockFundamentalsFinancial")== 0); //Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(FINANCIAL_AS_REPORTED_).compare("StockFundamentalsFinancialAsReported")== 0);
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(REVENUE_BREAKDOWN_).compare("StockFundamentalsRevenueBreakdown")== 0);
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(SEC_FILINGS_).compare("StockFundamentalsSECFilings")== 0);
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(INTERNATIONAL_FILINGS_).compare("StockFundamentalsInternationalFilings")== 0); //Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(SEC_SENTIMENT_ANALYSIS_).compare("StockFundamentalsSECSentimentAnalysis")== 0); //Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(SIMILARITY_INDEX_).compare("StockFundamentalsSimilarityIndex")== 0); //Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(IPO_CALENDAR_).compare("StockFundamentalsIPOCalendar")== 0);
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(DIVIDENDS_).compare("StockFundamentalsDividends")== 0); //Premium

		// Stock Estimates
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(STOCK_ESTIMATES_RECOMMENDATION_TRENDS_).compare("StockEstimatesRecommendationTrends")== 0);
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(STOCK_ESTIMATES_PRICE_TARGET_).compare("StockEstimatesPriceTarget")== 0);
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(STOCK_ESTIMATES_UPGRADE_DOWNGRADE_).compare("StockEstimatesUpgradeDownGrade")== 0); // Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(STOCK_ESTIMATES_REVENUE_ESTIMATES_).compare("StockEstimatesRevenueEstimates")== 0); // Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(STOCK_ESTIMATES_EPS_ESTIMATES_).compare("StockEstimatesEPSEstimates")== 0); // Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(STOCK_ESTIMATES_EBITDA_ESTIMATES_).compare("StockEstimatesEBITDAEstimates")== 0); // Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(STOCK_ESTIMATES_EBIT_ESTIMATES_).compare("StockEstimatesEBITEstimates")== 0); // Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(STOCK_ESTIMATES_EPS_SURPRISE_).compare("StockEstimatesEPSSurprise")== 0);
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(STOCK_ESTIMATES_EARNING_CALENDAR_).compare("StockEstimatesEarningCalendar")== 0);

		// Stock Price
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(STOCK_PRICE_QUOTE_).compare("StockPriceQuote")== 0); // 实时数据优先级最低
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(STOCK_PRICE_CANDLES_).compare("StockPriceCandles")== 0); // 历史数据优先级低
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(STOCK_PRICE_TICK_DATA_).compare("StockPriceTickData")== 0); //Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(STOCK_PRICE_HISTORICAL_NBBO_).compare("StockPriceHistoricalNBBO")== 0); // Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(STOCK_PRICE_LAST_BID_ASK_).compare("StockPriceLastBidAsk")== 0); //Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(STOCK_PRICE_SPLITS_).compare("StockPriceSplits")== 0); // Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(STOCK_PRICE_DIVIDEND2_).compare("StockPriceDividend")== 0); // Premium

		// ETF & Indices
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(ETFS_INDICES_CONSTITUENTS_).compare("ETFSIndicesConstituents")== 0);
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(ETFS_HISTORY_CONSTITUENTS_).compare("ETFSIndicesHistoryConstituents")== 0); // Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(ETFS_PROFILE_).compare("ETFSIndicesProfile")== 0); // Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(ETFS_HOLDINGS_).compare("ETFSIndicesHoldings")== 0); // Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(ETFS_SECTOR_).compare("ETFSIndicesSector")== 0); // Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(ETFS_COUNTRY_).compare("ETFSIndicesCountry")== 0); //Premium

		// Mutual funds
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(MUTUAL_FUND_PROFILE_).compare("MutualFundProfile")== 0); // Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(MUTUAL_FUND_HOLDINGS_).compare("MutualFundHoldings")== 0); // Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(MUTUAL_FUND_SECTOR_).compare("MutualFundSector")== 0); // Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(MUTUAL_FUND_COUNTRY_).compare("MutualFundCountry")== 0); // Premium

		// Bonds
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(BOND_PROFILE_).compare("BondProfile")== 0); // Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(BOND_PRICE_).compare("BondPrice")== 0); // Premium

		// Forex
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(FOREX_EXCHANGE_).compare("ForexExchange")== 0);
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(FOREX_SYMBOLS_).compare("ForexSymbols")== 0);
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(FOREX_CANDLES_).compare("ForexCandles")== 0); // 历史数据优先级低 Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(FOREX_ALL_RATES_).compare("ForexAllRates")== 0); // Premium

		// Crypto
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(CRYPTO_EXCHANGE_).compare("CryptoExchange")== 0);
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(CRYPTO_SYMBOLS_).compare("CryptoSymbols")== 0);
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(CRYPTO_PROFILE_).compare("CryptoProfile")== 0); // Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(CRYPTO_CANDLES_).compare("CryptoCandles")== 0);

		// Technical Analysis
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(TECHNICAL_ANALYSIS_PATTERN_RECOGNITION_).compare("TechnicalAnalysisPatternRecognition")== 0); // Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(TECHNICAL_ANALYSIS_SUPPORT_RESISTANCE_).compare("TechnicalAnalysisSupportResistance")== 0); // Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(TECHNICAL_ANALYSIS_AGGREGATE_INDICATORS_).compare("TechnicalAnalysisAggregateIndicators")== 0); // Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(TECHNICAL_ANALYSIS_TECHNICAL_INDICATORS_).compare("TechnicalAnalysisTechnicalIndicators")== 0);

		// Alternative Data
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(ALTERNATIVE_DATA_TRANSCRIPT_LIST_).compare("AlternativeDataTranscriptList")== 0); //Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(ALTERNATIVE_DATA_TRANSCRIPT_).compare("AlternativeDataTranscript")== 0); //Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(ALTERNATIVE_DATA_SOCIAL_SENTIMENT_).compare("AlternativeDataSentiment")== 0);
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(ALTERNATIVE_DATA_INVEST_THEMES_).compare("AlternativeDataInvestThemes")== 0); // Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(ALTERNATIVE_DATA_SUPPLY_CHAIN_).compare("AlternativeDataSupplyChain")== 0); // Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(ALTERNATIVE_DATA_COMPANY_ESG_).compare("AlternativeDataCompanyESG")== 0); // Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(ALTERNATIVE_DATA_EARNING_QUALITY_SCORE_).compare("AlternativeDataQualityScore")== 0); // Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(ALTERNATIVE_DATA_USPTO_PATENTS_).compare("AlternativeDataUSPTOPatents")== 0);
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(ALTERNATIVE_DATA_VISA_APPLICATION_).compare("AlternativeDataVISAApplication")== 0); //
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(ALTERNATIVE_DATA_COVID_19_).compare("AlternativeDataCOVID_19")== 0);
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(ALTERNATIVE_DATA_FDA_CALENDAR_).compare("AlternativeDataFDACalendar")== 0);

		// Economic
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(ECONOMIC_COUNTRY_LIST_).compare("EconomicCountryList")== 0);
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(ECONOMIC_CALENDAR_).compare("EconomicCalendar")== 0); //Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(ECONOMIC_CODES_).compare("EconomicCodes")== 0); //Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(ECONOMIC_ECONOMIC_).compare("EconomicEconomic")== 0); //Premium
	}

	TEST_F(CXferFinnhubInquiryTest, TestGetFinnhubInquiryIndex) {
		// Web Socket
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("WebSocketTrades"), WEBSOCKET_TRADES_);
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("WebSocketNews"), WEBSOCKET_NEWS_); // Premium

		// Stock Fundamentals
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("StockFundamentalsSymbolLookup"), SYMBOL_LOOKUP_);
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("StockFundamentalsSymbols"), STOCK_SYMBOLS_);
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("StockFundamentalsMarketStatus"), MARKET_STATUS_);
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("StockFundamentalsMarketHoliday"), MARKET_HOLIDAY_);
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("StockFundamentalsCompanyProfile"), COMPANY_PROFILE_); //Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("StockFundamentalsCompanyProfileConcise"), COMPANY_PROFILE_CONCISE_);
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("StockFundamentalsCompanyExecutive"), COMPANY_EXECUTIVE_); //Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("StockFundamentalsMarketNews"), MARKET_NEWS_);
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("StockFundamentalsCompanyNews"), COMPANY_NEWS_);
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("StockFundamentalsPressRelease"), PRESS_RELEASE_); //Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("StockFundamentalsNewsSentiment"), NEWS_SENTIMENT_);
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("StockFundamentalsPeer"), PEERS_);
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("StockFundamentalsBasicFinancials"), BASIC_FINANCIALS_);
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("StockFundamentalsOwnership"), OWNERSHIP_); //Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("StockFundamentalsFundOwnership"), FUND_OWNERSHIP_); //Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("StockFundamentalsInsiderTransaction"), INSIDER_TRANSACTION_);
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("StockFundamentalsInsiderSentiment"), INSIDER_SENTIMENT_);
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("StockFundamentalsFinancial"), FINANCIAL_); //Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("StockFundamentalsFinancialAsReported"), FINANCIAL_AS_REPORTED_);
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("StockFundamentalsRevenueBreakdown"), REVENUE_BREAKDOWN_);
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("StockFundamentalsSECFilings"), SEC_FILINGS_);
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("StockFundamentalsInternationalFilings"), INTERNATIONAL_FILINGS_); //Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("StockFundamentalsSECSentimentAnalysis"), SEC_SENTIMENT_ANALYSIS_); //Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("StockFundamentalsSimilarityIndex"), SIMILARITY_INDEX_); //Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("StockFundamentalsIPOCalendar"), IPO_CALENDAR_);
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("StockFundamentalsDividends"), DIVIDENDS_); //Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("StockFundamentalsSectorMetrics"), SECTOR_METRICS_); //Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("StockFundamentalsPriceMetrics"), PRICE_METRICS_); //Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("StockFundamentalsSymbolChange"), SYMBOL_CHANGE_); //Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("StockFundamentalsISINChange"), ISIN_CHANGE_); //Premium

		// Stock Estimates
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("StockEstimatesRecommendationTrends"), STOCK_ESTIMATES_RECOMMENDATION_TRENDS_);
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("StockEstimatesPriceTarget"), STOCK_ESTIMATES_PRICE_TARGET_);
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("StockEstimatesUpgradeDownGrade"), STOCK_ESTIMATES_UPGRADE_DOWNGRADE_); // Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("StockEstimatesRevenueEstimates"), STOCK_ESTIMATES_REVENUE_ESTIMATES_); // Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("StockEstimatesEPSEstimates"), STOCK_ESTIMATES_EPS_ESTIMATES_); // Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("StockEstimatesEBITDAEstimates"), STOCK_ESTIMATES_EBITDA_ESTIMATES_); // Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("StockEstimatesEBITEstimates"), STOCK_ESTIMATES_EBIT_ESTIMATES_); // Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("StockEstimatesEPSSurprise"), STOCK_ESTIMATES_EPS_SURPRISE_);
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("StockEstimatesEarningCalendar"), STOCK_ESTIMATES_EARNING_CALENDAR_);

		// Stock Price
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("StockPriceQuote"), STOCK_PRICE_QUOTE_); // 实时数据优先级最低
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("StockPriceCandles"), STOCK_PRICE_CANDLES_); // 历史数据优先级低
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("StockPriceTickData"), STOCK_PRICE_TICK_DATA_); //Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("StockPriceHistoricalNBBO"), STOCK_PRICE_HISTORICAL_NBBO_); // Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("StockPriceLastBidAsk"), STOCK_PRICE_LAST_BID_ASK_); //Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("StockPriceSplits"), STOCK_PRICE_SPLITS_); // Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("StockPriceDividend"), STOCK_PRICE_DIVIDEND2_); // Premium

		// ETF & Indices
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("ETFSIndicesConstituents"), ETFS_INDICES_CONSTITUENTS_);
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("ETFSIndicesHistoryConstituents"), ETFS_HISTORY_CONSTITUENTS_); // Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("ETFSIndicesProfile"), ETFS_PROFILE_); // Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("ETFSIndicesHoldings"), ETFS_HOLDINGS_); // Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("ETFSIndicesSector"), ETFS_SECTOR_); // Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("ETFSIndicesCountry"), ETFS_COUNTRY_); //Premium

		// Mutual funds
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("MutualFundProfile"), MUTUAL_FUND_PROFILE_); // Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("MutualFundHoldings"), MUTUAL_FUND_HOLDINGS_); // Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("MutualFundSector"), MUTUAL_FUND_SECTOR_); // Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("MutualFundCountry"), MUTUAL_FUND_COUNTRY_); // Premium

		// Bonds
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("BondProfile"), BOND_PROFILE_); // Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("BondPrice"), BOND_PRICE_); // Premium

		// Forex
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("ForexExchange"), FOREX_EXCHANGE_);
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("ForexSymbols"), FOREX_SYMBOLS_);
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("ForexCandles"), FOREX_CANDLES_); // 历史数据优先级低 Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("ForexAllRates"), FOREX_ALL_RATES_); // Premium

		// Crypto
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("CryptoExchange"), CRYPTO_EXCHANGE_);
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("CryptoSymbols"), CRYPTO_SYMBOLS_);
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("CryptoProfile"), CRYPTO_PROFILE_); // Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("CryptoCandles"), CRYPTO_CANDLES_);

		// Technical Analysis
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("TechnicalAnalysisPatternRecognition"), TECHNICAL_ANALYSIS_PATTERN_RECOGNITION_); // Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("TechnicalAnalysisSupportResistance"), TECHNICAL_ANALYSIS_SUPPORT_RESISTANCE_); // Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("TechnicalAnalysisAggregateIndicators"), TECHNICAL_ANALYSIS_AGGREGATE_INDICATORS_); // Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("TechnicalAnalysisTechnicalIndicators"), TECHNICAL_ANALYSIS_TECHNICAL_INDICATORS_);

		// Alternative Data
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("AlternativeDataTranscriptList"), ALTERNATIVE_DATA_TRANSCRIPT_LIST_); //Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("AlternativeDataTranscript"), ALTERNATIVE_DATA_TRANSCRIPT_); //Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("AlternativeDataSentiment"), ALTERNATIVE_DATA_SOCIAL_SENTIMENT_);
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("AlternativeDataInvestThemes"), ALTERNATIVE_DATA_INVEST_THEMES_); // Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("AlternativeDataSupplyChain"), ALTERNATIVE_DATA_SUPPLY_CHAIN_); // Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("AlternativeDataCompanyESG"), ALTERNATIVE_DATA_COMPANY_ESG_); // Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("AlternativeDataQualityScore"), ALTERNATIVE_DATA_EARNING_QUALITY_SCORE_); // Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("AlternativeDataUSPTOPatents"), ALTERNATIVE_DATA_USPTO_PATENTS_);
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("AlternativeDataVISAApplication"), ALTERNATIVE_DATA_VISA_APPLICATION_); //
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("AlternativeDataCOVID_19"), ALTERNATIVE_DATA_COVID_19_);
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("AlternativeDataFDACalendar"), ALTERNATIVE_DATA_FDA_CALENDAR_);

		// Economic
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("EconomicCountryList"), ECONOMIC_COUNTRY_LIST_);
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("EconomicCalendar"), ECONOMIC_CALENDAR_); //Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("EconomicCodes"), ECONOMIC_CODES_); //Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType("EconomicEconomic"), ECONOMIC_ECONOMIC_); //Premium
	}
}
