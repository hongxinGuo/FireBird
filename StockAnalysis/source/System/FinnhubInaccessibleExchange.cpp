#include "pch.h"

#include"FinnhubInquiryType.h"

#include "FinnhubInaccessibleExchange.h"

CInaccessibleExchanges::CInaccessibleExchanges(void) {
	m_sFunction = _T("");
	m_iFunction = 0;
	m_vExchange.clear();
	m_setExchange.clear();
}

CInaccessibleExchanges::CInaccessibleExchanges(string sFunction, int iFunction, vector<string>& vExchange) {
	m_sFunction = sFunction;
	m_iFunction = iFunction;
	for (auto& s : vExchange) {
		m_vExchange.push_back(s);
		m_setExchange.insert(s);
	}
}

CInaccessibleExchanges::~CInaccessibleExchanges() {
}

bool CInaccessibleExchanges::Assign(string sFunction, int iFunction, vector<string>& vExchange) {
	m_sFunction = sFunction;
	m_iFunction = iFunction;
	m_vExchange.clear();
	m_setExchange.clear();
	for (auto& s : vExchange) {
		m_vExchange.push_back(s);
		m_setExchange.insert(s);
	}
	return true;
}

bool CInaccessibleExchanges::AddExchange(string sExchangeName) {
	m_vExchange.push_back(sExchangeName);
	m_setExchange.insert(sExchangeName);
	return true;
}

bool CInaccessibleExchanges::DeleteExchange(string sExchangeName) {
	int position = 0;
	if (m_setExchange.count(sExchangeName) > 0) { // 集合中存在此元素？
		m_setExchange.erase(sExchangeName);
		for (position = 0; position < m_vExchange.size(); position++) {
			if (m_vExchange.at(position).compare(sExchangeName) == 0) {
				m_vExchange.erase(m_vExchange.begin() + position);
				break;
			}
		}
	}
	return true;
}

bool CInaccessibleExchanges::HaveExchange(string sExchange) {
	if (m_setExchange.count(sExchange) > 0) return true;
	else return false;
}

CFinnhubInaccessibleExchange::CFinnhubInaccessibleExchange() {
	static int siInstance = 0;

	if (++siInstance > 1) {
		TRACE(_T("GlobeOption全局变量只允许存在一个实例\n"));
#ifdef _DEBUG
		ASSERT(FALSE);
#endif // _DEBUG
	}

	CreateFinnhubInquiryMap();
}

CFinnhubInaccessibleExchange::~CFinnhubInaccessibleExchange() {
}

bool CFinnhubInaccessibleExchange::LoadDB(void) {
	return false;
}

void CFinnhubInaccessibleExchange::SaveDB(void) {
}

void CFinnhubInaccessibleExchange::CreateDefaultVector(void) {
}

