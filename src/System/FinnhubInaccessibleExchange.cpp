#include "pch.h"

#include"FinnhubInquiryType.h"

#include "FinnhubInaccessibleExchange.h"

using namespace std;
#include<string>
#include<iostream>
#include<fstream>
#include<memory>
using std::fstream;
using std::ios;
using std::make_shared;
using std::exception;

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

CInaccessibleExchanges::CInaccessibleExchanges(const CString& sFunction, const int iFunction, const vector<CString>& vExchange) {
	m_sFunction = sFunction;
	m_iFunction = iFunction;
	for (auto& s : vExchange) {
		m_vExchange.push_back(s);
		m_setExchange.insert(s);
	}
}

bool CInaccessibleExchanges::Assign(const CString& sFunction, const int iFunction, const vector<CString>& vExchange) {
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

bool CInaccessibleExchanges::AddExchange(const CString& sExchangeName) {
	ASSERT(sExchangeName.Compare(_T("US")) != 0); // 系统中不存储美国市场代码（US)。
	m_vExchange.push_back(sExchangeName);
	m_setExchange.insert(sExchangeName);
	return true;
}

bool CInaccessibleExchanges::DeleteExchange(const CString& sExchangeName) {
	if (m_setExchange.contains(sExchangeName)) { // 集合中存在此元素？
		m_setExchange.erase(sExchangeName);
		for (int position = 0; position < m_vExchange.size(); position++) {
			if (m_vExchange.at(position).Compare(sExchangeName) == 0) {
				m_vExchange.erase(m_vExchange.begin() + position);
				break;
			}
		}
	}
	return true;
}

bool CInaccessibleExchanges::HaveExchange(const CString& sExchange) const {
	if (m_setExchange.contains(sExchange)) return true;
	else return false;
}

bool CInaccessibleExchanges::HaveExchange(void) const {
	if (m_vExchange.empty()) return false;
	else return true;
}

CFinnhubInaccessibleExchange::CFinnhubInaccessibleExchange() {
	if (static int siInstance = 0; ++siInstance > 1) {
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
	if (LoadDB()) { Update(); }
}

CFinnhubInaccessibleExchange::~CFinnhubInaccessibleExchange() {
	const CString strOld = m_strFileName.Left(m_strFileName.GetLength() - 4) + _T("json");
	const CString strNew = m_strFileName.Left(m_strFileName.GetLength() - 4) + _T("bak");

	if (m_fUpdate) {
		DeleteFile(gl_systemConfiguration.GetDefaultFileDirectory() + strNew);
		rename(gl_systemConfiguration.GetDefaultFileDirectory() + strOld, gl_systemConfiguration.GetDefaultFileDirectory() + strNew); // 保存备份
		UpdateJson();
		SaveDB();
	}
}

bool CFinnhubInaccessibleExchange::LoadDB(void) {
	fstream f(gl_systemConfiguration.GetDefaultFileDirectory() + m_strFileName, ios::in);
	if (f.is_open()) {
		f >> m_finnhubInaccessibleExchange;
		return true;
	}
	return false;
}

bool CFinnhubInaccessibleExchange::LoadDB(CString strFileDirectory) {
	fstream f(strFileDirectory + m_strFileName, ios::in);
	if (f.is_open()) {
		f >> m_finnhubInaccessibleExchange;
		return true;
	}
	return false;
}

void CFinnhubInaccessibleExchange::SaveDB(void) const {
	fstream f(gl_systemConfiguration.GetDefaultFileDirectory() + m_strFileName, ios::out);
	f << m_finnhubInaccessibleExchange;
	f.close();
}

void CFinnhubInaccessibleExchange::Update(void) {
	CInaccessibleExchangesPtr pExchange = nullptr;
	for (int i = 0; i < m_finnhubInaccessibleExchange.at(_T("InaccessibleExchange")).size(); i++) {
		const int size = m_finnhubInaccessibleExchange.at(_T("InaccessibleExchange")).at(i).at(_T("Exchange")).size();
		if (size > 0) {
			// 有exchange数据的话才建立数据集
			pExchange = make_shared<CInaccessibleExchanges>();
			string s2 = m_finnhubInaccessibleExchange[_T("InaccessibleExchange")].at(i).at(_T("Function")); // 从json解析出的字符串格式为std::string
			pExchange->SetFunctionString(s2.c_str());
			pExchange->SetFunction(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(pExchange->GetFunctionString()));
			for (int j = 0; j < size; j++) {
				string s = m_finnhubInaccessibleExchange.at(_T("InaccessibleExchange")).at(i).at(_T("Exchange")).at(j);
				pExchange->AddExchange(s.c_str());
			}
			gl_finnhubInaccessibleExchange.m_mapInaccessibleExchange[gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(pExchange->GetFunctionString())] = pExchange;
		}
	}
}

void CFinnhubInaccessibleExchange::UpdateJson(void) {
	m_finnhubInaccessibleExchange.clear();

	for (const auto& pExchange : m_mapInaccessibleExchange) {
		if (pExchange.second->HaveExchange()) {
			// 有exchange数据的话才建立数据集
			auto jsonExchange = json{{"Function", pExchange.second->GetFunctionString()}};
			for (int i = 0; i < pExchange.second->ExchangeSize(); i++) {
				auto s = pExchange.second->GetExchange(i);
				jsonExchange[_T("Exchange")].push_back(s);
			}

			m_finnhubInaccessibleExchange[_T("InaccessibleExchange")].push_back(jsonExchange);
		}
	}
}

void CFinnhubInaccessibleExchange::CreateFinnhubInquiryIndexToStringMap() {
	// Web Socket
	m_mapFinnhubInquiryIndexToString[WEBSOCKET_TRADES_] = _T("WebSocketTrades");
	m_mapFinnhubInquiryIndexToString[WEBSOCKET_NEWS_] = _T("WebSocketNews"); // Premium

	// Stock Fundamentals
	m_mapFinnhubInquiryIndexToString[SYMBOL_LOOKUP_] = _T("StockFundamentalsSymolLookup");
	m_mapFinnhubInquiryIndexToString[STOCK_SYMBOLS_] = _T("StockFundamentalsSymbols");
	m_mapFinnhubInquiryIndexToString[COMPANY_PROFILE_] = _T("StockFundamentalsCompanyProfile"); //Premium
	m_mapFinnhubInquiryIndexToString[COMPANY_PROFILE_CONCISE_] = _T("StockFundamentalsCompanyProfileConcise");
	m_mapFinnhubInquiryIndexToString[COMPANY_EXECTIVE_] = _T("StockFundamentalsCompanyExective"); //Premium
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
	m_mapFinnhubInquiryIndexToString[FINANCIAL_] = _T("StockFundamentalsFiniacial"); //Premium
	m_mapFinnhubInquiryIndexToString[FINANCIAL_AS_REPORTED_] = _T("StockFundamentalsFinaicialAsReported");
	m_mapFinnhubInquiryIndexToString[REVENUE_BREAKDOWN_] = _T("StockFundamentalsRevenueBreakdown");
	m_mapFinnhubInquiryIndexToString[SEC_FILINGS_] = _T("StockFundamentalsSECFilings");
	m_mapFinnhubInquiryIndexToString[INTERNATIONAL_FILINGS_] = _T("StockFundamentalsInternationalFilings"); //Premium
	m_mapFinnhubInquiryIndexToString[SEC_SENTIMENT_ANALYSIS_] = _T("StockFundamentalsSECSentimentAnalysis"); //Premium
	m_mapFinnhubInquiryIndexToString[SIMILARITY_INDEX_] = _T("StockFundamentalsSemilarityIndex"); //Premium
	m_mapFinnhubInquiryIndexToString[IPO_CALENDAR_] = _T("StockFundamentalsIPOCanlendar");
	m_mapFinnhubInquiryIndexToString[DIVIDENDS_] = _T("StockFundamentalsDividends"); //Premium

	// Stock Estimates
	m_mapFinnhubInquiryIndexToString[STOCK_ESTIMATES_RECOMMENDATION_TRENDS_] = _T("StockExtimatesRecommendationTrend");
	m_mapFinnhubInquiryIndexToString[STOCK_ESTIMATES_PRICE_TARGET_] = _T("StockExtimatesPriceTarget");
	m_mapFinnhubInquiryIndexToString[STOCK_ESTIMATES_UPGRADE_DOWNGRADE_] = _T("StockExtimatesUpgradeDownGrade"); // Primium
	m_mapFinnhubInquiryIndexToString[STOCK_ESTIMATES_REVENUE_ESTIMATES_] = _T("StockExtimatesRevenueExtimates"); // Premium
	m_mapFinnhubInquiryIndexToString[STOCK_ESTIMATES_EPS_ESTIMATES_] = _T("StockExtimatesEPSExtimates"); // Premium
	m_mapFinnhubInquiryIndexToString[STOCK_ESTIMATES_EBITDA_ESTIMATES_] = _T("StockExtimatesEBITDAExtimates"); // Premium
	m_mapFinnhubInquiryIndexToString[STOCK_ESTIMATES_EBIT_ESTIMATES_] = _T("StockExtimatesEBITExtimates"); // Premium
	m_mapFinnhubInquiryIndexToString[STOCK_ESTIMATES_EPS_SURPRISE_] = _T("StockExtimatesEPSSurprise");
	m_mapFinnhubInquiryIndexToString[STOCK_ESTIMATES_EARNING_CALENDAR_] = _T("StockExtimatesEarningCalendar");

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
	m_mapFinnhubInquiryIndexToString[ETFS_HISTORY_CONSTITUENTS_] = _T("ETFSIndicesHistoryConstitunets"); // Premium
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
	m_mapFinnhubInquiryIndexToString[TECHNICAL_ANALYSIS_SUPPORT_RESISTANCE_] = _T("TechnicalAnalysisSurportResistance"); // Premium
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
	m_mapFinnhubInquiryIndexToString[ALTERNATIVE_DATA_USPTO_PATENTS_] = _T("AlternativeDataUSPTOpatents");
	m_mapFinnhubInquiryIndexToString[ALTERNATIVE_DATA_VISA_APPLICATION_] = _T("AlternativeDataVISAApplication"); //
	m_mapFinnhubInquiryIndexToString[ALTERNATIVE_DATA_COVID_19_] = _T("AlternativeDataCOVID_19");
	m_mapFinnhubInquiryIndexToString[ALTERNATIVE_DATA_FDA_CALENDAR_] = _T("AlternativeDataFDACalendar");

	// Economic
	m_mapFinnhubInquiryIndexToString[ECONOMIC_COUNTRY_LIST_] = _T("EconomicCountryList");
	m_mapFinnhubInquiryIndexToString[ECONOMIC_CALENDAR_] = _T("EconomicCalendar"); //Premium
	m_mapFinnhubInquiryIndexToString[ECONOMIC_CODES_] = _T("EconomicCodes"); //Premium
	m_mapFinnhubInquiryIndexToString[ECONOMIC_ECONOMIC_] = _T("EconomicEconomic"); //Premium
}

void CFinnhubInaccessibleExchange::CreateFinnhubInquiryStringToIndexMap() {
	// Web Socket
	m_mapFinnhubInquiryStringToIndex[_T("WebSocketTrades")] = WEBSOCKET_TRADES_;
	m_mapFinnhubInquiryStringToIndex[_T("WebSocketNews")] = WEBSOCKET_NEWS_; // Premium

	// Stock Fundamentals
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsSymolLookup")] = SYMBOL_LOOKUP_;
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsSymbols")] = STOCK_SYMBOLS_;
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsCompanyProfile")] = COMPANY_PROFILE_; //Premium
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsCompanyProfileConcise")] = COMPANY_PROFILE_CONCISE_;
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsCompanyExective")] = COMPANY_EXECTIVE_; //Premium
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
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsFiniacial")] = FINANCIAL_; //Premium
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsFinaicialAsReported")] = FINANCIAL_AS_REPORTED_;
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsRevenueBreakdown")] = REVENUE_BREAKDOWN_;
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsSECFilings")] = SEC_FILINGS_;
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsInternationalFilings")] = INTERNATIONAL_FILINGS_; //Premium
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsSECSentimentAnalysis")] = SEC_SENTIMENT_ANALYSIS_; //Premium
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsSemilarityIndex")] = SIMILARITY_INDEX_; //Premium
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsIPOCanlendar")] = IPO_CALENDAR_;
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsDividends")] = DIVIDENDS_; //Premium

	// Stock Estimates
	m_mapFinnhubInquiryStringToIndex[_T("StockExtimatesRecommendationTrend")] = STOCK_ESTIMATES_RECOMMENDATION_TRENDS_;
	m_mapFinnhubInquiryStringToIndex[_T("StockExtimatesPriceTarget")] = STOCK_ESTIMATES_PRICE_TARGET_;
	m_mapFinnhubInquiryStringToIndex[_T("StockExtimatesUpgradeDownGrade")] = STOCK_ESTIMATES_UPGRADE_DOWNGRADE_; // Primium
	m_mapFinnhubInquiryStringToIndex[_T("StockExtimatesRevenueExtimates")] = STOCK_ESTIMATES_REVENUE_ESTIMATES_; // Premium
	m_mapFinnhubInquiryStringToIndex[_T("StockExtimatesEPSExtimates")] = STOCK_ESTIMATES_EPS_ESTIMATES_; // Premium
	m_mapFinnhubInquiryStringToIndex[_T("StockExtimatesEBITDAExtimates"] = STOCK_ESTIMATES_EBITDA_ESTIMATES_); // Premium
	m_mapFinnhubInquiryStringToIndex[_T("StockExtimatesEBITExtimates")] = STOCK_ESTIMATES_EBIT_ESTIMATES_; // Premium
	m_mapFinnhubInquiryStringToIndex[_T("StockExtimatesEPSSurprise")] = STOCK_ESTIMATES_EPS_SURPRISE_;
	m_mapFinnhubInquiryStringToIndex[_T("StockExtimatesEarningCalendar")] = STOCK_ESTIMATES_EARNING_CALENDAR_;

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
	m_mapFinnhubInquiryStringToIndex[_T("ETFSIndicesHistoryConstitunets")] = ETFS_HISTORY_CONSTITUENTS_; // Premium
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
	m_mapFinnhubInquiryStringToIndex[_T("TechnicalAnalysisSurportResistance")] = TECHNICAL_ANALYSIS_SUPPORT_RESISTANCE_; // Premium
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
	m_mapFinnhubInquiryStringToIndex[_T("AlternativeDataUSPTOpatents")] = ALTERNATIVE_DATA_USPTO_PATENTS_;
	m_mapFinnhubInquiryStringToIndex[_T("AlternativeDataVISAApplication")] = ALTERNATIVE_DATA_VISA_APPLICATION_; //
	m_mapFinnhubInquiryStringToIndex[_T("AlternativeDataCOVID_19")] = ALTERNATIVE_DATA_COVID_19_;
	m_mapFinnhubInquiryStringToIndex[_T("AlternativeDataFDACalendar")] = ALTERNATIVE_DATA_FDA_CALENDAR_;

	// Economic
	m_mapFinnhubInquiryStringToIndex[_T("EconomicCountryList")] = ECONOMIC_COUNTRY_LIST_;
	m_mapFinnhubInquiryStringToIndex[_T("EconomicCalendar")] = ECONOMIC_CALENDAR_; //Premium
	m_mapFinnhubInquiryStringToIndex[_T("EconomicCodes")] = ECONOMIC_CODES_; //Premium
	m_mapFinnhubInquiryStringToIndex[_T("EconomicEconomic")] = ECONOMIC_ECONOMIC_; //Premium
}

CInaccessibleExchangesPtr CFinnhubInaccessibleExchange::GetInaccessibleExchange(const int iInquiryType) {
	return m_mapInaccessibleExchange.at(iInquiryType);
}

bool CFinnhubInaccessibleExchange::IsInaccessible(const int iInquiryType, const CString& strExchangeCode) {
	try {
		if (m_mapInaccessibleExchange.at(iInquiryType)->HaveExchange(strExchangeCode)) {
			return true;
		}
		else {
			return false;
		}
	}
	catch (exception&) {
		return false;
	}
}
