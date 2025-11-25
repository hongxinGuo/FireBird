#include"pch.h"

#include"FinnhubInquiryType.h"

CFinnhubInquiryType::CFinnhubInquiryType() {
	ASSERT(gl_systemConfiguration.IsInitialized());
	if (static int siInstance = 0; ++siInstance > 1) {
		TRACE("XferFinnhubInquiryType全局变量只允许存在一个实例\n");
#ifdef _DEBUG
		ASSERT(FALSE);
#endif // _DEBUG
	}

	CreateFinnhubInquiryIndexToStringMap();
	CreateFinnhubInquiryStringToIndexMap();
}

string CFinnhubInquiryType::GetInquiryString(const int lInquiryType) {
	return m_mapFinnhubInquiryIndexToString.at(lInquiryType);
}

int CFinnhubInquiryType::GetInquiryType(const string& strInquiryString) const {
	return m_mapFinnhubInquiryStringToIndex.at(strInquiryString);
}

void CFinnhubInquiryType::CreateFinnhubInquiryIndexToStringMap() {
	// Web Socket
	m_mapFinnhubInquiryIndexToString[WEBSOCKET_TRADES_] = "WebSocketTrades";
	m_mapFinnhubInquiryIndexToString[WEBSOCKET_NEWS_] = "WebSocketNews"; // Premium

	// Stock Fundamentals
	m_mapFinnhubInquiryIndexToString[SYMBOL_LOOKUP_] = "StockFundamentalsSymbolLookup";
	m_mapFinnhubInquiryIndexToString[STOCK_SYMBOLS_] = "StockFundamentalsSymbols";
	m_mapFinnhubInquiryIndexToString[MARKET_STATUS_] = "StockFundamentalsMarketStatus";
	m_mapFinnhubInquiryIndexToString[MARKET_HOLIDAY_] = "StockFundamentalsMarketHoliday";
	m_mapFinnhubInquiryIndexToString[COMPANY_PROFILE_] = "StockFundamentalsCompanyProfile"; //Premium
	m_mapFinnhubInquiryIndexToString[COMPANY_PROFILE_CONCISE_] = "StockFundamentalsCompanyProfileConcise";
	m_mapFinnhubInquiryIndexToString[COMPANY_EXECUTIVE_] = "StockFundamentalsCompanyExecutive"; //Premium
	m_mapFinnhubInquiryIndexToString[MARKET_NEWS_] = "StockFundamentalsMarketNews";
	m_mapFinnhubInquiryIndexToString[COMPANY_NEWS_] = "StockFundamentalsCompanyNews";
	m_mapFinnhubInquiryIndexToString[PRESS_RELEASE_] = "StockFundamentalsPressRelease"; //Premium
	m_mapFinnhubInquiryIndexToString[NEWS_SENTIMENT_] = "StockFundamentalsNewsSentiment";
	m_mapFinnhubInquiryIndexToString[PEERS_] = "StockFundamentalsPeer";
	m_mapFinnhubInquiryIndexToString[BASIC_FINANCIALS_] = "StockFundamentalsBasicFinancials";
	m_mapFinnhubInquiryIndexToString[OWNERSHIP_] = "StockFundamentalsOwnership"; //Premium
	m_mapFinnhubInquiryIndexToString[FUND_OWNERSHIP_] = "StockFundamentalsFundOwnership"; //Premium
	m_mapFinnhubInquiryIndexToString[INSIDER_TRANSACTION_] = "StockFundamentalsInsiderTransaction";
	m_mapFinnhubInquiryIndexToString[INSIDER_SENTIMENT_] = "StockFundamentalsInsiderSentiment";
	m_mapFinnhubInquiryIndexToString[FINANCIAL_] = "StockFundamentalsFinancial"; //Premium
	m_mapFinnhubInquiryIndexToString[FINANCIAL_AS_REPORTED_] = "StockFundamentalsFinancialAsReported";
	m_mapFinnhubInquiryIndexToString[REVENUE_BREAKDOWN_] = "StockFundamentalsRevenueBreakdown";
	m_mapFinnhubInquiryIndexToString[SEC_FILINGS_] = "StockFundamentalsSECFilings";
	m_mapFinnhubInquiryIndexToString[INTERNATIONAL_FILINGS_] = "StockFundamentalsInternationalFilings"; //Premium
	m_mapFinnhubInquiryIndexToString[SEC_SENTIMENT_ANALYSIS_] = "StockFundamentalsSECSentimentAnalysis"; //Premium
	m_mapFinnhubInquiryIndexToString[SIMILARITY_INDEX_] = "StockFundamentalsSimilarityIndex"; //Premium
	m_mapFinnhubInquiryIndexToString[IPO_CALENDAR_] = "StockFundamentalsIPOCalendar";
	m_mapFinnhubInquiryIndexToString[DIVIDENDS_] = "StockFundamentalsDividends"; //Premium
	m_mapFinnhubInquiryIndexToString[SECTOR_METRICS_] = "SectorMetrics"; // Premium
	m_mapFinnhubInquiryIndexToString[PRICE_METRICS_] = "PriceMetrics"; // Premium
	m_mapFinnhubInquiryIndexToString[SYMBOL_CHANGE_] = "SymbolChange"; // Premium
	m_mapFinnhubInquiryIndexToString[ISIN_CHANGE_] = "ISINChange"; // Premium

	// Stock Estimates
	m_mapFinnhubInquiryIndexToString[STOCK_ESTIMATES_RECOMMENDATION_TRENDS_] = "StockEstimatesRecommendationTrends";
	m_mapFinnhubInquiryIndexToString[STOCK_ESTIMATES_PRICE_TARGET_] = "StockEstimatesPriceTarget";
	m_mapFinnhubInquiryIndexToString[STOCK_ESTIMATES_UPGRADE_DOWNGRADE_] = "StockEstimatesUpgradeDownGrade"; // Premium
	m_mapFinnhubInquiryIndexToString[STOCK_ESTIMATES_REVENUE_ESTIMATES_] = "StockEstimatesRevenueEstimates"; // Premium
	m_mapFinnhubInquiryIndexToString[STOCK_ESTIMATES_EPS_ESTIMATES_] = "StockEstimatesEPSEstimates"; // Premium
	m_mapFinnhubInquiryIndexToString[STOCK_ESTIMATES_EBITDA_ESTIMATES_] = "StockEstimatesEBITDAEstimates"; // Premium
	m_mapFinnhubInquiryIndexToString[STOCK_ESTIMATES_EBIT_ESTIMATES_] = "StockEstimatesEBITEstimates"; // Premium
	m_mapFinnhubInquiryIndexToString[STOCK_ESTIMATES_EPS_SURPRISE_] = "StockEstimatesEPSSurprise";
	m_mapFinnhubInquiryIndexToString[STOCK_ESTIMATES_EARNING_CALENDAR_] = "StockEstimatesEarningCalendar";

	// Stock Price
	m_mapFinnhubInquiryIndexToString[STOCK_PRICE_QUOTE_] = "StockPriceQuote"; // 实时数据优先级最低
	m_mapFinnhubInquiryIndexToString[STOCK_PRICE_CANDLES_] = "StockPriceCandles"; // 历史数据优先级低
	m_mapFinnhubInquiryIndexToString[STOCK_PRICE_TICK_DATA_] = "StockPriceTickData"; //Premium
	m_mapFinnhubInquiryIndexToString[STOCK_PRICE_HISTORICAL_NBBO_] = "StockPriceHistoricalNBBO"; // Premium
	m_mapFinnhubInquiryIndexToString[STOCK_PRICE_LAST_BID_ASK_] = "StockPriceLastBidAsk"; //Premium
	m_mapFinnhubInquiryIndexToString[STOCK_PRICE_SPLITS_] = "StockPriceSplits"; // Premium
	m_mapFinnhubInquiryIndexToString[STOCK_PRICE_DIVIDEND2_] = "StockPriceDividend"; // Premium

	// ETF & Indices
	m_mapFinnhubInquiryIndexToString[ETFS_INDICES_CONSTITUENTS_] = "ETFSIndicesConstituents";
	m_mapFinnhubInquiryIndexToString[ETFS_HISTORY_CONSTITUENTS_] = "ETFSIndicesHistoryConstituents"; // Premium
	m_mapFinnhubInquiryIndexToString[ETFS_PROFILE_] = "ETFSIndicesProfile"; // Premium
	m_mapFinnhubInquiryIndexToString[ETFS_HOLDINGS_] = "ETFSIndicesHoldings"; // Premium
	m_mapFinnhubInquiryIndexToString[ETFS_SECTOR_] = "ETFSIndicesSector"; // Premium
	m_mapFinnhubInquiryIndexToString[ETFS_COUNTRY_] = "ETFSIndicesCountry"; //Premium

	// Mutual funds
	m_mapFinnhubInquiryIndexToString[MUTUAL_FUND_PROFILE_] = "MutualFundProfile"; // Premium
	m_mapFinnhubInquiryIndexToString[MUTUAL_FUND_HOLDINGS_] = "MutualFundHoldings"; // Premium
	m_mapFinnhubInquiryIndexToString[MUTUAL_FUND_SECTOR_] = "MutualFundSector"; // Premium
	m_mapFinnhubInquiryIndexToString[MUTUAL_FUND_COUNTRY_] = "MutualFundCountry"; // Premium

	// Bonds
	m_mapFinnhubInquiryIndexToString[BOND_PROFILE_] = "BondProfile"; // Premium
	m_mapFinnhubInquiryIndexToString[BOND_PRICE_] = "BondPrice"; // Premium

	// Forex
	m_mapFinnhubInquiryIndexToString[FOREX_EXCHANGE_] = "ForexExchange";
	m_mapFinnhubInquiryIndexToString[FOREX_SYMBOLS_] = "ForexSymbols";
	m_mapFinnhubInquiryIndexToString[FOREX_CANDLES_] = "ForexCandles"; // 历史数据优先级低 Premium
	m_mapFinnhubInquiryIndexToString[FOREX_ALL_RATES_] = "ForexAllRates"; // Premium

	// Crypto
	m_mapFinnhubInquiryIndexToString[CRYPTO_EXCHANGE_] = "CryptoExchange";
	m_mapFinnhubInquiryIndexToString[CRYPTO_SYMBOLS_] = "CryptoSymbols";
	m_mapFinnhubInquiryIndexToString[CRYPTO_PROFILE_] = "CryptoProfile"; // Premium
	m_mapFinnhubInquiryIndexToString[CRYPTO_CANDLES_] = "CryptoCandles";

	// Technical Analysis
	m_mapFinnhubInquiryIndexToString[TECHNICAL_ANALYSIS_PATTERN_RECOGNITION_] = "TechnicalAnalysisPatternRecognition"; // Premium
	m_mapFinnhubInquiryIndexToString[TECHNICAL_ANALYSIS_SUPPORT_RESISTANCE_] = "TechnicalAnalysisSupportResistance"; // Premium
	m_mapFinnhubInquiryIndexToString[TECHNICAL_ANALYSIS_AGGREGATE_INDICATORS_] = "TechnicalAnalysisAggregateIndicators"; // Premium
	m_mapFinnhubInquiryIndexToString[TECHNICAL_ANALYSIS_TECHNICAL_INDICATORS_] = "TechnicalAnalysisTechnicalIndicators";

	// Alternative Data
	m_mapFinnhubInquiryIndexToString[ALTERNATIVE_DATA_TRANSCRIPT_LIST_] = "AlternativeDataTranscriptList"; //Premium
	m_mapFinnhubInquiryIndexToString[ALTERNATIVE_DATA_TRANSCRIPT_] = "AlternativeDataTranscript"; //Premium
	m_mapFinnhubInquiryIndexToString[ALTERNATIVE_DATA_SOCIAL_SENTIMENT_] = "AlternativeDataSentiment";
	m_mapFinnhubInquiryIndexToString[ALTERNATIVE_DATA_INVEST_THEMES_] = "AlternativeDataInvestThemes"; // Premium
	m_mapFinnhubInquiryIndexToString[ALTERNATIVE_DATA_SUPPLY_CHAIN_] = "AlternativeDataSupplyChain"; // Premium
	m_mapFinnhubInquiryIndexToString[ALTERNATIVE_DATA_COMPANY_ESG_] = "AlternativeDataCompanyESG"; // Premium
	m_mapFinnhubInquiryIndexToString[ALTERNATIVE_DATA_EARNING_QUALITY_SCORE_] = "AlternativeDataQualityScore"; // Premium
	m_mapFinnhubInquiryIndexToString[ALTERNATIVE_DATA_USPTO_PATENTS_] = "AlternativeDataUSPTOPatents";
	m_mapFinnhubInquiryIndexToString[ALTERNATIVE_DATA_VISA_APPLICATION_] = "AlternativeDataVISAApplication"; //
	m_mapFinnhubInquiryIndexToString[ALTERNATIVE_DATA_SENATE_LOBBYING_] = "AlternativeDataSenateLobbying"; //
	m_mapFinnhubInquiryIndexToString[ALTERNATIVE_DATA_USA_SPENDING_] = "AlternativeDataUSASpending"; //
	m_mapFinnhubInquiryIndexToString[ALTERNATIVE_DATA_CONGRESSIONAL_TRADING_] = "AlternativeDataCongressionalTrading"; //
	m_mapFinnhubInquiryIndexToString[ALTERNATIVE_DATA_COVID_19_] = "AlternativeDataCOVID_19";
	m_mapFinnhubInquiryIndexToString[ALTERNATIVE_DATA_FDA_CALENDAR_] = "AlternativeDataFDACalendar";

	// Economic
	m_mapFinnhubInquiryIndexToString[ECONOMIC_COUNTRY_LIST_] = "EconomicCountryList";
	m_mapFinnhubInquiryIndexToString[ECONOMIC_CALENDAR_] = "EconomicCalendar"; //Premium
	m_mapFinnhubInquiryIndexToString[ECONOMIC_CODES_] = "EconomicCodes"; //Premium
	m_mapFinnhubInquiryIndexToString[ECONOMIC_ECONOMIC_] = "EconomicEconomic"; //Premium
}