void CFinnhubInaccessibleExchange::CreateFinnhubInquiryMap() {
	// Web Socket
	m_mapFinnhubInquiryString[__WEBSOCKET__TRADES__] = _T("WebSocketTrades");
	m_mapFinnhubInquiryString[__WEBSOCKET__NEWS__] = _T("WebSocketNews");// Premium

	// Stock Fundamentals
	m_mapFinnhubInquiryString[__SYMBOL_LOOKUP__] = _T("StockFundamentalsSymolLookup");
	m_mapFinnhubInquiryString[__STOCK_SYMBOLS__] = _T("StockFundamentalsSymbols");
	m_mapFinnhubInquiryString[__COMPANY_PROFILE__] = _T("StockFundamentalsCompanyProfile"); //Premium
	m_mapFinnhubInquiryString[__COMPANY_PROFILE_CONCISE__] = _T("StockFundamentalsCompanyProfileConcise");
	m_mapFinnhubInquiryString[__COMPANY_EXECTIVE__] = _T("StockFundamentalsCompanyExective"); //Premium
	m_mapFinnhubInquiryString[__MARKET_NEWS__] = _T("StockFundamentalsMarketNews");
	m_mapFinnhubInquiryString[__COMPANY_NEWS__] = _T("StockFundamentalsCompanyNews");
	m_mapFinnhubInquiryString[__PRESS_RELEASE__] = _T("StockFundamentalsPressRelease"); //Premium
	m_mapFinnhubInquiryString[__NEWS_SENTIMENT__] = _T("StockFundamentalsNewsSentiment");
	m_mapFinnhubInquiryString[__PEERS__] = _T("StockFundamentalsPeer");
	m_mapFinnhubInquiryString[__BASIC_FINANCIALS__] = _T("StockFundamentalsBasicFinancials");
	m_mapFinnhubInquiryString[__OWNERSHIP__] = _T("StockFundamentalsOwnership"); //Premium
	m_mapFinnhubInquiryString[__FUND_OWNERSHIP__] = _T("StockFundamentalsFundOwnership"); //Premium
	m_mapFinnhubInquiryString[__INSIDER_TRANSACTION__] = _T("StockFundamentalsInsiderTransaction");
	m_mapFinnhubInquiryString[__INSIDER_SENTIMENT__] = _T("StockFundamentalsInsiderSentiment");
	m_mapFinnhubInquiryString[__FINANCIAL__] = _T("StockFundamentalsFiniacial"); //Premium
	m_mapFinnhubInquiryString[__FINAICIAL_AS_REPORTED__] = _T("StockFundamentalsFinaicialAsReported");
	m_mapFinnhubInquiryString[__REVENUE_BREAKDOWN__] = _T("StockFundamentalsRevenueBreakdown");
	m_mapFinnhubInquiryString[__SEC_FILINGS__] = _T("StockFundamentalsSECFilings");
	m_mapFinnhubInquiryString[__INTERNATIONAL_FILINGS__] = _T("StockFundamentalsInternationalFilings"); //Premium
	m_mapFinnhubInquiryString[__SEC_SENTIMENT_ANALYSIS__] = _T("StockFundamentalsSECSentimentAnalysis"); //Premium
	m_mapFinnhubInquiryString[__SIMILARITY_INDEX__] = _T("StockFundamentalsSemilarityIndex"); //Premium
	m_mapFinnhubInquiryString[__IPO_CALENDAR__] = _T("StockFundamentalsIPOCanlendar");
	m_mapFinnhubInquiryString[__DIVIDENDS__] = _T("StockFundamentalsDividends"); //Premium

	// Stock Estimates
	m_mapFinnhubInquiryString[__STOCK_ESTIMATES_RECOMMENDATION_TRENDS__] = _T("StockExtimatesRecommendationTrend");
	m_mapFinnhubInquiryString[__STOCK_ESTIMATES_PRICE_TARGET__] = _T("StockExtimatesPriceTarget");
	m_mapFinnhubInquiryString[__STOCK_ESTIMATES_UPGRADE_DOWNGRADE__] = _T("StockExtimatesUpgradeDownGrade"); // Primium
	m_mapFinnhubInquiryString[__STOCK_ESTIMATES_REVENUE_EXTIMATES__] = _T("StockExtimatesRevenueExtimates"); // Premium
	m_mapFinnhubInquiryString[__STOCK_ESTIMATES_EPS_EXTIMATES__] = _T("StockExtimatesEPSExtimates"); // Premium
	m_mapFinnhubInquiryString[__STOCK_EXTIMATES_EBITDA_EXTIMATES__] = _T("StockExtimatesEBITDAExtimates"); // Premium
	m_mapFinnhubInquiryString[__STOCK_EXTIMATES_EBIT_EXTIMATES__] = _T("StockExtimatesEBITExtimates"); // Premium
	m_mapFinnhubInquiryString[__STOCK_ESTIMATES_EPS_SURPRISE__] = _T("StockExtimatesEPSSurprise");
	m_mapFinnhubInquiryString[__STOCK_ESTIMATES_EARNING_CALENDAR__] = _T("StockExtimatesEarningCalendar");

	// Stock Price
	m_mapFinnhubInquiryString[__STOCK_PRICE_QUOTE__] = _T("StockPriceQuote"); // 实时数据优先级最低
	m_mapFinnhubInquiryString[__STOCK_PRICE_CANDLES__] = _T("StockPriceCandles"); // 历史数据优先级低
	m_mapFinnhubInquiryString[__STOCK_PRICE_TICK_DATA__] = _T("StockPriceTickData"); //Premium
	m_mapFinnhubInquiryString[__STOCK_PRICE_HISTORICAL_NBBO__] = _T("StockPriceHistoricalNBBO"); // Premium
	m_mapFinnhubInquiryString[__STOCK_PRICE_LAST_BID_ASK__] = _T("StockPriceLastBidAsk"); //Premium
	m_mapFinnhubInquiryString[__STOCK_PRICE_SPLITS__] = _T("StockPriceSplits"); // Premium
	m_mapFinnhubInquiryString[__STOCK_PRICE_DIVIDEND2__] = _T("StockPriceDividend"); // Premium

	// ETF & Indices
	m_mapFinnhubInquiryString[__ETFS_INDICES_CONSTITUENTS__] = _T("ETFSIndicesConstituents");
	m_mapFinnhubInquiryString[__ETFS_HISTORY_CONSTITUENTS__] = _T("ETFSIndicesHistoryConstitunets"); // Premium
	m_mapFinnhubInquiryString[__ETFS_PROFILE__] = _T("ETFSIndicesProfile"); // Premium
	m_mapFinnhubInquiryString[__ETFS_HOLDINGS__] = _T("ETFSIndicesHoldings"); // Premium
	m_mapFinnhubInquiryString[__ETFS_SECTOR__] = _T("ETFSIndicesSector"); // Premium
	m_mapFinnhubInquiryString[__ETFS_COUNTRY__] = _T("ETFSIndicesCountry"); //Premium

	// Mutual funds
	m_mapFinnhubInquiryString[__MUTUAL_FUND_PROFILE__] = _T("MutualFundProfile"); // Premium
	m_mapFinnhubInquiryString[__MUTUAL_FUND_HOLDINGS__] = _T("MutualFundHoldings");// Premium
	m_mapFinnhubInquiryString[__MUTUAL_FUND_SECTOR__] = _T("MutualFundSector"); // Premium
	m_mapFinnhubInquiryString[__MUTUAL_FUND_COUNTRY__] = _T("MutualFundCountry"); // Premium

	// Bonds
	m_mapFinnhubInquiryString[__BOND_PROFILE__] = _T("BondProfile"); // Premium
	m_mapFinnhubInquiryString[__BOND_PRICE__] = _T("BondPrice"); // Premium

	// Forex
	m_mapFinnhubInquiryString[__FOREX_EXCHANGE__] = _T("ForexExchange");
	m_mapFinnhubInquiryString[__FOREX_SYMBOLS__] = _T("ForexSymbols");
	m_mapFinnhubInquiryString[__FOREX_CANDLES__] = _T("ForexCandles"); // 历史数据优先级低 Premium
	m_mapFinnhubInquiryString[__FOREX_ALL_RATES__] = _T("ForexAllRates");  // Premium

	// Crypto
	m_mapFinnhubInquiryString[__CRYPTO_EXCHANGE__] = _T("CryptoExchange");
	m_mapFinnhubInquiryString[__CRYPTO_SYMBOLS__] = _T("CryptoSymbols");
	m_mapFinnhubInquiryString[__CRYPTO_PROFILE__] = _T("CryptoProfile"); // Premium
	m_mapFinnhubInquiryString[__CRYPTO_CANDLES__] = _T("CryptoCandles");

	// Technical Analysis
	m_mapFinnhubInquiryString[__TECHNICAL_ANALYSIS_PATTERN_RECOGNITION__] = _T("TechnicalAnalysisPatternRecognition"); // Premium
	m_mapFinnhubInquiryString[__TECHNICAL_ANALYSIS_SURPORT_RESISTANCE__] = _T("TechnicalAnalysisSurportResistance"); // Premium
	m_mapFinnhubInquiryString[__TECHNICAL_ANALYSIS_AGGREGATE_INDICATORS__] = _T("TechnicalAnalysisAggregateIndicators"); // Premium
	m_mapFinnhubInquiryString[__TECHNICAL_ANALYSIS_TECHNICAL_INDICATORS__] = _T("TechnicalAnalysisTechnicalIndicators");

	// Alternative Data
	m_mapFinnhubInquiryString[__ALTERNATIVE_DATA_TRANSCRIPT_LIST__] = _T("AlternativeDataTranscriptList"); //Premium
	m_mapFinnhubInquiryString[__ALTERNATIVE_DATA_TRANSCRIPT__] = _T("AlternativeDataTranscript"); //Premium
	m_mapFinnhubInquiryString[__ALTERNATIVE_DATA_SOCIAL_SETIMENT__] = _T("AlternativeDataSetiment");
	m_mapFinnhubInquiryString[__ALTERNATIVE_DATA_INVEST_THEMES__] = _T("AlternativeDataInvestThemes"); // Premium
	m_mapFinnhubInquiryString[__ALTERNATIVE_DATA_SUPPLY_CHAIN__] = _T("AlternativeDataSupplyChain");  // Premium
	m_mapFinnhubInquiryString[__ALTERNATIVE_DATA_COMPANY_ESG__] = _T("AlternativeDataCompanyESG"); // Premium
	m_mapFinnhubInquiryString[__ALTERNATIVE_DATA_EARNING_QUALITY_SCORE__] = _T("AlternativeDataQualityScore"); // Premium
	m_mapFinnhubInquiryString[__ALTERNATIVE_DATA_USPTO_PATENTS__] = _T("AlternativeDataUSPTOpatents");
	m_mapFinnhubInquiryString[__ALTERNATIVE_DATA_VISA_APPLICATION__] = _T("AlternativeDataVISAApplication"); //
	m_mapFinnhubInquiryString[__ALTERNATIVE_DATA_COVID_19__] = _T("AlternativeDataCOVID_19");
	m_mapFinnhubInquiryString[__ALTERNATIVE_DATA_FDA_CALENDAR__] = _T("AlternativeDataFDACalendar");

	// Econimic
	m_mapFinnhubInquiryString[__ECONOMIC_COUNTRY_LIST__] = _T("EconomicCountryList");
	m_mapFinnhubInquiryString[__ECONOMIC_CALENDAR__] = _T("EconomicCalendar"); //Premium
	m_mapFinnhubInquiryString[__ECONOMIC_CODES__] = _T("EconomicCodes"); //Premium
	m_mapFinnhubInquiryString[__ECONOMIC_ECONOMIC__] = _T("EconomicEconomic"); //Premium
}