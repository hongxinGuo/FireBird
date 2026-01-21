#pragma once

// Finnhub申请类别和代码，免费账户无法申请Premium类的信息
enum {
	// Web Socket
	WEBSOCKET_TRADES_ = 1,
	WEBSOCKET_NEWS_, // Premium

	// Stock Fundamentals
	SYMBOL_LOOKUP_,
	STOCK_SYMBOLS_,
	MARKET_STATUS_,
	MARKET_HOLIDAY_,
	COMPANY_PROFILE_, //Premium
	COMPANY_PROFILE_CONCISE_,
	COMPANY_EXECUTIVE_, //Premium
	MARKET_NEWS_,
	COMPANY_NEWS_,
	PRESS_RELEASE_, //Premium
	NEWS_SENTIMENT_,
	PEERS_,
	BASIC_FINANCIALS_,
	OWNERSHIP_, //Premium
	FUND_OWNERSHIP_, //Premium
	INSIDER_TRANSACTION_,
	INSIDER_SENTIMENT_,
	FINANCIAL_, //Premium
	FINANCIAL_AS_REPORTED_,
	REVENUE_BREAKDOWN_,
	SEC_FILINGS_, // not implemented
	INTERNATIONAL_FILINGS_, //Premium
	SEC_SENTIMENT_ANALYSIS_, //Premium
	SIMILARITY_INDEX_, //Premium
	IPO_CALENDAR_,
	DIVIDENDS_, //Premium
	SECTOR_METRICS_, // Premium
	PRICE_METRICS_, // Premium
	SYMBOL_CHANGE_, // Premium
	ISIN_CHANGE_, // Premium

	// Stock Estimates
	STOCK_ESTIMATES_RECOMMENDATION_TRENDS_, // not implemented
	STOCK_ESTIMATES_PRICE_TARGET_, // Premium
	STOCK_ESTIMATES_UPGRADE_DOWNGRADE_, // Premium
	STOCK_ESTIMATES_REVENUE_ESTIMATES_, // Premium
	STOCK_ESTIMATES_EPS_ESTIMATES_, // Premium
	STOCK_ESTIMATES_EBITDA_ESTIMATES_, // Premium
	STOCK_ESTIMATES_EBIT_ESTIMATES_, // Premium
	STOCK_ESTIMATES_EPS_SURPRISE_,
	STOCK_ESTIMATES_EARNING_CALENDAR_,

	// Stock Price
	STOCK_PRICE_QUOTE_, // 实时数据优先级最低
	STOCK_PRICE_CANDLES_, // 历史数据优先级低
	STOCK_PRICE_TICK_DATA_, // Premium
	STOCK_PRICE_HISTORICAL_NBBO_, //Premium
	STOCK_PRICE_LAST_BID_ASK_, // Premium
	STOCK_PRICE_SPLITS_, // Premium
	STOCK_PRICE_DIVIDEND2_, //Premium

	// ETF & Indices
	ETFS_INDICES_CONSTITUENTS_, // Premium
	ETFS_HISTORY_CONSTITUENTS_, // Premium
	ETFS_PROFILE_, // Premium
	ETFS_HOLDINGS_, // Premium
	ETFS_SECTOR_, // Premium
	ETFS_COUNTRY_, //Premium

	// Mutual funds
	MUTUAL_FUND_PROFILE_, // Premium
	MUTUAL_FUND_HOLDINGS_, // Premium
	MUTUAL_FUND_SECTOR_, // Premium
	MUTUAL_FUND_COUNTRY_, // Premium
	MUTUAL_FUNDS_EET_, // Premium
	MUTUAL_FUNDS_EET_PAI_, // Premium

	// Bonds
	BOND_PROFILE_, // Premium
	BOND_PRICE_, // Premium
	BOND_TICK_TRADE_DATA_, // Premium
	BOND_YIELD_CURVE_, // Premium

	// Forex
	FOREX_EXCHANGE_,
	FOREX_SYMBOLS_,
	FOREX_CANDLES_, // Premium
	FOREX_ALL_RATES_, // Premium

	// Crypto
	CRYPTO_EXCHANGE_,
	CRYPTO_SYMBOLS_,
	CRYPTO_PROFILE_, // Premium
	CRYPTO_CANDLES_,

	// Technical Analysis
	TECHNICAL_ANALYSIS_PATTERN_RECOGNITION_, // Premium
	TECHNICAL_ANALYSIS_SUPPORT_RESISTANCE_, // Premium
	TECHNICAL_ANALYSIS_AGGREGATE_INDICATORS_, // Premium
	TECHNICAL_ANALYSIS_TECHNICAL_INDICATORS_,

	// Alternative Data
	ALTERNATIVE_DATA_TRANSCRIPT_LIST_, //Premium
	ALTERNATIVE_DATA_TRANSCRIPT_, //Premium
	ALTERNATIVE_DATA_SOCIAL_SENTIMENT_, // Premium
	ALTERNATIVE_DATA_INVEST_THEMES_, // Premium
	ALTERNATIVE_DATA_SUPPLY_CHAIN_, // Premium
	ALTERNATIVE_DATA_COMPANY_ESG_, // Premium
	ALTERNATIVE_DATA_EARNING_QUALITY_SCORE_, // Premium
	ALTERNATIVE_DATA_USPTO_PATENTS_,
	ALTERNATIVE_DATA_VISA_APPLICATION_,
	ALTERNATIVE_DATA_SENATE_LOBBYING_, // not implemented
	ALTERNATIVE_DATA_USA_SPENDING_, // not implemented
	ALTERNATIVE_DATA_CONGRESSIONAL_TRADING_, // Premium 
	ALTERNATIVE_DATA_COVID_19_,
	ALTERNATIVE_DATA_FDA_CALENDAR_,

	// Economic
	ECONOMIC_COUNTRY_LIST_,
	ECONOMIC_CALENDAR_, //Premium
	ECONOMIC_CODES_, //Premium
	ECONOMIC_ECONOMIC_,	//Premium

	// Global Filings Search

	// Tiingo.com
	TIINGO_FUNDAMENTAL_DEFINITION_,
	TIINGO_STOCK_DAILY_META_,
	TIINGO_FINANCIAL_STATEMENT_,
	TIINGO_IEX_TOP_OF_BOOK_,

	// Accessory product
	ACCESSORY_INDEX_NASDAQ100_STOCKS_,

	// 结束标志
	END_OF_ALL_INQUIRY_TYPE_
};

class CFinnhubInquiryType {
public:
	CFinnhubInquiryType();
	CFinnhubInquiryType const& operator=(CFinnhubInquiryType const&) = delete;
	CFinnhubInquiryType(CFinnhubInquiryType const&) = delete;
	CFinnhubInquiryType(CFinnhubInquiryType&&) noexcept = delete;
	CFinnhubInquiryType& operator=(CFinnhubInquiryType&&) noexcept = delete;
	~CFinnhubInquiryType() = default;

	string GetInquiryString(int lInquiryType);
	int GetInquiryType(const string& strInquiryString) const;

	void CreateFinnhubInquiryIndexToStringMap();
	void CreateFinnhubInquiryStringToIndexMap();

protected:
	map<int, string> m_mapFinnhubInquiryIndexToString;
	map<string, int> m_mapFinnhubInquiryStringToIndex;
};

extern CFinnhubInquiryType gl_FinnhubInquiryType;
