#include"pch.h"

#include"SystemConfiguration.h"

#include"FinnhubInquiryType.h"

CFinnhubInquiryType::CFinnhubInquiryType() {
	ASSERT(gl_systemConfiguration.IsInitialized());
	if (static int siInstance = 0; ++siInstance > 1) {
		TRACE(_T("XferFinnhubInquiryType全局变量只允许存在一个实例\n"));
#ifdef _DEBUG
		ASSERT(FALSE);
#endif // _DEBUG
	}

	CreateFinnhubInquiryIndexToStringMap();
	CreateFinnhubInquiryStringToIndexMap();
}

CString CFinnhubInquiryType::GetInquiryString(const int lInquiryType) {
	return m_mapFinnhubInquiryIndexToString.at(lInquiryType);
}

int CFinnhubInquiryType::GetInquiryType(const CString& strInquiryString) const {
	return m_mapFinnhubInquiryStringToIndex.at(strInquiryString);
}

void CFinnhubInquiryType::CreateFinnhubInquiryIndexToStringMap() {
	// Web Socket
	m_mapFinnhubInquiryIndexToString[WEBSOCKET_TRADES_] = _T("WebSocketTrades");
	m_mapFinnhubInquiryIndexToString[WEBSOCKET_NEWS_] = _T("WebSocketNews"); // Premium

	// Stock Fundamentals
	m_mapFinnhubInquiryIndexToString[SYMBOL_LOOKUP_] = _T("StockFundamentalsSymbolLookup");
	m_mapFinnhubInquiryIndexToString[STOCK_SYMBOLS_] = _T("StockFundamentalsSymbols");
	m_mapFinnhubInquiryIndexToString[MARKET_STATUS_] = _T("StockFundamentalsMarketStatus");
	m_mapFinnhubInquiryIndexToString[MARKET_HOLIDAY_] = _T("StockFundamentalsMarketHoliday");
	m_mapFinnhubInquiryIndexToString[COMPANY_PROFILE_] = _T("StockFundamentalsCompanyProfile"); //Premium
	m_mapFinnhubInquiryIndexToString[COMPANY_PROFILE_CONCISE_] = _T("StockFundamentalsCompanyProfileConcise");
	m_mapFinnhubInquiryIndexToString[COMPANY_EXECUTIVE_] = _T("StockFundamentalsCompanyExecutive"); //Premium
	m_mapFinnhubInquiryIndexToString[MARKET_NEWS_] = _T("StockFundamentalsMarketNews");
	m_mapFinnhubInquiryIndexToString[COMPANY_NEWS_] = _T("StockFundamentalsCompanyNews");
	m_mapFinnhubInquiryIndexToString[PRESS_RELEASE_] = _T("StockFundamentalsPressRelease"); //Premium
	m_mapFinnhubInquiryIndexToString[NEWS_SENTIMENT_] = _T("StockFundamentalsNewsSentiment");
	m_mapFinnhubInquiryIndexToString[PEERS_] = _T("StockFundamentalsPeer");
	m_mapFinnhubInquiryIndexToString[BASIC_FINANCIALS_] = _T("StockFundamentalsBasicFinancials");
	m_mapFinnhubInquiryIndexToString[OWNERSHIP_] = _T("StockFundamentalsOwnership"); //Premium
	m_mapFinnhubInquiryIndexToString[FUND_OWNERSHIP_] = _T("StockFundamentalsFundOwnership"); //Premium
	m_mapFinnhubInquiryIndexToString[INSIDER_TRANSACTION_] = _T("StockFundamentalsInsiderTransaction");
	m_mapFinnhubInquiryIndexToString[INSIDER_SENTIMENT_] = _T("StockFundamentalsInsiderSentiment");
	m_mapFinnhubInquiryIndexToString[FINANCIAL_] = _T("StockFundamentalsFinancial"); //Premium
	m_mapFinnhubInquiryIndexToString[FINANCIAL_AS_REPORTED_] = _T("StockFundamentalsFinancialAsReported");
	m_mapFinnhubInquiryIndexToString[REVENUE_BREAKDOWN_] = _T("StockFundamentalsRevenueBreakdown");
	m_mapFinnhubInquiryIndexToString[SEC_FILINGS_] = _T("StockFundamentalsSECFilings");
	m_mapFinnhubInquiryIndexToString[INTERNATIONAL_FILINGS_] = _T("StockFundamentalsInternationalFilings"); //Premium
	m_mapFinnhubInquiryIndexToString[SEC_SENTIMENT_ANALYSIS_] = _T("StockFundamentalsSECSentimentAnalysis"); //Premium
	m_mapFinnhubInquiryIndexToString[SIMILARITY_INDEX_] = _T("StockFundamentalsSimilarityIndex"); //Premium
	m_mapFinnhubInquiryIndexToString[IPO_CALENDAR_] = _T("StockFundamentalsIPOCalendar");
	m_mapFinnhubInquiryIndexToString[DIVIDENDS_] = _T("StockFundamentalsDividends"); //Premium
	m_mapFinnhubInquiryIndexToString[SECTOR_METRICS_] = _T("SectorMetrics"); // Premium
	m_mapFinnhubInquiryIndexToString[PRICE_METRICS_] = _T("PriceMetrics"); // Premium
	m_mapFinnhubInquiryIndexToString[SYMBOL_CHANGE_] = _T("SymbolChange"); // Premium
	m_mapFinnhubInquiryIndexToString[ISIN_CHANGE_] = _T("ISINChange"); // Premium

	// Stock Estimates
	m_mapFinnhubInquiryIndexToString[STOCK_ESTIMATES_RECOMMENDATION_TRENDS_] = _T("StockEstimatesRecommendationTrends");
	m_mapFinnhubInquiryIndexToString[STOCK_ESTIMATES_PRICE_TARGET_] = _T("StockEstimatesPriceTarget");
	m_mapFinnhubInquiryIndexToString[STOCK_ESTIMATES_UPGRADE_DOWNGRADE_] = _T("StockEstimatesUpgradeDownGrade"); // Premium
	m_mapFinnhubInquiryIndexToString[STOCK_ESTIMATES_REVENUE_ESTIMATES_] = _T("StockEstimatesRevenueEstimates"); // Premium
	m_mapFinnhubInquiryIndexToString[STOCK_ESTIMATES_EPS_ESTIMATES_] = _T("StockEstimatesEPSEstimates"); // Premium
	m_mapFinnhubInquiryIndexToString[STOCK_ESTIMATES_EBITDA_ESTIMATES_] = _T("StockEstimatesEBITDAEstimates"); // Premium
	m_mapFinnhubInquiryIndexToString[STOCK_ESTIMATES_EBIT_ESTIMATES_] = _T("StockEstimatesEBITEstimates"); // Premium
	m_mapFinnhubInquiryIndexToString[STOCK_ESTIMATES_EPS_SURPRISE_] = _T("StockEstimatesEPSSurprise");
	m_mapFinnhubInquiryIndexToString[STOCK_ESTIMATES_EARNING_CALENDAR_] = _T("StockEstimatesEarningCalendar");

	// Stock Price
	m_mapFinnhubInquiryIndexToString[STOCK_PRICE_QUOTE_] = _T("StockPriceQuote"); // 实时数据优先级最低
	m_mapFinnhubInquiryIndexToString[STOCK_PRICE_CANDLES_] = _T("StockPriceCandles"); // 历史数据优先级低
	m_mapFinnhubInquiryIndexToString[STOCK_PRICE_TICK_DATA_] = _T("StockPriceTickData"); //Premium
	m_mapFinnhubInquiryIndexToString[STOCK_PRICE_HISTORICAL_NBBO_] = _T("StockPriceHistoricalNBBO"); // Premium
	m_mapFinnhubInquiryIndexToString[STOCK_PRICE_LAST_BID_ASK_] = _T("StockPriceLastBidAsk"); //Premium
	m_mapFinnhubInquiryIndexToString[STOCK_PRICE_SPLITS_] = _T("StockPriceSplits"); // Premium
	m_mapFinnhubInquiryIndexToString[STOCK_PRICE_DIVIDEND2_] = _T("StockPriceDividend"); // Premium

	// ETF & Indices
	m_mapFinnhubInquiryIndexToString[ETFS_INDICES_CONSTITUENTS_] = _T("ETFSIndicesConstituents");
	m_mapFinnhubInquiryIndexToString[ETFS_HISTORY_CONSTITUENTS_] = _T("ETFSIndicesHistoryConstituents"); // Premium
	m_mapFinnhubInquiryIndexToString[ETFS_PROFILE_] = _T("ETFSIndicesProfile"); // Premium
	m_mapFinnhubInquiryIndexToString[ETFS_HOLDINGS_] = _T("ETFSIndicesHoldings"); // Premium
	m_mapFinnhubInquiryIndexToString[ETFS_SECTOR_] = _T("ETFSIndicesSector"); // Premium
	m_mapFinnhubInquiryIndexToString[ETFS_COUNTRY_] = _T("ETFSIndicesCountry"); //Premium

	// Mutual funds
	m_mapFinnhubInquiryIndexToString[MUTUAL_FUND_PROFILE_] = _T("MutualFundProfile"); // Premium
	m_mapFinnhubInquiryIndexToString[MUTUAL_FUND_HOLDINGS_] = _T("MutualFundHoldings"); // Premium
	m_mapFinnhubInquiryIndexToString[MUTUAL_FUND_SECTOR_] = _T("MutualFundSector"); // Premium
	m_mapFinnhubInquiryIndexToString[MUTUAL_FUND_COUNTRY_] = _T("MutualFundCountry"); // Premium

	// Bonds
	m_mapFinnhubInquiryIndexToString[BOND_PROFILE_] = _T("BondProfile"); // Premium
	m_mapFinnhubInquiryIndexToString[BOND_PRICE_] = _T("BondPrice"); // Premium

	// Forex
	m_mapFinnhubInquiryIndexToString[FOREX_EXCHANGE_] = _T("ForexExchange");
	m_mapFinnhubInquiryIndexToString[FOREX_SYMBOLS_] = _T("ForexSymbols");
	m_mapFinnhubInquiryIndexToString[FOREX_CANDLES_] = _T("ForexCandles"); // 历史数据优先级低 Premium
	m_mapFinnhubInquiryIndexToString[FOREX_ALL_RATES_] = _T("ForexAllRates"); // Premium

	// Crypto
	m_mapFinnhubInquiryIndexToString[CRYPTO_EXCHANGE_] = _T("CryptoExchange");
	m_mapFinnhubInquiryIndexToString[CRYPTO_SYMBOLS_] = _T("CryptoSymbols");
	m_mapFinnhubInquiryIndexToString[CRYPTO_PROFILE_] = _T("CryptoProfile"); // Premium
	m_mapFinnhubInquiryIndexToString[CRYPTO_CANDLES_] = _T("CryptoCandles");

	// Technical Analysis
	m_mapFinnhubInquiryIndexToString[TECHNICAL_ANALYSIS_PATTERN_RECOGNITION_] = _T("TechnicalAnalysisPatternRecognition"); // Premium
	m_mapFinnhubInquiryIndexToString[TECHNICAL_ANALYSIS_SUPPORT_RESISTANCE_] = _T("TechnicalAnalysisSupportResistance"); // Premium
	m_mapFinnhubInquiryIndexToString[TECHNICAL_ANALYSIS_AGGREGATE_INDICATORS_] = _T("TechnicalAnalysisAggregateIndicators"); // Premium
	m_mapFinnhubInquiryIndexToString[TECHNICAL_ANALYSIS_TECHNICAL_INDICATORS_] = _T("TechnicalAnalysisTechnicalIndicators");

	// Alternative Data
	m_mapFinnhubInquiryIndexToString[ALTERNATIVE_DATA_TRANSCRIPT_LIST_] = _T("AlternativeDataTranscriptList"); //Premium
	m_mapFinnhubInquiryIndexToString[ALTERNATIVE_DATA_TRANSCRIPT_] = _T("AlternativeDataTranscript"); //Premium
	m_mapFinnhubInquiryIndexToString[ALTERNATIVE_DATA_SOCIAL_SENTIMENT_] = _T("AlternativeDataSentiment");
	m_mapFinnhubInquiryIndexToString[ALTERNATIVE_DATA_INVEST_THEMES_] = _T("AlternativeDataInvestThemes"); // Premium
	m_mapFinnhubInquiryIndexToString[ALTERNATIVE_DATA_SUPPLY_CHAIN_] = _T("AlternativeDataSupplyChain"); // Premium
	m_mapFinnhubInquiryIndexToString[ALTERNATIVE_DATA_COMPANY_ESG_] = _T("AlternativeDataCompanyESG"); // Premium
	m_mapFinnhubInquiryIndexToString[ALTERNATIVE_DATA_EARNING_QUALITY_SCORE_] = _T("AlternativeDataQualityScore"); // Premium
	m_mapFinnhubInquiryIndexToString[ALTERNATIVE_DATA_USPTO_PATENTS_] = _T("AlternativeDataUSPTOPatents");
	m_mapFinnhubInquiryIndexToString[ALTERNATIVE_DATA_VISA_APPLICATION_] = _T("AlternativeDataVISAApplication"); //
	m_mapFinnhubInquiryIndexToString[ALTERNATIVE_DATA_SENATE_LOBBYING_] = _T("AlternativeDataSenateLobbying"); //
	m_mapFinnhubInquiryIndexToString[ALTERNATIVE_DATA_USA_SPENDING_] = _T("AlternativeDataUSASpending"); //
	m_mapFinnhubInquiryIndexToString[ALTERNATIVE_DATA_CONGRESSIONAL_TRADING_] = _T("AlternativeDataCongressionalTrading"); //
	m_mapFinnhubInquiryIndexToString[ALTERNATIVE_DATA_COVID_19_] = _T("AlternativeDataCOVID_19");
	m_mapFinnhubInquiryIndexToString[ALTERNATIVE_DATA_FDA_CALENDAR_] = _T("AlternativeDataFDACalendar");

	// Economic
	m_mapFinnhubInquiryIndexToString[ECONOMIC_COUNTRY_LIST_] = _T("EconomicCountryList");
	m_mapFinnhubInquiryIndexToString[ECONOMIC_CALENDAR_] = _T("EconomicCalendar"); //Premium
	m_mapFinnhubInquiryIndexToString[ECONOMIC_CODES_] = _T("EconomicCodes"); //Premium
	m_mapFinnhubInquiryIndexToString[ECONOMIC_ECONOMIC_] = _T("EconomicEconomic"); //Premium
}

