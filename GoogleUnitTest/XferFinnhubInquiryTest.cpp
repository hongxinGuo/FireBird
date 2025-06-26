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
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(WEBSOCKET_TRADES_).compare(_T("WebSocketTrades")) == 0);
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(WEBSOCKET_NEWS_).compare(_T("WebSocketNews"))== 0); // Premium

		// Stock Fundamentals
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(SYMBOL_LOOKUP_).compare(_T("StockFundamentalsSymbolLookup"))== 0);
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(STOCK_SYMBOLS_).compare(_T("StockFundamentalsSymbols"))== 0);
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(MARKET_STATUS_).compare(_T("StockFundamentalsMarketStatus"))== 0);
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(MARKET_HOLIDAY_).compare(_T("StockFundamentalsMarketHoliday"))== 0);
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(COMPANY_PROFILE_).compare(_T("StockFundamentalsCompanyProfile"))== 0); //Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(COMPANY_PROFILE_CONCISE_).compare(_T("StockFundamentalsCompanyProfileConcise"))== 0);
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(COMPANY_EXECUTIVE_).compare(_T("StockFundamentalsCompanyExecutive"))== 0); //Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(MARKET_NEWS_).compare(_T("StockFundamentalsMarketNews"))== 0);
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(COMPANY_NEWS_).compare(_T("StockFundamentalsCompanyNews"))== 0);
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(PRESS_RELEASE_).compare(_T("StockFundamentalsPressRelease"))== 0); //Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(NEWS_SENTIMENT_).compare(_T("StockFundamentalsNewsSentiment"))== 0);
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(PEERS_).compare(_T("StockFundamentalsPeer"))== 0);
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(BASIC_FINANCIALS_).compare(_T("StockFundamentalsBasicFinancials"))== 0);
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(OWNERSHIP_).compare(_T("StockFundamentalsOwnership"))== 0); //Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(FUND_OWNERSHIP_).compare(_T("StockFundamentalsFundOwnership"))== 0); //Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(INSIDER_TRANSACTION_).compare(_T("StockFundamentalsInsiderTransaction"))== 0);
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(INSIDER_SENTIMENT_).compare(_T("StockFundamentalsInsiderSentiment"))== 0);
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(FINANCIAL_).compare(_T("StockFundamentalsFinancial"))== 0); //Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(FINANCIAL_AS_REPORTED_).compare(_T("StockFundamentalsFinancialAsReported"))== 0);
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(REVENUE_BREAKDOWN_).compare(_T("StockFundamentalsRevenueBreakdown"))== 0);
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(SEC_FILINGS_).compare(_T("StockFundamentalsSECFilings"))== 0);
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(INTERNATIONAL_FILINGS_).compare(_T("StockFundamentalsInternationalFilings"))== 0); //Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(SEC_SENTIMENT_ANALYSIS_).compare(_T("StockFundamentalsSECSentimentAnalysis"))== 0); //Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(SIMILARITY_INDEX_).compare(_T("StockFundamentalsSimilarityIndex"))== 0); //Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(IPO_CALENDAR_).compare(_T("StockFundamentalsIPOCalendar"))== 0);
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(DIVIDENDS_).compare(_T("StockFundamentalsDividends"))== 0); //Premium

		// Stock Estimates
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(STOCK_ESTIMATES_RECOMMENDATION_TRENDS_).compare(_T("StockEstimatesRecommendationTrends"))== 0);
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(STOCK_ESTIMATES_PRICE_TARGET_).compare(_T("StockEstimatesPriceTarget"))== 0);
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(STOCK_ESTIMATES_UPGRADE_DOWNGRADE_).compare(_T("StockEstimatesUpgradeDownGrade"))== 0); // Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(STOCK_ESTIMATES_REVENUE_ESTIMATES_).compare(_T("StockEstimatesRevenueEstimates"))== 0); // Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(STOCK_ESTIMATES_EPS_ESTIMATES_).compare(_T("StockEstimatesEPSEstimates"))== 0); // Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(STOCK_ESTIMATES_EBITDA_ESTIMATES_).compare(_T("StockEstimatesEBITDAEstimates"))== 0); // Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(STOCK_ESTIMATES_EBIT_ESTIMATES_).compare(_T("StockEstimatesEBITEstimates"))== 0); // Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(STOCK_ESTIMATES_EPS_SURPRISE_).compare(_T("StockEstimatesEPSSurprise"))== 0);
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(STOCK_ESTIMATES_EARNING_CALENDAR_).compare(_T("StockEstimatesEarningCalendar"))== 0);

		// Stock Price
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(STOCK_PRICE_QUOTE_).compare(_T("StockPriceQuote"))== 0); // 实时数据优先级最低
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(STOCK_PRICE_CANDLES_).compare(_T("StockPriceCandles"))== 0); // 历史数据优先级低
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(STOCK_PRICE_TICK_DATA_).compare(_T("StockPriceTickData"))== 0); //Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(STOCK_PRICE_HISTORICAL_NBBO_).compare(_T("StockPriceHistoricalNBBO"))== 0); // Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(STOCK_PRICE_LAST_BID_ASK_).compare(_T("StockPriceLastBidAsk"))== 0); //Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(STOCK_PRICE_SPLITS_).compare(_T("StockPriceSplits"))== 0); // Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(STOCK_PRICE_DIVIDEND2_).compare(_T("StockPriceDividend"))== 0); // Premium

		// ETF & Indices
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(ETFS_INDICES_CONSTITUENTS_).compare(_T("ETFSIndicesConstituents"))== 0);
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(ETFS_HISTORY_CONSTITUENTS_).compare(_T("ETFSIndicesHistoryConstituents"))== 0); // Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(ETFS_PROFILE_).compare(_T("ETFSIndicesProfile"))== 0); // Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(ETFS_HOLDINGS_).compare(_T("ETFSIndicesHoldings"))== 0); // Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(ETFS_SECTOR_).compare(_T("ETFSIndicesSector"))== 0); // Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(ETFS_COUNTRY_).compare(_T("ETFSIndicesCountry"))== 0); //Premium

		// Mutual funds
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(MUTUAL_FUND_PROFILE_).compare(_T("MutualFundProfile"))== 0); // Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(MUTUAL_FUND_HOLDINGS_).compare(_T("MutualFundHoldings"))== 0); // Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(MUTUAL_FUND_SECTOR_).compare(_T("MutualFundSector"))== 0); // Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(MUTUAL_FUND_COUNTRY_).compare(_T("MutualFundCountry"))== 0); // Premium

		// Bonds
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(BOND_PROFILE_).compare(_T("BondProfile"))== 0); // Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(BOND_PRICE_).compare(_T("BondPrice"))== 0); // Premium

		// Forex
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(FOREX_EXCHANGE_).compare(_T("ForexExchange"))== 0);
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(FOREX_SYMBOLS_).compare(_T("ForexSymbols"))== 0);
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(FOREX_CANDLES_).compare(_T("ForexCandles"))== 0); // 历史数据优先级低 Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(FOREX_ALL_RATES_).compare(_T("ForexAllRates"))== 0); // Premium

		// Crypto
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(CRYPTO_EXCHANGE_).compare(_T("CryptoExchange"))== 0);
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(CRYPTO_SYMBOLS_).compare(_T("CryptoSymbols"))== 0);
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(CRYPTO_PROFILE_).compare(_T("CryptoProfile"))== 0); // Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(CRYPTO_CANDLES_).compare(_T("CryptoCandles"))== 0);

		// Technical Analysis
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(TECHNICAL_ANALYSIS_PATTERN_RECOGNITION_).compare(_T("TechnicalAnalysisPatternRecognition"))== 0); // Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(TECHNICAL_ANALYSIS_SUPPORT_RESISTANCE_).compare(_T("TechnicalAnalysisSupportResistance"))== 0); // Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(TECHNICAL_ANALYSIS_AGGREGATE_INDICATORS_).compare(_T("TechnicalAnalysisAggregateIndicators"))== 0); // Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(TECHNICAL_ANALYSIS_TECHNICAL_INDICATORS_).compare(_T("TechnicalAnalysisTechnicalIndicators"))== 0);

		// Alternative Data
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(ALTERNATIVE_DATA_TRANSCRIPT_LIST_).compare(_T("AlternativeDataTranscriptList"))== 0); //Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(ALTERNATIVE_DATA_TRANSCRIPT_).compare(_T("AlternativeDataTranscript"))== 0); //Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(ALTERNATIVE_DATA_SOCIAL_SENTIMENT_).compare(_T("AlternativeDataSentiment"))== 0);
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(ALTERNATIVE_DATA_INVEST_THEMES_).compare(_T("AlternativeDataInvestThemes"))== 0); // Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(ALTERNATIVE_DATA_SUPPLY_CHAIN_).compare(_T("AlternativeDataSupplyChain"))== 0); // Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(ALTERNATIVE_DATA_COMPANY_ESG_).compare(_T("AlternativeDataCompanyESG"))== 0); // Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(ALTERNATIVE_DATA_EARNING_QUALITY_SCORE_).compare(_T("AlternativeDataQualityScore"))== 0); // Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(ALTERNATIVE_DATA_USPTO_PATENTS_).compare(_T("AlternativeDataUSPTOPatents"))== 0);
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(ALTERNATIVE_DATA_VISA_APPLICATION_).compare(_T("AlternativeDataVISAApplication"))== 0); //
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(ALTERNATIVE_DATA_COVID_19_).compare(_T("AlternativeDataCOVID_19"))== 0);
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(ALTERNATIVE_DATA_FDA_CALENDAR_).compare(_T("AlternativeDataFDACalendar"))== 0);

		// Economic
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(ECONOMIC_COUNTRY_LIST_).compare(_T("EconomicCountryList"))== 0);
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(ECONOMIC_CALENDAR_).compare(_T("EconomicCalendar"))== 0); //Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(ECONOMIC_CODES_).compare(_T("EconomicCodes"))== 0); //Premium
		EXPECT_TRUE(gl_FinnhubInquiryType.GetInquiryString(ECONOMIC_ECONOMIC_).compare(_T("EconomicEconomic"))== 0); //Premium
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
