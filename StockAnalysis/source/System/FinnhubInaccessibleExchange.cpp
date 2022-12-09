#include "pch.h"

#include"FinnhubInquiryType.h"

#include "FinnhubInaccessibleExchange.h"


#include<string>
#include<iostream>
#include<fstream>

std::string gl_sFinnhubInaccessibleExchange = R"(
{ "InaccessibleExchange" :
[
{
	"Function" : "StockFundamentalsCompanyProfileConcise",
	"Exchange" : ["SS", "SZ"]
},
{
	"Function" : "StockFundamentalsBasicFinancials",
	"Exchange" : ["SS", "SZ"]
}
]})";

CInaccessibleExchanges::CInaccessibleExchanges(void) {
	m_sFunction = _T("");
	m_iFunction = 0;
	m_vExchange.clear();
	m_setExchange.clear();
}

CInaccessibleExchanges::CInaccessibleExchanges(CString sFunction, int iFunction, vector<CString>& vExchange) {
	m_sFunction = sFunction;
	m_iFunction = iFunction;
	for (auto& s : vExchange) {
		m_vExchange.push_back(s);
		m_setExchange.insert(s);
	}
}

CInaccessibleExchanges::~CInaccessibleExchanges() {
}

bool CInaccessibleExchanges::Assign(CString sFunction, int iFunction, vector<CString>& vExchange) {
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

bool CInaccessibleExchanges::AddExchange(CString sExchangeName) {
	ASSERT(sExchangeName.Compare(_T("US")) != 0); // 系统中不存储美国市场代码（US)。
	m_vExchange.push_back(sExchangeName);
	m_setExchange.insert(sExchangeName);
	return true;
}

bool CInaccessibleExchanges::DeleteExchange(CString sExchangeName) {
	int position = 0;
	if (m_setExchange.count(sExchangeName) > 0) { // 集合中存在此元素？
		m_setExchange.erase(sExchangeName);
		for (position = 0; position < m_vExchange.size(); position++) {
			if (m_vExchange.at(position).Compare(sExchangeName) == 0) {
				m_vExchange.erase(m_vExchange.begin() + position);
				break;
			}
		}
	}
	return true;
}

bool CInaccessibleExchanges::HaveExchange(CString sExchange) {
	if (m_setExchange.count(sExchange) > 0) return true;
	else return false;
}

bool CInaccessibleExchanges::HaveExchange(void) {
	if (m_vExchange.size() > 0) return true;
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

	CreateFinnhubInquiryIndexToStringMap();
	CreateFinnhubInquiryStringToIndexMap();

	m_fUpdate = false; // update flag
	m_fInitialized = true;
	m_strFileName = _T("FinnhubInaccessibleExchange.json"); // json file name

	ASSERT(m_strFileName.Compare(_T("FinnhubInaccessibleExchange.json")) == 0);
	if (LoadDB()) {
		Update();
	}
}

CFinnhubInaccessibleExchange::~CFinnhubInaccessibleExchange() {
	CString strOld = m_strFileName.Left(m_strFileName.GetLength() - 4) + _T("json");
	CString strNew = m_strFileName.Left(m_strFileName.GetLength() - 4) + _T("bak");

	if (m_fUpdate) {
		DeleteFile(gl_systemConfigeration.GetDefaultFileDirectory() + strNew);
		rename(gl_systemConfigeration.GetDefaultFileDirectory() + strOld, gl_systemConfigeration.GetDefaultFileDirectory() + strNew); // 保存备份
		UpdateJson();
		SaveDB();
	}
}

bool CFinnhubInaccessibleExchange::LoadDB(void) {
	fstream f(gl_systemConfigeration.GetDefaultFileDirectory() + m_strFileName, ios::in);
	if (f.is_open()) {
		f >> m_finnhubInaccessibleExange;
		return true;
	}
	return false;
}

void CFinnhubInaccessibleExchange::SaveDB(void) {
	fstream f(gl_systemConfigeration.GetDefaultFileDirectory() + m_strFileName, ios::out);
	f << m_finnhubInaccessibleExange;
	f.close();
}

void CFinnhubInaccessibleExchange::Update(void) {
	CInaccessibleExchangesPtr pExchange = nullptr;
	for (int i = 0; i < m_finnhubInaccessibleExange.at(_T("InaccessibleExchange")).size(); i++) {
		int size = m_finnhubInaccessibleExange.at(_T("InaccessibleExchange")).at(i).at(_T("Exchange")).size();
		if (size > 0) { // 有exchange数据的话才建立数据集
			pExchange = make_shared<CInaccessibleExchanges>();
			string s2 = m_finnhubInaccessibleExange[_T("InaccessibleExchange")].at(i).at(_T("Function")); // 从json解析出的字符串格式为std::string
			pExchange->SetFunctionString(s2.c_str());
			pExchange->SetFunction(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(pExchange->GetFunctionString()));
			for (int j = 0; j < size; j++) {
				string s = m_finnhubInaccessibleExange.at(_T("InaccessibleExchange")).at(i).at(_T("Exchange")).at(j);
				pExchange->AddExchange(s.c_str());
			}
			gl_finnhubInaccessibleExchange.m_mapInaccessibleExchange[gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(pExchange->GetFunctionString())] = pExchange;
		}
	}
}

void CFinnhubInaccessibleExchange::UpdateJson(void) {
	m_finnhubInaccessibleExange.clear();

	for (auto& pExchange : m_mapInaccessibleExchange) {
		json jsonExchange;
		if (pExchange.second->HaveExchange()) {// 有exchange数据的话才建立数据集
			jsonExchange = json{ {"Function", pExchange.second->GetFunctionString()} };
			for (int i = 0; i < pExchange.second->ExchangeSize(); i++) {
				auto s = pExchange.second->GetExchange(i);
				jsonExchange[_T("Exchange")].push_back(s);
			}

			m_finnhubInaccessibleExange[_T("InaccessibleExchange")].push_back(jsonExchange);
		}
	}
}

void CFinnhubInaccessibleExchange::CreateFinnhubInquiryIndexToStringMap() {
	// Web Socket
	m_mapFinnhubInquiryIndexToString[_WEBSOCKET_TRADES_] = _T("WebSocketTrades");
	m_mapFinnhubInquiryIndexToString[_WEBSOCKET_NEWS_] = _T("WebSocketNews");// Premium

	// Stock Fundamentals
	m_mapFinnhubInquiryIndexToString[_SYMBOL_LOOKUP_] = _T("StockFundamentalsSymolLookup");
	m_mapFinnhubInquiryIndexToString[_STOCK_SYMBOLS_] = _T("StockFundamentalsSymbols");
	m_mapFinnhubInquiryIndexToString[_COMPANY_PROFILE_] = _T("StockFundamentalsCompanyProfile"); //Premium
	m_mapFinnhubInquiryIndexToString[_COMPANY_PROFILE_CONCISE_] = _T("StockFundamentalsCompanyProfileConcise");
	m_mapFinnhubInquiryIndexToString[_COMPANY_EXECTIVE_] = _T("StockFundamentalsCompanyExective"); //Premium
	m_mapFinnhubInquiryIndexToString[_MARKET_NEWS_] = _T("StockFundamentalsMarketNews");
	m_mapFinnhubInquiryIndexToString[_COMPANY_NEWS_] = _T("StockFundamentalsCompanyNews");
	m_mapFinnhubInquiryIndexToString[_PRESS_RELEASE_] = _T("StockFundamentalsPressRelease"); //Premium
	m_mapFinnhubInquiryIndexToString[_NEWS_SENTIMENT_] = _T("StockFundamentalsNewsSentiment");
	m_mapFinnhubInquiryIndexToString[_PEERS_] = _T("StockFundamentalsPeer");
	m_mapFinnhubInquiryIndexToString[_BASIC_FINANCIALS_] = _T("StockFundamentalsBasicFinancials");
	m_mapFinnhubInquiryIndexToString[_OWNERSHIP_] = _T("StockFundamentalsOwnership"); //Premium
	m_mapFinnhubInquiryIndexToString[_FUND_OWNERSHIP_] = _T("StockFundamentalsFundOwnership"); //Premium
	m_mapFinnhubInquiryIndexToString[_INSIDER_TRANSACTION_] = _T("StockFundamentalsInsiderTransaction");
	m_mapFinnhubInquiryIndexToString[_INSIDER_SENTIMENT_] = _T("StockFundamentalsInsiderSentiment");
	m_mapFinnhubInquiryIndexToString[_FINANCIAL_] = _T("StockFundamentalsFiniacial"); //Premium
	m_mapFinnhubInquiryIndexToString[_FINAICIAL_AS_REPORTED_] = _T("StockFundamentalsFinaicialAsReported");
	m_mapFinnhubInquiryIndexToString[_REVENUE_BREAKDOWN_] = _T("StockFundamentalsRevenueBreakdown");
	m_mapFinnhubInquiryIndexToString[_SEC_FILINGS_] = _T("StockFundamentalsSECFilings");
	m_mapFinnhubInquiryIndexToString[_INTERNATIONAL_FILINGS_] = _T("StockFundamentalsInternationalFilings"); //Premium
	m_mapFinnhubInquiryIndexToString[_SEC_SENTIMENT_ANALYSIS_] = _T("StockFundamentalsSECSentimentAnalysis"); //Premium
	m_mapFinnhubInquiryIndexToString[_SIMILARITY_INDEX_] = _T("StockFundamentalsSemilarityIndex"); //Premium
	m_mapFinnhubInquiryIndexToString[_IPO_CALENDAR_] = _T("StockFundamentalsIPOCanlendar");
	m_mapFinnhubInquiryIndexToString[_DIVIDENDS_] = _T("StockFundamentalsDividends"); //Premium

	// Stock Estimates
	m_mapFinnhubInquiryIndexToString[_STOCK_ESTIMATES_RECOMMENDATION_TRENDS_] = _T("StockExtimatesRecommendationTrend");
	m_mapFinnhubInquiryIndexToString[_STOCK_ESTIMATES_PRICE_TARGET_] = _T("StockExtimatesPriceTarget");
	m_mapFinnhubInquiryIndexToString[_STOCK_ESTIMATES_UPGRADE_DOWNGRADE_] = _T("StockExtimatesUpgradeDownGrade"); // Primium
	m_mapFinnhubInquiryIndexToString[_STOCK_ESTIMATES_REVENUE_EXTIMATES_] = _T("StockExtimatesRevenueExtimates"); // Premium
	m_mapFinnhubInquiryIndexToString[_STOCK_ESTIMATES_EPS_EXTIMATES_] = _T("StockExtimatesEPSExtimates"); // Premium
	m_mapFinnhubInquiryIndexToString[_STOCK_EXTIMATES_EBITDA_EXTIMATES_] = _T("StockExtimatesEBITDAExtimates"); // Premium
	m_mapFinnhubInquiryIndexToString[_STOCK_EXTIMATES_EBIT_EXTIMATES_] = _T("StockExtimatesEBITExtimates"); // Premium
	m_mapFinnhubInquiryIndexToString[_STOCK_ESTIMATES_EPS_SURPRISE_] = _T("StockExtimatesEPSSurprise");
	m_mapFinnhubInquiryIndexToString[_STOCK_ESTIMATES_EARNING_CALENDAR_] = _T("StockExtimatesEarningCalendar");

	// Stock Price
	m_mapFinnhubInquiryIndexToString[_STOCK_PRICE_QUOTE_] = _T("StockPriceQuote"); // 实时数据优先级最低
	m_mapFinnhubInquiryIndexToString[_STOCK_PRICE_CANDLES_] = _T("StockPriceCandles"); // 历史数据优先级低
	m_mapFinnhubInquiryIndexToString[_STOCK_PRICE_TICK_DATA_] = _T("StockPriceTickData"); //Premium
	m_mapFinnhubInquiryIndexToString[_STOCK_PRICE_HISTORICAL_NBBO_] = _T("StockPriceHistoricalNBBO"); // Premium
	m_mapFinnhubInquiryIndexToString[_STOCK_PRICE_LAST_BID_ASK_] = _T("StockPriceLastBidAsk"); //Premium
	m_mapFinnhubInquiryIndexToString[_STOCK_PRICE_SPLITS_] = _T("StockPriceSplits"); // Premium
	m_mapFinnhubInquiryIndexToString[_STOCK_PRICE_DIVIDEND2_] = _T("StockPriceDividend"); // Premium

	// ETF & Indices
	m_mapFinnhubInquiryIndexToString[_ETFS_INDICES_CONSTITUENTS_] = _T("ETFSIndicesConstituents");
	m_mapFinnhubInquiryIndexToString[_ETFS_HISTORY_CONSTITUENTS_] = _T("ETFSIndicesHistoryConstitunets"); // Premium
	m_mapFinnhubInquiryIndexToString[_ETFS_PROFILE_] = _T("ETFSIndicesProfile"); // Premium
	m_mapFinnhubInquiryIndexToString[_ETFS_HOLDINGS_] = _T("ETFSIndicesHoldings"); // Premium
	m_mapFinnhubInquiryIndexToString[_ETFS_SECTOR_] = _T("ETFSIndicesSector"); // Premium
	m_mapFinnhubInquiryIndexToString[_ETFS_COUNTRY_] = _T("ETFSIndicesCountry"); //Premium

	// Mutual funds
	m_mapFinnhubInquiryIndexToString[_MUTUAL_FUND_PROFILE_] = _T("MutualFundProfile"); // Premium
	m_mapFinnhubInquiryIndexToString[_MUTUAL_FUND_HOLDINGS_] = _T("MutualFundHoldings");// Premium
	m_mapFinnhubInquiryIndexToString[_MUTUAL_FUND_SECTOR_] = _T("MutualFundSector"); // Premium
	m_mapFinnhubInquiryIndexToString[_MUTUAL_FUND_COUNTRY_] = _T("MutualFundCountry"); // Premium

	// Bonds
	m_mapFinnhubInquiryIndexToString[_BOND_PROFILE_] = _T("BondProfile"); // Premium
	m_mapFinnhubInquiryIndexToString[_BOND_PRICE_] = _T("BondPrice"); // Premium

	// Forex
	m_mapFinnhubInquiryIndexToString[_FOREX_EXCHANGE_] = _T("ForexExchange");
	m_mapFinnhubInquiryIndexToString[_FOREX_SYMBOLS_] = _T("ForexSymbols");
	m_mapFinnhubInquiryIndexToString[_FOREX_CANDLES_] = _T("ForexCandles"); // 历史数据优先级低 Premium
	m_mapFinnhubInquiryIndexToString[_FOREX_ALL_RATES_] = _T("ForexAllRates");  // Premium

	// Crypto
	m_mapFinnhubInquiryIndexToString[_CRYPTO_EXCHANGE_] = _T("CryptoExchange");
	m_mapFinnhubInquiryIndexToString[_CRYPTO_SYMBOLS_] = _T("CryptoSymbols");
	m_mapFinnhubInquiryIndexToString[_CRYPTO_PROFILE_] = _T("CryptoProfile"); // Premium
	m_mapFinnhubInquiryIndexToString[_CRYPTO_CANDLES_] = _T("CryptoCandles");

	// Technical Analysis
	m_mapFinnhubInquiryIndexToString[_TECHNICAL_ANALYSIS_PATTERN_RECOGNITION_] = _T("TechnicalAnalysisPatternRecognition"); // Premium
	m_mapFinnhubInquiryIndexToString[_TECHNICAL_ANALYSIS_SURPORT_RESISTANCE_] = _T("TechnicalAnalysisSurportResistance"); // Premium
	m_mapFinnhubInquiryIndexToString[_TECHNICAL_ANALYSIS_AGGREGATE_INDICATORS_] = _T("TechnicalAnalysisAggregateIndicators"); // Premium
	m_mapFinnhubInquiryIndexToString[_TECHNICAL_ANALYSIS_TECHNICAL_INDICATORS_] = _T("TechnicalAnalysisTechnicalIndicators");

	// Alternative Data
	m_mapFinnhubInquiryIndexToString[_ALTERNATIVE_DATA_TRANSCRIPT_LIST_] = _T("AlternativeDataTranscriptList"); //Premium
	m_mapFinnhubInquiryIndexToString[_ALTERNATIVE_DATA_TRANSCRIPT_] = _T("AlternativeDataTranscript"); //Premium
	m_mapFinnhubInquiryIndexToString[_ALTERNATIVE_DATA_SOCIAL_SETIMENT_] = _T("AlternativeDataSetiment");
	m_mapFinnhubInquiryIndexToString[_ALTERNATIVE_DATA_INVEST_THEMES_] = _T("AlternativeDataInvestThemes"); // Premium
	m_mapFinnhubInquiryIndexToString[_ALTERNATIVE_DATA_SUPPLY_CHAIN_] = _T("AlternativeDataSupplyChain");  // Premium
	m_mapFinnhubInquiryIndexToString[_ALTERNATIVE_DATA_COMPANY_ESG_] = _T("AlternativeDataCompanyESG"); // Premium
	m_mapFinnhubInquiryIndexToString[_ALTERNATIVE_DATA_EARNING_QUALITY_SCORE_] = _T("AlternativeDataQualityScore"); // Premium
	m_mapFinnhubInquiryIndexToString[_ALTERNATIVE_DATA_USPTO_PATENTS_] = _T("AlternativeDataUSPTOpatents");
	m_mapFinnhubInquiryIndexToString[_ALTERNATIVE_DATA_VISA_APPLICATION_] = _T("AlternativeDataVISAApplication"); //
	m_mapFinnhubInquiryIndexToString[_ALTERNATIVE_DATA_COVID_19_] = _T("AlternativeDataCOVID_19");
	m_mapFinnhubInquiryIndexToString[_ALTERNATIVE_DATA_FDA_CALENDAR_] = _T("AlternativeDataFDACalendar");

	// Econimic
	m_mapFinnhubInquiryIndexToString[_ECONOMIC_COUNTRY_LIST_] = _T("EconomicCountryList");
	m_mapFinnhubInquiryIndexToString[_ECONOMIC_CALENDAR_] = _T("EconomicCalendar"); //Premium
	m_mapFinnhubInquiryIndexToString[_ECONOMIC_CODES_] = _T("EconomicCodes"); //Premium
	m_mapFinnhubInquiryIndexToString[_ECONOMIC_ECONOMIC_] = _T("EconomicEconomic"); //Premium
}

void CFinnhubInaccessibleExchange::CreateFinnhubInquiryStringToIndexMap() {
	// Web Socket
	m_mapFinnhubInquiryStringToIndex[_T("WebSocketTrades")] = _WEBSOCKET_TRADES_;
	m_mapFinnhubInquiryStringToIndex[_T("WebSocketNews")] = _WEBSOCKET_NEWS_;// Premium

	// Stock Fundamentals
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsSymolLookup")] = _SYMBOL_LOOKUP_;
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsSymbols")] = _STOCK_SYMBOLS_;
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsCompanyProfile")] = _COMPANY_PROFILE_; //Premium
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsCompanyProfileConcise")] = _COMPANY_PROFILE_CONCISE_;
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsCompanyExective")] = _COMPANY_EXECTIVE_; //Premium
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsMarketNews")] = _MARKET_NEWS_;
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsCompanyNews"] = _COMPANY_NEWS_);
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsPressRelease")] = _PRESS_RELEASE_; //Premium
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsNewsSentiment"] = _NEWS_SENTIMENT_);
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsPeer")] = _PEERS_;
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsBasicFinancials")] = _BASIC_FINANCIALS_;
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsOwnership"] = _OWNERSHIP_); //Premium
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsFundOwnership")] = _FUND_OWNERSHIP_; //Premium
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsInsiderTransaction")] = _INSIDER_TRANSACTION_;
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsInsiderSentiment")] = _INSIDER_SENTIMENT_;
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsFiniacial")] = _FINANCIAL_; //Premium
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsFinaicialAsReported")] = _FINAICIAL_AS_REPORTED_;
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsRevenueBreakdown")] = _REVENUE_BREAKDOWN_;
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsSECFilings")] = _SEC_FILINGS_;
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsInternationalFilings")] = _INTERNATIONAL_FILINGS_; //Premium
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsSECSentimentAnalysis")] = _SEC_SENTIMENT_ANALYSIS_; //Premium
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsSemilarityIndex")] = _SIMILARITY_INDEX_; //Premium
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsIPOCanlendar")] = _IPO_CALENDAR_;
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsDividends")] = _DIVIDENDS_; //Premium

	// Stock Estimates
	m_mapFinnhubInquiryStringToIndex[_T("StockExtimatesRecommendationTrend")] = _STOCK_ESTIMATES_RECOMMENDATION_TRENDS_;
	m_mapFinnhubInquiryStringToIndex[_T("StockExtimatesPriceTarget")] = _STOCK_ESTIMATES_PRICE_TARGET_;
	m_mapFinnhubInquiryStringToIndex[_T("StockExtimatesUpgradeDownGrade")] = _STOCK_ESTIMATES_UPGRADE_DOWNGRADE_; // Primium
	m_mapFinnhubInquiryStringToIndex[_T("StockExtimatesRevenueExtimates")] = _STOCK_ESTIMATES_REVENUE_EXTIMATES_; // Premium
	m_mapFinnhubInquiryStringToIndex[_T("StockExtimatesEPSExtimates")] = _STOCK_ESTIMATES_EPS_EXTIMATES_; // Premium
	m_mapFinnhubInquiryStringToIndex[_T("StockExtimatesEBITDAExtimates"] = _STOCK_EXTIMATES_EBITDA_EXTIMATES_); // Premium
	m_mapFinnhubInquiryStringToIndex[_T("StockExtimatesEBITExtimates")] = _STOCK_EXTIMATES_EBIT_EXTIMATES_; // Premium
	m_mapFinnhubInquiryStringToIndex[_T("StockExtimatesEPSSurprise")] = _STOCK_ESTIMATES_EPS_SURPRISE_;
	m_mapFinnhubInquiryStringToIndex[_T("StockExtimatesEarningCalendar")] = _STOCK_ESTIMATES_EARNING_CALENDAR_;

	// Stock Price
	m_mapFinnhubInquiryStringToIndex[_T("StockPriceQuote")] = _STOCK_PRICE_QUOTE_; // 实时数据优先级最低
	m_mapFinnhubInquiryStringToIndex[_T("StockPriceCandles")] = _STOCK_PRICE_CANDLES_; // 历史数据优先级低
	m_mapFinnhubInquiryStringToIndex[_T("StockPriceTickData")] = _STOCK_PRICE_TICK_DATA_; //Premium
	m_mapFinnhubInquiryStringToIndex[_T("StockPriceHistoricalNBBO")] = _STOCK_PRICE_HISTORICAL_NBBO_; // Premium
	m_mapFinnhubInquiryStringToIndex[_T("StockPriceLastBidAsk")] = _STOCK_PRICE_LAST_BID_ASK_; //Premium
	m_mapFinnhubInquiryStringToIndex[_T("StockPriceSplits")] = _STOCK_PRICE_SPLITS_; // Premium
	m_mapFinnhubInquiryStringToIndex[_T("StockPriceDividend")] = _STOCK_PRICE_DIVIDEND2_; // Premium

	// ETF & Indices
	m_mapFinnhubInquiryStringToIndex[_T("ETFSIndicesConstituents")] = _ETFS_INDICES_CONSTITUENTS_;
	m_mapFinnhubInquiryStringToIndex[_T("ETFSIndicesHistoryConstitunets")] = _ETFS_HISTORY_CONSTITUENTS_; // Premium
	m_mapFinnhubInquiryStringToIndex[_T("ETFSIndicesProfile")] = _ETFS_PROFILE_; // Premium
	m_mapFinnhubInquiryStringToIndex[_T("ETFSIndicesHoldings")] = _ETFS_HOLDINGS_; // Premium
	m_mapFinnhubInquiryStringToIndex[_T("ETFSIndicesSector")] = _ETFS_SECTOR_; // Premium
	m_mapFinnhubInquiryStringToIndex[_T("ETFSIndicesCountry")] = _ETFS_COUNTRY_; //Premium

	// Mutual funds
	m_mapFinnhubInquiryStringToIndex[_T("MutualFundProfile")] = _MUTUAL_FUND_PROFILE_; // Premium
	m_mapFinnhubInquiryStringToIndex[_T("MutualFundHoldings")] = _MUTUAL_FUND_HOLDINGS_;// Premium
	m_mapFinnhubInquiryStringToIndex[_T("MutualFundSector")] = _MUTUAL_FUND_SECTOR_; // Premium
	m_mapFinnhubInquiryStringToIndex[_T("MutualFundCountry")] = _MUTUAL_FUND_COUNTRY_; // Premium

	// Bonds
	m_mapFinnhubInquiryStringToIndex[_T("BondProfile")] = _BOND_PROFILE_; // Premium
	m_mapFinnhubInquiryStringToIndex[_T("BondPrice")] = _BOND_PRICE_; // Premium

	// Forex
	m_mapFinnhubInquiryStringToIndex[_T("ForexExchange")] = _FOREX_EXCHANGE_;
	m_mapFinnhubInquiryStringToIndex[_T("ForexSymbols")] = _FOREX_SYMBOLS_;
	m_mapFinnhubInquiryStringToIndex[_T("ForexCandles")] = _FOREX_CANDLES_; // 历史数据优先级低 Premium
	m_mapFinnhubInquiryStringToIndex[_T("ForexAllRates")] = _FOREX_ALL_RATES_;  // Premium

	// Crypto
	m_mapFinnhubInquiryStringToIndex[_T("CryptoExchange")] = _CRYPTO_EXCHANGE_;
	m_mapFinnhubInquiryStringToIndex[_T("CryptoSymbols")] = _CRYPTO_SYMBOLS_;
	m_mapFinnhubInquiryStringToIndex[_T("CryptoProfile")] = _CRYPTO_PROFILE_; // Premium
	m_mapFinnhubInquiryStringToIndex[_T("CryptoCandles")] = _CRYPTO_CANDLES_;

	// Technical Analysis
	m_mapFinnhubInquiryStringToIndex[_T("TechnicalAnalysisPatternRecognition")] = _TECHNICAL_ANALYSIS_PATTERN_RECOGNITION_; // Premium
	m_mapFinnhubInquiryStringToIndex[_T("TechnicalAnalysisSurportResistance")] = _TECHNICAL_ANALYSIS_SURPORT_RESISTANCE_; // Premium
	m_mapFinnhubInquiryStringToIndex[_T("TechnicalAnalysisAggregateIndicators")] = _TECHNICAL_ANALYSIS_AGGREGATE_INDICATORS_; // Premium
	m_mapFinnhubInquiryStringToIndex[_T("TechnicalAnalysisTechnicalIndicators")] = _TECHNICAL_ANALYSIS_TECHNICAL_INDICATORS_;

	// Alternative Data
	m_mapFinnhubInquiryStringToIndex[_T("AlternativeDataTranscriptList")] = _ALTERNATIVE_DATA_TRANSCRIPT_LIST_; //Premium
	m_mapFinnhubInquiryStringToIndex[_T("AlternativeDataTranscript")] = _ALTERNATIVE_DATA_TRANSCRIPT_; //Premium
	m_mapFinnhubInquiryStringToIndex[_T("AlternativeDataSetiment")] = _ALTERNATIVE_DATA_SOCIAL_SETIMENT_;
	m_mapFinnhubInquiryStringToIndex[_T("AlternativeDataInvestThemes")] = _ALTERNATIVE_DATA_INVEST_THEMES_; // Premium
	m_mapFinnhubInquiryStringToIndex[_T("AlternativeDataSupplyChain")] = _ALTERNATIVE_DATA_SUPPLY_CHAIN_;  // Premium
	m_mapFinnhubInquiryStringToIndex[_T("AlternativeDataCompanyESG")] = _ALTERNATIVE_DATA_COMPANY_ESG_; // Premium
	m_mapFinnhubInquiryStringToIndex[_T("AlternativeDataQualityScore")] = _ALTERNATIVE_DATA_EARNING_QUALITY_SCORE_; // Premium
	m_mapFinnhubInquiryStringToIndex[_T("AlternativeDataUSPTOpatents")] = _ALTERNATIVE_DATA_USPTO_PATENTS_;
	m_mapFinnhubInquiryStringToIndex[_T("AlternativeDataVISAApplication")] = _ALTERNATIVE_DATA_VISA_APPLICATION_; //
	m_mapFinnhubInquiryStringToIndex[_T("AlternativeDataCOVID_19")] = _ALTERNATIVE_DATA_COVID_19_;
	m_mapFinnhubInquiryStringToIndex[_T("AlternativeDataFDACalendar")] = _ALTERNATIVE_DATA_FDA_CALENDAR_;

	// Econimic
	m_mapFinnhubInquiryStringToIndex[_T("EconomicCountryList")] = _ECONOMIC_COUNTRY_LIST_;
	m_mapFinnhubInquiryStringToIndex[_T("EconomicCalendar")] = _ECONOMIC_CALENDAR_; //Premium
	m_mapFinnhubInquiryStringToIndex[_T("EconomicCodes")] = _ECONOMIC_CODES_; //Premium
	m_mapFinnhubInquiryStringToIndex[_T("EconomicEconomic")] = _ECONOMIC_ECONOMIC_; //Premium
}

CInaccessibleExchangesPtr CFinnhubInaccessibleExchange::GetInaccessibleExchange(int iInquiryType) {
	return m_mapInaccessibleExchange.at(iInquiryType);
}

bool CFinnhubInaccessibleExchange::IsInaccessible(int iInquiryType, CString strExchangeCode) {
	try {
		if (m_mapInaccessibleExchange.at(iInquiryType)->HaveExchange(strExchangeCode)) {
			return true;
		}
		else {
			return false;
		}
	}
	catch (out_of_range&) {
		return false;
	}
	return false;
}