void CFinnhubInquiryType::CreateFinnhubInquiryStringToIndexMap() {
	// Web Socket
	m_mapFinnhubInquiryStringToIndex[_T("WebSocketTrades")] = WEBSOCKET_TRADES_;
	m_mapFinnhubInquiryStringToIndex[_T("WebSocketNews")] = WEBSOCKET_NEWS_; // Premium

	// Stock Fundamentals
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsSymbolLookup")] = SYMBOL_LOOKUP_;
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsSymbols")] = STOCK_SYMBOLS_;
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsMarketStatus")] = MARKET_STATUS_;
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsMarketHoliday")] = MARKET_HOLIDAY_;
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsCompanyProfile")] = COMPANY_PROFILE_; //Premium
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsCompanyProfileConcise")] = COMPANY_PROFILE_CONCISE_;
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsCompanyExecutive")] = COMPANY_EXECUTIVE_; //Premium
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsMarketNews")] = MARKET_NEWS_;
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsCompanyNews"] = COMPANY_NEWS_);
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsPressRelease")] = PRESS_RELEASE_; //Premium
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsNewsSentiment"] = NEWS_SENTIMENT_);
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsPeer")] = PEERS_;
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsBasicFinancials")] = BASIC_FINANCIALS_;
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsOwnership"] = OWNERSHIP_); //Premium
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsFundOwnership")] = FUND_OWNERSHIP_; //Premium
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsInsiderTransaction")] = INSIDER_TRANSACTION_;
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsInsiderSentiment")] = INSIDER_SENTIMENT_;
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsFinancial")] = FINANCIAL_; //Premium
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsFinancialAsReported")] = FINANCIAL_AS_REPORTED_;
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsRevenueBreakdown")] = REVENUE_BREAKDOWN_;
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsSECFilings")] = SEC_FILINGS_;
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsInternationalFilings")] = INTERNATIONAL_FILINGS_; //Premium
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsSECSentimentAnalysis")] = SEC_SENTIMENT_ANALYSIS_; //Premium
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsSimilarityIndex")] = SIMILARITY_INDEX_; //Premium
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsIPOCalendar")] = IPO_CALENDAR_;
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsDividends")] = DIVIDENDS_; //Premium
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsSectorMetrics")] = SECTOR_METRICS_; // Premium
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsPriceMetrics")] = PRICE_METRICS_; // Premium
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsSymbolChange")] = SYMBOL_CHANGE_; // Premium
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsISINChange")] = ISIN_CHANGE_; // Premium

	// Stock Estimates
	m_mapFinnhubInquiryStringToIndex[_T("StockEstimatesRecommendationTrends")] = STOCK_ESTIMATES_RECOMMENDATION_TRENDS_;
	m_mapFinnhubInquiryStringToIndex[_T("StockEstimatesPriceTarget")] = STOCK_ESTIMATES_PRICE_TARGET_;
	m_mapFinnhubInquiryStringToIndex[_T("StockEstimatesUpgradeDownGrade")] = STOCK_ESTIMATES_UPGRADE_DOWNGRADE_; // Premium
	m_mapFinnhubInquiryStringToIndex[_T("StockEstimatesRevenueEstimates")] = STOCK_ESTIMATES_REVENUE_ESTIMATES_; // Premium
	m_mapFinnhubInquiryStringToIndex[_T("StockEstimatesEPSEstimates")] = STOCK_ESTIMATES_EPS_ESTIMATES_; // Premium
	m_mapFinnhubInquiryStringToIndex[_T("StockEstimatesEBITDAEstimates"] = STOCK_ESTIMATES_EBITDA_ESTIMATES_); // Premium
	m_mapFinnhubInquiryStringToIndex[_T("StockEstimatesEBITEstimates")] = STOCK_ESTIMATES_EBIT_ESTIMATES_; // Premium
	m_mapFinnhubInquiryStringToIndex[_T("StockEstimatesEPSSurprise")] = STOCK_ESTIMATES_EPS_SURPRISE_;
	m_mapFinnhubInquiryStringToIndex[_T("StockEstimatesEarningCalendar")] = STOCK_ESTIMATES_EARNING_CALENDAR_;

	// Stock Price
	m_mapFinnhubInquiryStringToIndex[_T("StockPriceQuote")] = STOCK_PRICE_QUOTE_; // 实时数据优先级最低
	m_mapFinnhubInquiryStringToIndex[_T("StockPriceCandles")] = STOCK_PRICE_CANDLES_; // 历史数据优先级低
	m_mapFinnhubInquiryStringToIndex[_T("StockPriceTickData")] = STOCK_PRICE_TICK_DATA_; //Premium
	m_mapFinnhubInquiryStringToIndex[_T("StockPriceHistoricalNBBO")] = STOCK_PRICE_HISTORICAL_NBBO_; // Premium
	m_mapFinnhubInquiryStringToIndex[_T("StockPriceLastBidAsk")] = STOCK_PRICE_LAST_BID_ASK_; //Premium
	m_mapFinnhubInquiryStringToIndex[_T("StockPriceSplits")] = STOCK_PRICE_SPLITS_; // Premium
	m_mapFinnhubInquiryStringToIndex[_T("StockPriceDividend")] = STOCK_PRICE_DIVIDEND2_; // Premium

	// ETF & Indices
	m_mapFinnhubInquiryStringToIndex[_T("ETFSIndicesConstituents")] = ETFS_INDICES_CONSTITUENTS_;
	m_mapFinnhubInquiryStringToIndex[_T("ETFSIndicesHistoryConstituents")] = ETFS_HISTORY_CONSTITUENTS_; // Premium
	m_mapFinnhubInquiryStringToIndex[_T("ETFSIndicesProfile")] = ETFS_PROFILE_; // Premium
	m_mapFinnhubInquiryStringToIndex[_T("ETFSIndicesHoldings")] = ETFS_HOLDINGS_; // Premium
	m_mapFinnhubInquiryStringToIndex[_T("ETFSIndicesSector")] = ETFS_SECTOR_; // Premium
	m_mapFinnhubInquiryStringToIndex[_T("ETFSIndicesCountry")] = ETFS_COUNTRY_; //Premium

	// Mutual funds
	m_mapFinnhubInquiryStringToIndex[_T("MutualFundProfile")] = MUTUAL_FUND_PROFILE_; // Premium
	m_mapFinnhubInquiryStringToIndex[_T("MutualFundHoldings")] = MUTUAL_FUND_HOLDINGS_; // Premium
	m_mapFinnhubInquiryStringToIndex[_T("MutualFundSector")] = MUTUAL_FUND_SECTOR_; // Premium
	m_mapFinnhubInquiryStringToIndex[_T("MutualFundCountry")] = MUTUAL_FUND_COUNTRY_; // Premium

	// Bonds
	m_mapFinnhubInquiryStringToIndex[_T("BondProfile")] = BOND_PROFILE_; // Premium
	m_mapFinnhubInquiryStringToIndex[_T("BondPrice")] = BOND_PRICE_; // Premium

	// Forex
	m_mapFinnhubInquiryStringToIndex[_T("ForexExchange")] = FOREX_EXCHANGE_;
	m_mapFinnhubInquiryStringToIndex[_T("ForexSymbols")] = FOREX_SYMBOLS_;
	m_mapFinnhubInquiryStringToIndex[_T("ForexCandles")] = FOREX_CANDLES_; // 历史数据优先级低 Premium
	m_mapFinnhubInquiryStringToIndex[_T("ForexAllRates")] = FOREX_ALL_RATES_; // Premium

	// Crypto
	m_mapFinnhubInquiryStringToIndex[_T("CryptoExchange")] = CRYPTO_EXCHANGE_;
	m_mapFinnhubInquiryStringToIndex[_T("CryptoSymbols")] = CRYPTO_SYMBOLS_;
	m_mapFinnhubInquiryStringToIndex[_T("CryptoProfile")] = CRYPTO_PROFILE_; // Premium
	m_mapFinnhubInquiryStringToIndex[_T("CryptoCandles")] = CRYPTO_CANDLES_;

	// Technical Analysis
	m_mapFinnhubInquiryStringToIndex[_T("TechnicalAnalysisPatternRecognition")] = TECHNICAL_ANALYSIS_PATTERN_RECOGNITION_; // Premium
	m_mapFinnhubInquiryStringToIndex[_T("TechnicalAnalysisSupportResistance")] = TECHNICAL_ANALYSIS_SUPPORT_RESISTANCE_; // Premium
	m_mapFinnhubInquiryStringToIndex[_T("TechnicalAnalysisAggregateIndicators")] = TECHNICAL_ANALYSIS_AGGREGATE_INDICATORS_; // Premium
	m_mapFinnhubInquiryStringToIndex[_T("TechnicalAnalysisTechnicalIndicators")] = TECHNICAL_ANALYSIS_TECHNICAL_INDICATORS_;

	// Alternative Data
	m_mapFinnhubInquiryStringToIndex[_T("AlternativeDataTranscriptList")] = ALTERNATIVE_DATA_TRANSCRIPT_LIST_; //Premium
	m_mapFinnhubInquiryStringToIndex[_T("AlternativeDataTranscript")] = ALTERNATIVE_DATA_TRANSCRIPT_; //Premium
	m_mapFinnhubInquiryStringToIndex[_T("AlternativeDataSentiment")] = ALTERNATIVE_DATA_SOCIAL_SENTIMENT_;
	m_mapFinnhubInquiryStringToIndex[_T("AlternativeDataInvestThemes")] = ALTERNATIVE_DATA_INVEST_THEMES_; // Premium
	m_mapFinnhubInquiryStringToIndex[_T("AlternativeDataSupplyChain")] = ALTERNATIVE_DATA_SUPPLY_CHAIN_; // Premium
	m_mapFinnhubInquiryStringToIndex[_T("AlternativeDataCompanyESG")] = ALTERNATIVE_DATA_COMPANY_ESG_; // Premium
	m_mapFinnhubInquiryStringToIndex[_T("AlternativeDataQualityScore")] = ALTERNATIVE_DATA_EARNING_QUALITY_SCORE_; // Premium
	m_mapFinnhubInquiryStringToIndex[_T("AlternativeDataUSPTOPatents")] = ALTERNATIVE_DATA_USPTO_PATENTS_;
	m_mapFinnhubInquiryStringToIndex[_T("AlternativeDataVISAApplication")] = ALTERNATIVE_DATA_VISA_APPLICATION_; //
	m_mapFinnhubInquiryStringToIndex[_T("AlternativeDataSenateLobbying")] = ALTERNATIVE_DATA_SENATE_LOBBYING_; //
	m_mapFinnhubInquiryStringToIndex[_T("AlternativeDataUSASpending")] = ALTERNATIVE_DATA_USA_SPENDING_; //
	m_mapFinnhubInquiryStringToIndex[_T("AlternativeDataCongressionalTrading")] = ALTERNATIVE_DATA_CONGRESSIONAL_TRADING_; //
	m_mapFinnhubInquiryStringToIndex[_T("AlternativeDataCOVID_19")] = ALTERNATIVE_DATA_COVID_19_;
	m_mapFinnhubInquiryStringToIndex[_T("AlternativeDataFDACalendar")] = ALTERNATIVE_DATA_FDA_CALENDAR_;

	// Economic
	m_mapFinnhubInquiryStringToIndex[_T("EconomicCountryList")] = ECONOMIC_COUNTRY_LIST_;
	m_mapFinnhubInquiryStringToIndex[_T("EconomicCalendar")] = ECONOMIC_CALENDAR_; //Premium
	m_mapFinnhubInquiryStringToIndex[_T("EconomicCodes")] = ECONOMIC_CODES_; //Premium
	m_mapFinnhubInquiryStringToIndex[_T("EconomicEconomic")] = ECONOMIC_ECONOMIC_; //Premium
}
