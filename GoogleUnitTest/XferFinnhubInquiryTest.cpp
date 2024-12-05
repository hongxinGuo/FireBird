#include"pch.h"

//#include"gtest/gtest.h"

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
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(WEBSOCKET_TRADES_), _T("WebSocketTrades"));
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(WEBSOCKET_NEWS_), _T("WebSocketNews")); // Premium

		// Stock Fundamentals
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(SYMBOL_LOOKUP_), _T("StockFundamentalsSymbolLookup"));
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(STOCK_SYMBOLS_), _T("StockFundamentalsSymbols"));
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(MARKET_STATUS_), _T("StockFundamentalsMarketStatus"));
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(MARKET_HOLIDAY_), _T("StockFundamentalsMarketHoliday"));
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(COMPANY_PROFILE_), _T("StockFundamentalsCompanyProfile")); //Premium
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(COMPANY_PROFILE_CONCISE_), _T("StockFundamentalsCompanyProfileConcise"));
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(COMPANY_EXECUTIVE_), _T("StockFundamentalsCompanyExecutive")); //Premium
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(MARKET_NEWS_), _T("StockFundamentalsMarketNews"));
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(COMPANY_NEWS_), _T("StockFundamentalsCompanyNews"));
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(PRESS_RELEASE_), _T("StockFundamentalsPressRelease")); //Premium
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(NEWS_SENTIMENT_), _T("StockFundamentalsNewsSentiment"));
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(PEERS_), _T("StockFundamentalsPeer"));
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(BASIC_FINANCIALS_), _T("StockFundamentalsBasicFinancials"));
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(OWNERSHIP_), _T("StockFundamentalsOwnership")); //Premium
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(FUND_OWNERSHIP_), _T("StockFundamentalsFundOwnership")); //Premium
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(INSIDER_TRANSACTION_), _T("StockFundamentalsInsiderTransaction"));
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(INSIDER_SENTIMENT_), _T("StockFundamentalsInsiderSentiment"));
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(FINANCIAL_), _T("StockFundamentalsFinancial")); //Premium
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(FINANCIAL_AS_REPORTED_), _T("StockFundamentalsFinancialAsReported"));
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(REVENUE_BREAKDOWN_), _T("StockFundamentalsRevenueBreakdown"));
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(SEC_FILINGS_), _T("StockFundamentalsSECFilings"));
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(INTERNATIONAL_FILINGS_), _T("StockFundamentalsInternationalFilings")); //Premium
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(SEC_SENTIMENT_ANALYSIS_), _T("StockFundamentalsSECSentimentAnalysis")); //Premium
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(SIMILARITY_INDEX_), _T("StockFundamentalsSimilarityIndex")); //Premium
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(IPO_CALENDAR_), _T("StockFundamentalsIPOCalendar"));
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(DIVIDENDS_), _T("StockFundamentalsDividends")); //Premium

		// Stock Estimates
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(STOCK_ESTIMATES_RECOMMENDATION_TRENDS_), _T("StockEstimatesRecommendationTrends"));
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(STOCK_ESTIMATES_PRICE_TARGET_), _T("StockEstimatesPriceTarget"));
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(STOCK_ESTIMATES_UPGRADE_DOWNGRADE_), _T("StockEstimatesUpgradeDownGrade")); // Premium
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(STOCK_ESTIMATES_REVENUE_ESTIMATES_), _T("StockEstimatesRevenueEstimates")); // Premium
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(STOCK_ESTIMATES_EPS_ESTIMATES_), _T("StockEstimatesEPSEstimates")); // Premium
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(STOCK_ESTIMATES_EBITDA_ESTIMATES_), _T("StockEstimatesEBITDAEstimates")); // Premium
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(STOCK_ESTIMATES_EBIT_ESTIMATES_), _T("StockEstimatesEBITEstimates")); // Premium
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(STOCK_ESTIMATES_EPS_SURPRISE_), _T("StockEstimatesEPSSurprise"));
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(STOCK_ESTIMATES_EARNING_CALENDAR_), _T("StockEstimatesEarningCalendar"));

		// Stock Price
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(STOCK_PRICE_QUOTE_), _T("StockPriceQuote")); // 实时数据优先级最低
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(STOCK_PRICE_CANDLES_), _T("StockPriceCandles")); // 历史数据优先级低
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(STOCK_PRICE_TICK_DATA_), _T("StockPriceTickData")); //Premium
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(STOCK_PRICE_HISTORICAL_NBBO_), _T("StockPriceHistoricalNBBO")); // Premium
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(STOCK_PRICE_LAST_BID_ASK_), _T("StockPriceLastBidAsk")); //Premium
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(STOCK_PRICE_SPLITS_), _T("StockPriceSplits")); // Premium
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(STOCK_PRICE_DIVIDEND2_), _T("StockPriceDividend")); // Premium

		// ETF & Indices
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(ETFS_INDICES_CONSTITUENTS_), _T("ETFSIndicesConstituents"));
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(ETFS_HISTORY_CONSTITUENTS_), _T("ETFSIndicesHistoryConstituents")); // Premium
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(ETFS_PROFILE_), _T("ETFSIndicesProfile")); // Premium
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(ETFS_HOLDINGS_), _T("ETFSIndicesHoldings")); // Premium
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(ETFS_SECTOR_), _T("ETFSIndicesSector")); // Premium
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(ETFS_COUNTRY_), _T("ETFSIndicesCountry")); //Premium

		// Mutual funds
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(MUTUAL_FUND_PROFILE_), _T("MutualFundProfile")); // Premium
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(MUTUAL_FUND_HOLDINGS_), _T("MutualFundHoldings")); // Premium
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(MUTUAL_FUND_SECTOR_), _T("MutualFundSector")); // Premium
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(MUTUAL_FUND_COUNTRY_), _T("MutualFundCountry")); // Premium

		// Bonds
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(BOND_PROFILE_), _T("BondProfile")); // Premium
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(BOND_PRICE_), _T("BondPrice")); // Premium

		// Forex
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(FOREX_EXCHANGE_), _T("ForexExchange"));
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(FOREX_SYMBOLS_), _T("ForexSymbols"));
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(FOREX_CANDLES_), _T("ForexCandles")); // 历史数据优先级低 Premium
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(FOREX_ALL_RATES_), _T("ForexAllRates")); // Premium

		// Crypto
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(CRYPTO_EXCHANGE_), _T("CryptoExchange"));
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(CRYPTO_SYMBOLS_), _T("CryptoSymbols"));
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(CRYPTO_PROFILE_), _T("CryptoProfile")); // Premium
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(CRYPTO_CANDLES_), _T("CryptoCandles"));

		// Technical Analysis
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(TECHNICAL_ANALYSIS_PATTERN_RECOGNITION_), _T("TechnicalAnalysisPatternRecognition")); // Premium
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(TECHNICAL_ANALYSIS_SUPPORT_RESISTANCE_), _T("TechnicalAnalysisSupportResistance")); // Premium
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(TECHNICAL_ANALYSIS_AGGREGATE_INDICATORS_), _T("TechnicalAnalysisAggregateIndicators")); // Premium
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(TECHNICAL_ANALYSIS_TECHNICAL_INDICATORS_), _T("TechnicalAnalysisTechnicalIndicators"));

		// Alternative Data
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(ALTERNATIVE_DATA_TRANSCRIPT_LIST_), _T("AlternativeDataTranscriptList")); //Premium
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(ALTERNATIVE_DATA_TRANSCRIPT_), _T("AlternativeDataTranscript")); //Premium
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(ALTERNATIVE_DATA_SOCIAL_SENTIMENT_), _T("AlternativeDataSentiment"));
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(ALTERNATIVE_DATA_INVEST_THEMES_), _T("AlternativeDataInvestThemes")); // Premium
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(ALTERNATIVE_DATA_SUPPLY_CHAIN_), _T("AlternativeDataSupplyChain")); // Premium
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(ALTERNATIVE_DATA_COMPANY_ESG_), _T("AlternativeDataCompanyESG")); // Premium
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(ALTERNATIVE_DATA_EARNING_QUALITY_SCORE_), _T("AlternativeDataQualityScore")); // Premium
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(ALTERNATIVE_DATA_USPTO_PATENTS_), _T("AlternativeDataUSPTOPatents"));
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(ALTERNATIVE_DATA_VISA_APPLICATION_), _T("AlternativeDataVISAApplication")); //
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(ALTERNATIVE_DATA_COVID_19_), _T("AlternativeDataCOVID_19"));
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(ALTERNATIVE_DATA_FDA_CALENDAR_), _T("AlternativeDataFDACalendar"));

		// Economic
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(ECONOMIC_COUNTRY_LIST_), _T("EconomicCountryList"));
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(ECONOMIC_CALENDAR_), _T("EconomicCalendar")); //Premium
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(ECONOMIC_CODES_), _T("EconomicCodes")); //Premium
		EXPECT_STREQ(gl_FinnhubInquiryType.GetInquiryString(ECONOMIC_ECONOMIC_), _T("EconomicEconomic")); //Premium
	}

	TEST_F(CXferFinnhubInquiryTest, TestGetFinnhubInquiryIndex) {
		// Web Socket
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("WebSocketTrades")), WEBSOCKET_TRADES_);
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("WebSocketNews")), WEBSOCKET_NEWS_); // Premium

		// Stock Fundamentals
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("StockFundamentalsSymbolLookup")), SYMBOL_LOOKUP_);
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("StockFundamentalsSymbols")), STOCK_SYMBOLS_);
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("StockFundamentalsMarketStatus")), MARKET_STATUS_);
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("StockFundamentalsMarketHoliday")), MARKET_HOLIDAY_);
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("StockFundamentalsCompanyProfile")), COMPANY_PROFILE_); //Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("StockFundamentalsCompanyProfileConcise")), COMPANY_PROFILE_CONCISE_);
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("StockFundamentalsCompanyExecutive")), COMPANY_EXECUTIVE_); //Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("StockFundamentalsMarketNews")), MARKET_NEWS_);
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("StockFundamentalsCompanyNews")), COMPANY_NEWS_);
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("StockFundamentalsPressRelease")), PRESS_RELEASE_); //Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("StockFundamentalsNewsSentiment")), NEWS_SENTIMENT_);
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("StockFundamentalsPeer")), PEERS_);
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("StockFundamentalsBasicFinancials")), BASIC_FINANCIALS_);
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("StockFundamentalsOwnership")), OWNERSHIP_); //Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("StockFundamentalsFundOwnership")), FUND_OWNERSHIP_); //Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("StockFundamentalsInsiderTransaction")), INSIDER_TRANSACTION_);
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("StockFundamentalsInsiderSentiment")), INSIDER_SENTIMENT_);
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("StockFundamentalsFinancial")), FINANCIAL_); //Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("StockFundamentalsFinancialAsReported")), FINANCIAL_AS_REPORTED_);
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("StockFundamentalsRevenueBreakdown")), REVENUE_BREAKDOWN_);
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("StockFundamentalsSECFilings")), SEC_FILINGS_);
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("StockFundamentalsInternationalFilings")), INTERNATIONAL_FILINGS_); //Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("StockFundamentalsSECSentimentAnalysis")), SEC_SENTIMENT_ANALYSIS_); //Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("StockFundamentalsSimilarityIndex")), SIMILARITY_INDEX_); //Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("StockFundamentalsIPOCalendar")), IPO_CALENDAR_);
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("StockFundamentalsDividends")), DIVIDENDS_); //Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("StockFundamentalsSectorMetrics")), SECTOR_METRICS_); //Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("StockFundamentalsPriceMetrics")), PRICE_METRICS_); //Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("StockFundamentalsSymbolChange")), SYMBOL_CHANGE_); //Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("StockFundamentalsISINChange")), ISIN_CHANGE_); //Premium

		// Stock Estimates
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("StockEstimatesRecommendationTrends")), STOCK_ESTIMATES_RECOMMENDATION_TRENDS_);
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("StockEstimatesPriceTarget")), STOCK_ESTIMATES_PRICE_TARGET_);
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("StockEstimatesUpgradeDownGrade")), STOCK_ESTIMATES_UPGRADE_DOWNGRADE_); // Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("StockEstimatesRevenueEstimates")), STOCK_ESTIMATES_REVENUE_ESTIMATES_); // Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("StockEstimatesEPSEstimates")), STOCK_ESTIMATES_EPS_ESTIMATES_); // Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("StockEstimatesEBITDAEstimates")), STOCK_ESTIMATES_EBITDA_ESTIMATES_); // Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("StockEstimatesEBITEstimates")), STOCK_ESTIMATES_EBIT_ESTIMATES_); // Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("StockEstimatesEPSSurprise")), STOCK_ESTIMATES_EPS_SURPRISE_);
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("StockEstimatesEarningCalendar")), STOCK_ESTIMATES_EARNING_CALENDAR_);

		// Stock Price
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("StockPriceQuote")), STOCK_PRICE_QUOTE_); // 实时数据优先级最低
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("StockPriceCandles")), STOCK_PRICE_CANDLES_); // 历史数据优先级低
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("StockPriceTickData")), STOCK_PRICE_TICK_DATA_); //Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("StockPriceHistoricalNBBO")), STOCK_PRICE_HISTORICAL_NBBO_); // Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("StockPriceLastBidAsk")), STOCK_PRICE_LAST_BID_ASK_); //Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("StockPriceSplits")), STOCK_PRICE_SPLITS_); // Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("StockPriceDividend")), STOCK_PRICE_DIVIDEND2_); // Premium

		// ETF & Indices
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("ETFSIndicesConstituents")), ETFS_INDICES_CONSTITUENTS_);
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("ETFSIndicesHistoryConstituents")), ETFS_HISTORY_CONSTITUENTS_); // Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("ETFSIndicesProfile")), ETFS_PROFILE_); // Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("ETFSIndicesHoldings")), ETFS_HOLDINGS_); // Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("ETFSIndicesSector")), ETFS_SECTOR_); // Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("ETFSIndicesCountry")), ETFS_COUNTRY_); //Premium

		// Mutual funds
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("MutualFundProfile")), MUTUAL_FUND_PROFILE_); // Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("MutualFundHoldings")), MUTUAL_FUND_HOLDINGS_); // Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("MutualFundSector")), MUTUAL_FUND_SECTOR_); // Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("MutualFundCountry")), MUTUAL_FUND_COUNTRY_); // Premium

		// Bonds
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("BondProfile")), BOND_PROFILE_); // Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("BondPrice")), BOND_PRICE_); // Premium

		// Forex
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("ForexExchange")), FOREX_EXCHANGE_);
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("ForexSymbols")), FOREX_SYMBOLS_);
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("ForexCandles")), FOREX_CANDLES_); // 历史数据优先级低 Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("ForexAllRates")), FOREX_ALL_RATES_); // Premium

		// Crypto
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("CryptoExchange")), CRYPTO_EXCHANGE_);
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("CryptoSymbols")), CRYPTO_SYMBOLS_);
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("CryptoProfile")), CRYPTO_PROFILE_); // Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("CryptoCandles")), CRYPTO_CANDLES_);

		// Technical Analysis
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("TechnicalAnalysisPatternRecognition")), TECHNICAL_ANALYSIS_PATTERN_RECOGNITION_); // Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("TechnicalAnalysisSupportResistance")), TECHNICAL_ANALYSIS_SUPPORT_RESISTANCE_); // Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("TechnicalAnalysisAggregateIndicators")), TECHNICAL_ANALYSIS_AGGREGATE_INDICATORS_); // Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("TechnicalAnalysisTechnicalIndicators")), TECHNICAL_ANALYSIS_TECHNICAL_INDICATORS_);

		// Alternative Data
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("AlternativeDataTranscriptList")), ALTERNATIVE_DATA_TRANSCRIPT_LIST_); //Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("AlternativeDataTranscript")), ALTERNATIVE_DATA_TRANSCRIPT_); //Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("AlternativeDataSentiment")), ALTERNATIVE_DATA_SOCIAL_SENTIMENT_);
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("AlternativeDataInvestThemes")), ALTERNATIVE_DATA_INVEST_THEMES_); // Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("AlternativeDataSupplyChain")), ALTERNATIVE_DATA_SUPPLY_CHAIN_); // Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("AlternativeDataCompanyESG")), ALTERNATIVE_DATA_COMPANY_ESG_); // Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("AlternativeDataQualityScore")), ALTERNATIVE_DATA_EARNING_QUALITY_SCORE_); // Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("AlternativeDataUSPTOPatents")), ALTERNATIVE_DATA_USPTO_PATENTS_);
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("AlternativeDataVISAApplication")), ALTERNATIVE_DATA_VISA_APPLICATION_); //
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("AlternativeDataCOVID_19")), ALTERNATIVE_DATA_COVID_19_);
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("AlternativeDataFDACalendar")), ALTERNATIVE_DATA_FDA_CALENDAR_);

		// Economic
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("EconomicCountryList")), ECONOMIC_COUNTRY_LIST_);
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("EconomicCalendar")), ECONOMIC_CALENDAR_); //Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("EconomicCodes")), ECONOMIC_CODES_); //Premium
		EXPECT_EQ(gl_FinnhubInquiryType.GetInquiryType(_T("EconomicEconomic")), ECONOMIC_ECONOMIC_); //Premium
	}
}
