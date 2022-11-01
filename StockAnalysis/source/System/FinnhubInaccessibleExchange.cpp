#include "pch.h"

#include"FinnhubInquiryType.h"

#include "FinnhubInaccessibleExchange.h"

using namespace std;
#include<string>
#include<iostream>
#include<fstream>

std::string gl_sFinnhubInaccessibleExchange = R"(
{ "InaccessibleExchange" :
[
{
	"Function" : "StockFundamentalsCompanyProfileConcise",
	"Exchange" : [ "US", "SS", "SZ"]
},
{
	"Function" : "StockFundamentalsBasicFinancials",
	"Exchange" : [ "US", "SS", "SZ"]
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
	m_mapFinnhubInquiryIndexToString[__WEBSOCKET__TRADES__] = _T("WebSocketTrades");
	m_mapFinnhubInquiryIndexToString[__WEBSOCKET__NEWS__] = _T("WebSocketNews");// Premium

	// Stock Fundamentals
	m_mapFinnhubInquiryIndexToString[__SYMBOL_LOOKUP__] = _T("StockFundamentalsSymolLookup");
	m_mapFinnhubInquiryIndexToString[__STOCK_SYMBOLS__] = _T("StockFundamentalsSymbols");
	m_mapFinnhubInquiryIndexToString[__COMPANY_PROFILE__] = _T("StockFundamentalsCompanyProfile"); //Premium
	m_mapFinnhubInquiryIndexToString[__COMPANY_PROFILE_CONCISE__] = _T("StockFundamentalsCompanyProfileConcise");
	m_mapFinnhubInquiryIndexToString[__COMPANY_EXECTIVE__] = _T("StockFundamentalsCompanyExective"); //Premium
	m_mapFinnhubInquiryIndexToString[__MARKET_NEWS__] = _T("StockFundamentalsMarketNews");
	m_mapFinnhubInquiryIndexToString[__COMPANY_NEWS__] = _T("StockFundamentalsCompanyNews");
	m_mapFinnhubInquiryIndexToString[__PRESS_RELEASE__] = _T("StockFundamentalsPressRelease"); //Premium
	m_mapFinnhubInquiryIndexToString[__NEWS_SENTIMENT__] = _T("StockFundamentalsNewsSentiment");
	m_mapFinnhubInquiryIndexToString[__PEERS__] = _T("StockFundamentalsPeer");
	m_mapFinnhubInquiryIndexToString[__BASIC_FINANCIALS__] = _T("StockFundamentalsBasicFinancials");
	m_mapFinnhubInquiryIndexToString[__OWNERSHIP__] = _T("StockFundamentalsOwnership"); //Premium
	m_mapFinnhubInquiryIndexToString[__FUND_OWNERSHIP__] = _T("StockFundamentalsFundOwnership"); //Premium
	m_mapFinnhubInquiryIndexToString[__INSIDER_TRANSACTION__] = _T("StockFundamentalsInsiderTransaction");
	m_mapFinnhubInquiryIndexToString[__INSIDER_SENTIMENT__] = _T("StockFundamentalsInsiderSentiment");
	m_mapFinnhubInquiryIndexToString[__FINANCIAL__] = _T("StockFundamentalsFiniacial"); //Premium
	m_mapFinnhubInquiryIndexToString[__FINAICIAL_AS_REPORTED__] = _T("StockFundamentalsFinaicialAsReported");
	m_mapFinnhubInquiryIndexToString[__REVENUE_BREAKDOWN__] = _T("StockFundamentalsRevenueBreakdown");
	m_mapFinnhubInquiryIndexToString[__SEC_FILINGS__] = _T("StockFundamentalsSECFilings");
	m_mapFinnhubInquiryIndexToString[__INTERNATIONAL_FILINGS__] = _T("StockFundamentalsInternationalFilings"); //Premium
	m_mapFinnhubInquiryIndexToString[__SEC_SENTIMENT_ANALYSIS__] = _T("StockFundamentalsSECSentimentAnalysis"); //Premium
	m_mapFinnhubInquiryIndexToString[__SIMILARITY_INDEX__] = _T("StockFundamentalsSemilarityIndex"); //Premium
	m_mapFinnhubInquiryIndexToString[__IPO_CALENDAR__] = _T("StockFundamentalsIPOCanlendar");
	m_mapFinnhubInquiryIndexToString[__DIVIDENDS__] = _T("StockFundamentalsDividends"); //Premium

	// Stock Estimates
	m_mapFinnhubInquiryIndexToString[__STOCK_ESTIMATES_RECOMMENDATION_TRENDS__] = _T("StockExtimatesRecommendationTrend");
	m_mapFinnhubInquiryIndexToString[__STOCK_ESTIMATES_PRICE_TARGET__] = _T("StockExtimatesPriceTarget");
	m_mapFinnhubInquiryIndexToString[__STOCK_ESTIMATES_UPGRADE_DOWNGRADE__] = _T("StockExtimatesUpgradeDownGrade"); // Primium
	m_mapFinnhubInquiryIndexToString[__STOCK_ESTIMATES_REVENUE_EXTIMATES__] = _T("StockExtimatesRevenueExtimates"); // Premium
	m_mapFinnhubInquiryIndexToString[__STOCK_ESTIMATES_EPS_EXTIMATES__] = _T("StockExtimatesEPSExtimates"); // Premium
	m_mapFinnhubInquiryIndexToString[__STOCK_EXTIMATES_EBITDA_EXTIMATES__] = _T("StockExtimatesEBITDAExtimates"); // Premium
	m_mapFinnhubInquiryIndexToString[__STOCK_EXTIMATES_EBIT_EXTIMATES__] = _T("StockExtimatesEBITExtimates"); // Premium
	m_mapFinnhubInquiryIndexToString[__STOCK_ESTIMATES_EPS_SURPRISE__] = _T("StockExtimatesEPSSurprise");
	m_mapFinnhubInquiryIndexToString[__STOCK_ESTIMATES_EARNING_CALENDAR__] = _T("StockExtimatesEarningCalendar");

	// Stock Price
	m_mapFinnhubInquiryIndexToString[__STOCK_PRICE_QUOTE__] = _T("StockPriceQuote"); // 实时数据优先级最低
	m_mapFinnhubInquiryIndexToString[__STOCK_PRICE_CANDLES__] = _T("StockPriceCandles"); // 历史数据优先级低
	m_mapFinnhubInquiryIndexToString[__STOCK_PRICE_TICK_DATA__] = _T("StockPriceTickData"); //Premium
	m_mapFinnhubInquiryIndexToString[__STOCK_PRICE_HISTORICAL_NBBO__] = _T("StockPriceHistoricalNBBO"); // Premium
	m_mapFinnhubInquiryIndexToString[__STOCK_PRICE_LAST_BID_ASK__] = _T("StockPriceLastBidAsk"); //Premium
	m_mapFinnhubInquiryIndexToString[__STOCK_PRICE_SPLITS__] = _T("StockPriceSplits"); // Premium
	m_mapFinnhubInquiryIndexToString[__STOCK_PRICE_DIVIDEND2__] = _T("StockPriceDividend"); // Premium

	// ETF & Indices
	m_mapFinnhubInquiryIndexToString[__ETFS_INDICES_CONSTITUENTS__] = _T("ETFSIndicesConstituents");
	m_mapFinnhubInquiryIndexToString[__ETFS_HISTORY_CONSTITUENTS__] = _T("ETFSIndicesHistoryConstitunets"); // Premium
	m_mapFinnhubInquiryIndexToString[__ETFS_PROFILE__] = _T("ETFSIndicesProfile"); // Premium
	m_mapFinnhubInquiryIndexToString[__ETFS_HOLDINGS__] = _T("ETFSIndicesHoldings"); // Premium
	m_mapFinnhubInquiryIndexToString[__ETFS_SECTOR__] = _T("ETFSIndicesSector"); // Premium
	m_mapFinnhubInquiryIndexToString[__ETFS_COUNTRY__] = _T("ETFSIndicesCountry"); //Premium

	// Mutual funds
	m_mapFinnhubInquiryIndexToString[__MUTUAL_FUND_PROFILE__] = _T("MutualFundProfile"); // Premium
	m_mapFinnhubInquiryIndexToString[__MUTUAL_FUND_HOLDINGS__] = _T("MutualFundHoldings");// Premium
	m_mapFinnhubInquiryIndexToString[__MUTUAL_FUND_SECTOR__] = _T("MutualFundSector"); // Premium
	m_mapFinnhubInquiryIndexToString[__MUTUAL_FUND_COUNTRY__] = _T("MutualFundCountry"); // Premium

	// Bonds
	m_mapFinnhubInquiryIndexToString[__BOND_PROFILE__] = _T("BondProfile"); // Premium
	m_mapFinnhubInquiryIndexToString[__BOND_PRICE__] = _T("BondPrice"); // Premium

	// Forex
	m_mapFinnhubInquiryIndexToString[__FOREX_EXCHANGE__] = _T("ForexExchange");
	m_mapFinnhubInquiryIndexToString[__FOREX_SYMBOLS__] = _T("ForexSymbols");
	m_mapFinnhubInquiryIndexToString[__FOREX_CANDLES__] = _T("ForexCandles"); // 历史数据优先级低 Premium
	m_mapFinnhubInquiryIndexToString[__FOREX_ALL_RATES__] = _T("ForexAllRates");  // Premium

	// Crypto
	m_mapFinnhubInquiryIndexToString[__CRYPTO_EXCHANGE__] = _T("CryptoExchange");
	m_mapFinnhubInquiryIndexToString[__CRYPTO_SYMBOLS__] = _T("CryptoSymbols");
	m_mapFinnhubInquiryIndexToString[__CRYPTO_PROFILE__] = _T("CryptoProfile"); // Premium
	m_mapFinnhubInquiryIndexToString[__CRYPTO_CANDLES__] = _T("CryptoCandles");

	// Technical Analysis
	m_mapFinnhubInquiryIndexToString[__TECHNICAL_ANALYSIS_PATTERN_RECOGNITION__] = _T("TechnicalAnalysisPatternRecognition"); // Premium
	m_mapFinnhubInquiryIndexToString[__TECHNICAL_ANALYSIS_SURPORT_RESISTANCE__] = _T("TechnicalAnalysisSurportResistance"); // Premium
	m_mapFinnhubInquiryIndexToString[__TECHNICAL_ANALYSIS_AGGREGATE_INDICATORS__] = _T("TechnicalAnalysisAggregateIndicators"); // Premium
	m_mapFinnhubInquiryIndexToString[__TECHNICAL_ANALYSIS_TECHNICAL_INDICATORS__] = _T("TechnicalAnalysisTechnicalIndicators");

	// Alternative Data
	m_mapFinnhubInquiryIndexToString[__ALTERNATIVE_DATA_TRANSCRIPT_LIST__] = _T("AlternativeDataTranscriptList"); //Premium
	m_mapFinnhubInquiryIndexToString[__ALTERNATIVE_DATA_TRANSCRIPT__] = _T("AlternativeDataTranscript"); //Premium
	m_mapFinnhubInquiryIndexToString[__ALTERNATIVE_DATA_SOCIAL_SETIMENT__] = _T("AlternativeDataSetiment");
	m_mapFinnhubInquiryIndexToString[__ALTERNATIVE_DATA_INVEST_THEMES__] = _T("AlternativeDataInvestThemes"); // Premium
	m_mapFinnhubInquiryIndexToString[__ALTERNATIVE_DATA_SUPPLY_CHAIN__] = _T("AlternativeDataSupplyChain");  // Premium
	m_mapFinnhubInquiryIndexToString[__ALTERNATIVE_DATA_COMPANY_ESG__] = _T("AlternativeDataCompanyESG"); // Premium
	m_mapFinnhubInquiryIndexToString[__ALTERNATIVE_DATA_EARNING_QUALITY_SCORE__] = _T("AlternativeDataQualityScore"); // Premium
	m_mapFinnhubInquiryIndexToString[__ALTERNATIVE_DATA_USPTO_PATENTS__] = _T("AlternativeDataUSPTOpatents");
	m_mapFinnhubInquiryIndexToString[__ALTERNATIVE_DATA_VISA_APPLICATION__] = _T("AlternativeDataVISAApplication"); //
	m_mapFinnhubInquiryIndexToString[__ALTERNATIVE_DATA_COVID_19__] = _T("AlternativeDataCOVID_19");
	m_mapFinnhubInquiryIndexToString[__ALTERNATIVE_DATA_FDA_CALENDAR__] = _T("AlternativeDataFDACalendar");

	// Econimic
	m_mapFinnhubInquiryIndexToString[__ECONOMIC_COUNTRY_LIST__] = _T("EconomicCountryList");
	m_mapFinnhubInquiryIndexToString[__ECONOMIC_CALENDAR__] = _T("EconomicCalendar"); //Premium
	m_mapFinnhubInquiryIndexToString[__ECONOMIC_CODES__] = _T("EconomicCodes"); //Premium
	m_mapFinnhubInquiryIndexToString[__ECONOMIC_ECONOMIC__] = _T("EconomicEconomic"); //Premium
}

void CFinnhubInaccessibleExchange::CreateFinnhubInquiryStringToIndexMap() {
	// Web Socket
	m_mapFinnhubInquiryStringToIndex[_T("WebSocketTrades")] = __WEBSOCKET__TRADES__;
	m_mapFinnhubInquiryStringToIndex[_T("WebSocketNews")] = __WEBSOCKET__NEWS__;// Premium

	// Stock Fundamentals
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsSymolLookup")] = __SYMBOL_LOOKUP__;
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsSymbols")] = __STOCK_SYMBOLS__;
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsCompanyProfile")] = __COMPANY_PROFILE__; //Premium
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsCompanyProfileConcise")] = __COMPANY_PROFILE_CONCISE__;
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsCompanyExective")] = __COMPANY_EXECTIVE__; //Premium
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsMarketNews")] = __MARKET_NEWS__;
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsCompanyNews"] = __COMPANY_NEWS__);
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsPressRelease")] = __PRESS_RELEASE__; //Premium
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsNewsSentiment"] = __NEWS_SENTIMENT__);
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsPeer")] = __PEERS__;
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsBasicFinancials")] = __BASIC_FINANCIALS__;
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsOwnership"] = __OWNERSHIP__); //Premium
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsFundOwnership")] = __FUND_OWNERSHIP__; //Premium
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsInsiderTransaction")] = __INSIDER_TRANSACTION__;
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsInsiderSentiment")] = __INSIDER_SENTIMENT__;
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsFiniacial")] = __FINANCIAL__; //Premium
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsFinaicialAsReported")] = __FINAICIAL_AS_REPORTED__;
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsRevenueBreakdown")] = __REVENUE_BREAKDOWN__;
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsSECFilings")] = __SEC_FILINGS__;
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsInternationalFilings")] = __INTERNATIONAL_FILINGS__; //Premium
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsSECSentimentAnalysis")] = __SEC_SENTIMENT_ANALYSIS__; //Premium
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsSemilarityIndex")] = __SIMILARITY_INDEX__; //Premium
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsIPOCanlendar")] = __IPO_CALENDAR__;
	m_mapFinnhubInquiryStringToIndex[_T("StockFundamentalsDividends")] = __DIVIDENDS__; //Premium

	// Stock Estimates
	m_mapFinnhubInquiryStringToIndex[_T("StockExtimatesRecommendationTrend")] = __STOCK_ESTIMATES_RECOMMENDATION_TRENDS__;
	m_mapFinnhubInquiryStringToIndex[_T("StockExtimatesPriceTarget")] = __STOCK_ESTIMATES_PRICE_TARGET__;
	m_mapFinnhubInquiryStringToIndex[_T("StockExtimatesUpgradeDownGrade")] = __STOCK_ESTIMATES_UPGRADE_DOWNGRADE__; // Primium
	m_mapFinnhubInquiryStringToIndex[_T("StockExtimatesRevenueExtimates")] = __STOCK_ESTIMATES_REVENUE_EXTIMATES__; // Premium
	m_mapFinnhubInquiryStringToIndex[_T("StockExtimatesEPSExtimates")] = __STOCK_ESTIMATES_EPS_EXTIMATES__; // Premium
	m_mapFinnhubInquiryStringToIndex[_T("StockExtimatesEBITDAExtimates"] = __STOCK_EXTIMATES_EBITDA_EXTIMATES__); // Premium
	m_mapFinnhubInquiryStringToIndex[_T("StockExtimatesEBITExtimates")] = __STOCK_EXTIMATES_EBIT_EXTIMATES__; // Premium
	m_mapFinnhubInquiryStringToIndex[_T("StockExtimatesEPSSurprise")] = __STOCK_ESTIMATES_EPS_SURPRISE__;
	m_mapFinnhubInquiryStringToIndex[_T("StockExtimatesEarningCalendar")] = __STOCK_ESTIMATES_EARNING_CALENDAR__;

	// Stock Price
	m_mapFinnhubInquiryStringToIndex[_T("StockPriceQuote")] = __STOCK_PRICE_QUOTE__; // 实时数据优先级最低
	m_mapFinnhubInquiryStringToIndex[_T("StockPriceCandles")] = __STOCK_PRICE_CANDLES__; // 历史数据优先级低
	m_mapFinnhubInquiryStringToIndex[_T("StockPriceTickData")] = __STOCK_PRICE_TICK_DATA__; //Premium
	m_mapFinnhubInquiryStringToIndex[_T("StockPriceHistoricalNBBO")] = __STOCK_PRICE_HISTORICAL_NBBO__; // Premium
	m_mapFinnhubInquiryStringToIndex[_T("StockPriceLastBidAsk")] = __STOCK_PRICE_LAST_BID_ASK__; //Premium
	m_mapFinnhubInquiryStringToIndex[_T("StockPriceSplits")] = __STOCK_PRICE_SPLITS__; // Premium
	m_mapFinnhubInquiryStringToIndex[_T("StockPriceDividend")] = __STOCK_PRICE_DIVIDEND2__; // Premium

	// ETF & Indices
	m_mapFinnhubInquiryStringToIndex[_T("ETFSIndicesConstituents")] = __ETFS_INDICES_CONSTITUENTS__;
	m_mapFinnhubInquiryStringToIndex[_T("ETFSIndicesHistoryConstitunets")] = __ETFS_HISTORY_CONSTITUENTS__; // Premium
	m_mapFinnhubInquiryStringToIndex[_T("ETFSIndicesProfile")] = __ETFS_PROFILE__; // Premium
	m_mapFinnhubInquiryStringToIndex[_T("ETFSIndicesHoldings")] = __ETFS_HOLDINGS__; // Premium
	m_mapFinnhubInquiryStringToIndex[_T("ETFSIndicesSector")] = __ETFS_SECTOR__; // Premium
	m_mapFinnhubInquiryStringToIndex[_T("ETFSIndicesCountry")] = __ETFS_COUNTRY__; //Premium

	// Mutual funds
	m_mapFinnhubInquiryStringToIndex[_T("MutualFundProfile")] = __MUTUAL_FUND_PROFILE__; // Premium
	m_mapFinnhubInquiryStringToIndex[_T("MutualFundHoldings")] = __MUTUAL_FUND_HOLDINGS__;// Premium
	m_mapFinnhubInquiryStringToIndex[_T("MutualFundSector")] = __MUTUAL_FUND_SECTOR__; // Premium
	m_mapFinnhubInquiryStringToIndex[_T("MutualFundCountry")] = __MUTUAL_FUND_COUNTRY__; // Premium

	// Bonds
	m_mapFinnhubInquiryStringToIndex[_T("BondProfile")] = __BOND_PROFILE__; // Premium
	m_mapFinnhubInquiryStringToIndex[_T("BondPrice")] = __BOND_PRICE__; // Premium

	// Forex
	m_mapFinnhubInquiryStringToIndex[_T("ForexExchange")] = __FOREX_EXCHANGE__;
	m_mapFinnhubInquiryStringToIndex[_T("ForexSymbols")] = __FOREX_SYMBOLS__;
	m_mapFinnhubInquiryStringToIndex[_T("ForexCandles")] = __FOREX_CANDLES__; // 历史数据优先级低 Premium
	m_mapFinnhubInquiryStringToIndex[_T("ForexAllRates")] = __FOREX_ALL_RATES__;  // Premium

	// Crypto
	m_mapFinnhubInquiryStringToIndex[_T("CryptoExchange")] = __CRYPTO_EXCHANGE__;
	m_mapFinnhubInquiryStringToIndex[_T("CryptoSymbols")] = __CRYPTO_SYMBOLS__;
	m_mapFinnhubInquiryStringToIndex[_T("CryptoProfile")] = __CRYPTO_PROFILE__; // Premium
	m_mapFinnhubInquiryStringToIndex[_T("CryptoCandles")] = __CRYPTO_CANDLES__;

	// Technical Analysis
	m_mapFinnhubInquiryStringToIndex[_T("TechnicalAnalysisPatternRecognition")] = __TECHNICAL_ANALYSIS_PATTERN_RECOGNITION__; // Premium
	m_mapFinnhubInquiryStringToIndex[_T("TechnicalAnalysisSurportResistance")] = __TECHNICAL_ANALYSIS_SURPORT_RESISTANCE__; // Premium
	m_mapFinnhubInquiryStringToIndex[_T("TechnicalAnalysisAggregateIndicators")] = __TECHNICAL_ANALYSIS_AGGREGATE_INDICATORS__; // Premium
	m_mapFinnhubInquiryStringToIndex[_T("TechnicalAnalysisTechnicalIndicators")] = __TECHNICAL_ANALYSIS_TECHNICAL_INDICATORS__;

	// Alternative Data
	m_mapFinnhubInquiryStringToIndex[_T("AlternativeDataTranscriptList")] = __ALTERNATIVE_DATA_TRANSCRIPT_LIST__; //Premium
	m_mapFinnhubInquiryStringToIndex[_T("AlternativeDataTranscript")] = __ALTERNATIVE_DATA_TRANSCRIPT__; //Premium
	m_mapFinnhubInquiryStringToIndex[_T("AlternativeDataSetiment")] = __ALTERNATIVE_DATA_SOCIAL_SETIMENT__;
	m_mapFinnhubInquiryStringToIndex[_T("AlternativeDataInvestThemes")] = __ALTERNATIVE_DATA_INVEST_THEMES__; // Premium
	m_mapFinnhubInquiryStringToIndex[_T("AlternativeDataSupplyChain")] = __ALTERNATIVE_DATA_SUPPLY_CHAIN__;  // Premium
	m_mapFinnhubInquiryStringToIndex[_T("AlternativeDataCompanyESG")] = __ALTERNATIVE_DATA_COMPANY_ESG__; // Premium
	m_mapFinnhubInquiryStringToIndex[_T("AlternativeDataQualityScore")] = __ALTERNATIVE_DATA_EARNING_QUALITY_SCORE__; // Premium
	m_mapFinnhubInquiryStringToIndex[_T("AlternativeDataUSPTOpatents")] = __ALTERNATIVE_DATA_USPTO_PATENTS__;
	m_mapFinnhubInquiryStringToIndex[_T("AlternativeDataVISAApplication")] = __ALTERNATIVE_DATA_VISA_APPLICATION__; //
	m_mapFinnhubInquiryStringToIndex[_T("AlternativeDataCOVID_19")] = __ALTERNATIVE_DATA_COVID_19__;
	m_mapFinnhubInquiryStringToIndex[_T("AlternativeDataFDACalendar")] = __ALTERNATIVE_DATA_FDA_CALENDAR__;

	// Econimic
	m_mapFinnhubInquiryStringToIndex[_T("EconomicCountryList")] = __ECONOMIC_COUNTRY_LIST__;
	m_mapFinnhubInquiryStringToIndex[_T("EconomicCalendar")] = __ECONOMIC_CALENDAR__; //Premium
	m_mapFinnhubInquiryStringToIndex[_T("EconomicCodes")] = __ECONOMIC_CODES__; //Premium
	m_mapFinnhubInquiryStringToIndex[_T("EconomicEconomic")] = __ECONOMIC_ECONOMIC__; //Premium
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