void CFinnhubInquiryType::CreateFinnhubInquiryStringToIndexMap() {
	// Web Socket
	m_mapFinnhubInquiryStringToIndex["WebSocketTrades"] = WEBSOCKET_TRADES_;
	m_mapFinnhubInquiryStringToIndex["WebSocketNews"] = WEBSOCKET_NEWS_; // Premium

	// Stock Fundamentals
	m_mapFinnhubInquiryStringToIndex["StockFundamentalsSymbolLookup"] = SYMBOL_LOOKUP_;
	m_mapFinnhubInquiryStringToIndex["StockFundamentalsSymbols"] = STOCK_SYMBOLS_;
	m_mapFinnhubInquiryStringToIndex["StockFundamentalsMarketStatus"] = MARKET_STATUS_;
	m_mapFinnhubInquiryStringToIndex["StockFundamentalsMarketHoliday"] = MARKET_HOLIDAY_;
	m_mapFinnhubInquiryStringToIndex["StockFundamentalsCompanyProfile"] = COMPANY_PROFILE_; //Premium
	m_mapFinnhubInquiryStringToIndex["StockFundamentalsCompanyProfileConcise"] = COMPANY_PROFILE_CONCISE_;
	m_mapFinnhubInquiryStringToIndex["StockFundamentalsCompanyExecutive"] = COMPANY_EXECUTIVE_; //Premium
	m_mapFinnhubInquiryStringToIndex["StockFundamentalsMarketNews"] = MARKET_NEWS_;
	m_mapFinnhubInquiryStringToIndex["StockFundamentalsCompanyNews"] = COMPANY_NEWS_;
	m_mapFinnhubInquiryStringToIndex["StockFundamentalsPressRelease"] = PRESS_RELEASE_; //Premium
	m_mapFinnhubInquiryStringToIndex["StockFundamentalsNewsSentiment"] = NEWS_SENTIMENT_;
	m_mapFinnhubInquiryStringToIndex["StockFundamentalsPeer"] = PEERS_;
	m_mapFinnhubInquiryStringToIndex["StockFundamentalsBasicFinancials"] = BASIC_FINANCIALS_;
	m_mapFinnhubInquiryStringToIndex["StockFundamentalsOwnership"] = OWNERSHIP_; //Premium
	m_mapFinnhubInquiryStringToIndex["StockFundamentalsFundOwnership"] = FUND_OWNERSHIP_; //Premium
	m_mapFinnhubInquiryStringToIndex["StockFundamentalsInsiderTransaction"] = INSIDER_TRANSACTION_;
	m_mapFinnhubInquiryStringToIndex["StockFundamentalsInsiderSentiment"] = INSIDER_SENTIMENT_;
	m_mapFinnhubInquiryStringToIndex["StockFundamentalsFinancial"] = FINANCIAL_; //Premium
	m_mapFinnhubInquiryStringToIndex["StockFundamentalsFinancialAsReported"] = FINANCIAL_AS_REPORTED_;
	m_mapFinnhubInquiryStringToIndex["StockFundamentalsRevenueBreakdown"] = REVENUE_BREAKDOWN_;
	m_mapFinnhubInquiryStringToIndex["StockFundamentalsSECFilings"] = SEC_FILINGS_;
	m_mapFinnhubInquiryStringToIndex["StockFundamentalsInternationalFilings"] = INTERNATIONAL_FILINGS_; //Premium
	m_mapFinnhubInquiryStringToIndex["StockFundamentalsSECSentimentAnalysis"] = SEC_SENTIMENT_ANALYSIS_; //Premium
	m_mapFinnhubInquiryStringToIndex["StockFundamentalsSimilarityIndex"] = SIMILARITY_INDEX_; //Premium
	m_mapFinnhubInquiryStringToIndex["StockFundamentalsIPOCalendar"] = IPO_CALENDAR_;
	m_mapFinnhubInquiryStringToIndex["StockFundamentalsDividends"] = DIVIDENDS_; //Premium
	m_mapFinnhubInquiryStringToIndex["StockFundamentalsSectorMetrics"] = SECTOR_METRICS_; // Premium
	m_mapFinnhubInquiryStringToIndex["StockFundamentalsPriceMetrics"] = PRICE_METRICS_; // Premium
	m_mapFinnhubInquiryStringToIndex["StockFundamentalsSymbolChange"] = SYMBOL_CHANGE_; // Premium
	m_mapFinnhubInquiryStringToIndex["StockFundamentalsISINChange"] = ISIN_CHANGE_; // Premium

	// Stock Estimates
	m_mapFinnhubInquiryStringToIndex["StockEstimatesRecommendationTrends"] = STOCK_ESTIMATES_RECOMMENDATION_TRENDS_;
	m_mapFinnhubInquiryStringToIndex["StockEstimatesPriceTarget"] = STOCK_ESTIMATES_PRICE_TARGET_;
	m_mapFinnhubInquiryStringToIndex["StockEstimatesUpgradeDownGrade"] = STOCK_ESTIMATES_UPGRADE_DOWNGRADE_; // Premium
	m_mapFinnhubInquiryStringToIndex["StockEstimatesRevenueEstimates"] = STOCK_ESTIMATES_REVENUE_ESTIMATES_; // Premium
	m_mapFinnhubInquiryStringToIndex["StockEstimatesEPSEstimates"] = STOCK_ESTIMATES_EPS_ESTIMATES_; // Premium
	m_mapFinnhubInquiryStringToIndex["StockEstimatesEBITDAEstimates"] = STOCK_ESTIMATES_EBITDA_ESTIMATES_; // Premium
	m_mapFinnhubInquiryStringToIndex["StockEstimatesEBITEstimates"] = STOCK_ESTIMATES_EBIT_ESTIMATES_; // Premium
	m_mapFinnhubInquiryStringToIndex["StockEstimatesEPSSurprise"] = STOCK_ESTIMATES_EPS_SURPRISE_;
	m_mapFinnhubInquiryStringToIndex["StockEstimatesEarningCalendar"] = STOCK_ESTIMATES_EARNING_CALENDAR_;

	// Stock Price
	m_mapFinnhubInquiryStringToIndex["StockPriceQuote"] = STOCK_PRICE_QUOTE_; // 实时数据优先级最低
	m_mapFinnhubInquiryStringToIndex["StockPriceCandles"] = STOCK_PRICE_CANDLES_; // 历史数据优先级低
	m_mapFinnhubInquiryStringToIndex["StockPriceTickData"] = STOCK_PRICE_TICK_DATA_; //Premium
	m_mapFinnhubInquiryStringToIndex["StockPriceHistoricalNBBO"] = STOCK_PRICE_HISTORICAL_NBBO_; // Premium
	m_mapFinnhubInquiryStringToIndex["StockPriceLastBidAsk"] = STOCK_PRICE_LAST_BID_ASK_; //Premium
	m_mapFinnhubInquiryStringToIndex["StockPriceSplits"] = STOCK_PRICE_SPLITS_; // Premium
	m_mapFinnhubInquiryStringToIndex["StockPriceDividend"] = STOCK_PRICE_DIVIDEND2_; // Premium

	// ETF & Indices
	m_mapFinnhubInquiryStringToIndex["ETFSIndicesConstituents"] = ETFS_INDICES_CONSTITUENTS_;
	m_mapFinnhubInquiryStringToIndex["ETFSIndicesHistoryConstituents"] = ETFS_HISTORY_CONSTITUENTS_; // Premium
	m_mapFinnhubInquiryStringToIndex["ETFSIndicesProfile"] = ETFS_PROFILE_; // Premium
	m_mapFinnhubInquiryStringToIndex["ETFSIndicesHoldings"] = ETFS_HOLDINGS_; // Premium
	m_mapFinnhubInquiryStringToIndex["ETFSIndicesSector"] = ETFS_SECTOR_; // Premium
	m_mapFinnhubInquiryStringToIndex["ETFSIndicesCountry"] = ETFS_COUNTRY_; //Premium

	// Mutual funds
	m_mapFinnhubInquiryStringToIndex["MutualFundProfile"] = MUTUAL_FUND_PROFILE_; // Premium
	m_mapFinnhubInquiryStringToIndex["MutualFundHoldings"] = MUTUAL_FUND_HOLDINGS_; // Premium
	m_mapFinnhubInquiryStringToIndex["MutualFundSector"] = MUTUAL_FUND_SECTOR_; // Premium
	m_mapFinnhubInquiryStringToIndex["MutualFundCountry"] = MUTUAL_FUND_COUNTRY_; // Premium

	// Bonds
	m_mapFinnhubInquiryStringToIndex["BondProfile"] = BOND_PROFILE_; // Premium
	m_mapFinnhubInquiryStringToIndex["BondPrice"] = BOND_PRICE_; // Premium

	// Forex
	m_mapFinnhubInquiryStringToIndex["ForexExchange"] = FOREX_EXCHANGE_;
	m_mapFinnhubInquiryStringToIndex["ForexSymbols"] = FOREX_SYMBOLS_;
	m_mapFinnhubInquiryStringToIndex["ForexCandles"] = FOREX_CANDLES_; // 历史数据优先级低 Premium
	m_mapFinnhubInquiryStringToIndex["ForexAllRates"] = FOREX_ALL_RATES_; // Premium

	// Crypto
	m_mapFinnhubInquiryStringToIndex["CryptoExchange"] = CRYPTO_EXCHANGE_;
	m_mapFinnhubInquiryStringToIndex["CryptoSymbols"] = CRYPTO_SYMBOLS_;
	m_mapFinnhubInquiryStringToIndex["CryptoProfile"] = CRYPTO_PROFILE_; // Premium
	m_mapFinnhubInquiryStringToIndex["CryptoCandles"] = CRYPTO_CANDLES_;

	// Technical Analysis
	m_mapFinnhubInquiryStringToIndex["TechnicalAnalysisPatternRecognition"] = TECHNICAL_ANALYSIS_PATTERN_RECOGNITION_; // Premium
	m_mapFinnhubInquiryStringToIndex["TechnicalAnalysisSupportResistance"] = TECHNICAL_ANALYSIS_SUPPORT_RESISTANCE_; // Premium
	m_mapFinnhubInquiryStringToIndex["TechnicalAnalysisAggregateIndicators"] = TECHNICAL_ANALYSIS_AGGREGATE_INDICATORS_; // Premium
	m_mapFinnhubInquiryStringToIndex["TechnicalAnalysisTechnicalIndicators"] = TECHNICAL_ANALYSIS_TECHNICAL_INDICATORS_;

	// Alternative Data
	m_mapFinnhubInquiryStringToIndex["AlternativeDataTranscriptList"] = ALTERNATIVE_DATA_TRANSCRIPT_LIST_; //Premium
	m_mapFinnhubInquiryStringToIndex["AlternativeDataTranscript"] = ALTERNATIVE_DATA_TRANSCRIPT_; //Premium
	m_mapFinnhubInquiryStringToIndex["AlternativeDataSentiment"] = ALTERNATIVE_DATA_SOCIAL_SENTIMENT_;
	m_mapFinnhubInquiryStringToIndex["AlternativeDataInvestThemes"] = ALTERNATIVE_DATA_INVEST_THEMES_; // Premium
	m_mapFinnhubInquiryStringToIndex["AlternativeDataSupplyChain"] = ALTERNATIVE_DATA_SUPPLY_CHAIN_; // Premium
	m_mapFinnhubInquiryStringToIndex["AlternativeDataCompanyESG"] = ALTERNATIVE_DATA_COMPANY_ESG_; // Premium
	m_mapFinnhubInquiryStringToIndex["AlternativeDataQualityScore"] = ALTERNATIVE_DATA_EARNING_QUALITY_SCORE_; // Premium
	m_mapFinnhubInquiryStringToIndex["AlternativeDataUSPTOPatents"] = ALTERNATIVE_DATA_USPTO_PATENTS_;
	m_mapFinnhubInquiryStringToIndex["AlternativeDataVISAApplication"] = ALTERNATIVE_DATA_VISA_APPLICATION_; //
	m_mapFinnhubInquiryStringToIndex["AlternativeDataSenateLobbying"] = ALTERNATIVE_DATA_SENATE_LOBBYING_; //
	m_mapFinnhubInquiryStringToIndex["AlternativeDataUSASpending"] = ALTERNATIVE_DATA_USA_SPENDING_; //
	m_mapFinnhubInquiryStringToIndex["AlternativeDataCongressionalTrading"] = ALTERNATIVE_DATA_CONGRESSIONAL_TRADING_; //
	m_mapFinnhubInquiryStringToIndex["AlternativeDataCOVID_19"] = ALTERNATIVE_DATA_COVID_19_;
	m_mapFinnhubInquiryStringToIndex["AlternativeDataFDACalendar"] = ALTERNATIVE_DATA_FDA_CALENDAR_;

	// Economic
	m_mapFinnhubInquiryStringToIndex["EconomicCountryList"] = ECONOMIC_COUNTRY_LIST_;
	m_mapFinnhubInquiryStringToIndex["EconomicCalendar"] = ECONOMIC_CALENDAR_; //Premium
	m_mapFinnhubInquiryStringToIndex["EconomicCodes"] = ECONOMIC_CODES_; //Premium
	m_mapFinnhubInquiryStringToIndex["EconomicEconomic"] = ECONOMIC_ECONOMIC_; //Premium
}
