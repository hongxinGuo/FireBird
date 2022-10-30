#include"pch.h"

#include"WorldMarket.h"
#include"ChinaMarket.h"

#include"GeneralCheck.h"

#include"FinnhubInquiryType.h"

#include"FinnhubInaccessibleExchange.h"
#include"nlohmannJsonDeclaration.h" // ����˳�����json������ʹ�ô�ordered_json,�Ա�֤����������������ǰ��˳��һ�¡�

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
		EXPECT_TRUE(inaccessibleExchange.HaveExchange(_T("US"))) << "Ĭ�ϰ���US";
		EXPECT_TRUE(inaccessibleExchange.HaveExchange(_T("SZ"))) << "Ĭ�ϰ���US";
		EXPECT_TRUE(inaccessibleExchange.HaveExchange(_T("SS"))) << "Ĭ�ϰ���US";
		EXPECT_FALSE(inaccessibleExchange.HaveExchange(_T("FA"))) << "Ĭ�ϲ�����FA";
		vector<CString> v{ _T("US2"), _T("SZ2"), _T("SS2") };
		inaccessibleExchange.Assign(_T("Another"), 2, v);

		EXPECT_TRUE(inaccessibleExchange.HaveExchange(_T("US2")));
		EXPECT_TRUE(inaccessibleExchange.HaveExchange(_T("SZ2")));
		EXPECT_TRUE(inaccessibleExchange.HaveExchange(_T("SS2")));
		EXPECT_FALSE(inaccessibleExchange.HaveExchange(_T("US")));
		EXPECT_FALSE(inaccessibleExchange.HaveExchange(_T("SZ")));
		EXPECT_FALSE(inaccessibleExchange.HaveExchange(_T("SS")));
	}

	TEST_F(CInaccessibleExchangesTest, TestAddExchange) {
		EXPECT_FALSE(inaccessibleExchange.HaveExchange(_T("FA"))) << "Ĭ�ϲ�����FA";
		inaccessibleExchange.AddExchange(_T("FA"));
		EXPECT_TRUE(inaccessibleExchange.HaveExchange(_T("FA"))) << "��Ӻ�Ͱ���FA��";
	}

	TEST_F(CInaccessibleExchangesTest, TestDeleteExchange) {
		EXPECT_TRUE(inaccessibleExchange.HaveExchange(_T("US"))) << "Ĭ�ϰ���US";
		inaccessibleExchange.DeleteExchange(_T("US"));
		EXPECT_FALSE(inaccessibleExchange.HaveExchange(_T("US"))) << "ɾ����Ͳ�����US��";
	}

	TEST_F(CInaccessibleExchangesTest, TestHaveExchange) {
		EXPECT_TRUE(inaccessibleExchange.HaveExchange(_T("US"))) << "Ĭ�ϰ���US";
		EXPECT_TRUE(inaccessibleExchange.HaveExchange(_T("SZ"))) << "Ĭ�ϰ���US";
		EXPECT_TRUE(inaccessibleExchange.HaveExchange(_T("SS"))) << "Ĭ�ϰ���US";
		EXPECT_FALSE(inaccessibleExchange.HaveExchange(_T("FA"))) << "Ĭ�ϲ�����FA";
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
		for (int i = __WEBSOCKET__TRADES__; i <= __WEBSOCKET__NEWS__; i++) {
			s = gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(i);
			EXPECT_TRUE(s.size() > 0) << "ȷ�����е��������ж�Ӧ���ַ���";
		}
		for (int i = __SYMBOL_LOOKUP__; i <= __DIVIDENDS__; i++) {
			s = gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(i);
			EXPECT_TRUE(s.size() > 0) << "ȷ�����е��������ж�Ӧ���ַ���";
		}
		for (int i = __STOCK_ESTIMATES_RECOMMENDATION_TRENDS__; i <= __STOCK_ESTIMATES_EARNING_CALENDAR__; i++) {
			s = gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(i);
			EXPECT_TRUE(s.size() > 0) << "ȷ�����е��������ж�Ӧ���ַ���";
		}
		for (int i = __STOCK_PRICE_QUOTE__; i <= __STOCK_PRICE_DIVIDEND2__; i++) {
			s = gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(i);
			EXPECT_TRUE(s.size() > 0) << "ȷ�����е��������ж�Ӧ���ַ���";
		}
	}

	TEST_F(CFinnhubInaccessibleExchangeTest, TestGetFinnhubInquiryString2) {
		// Web Socket
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__WEBSOCKET__TRADES__), _T("WebSocketTrades"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__WEBSOCKET__NEWS__), _T("WebSocketNews"));// Premium

		// Stock Fundamentals
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__SYMBOL_LOOKUP__), _T("StockFundamentalsSymolLookup"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__STOCK_SYMBOLS__), _T("StockFundamentalsSymbols"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__COMPANY_PROFILE__), _T("StockFundamentalsCompanyProfile")); //Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__COMPANY_PROFILE_CONCISE__), _T("StockFundamentalsCompanyProfileConcise"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__COMPANY_EXECTIVE__), _T("StockFundamentalsCompanyExective")); //Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__MARKET_NEWS__), _T("StockFundamentalsMarketNews"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__COMPANY_NEWS__), _T("StockFundamentalsCompanyNews"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__PRESS_RELEASE__), _T("StockFundamentalsPressRelease")); //Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__NEWS_SENTIMENT__), _T("StockFundamentalsNewsSentiment"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__PEERS__), _T("StockFundamentalsPeer"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__BASIC_FINANCIALS__), _T("StockFundamentalsBasicFinancials"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__OWNERSHIP__), _T("StockFundamentalsOwnership")); //Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__FUND_OWNERSHIP__), _T("StockFundamentalsFundOwnership")); //Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__INSIDER_TRANSACTION__), _T("StockFundamentalsInsiderTransaction"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__INSIDER_SENTIMENT__), _T("StockFundamentalsInsiderSentiment"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__FINANCIAL__), _T("StockFundamentalsFiniacial")); //Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__FINAICIAL_AS_REPORTED__), _T("StockFundamentalsFinaicialAsReported"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__REVENUE_BREAKDOWN__), _T("StockFundamentalsRevenueBreakdown"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__SEC_FILINGS__), _T("StockFundamentalsSECFilings"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__INTERNATIONAL_FILINGS__), _T("StockFundamentalsInternationalFilings")); //Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__SEC_SENTIMENT_ANALYSIS__), _T("StockFundamentalsSECSentimentAnalysis")); //Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__SIMILARITY_INDEX__), _T("StockFundamentalsSemilarityIndex")); //Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__IPO_CALENDAR__), _T("StockFundamentalsIPOCanlendar"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__DIVIDENDS__), _T("StockFundamentalsDividends")); //Premium

		// Stock Estimates
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__STOCK_ESTIMATES_RECOMMENDATION_TRENDS__), _T("StockExtimatesRecommendationTrend"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__STOCK_ESTIMATES_PRICE_TARGET__), _T("StockExtimatesPriceTarget"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__STOCK_ESTIMATES_UPGRADE_DOWNGRADE__), _T("StockExtimatesUpgradeDownGrade")); // Primium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__STOCK_ESTIMATES_REVENUE_EXTIMATES__), _T("StockExtimatesRevenueExtimates")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__STOCK_ESTIMATES_EPS_EXTIMATES__), _T("StockExtimatesEPSExtimates")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__STOCK_EXTIMATES_EBITDA_EXTIMATES__), _T("StockExtimatesEBITDAExtimates")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__STOCK_EXTIMATES_EBIT_EXTIMATES__), _T("StockExtimatesEBITExtimates")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__STOCK_ESTIMATES_EPS_SURPRISE__), _T("StockExtimatesEPSSurprise"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__STOCK_ESTIMATES_EARNING_CALENDAR__), _T("StockExtimatesEarningCalendar"));

		// Stock Price
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__STOCK_PRICE_QUOTE__), _T("StockPriceQuote")); // ʵʱ�������ȼ����
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__STOCK_PRICE_CANDLES__), _T("StockPriceCandles")); // ��ʷ�������ȼ���
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__STOCK_PRICE_TICK_DATA__), _T("StockPriceTickData")); //Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__STOCK_PRICE_HISTORICAL_NBBO__), _T("StockPriceHistoricalNBBO")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__STOCK_PRICE_LAST_BID_ASK__), _T("StockPriceLastBidAsk")); //Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__STOCK_PRICE_SPLITS__), _T("StockPriceSplits")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__STOCK_PRICE_DIVIDEND2__), _T("StockPriceDividend")); // Premium

		// ETF & Indices
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__ETFS_INDICES_CONSTITUENTS__), _T("ETFSIndicesConstituents"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__ETFS_HISTORY_CONSTITUENTS__), _T("ETFSIndicesHistoryConstitunets")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__ETFS_PROFILE__), _T("ETFSIndicesProfile")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__ETFS_HOLDINGS__), _T("ETFSIndicesHoldings")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__ETFS_SECTOR__), _T("ETFSIndicesSector")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__ETFS_COUNTRY__), _T("ETFSIndicesCountry")); //Premium

		// Mutual funds
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__MUTUAL_FUND_PROFILE__), _T("MutualFundProfile")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__MUTUAL_FUND_HOLDINGS__), _T("MutualFundHoldings"));// Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__MUTUAL_FUND_SECTOR__), _T("MutualFundSector")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__MUTUAL_FUND_COUNTRY__), _T("MutualFundCountry")); // Premium

		// Bonds
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__BOND_PROFILE__), _T("BondProfile")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__BOND_PRICE__), _T("BondPrice")); // Premium

		// Forex
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__FOREX_EXCHANGE__), _T("ForexExchange"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__FOREX_SYMBOLS__), _T("ForexSymbols"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__FOREX_CANDLES__), _T("ForexCandles")); // ��ʷ�������ȼ��� Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__FOREX_ALL_RATES__), _T("ForexAllRates"));  // Premium

		// Crypto
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__CRYPTO_EXCHANGE__), _T("CryptoExchange"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__CRYPTO_SYMBOLS__), _T("CryptoSymbols"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__CRYPTO_PROFILE__), _T("CryptoProfile")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__CRYPTO_CANDLES__), _T("CryptoCandles"));

		// Technical Analysis
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__TECHNICAL_ANALYSIS_PATTERN_RECOGNITION__), _T("TechnicalAnalysisPatternRecognition")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__TECHNICAL_ANALYSIS_SURPORT_RESISTANCE__), _T("TechnicalAnalysisSurportResistance")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__TECHNICAL_ANALYSIS_AGGREGATE_INDICATORS__), _T("TechnicalAnalysisAggregateIndicators")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__TECHNICAL_ANALYSIS_TECHNICAL_INDICATORS__), _T("TechnicalAnalysisTechnicalIndicators"));

		// Alternative Data
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__ALTERNATIVE_DATA_TRANSCRIPT_LIST__), _T("AlternativeDataTranscriptList")); //Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__ALTERNATIVE_DATA_TRANSCRIPT__), _T("AlternativeDataTranscript")); //Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__ALTERNATIVE_DATA_SOCIAL_SETIMENT__), _T("AlternativeDataSetiment"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__ALTERNATIVE_DATA_INVEST_THEMES__), _T("AlternativeDataInvestThemes")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__ALTERNATIVE_DATA_SUPPLY_CHAIN__), _T("AlternativeDataSupplyChain"));  // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__ALTERNATIVE_DATA_COMPANY_ESG__), _T("AlternativeDataCompanyESG")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__ALTERNATIVE_DATA_EARNING_QUALITY_SCORE__), _T("AlternativeDataQualityScore")); // Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__ALTERNATIVE_DATA_USPTO_PATENTS__), _T("AlternativeDataUSPTOpatents"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__ALTERNATIVE_DATA_VISA_APPLICATION__), _T("AlternativeDataVISAApplication")); //
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__ALTERNATIVE_DATA_COVID_19__), _T("AlternativeDataCOVID_19"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__ALTERNATIVE_DATA_FDA_CALENDAR__), _T("AlternativeDataFDACalendar"));

		// Econimic
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__ECONOMIC_COUNTRY_LIST__), _T("EconomicCountryList"));
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__ECONOMIC_CALENDAR__), _T("EconomicCalendar")); //Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__ECONOMIC_CODES__), _T("EconomicCodes")); //Premium
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryString(__ECONOMIC_ECONOMIC__), _T("EconomicEconomic")); //Premium
	}

	TEST_F(CFinnhubInaccessibleExchangeTest, TestGetFinnhubInquiryIndex) {
		// Web Socket
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("WebSocketTrades")), __WEBSOCKET__TRADES__);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("WebSocketNews")), __WEBSOCKET__NEWS__);// Premium

		// Stock Fundamentals
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockFundamentalsSymolLookup")), __SYMBOL_LOOKUP__);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockFundamentalsSymbols")), __STOCK_SYMBOLS__);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockFundamentalsCompanyProfile")), __COMPANY_PROFILE__); //Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockFundamentalsCompanyProfileConcise")), __COMPANY_PROFILE_CONCISE__);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockFundamentalsCompanyExective")), __COMPANY_EXECTIVE__); //Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockFundamentalsMarketNews")), __MARKET_NEWS__);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockFundamentalsCompanyNews")), __COMPANY_NEWS__);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockFundamentalsPressRelease")), __PRESS_RELEASE__); //Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockFundamentalsNewsSentiment")), __NEWS_SENTIMENT__);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockFundamentalsPeer")), __PEERS__);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockFundamentalsBasicFinancials")), __BASIC_FINANCIALS__);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockFundamentalsOwnership")), __OWNERSHIP__); //Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockFundamentalsFundOwnership")), __FUND_OWNERSHIP__); //Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockFundamentalsInsiderTransaction")), __INSIDER_TRANSACTION__);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockFundamentalsInsiderSentiment")), __INSIDER_SENTIMENT__);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockFundamentalsFiniacial")), __FINANCIAL__); //Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockFundamentalsFinaicialAsReported")), __FINAICIAL_AS_REPORTED__);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockFundamentalsRevenueBreakdown")), __REVENUE_BREAKDOWN__);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockFundamentalsSECFilings")), __SEC_FILINGS__);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockFundamentalsInternationalFilings")), __INTERNATIONAL_FILINGS__); //Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockFundamentalsSECSentimentAnalysis")), __SEC_SENTIMENT_ANALYSIS__); //Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockFundamentalsSemilarityIndex")), __SIMILARITY_INDEX__); //Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockFundamentalsIPOCanlendar")), __IPO_CALENDAR__);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockFundamentalsDividends")), __DIVIDENDS__); //Premium

		// Stock Estimates
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockExtimatesRecommendationTrend")), __STOCK_ESTIMATES_RECOMMENDATION_TRENDS__);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockExtimatesPriceTarget")), __STOCK_ESTIMATES_PRICE_TARGET__);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockExtimatesUpgradeDownGrade")), __STOCK_ESTIMATES_UPGRADE_DOWNGRADE__); // Primium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockExtimatesRevenueExtimates")), __STOCK_ESTIMATES_REVENUE_EXTIMATES__); // Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockExtimatesEPSExtimates")), __STOCK_ESTIMATES_EPS_EXTIMATES__); // Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockExtimatesEBITDAExtimates")), __STOCK_EXTIMATES_EBITDA_EXTIMATES__); // Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockExtimatesEBITExtimates")), __STOCK_EXTIMATES_EBIT_EXTIMATES__); // Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockExtimatesEPSSurprise")), __STOCK_ESTIMATES_EPS_SURPRISE__);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockExtimatesEarningCalendar")), __STOCK_ESTIMATES_EARNING_CALENDAR__);

		// Stock Price
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockPriceQuote")), __STOCK_PRICE_QUOTE__); // ʵʱ�������ȼ����
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockPriceCandles")), __STOCK_PRICE_CANDLES__); // ��ʷ�������ȼ���
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockPriceTickData")), __STOCK_PRICE_TICK_DATA__); //Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockPriceHistoricalNBBO")), __STOCK_PRICE_HISTORICAL_NBBO__); // Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockPriceLastBidAsk")), __STOCK_PRICE_LAST_BID_ASK__); //Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockPriceSplits")), __STOCK_PRICE_SPLITS__); // Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("StockPriceDividend")), __STOCK_PRICE_DIVIDEND2__); // Premium

		// ETF & Indices
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("ETFSIndicesConstituents")), __ETFS_INDICES_CONSTITUENTS__);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("ETFSIndicesHistoryConstitunets")), __ETFS_HISTORY_CONSTITUENTS__); // Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("ETFSIndicesProfile")), __ETFS_PROFILE__); // Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("ETFSIndicesHoldings")), __ETFS_HOLDINGS__); // Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("ETFSIndicesSector")), __ETFS_SECTOR__); // Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("ETFSIndicesCountry")), __ETFS_COUNTRY__); //Premium

		// Mutual funds
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("MutualFundProfile")), __MUTUAL_FUND_PROFILE__); // Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("MutualFundHoldings")), __MUTUAL_FUND_HOLDINGS__);// Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("MutualFundSector")), __MUTUAL_FUND_SECTOR__); // Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("MutualFundCountry")), __MUTUAL_FUND_COUNTRY__); // Premium

		// Bonds
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("BondProfile")), __BOND_PROFILE__); // Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("BondPrice")), __BOND_PRICE__); // Premium

		// Forex
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("ForexExchange")), __FOREX_EXCHANGE__);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("ForexSymbols")), __FOREX_SYMBOLS__);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("ForexCandles")), __FOREX_CANDLES__); // ��ʷ�������ȼ��� Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("ForexAllRates")), __FOREX_ALL_RATES__);  // Premium

		// Crypto
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("CryptoExchange")), __CRYPTO_EXCHANGE__);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("CryptoSymbols")), __CRYPTO_SYMBOLS__);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("CryptoProfile")), __CRYPTO_PROFILE__); // Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("CryptoCandles")), __CRYPTO_CANDLES__);

		// Technical Analysis
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("TechnicalAnalysisPatternRecognition")), __TECHNICAL_ANALYSIS_PATTERN_RECOGNITION__); // Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("TechnicalAnalysisSurportResistance")), __TECHNICAL_ANALYSIS_SURPORT_RESISTANCE__); // Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("TechnicalAnalysisAggregateIndicators")), __TECHNICAL_ANALYSIS_AGGREGATE_INDICATORS__); // Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("TechnicalAnalysisTechnicalIndicators")), __TECHNICAL_ANALYSIS_TECHNICAL_INDICATORS__);

		// Alternative Data
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("AlternativeDataTranscriptList")), __ALTERNATIVE_DATA_TRANSCRIPT_LIST__); //Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("AlternativeDataTranscript")), __ALTERNATIVE_DATA_TRANSCRIPT__); //Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("AlternativeDataSetiment")), __ALTERNATIVE_DATA_SOCIAL_SETIMENT__);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("AlternativeDataInvestThemes")), __ALTERNATIVE_DATA_INVEST_THEMES__); // Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("AlternativeDataSupplyChain")), __ALTERNATIVE_DATA_SUPPLY_CHAIN__);  // Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("AlternativeDataCompanyESG")), __ALTERNATIVE_DATA_COMPANY_ESG__); // Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("AlternativeDataQualityScore")), __ALTERNATIVE_DATA_EARNING_QUALITY_SCORE__); // Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("AlternativeDataUSPTOpatents")), __ALTERNATIVE_DATA_USPTO_PATENTS__);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("AlternativeDataVISAApplication")), __ALTERNATIVE_DATA_VISA_APPLICATION__); //
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("AlternativeDataCOVID_19")), __ALTERNATIVE_DATA_COVID_19__);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("AlternativeDataFDACalendar")), __ALTERNATIVE_DATA_FDA_CALENDAR__);

		// Econimic
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("EconomicCountryList")), __ECONOMIC_COUNTRY_LIST__);
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("EconomicCalendar")), __ECONOMIC_CALENDAR__); //Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("EconomicCodes")), __ECONOMIC_CODES__); //Premium
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(_T("EconomicEconomic")), __ECONOMIC_ECONOMIC__); //Premium
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
		EXPECT_STREQ(exchange.GetExchange(0), _T("US"));
		EXPECT_STREQ(exchange.GetExchange(1), _T("SS"));
		EXPECT_STREQ(exchange.GetExchange(2), _T("SZ"));
	}

	TEST_F(CFinnhubInaccessibleExchangeTest, TestSaveDB) {
		CInaccessibleExchangesPtr pExchange = make_shared<CInaccessibleExchanges>();
		pExchange->SetFunctionString(_T("WebSocketTrades"));
		pExchange->AddExchange(_T("US"));
		pExchange->AddExchange(_T("SS"));
		pExchange->AddExchange(_T("SZ"));

		DeleteFile(gl_systemConfigeration.GetDefaultFileDirectory() + _T("FinnhubInaccessibleExchangeTest.json"));
		gl_finnhubInaccessibleExchange.SetDefaultFileName(_T("FinnhubInaccessibleExchangeTest.json"));

		gl_finnhubInaccessibleExchange.SetInaccessibleExchange(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(pExchange->GetFunctionString()), pExchange);
		gl_finnhubInaccessibleExchange.UpdateJson();
		gl_finnhubInaccessibleExchange.SaveDB();

		gl_finnhubInaccessibleExchange.Clear();
		gl_finnhubInaccessibleExchange.LoadDB();
		gl_finnhubInaccessibleExchange.Update();
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetInaccessibleExchangeSize(), 1);
		EXPECT_STREQ(gl_finnhubInaccessibleExchange.GetInaccessibleExchange(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(pExchange->GetFunctionString()))->GetFunctionString(), _T("WebSocketTrades"));
		EXPECT_EQ(gl_finnhubInaccessibleExchange.GetInaccessibleExchange(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(pExchange->GetFunctionString()))->ExchangeSize(), 3);
		CString str;
		str = gl_finnhubInaccessibleExchange.GetInaccessibleExchange(gl_finnhubInaccessibleExchange.GetFinnhubInquiryIndex(pExchange->GetFunctionString()))->GetExchange(0);
		EXPECT_STREQ(str, _T("US"));

		// �ָ�ԭ״
		DeleteFile(gl_systemConfigeration.GetDefaultFileDirectory() + _T("FinnhubInaccessibleExchangeTest.json"));
		gl_finnhubInaccessibleExchange.SetDefaultFileName(_T("FinnhubInaccessibleExchange.json"));
	}
}