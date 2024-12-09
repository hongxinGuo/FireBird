#include "pch.h"

import FireBird.System.Message;

#include"FinnhubStock.h"
#include"ProductFinnhubCompanyProfile.h"
#include"ProductFinnhubCompanyProfileConcise.h"
#include"ProductFinnhubCompanyNews.h"
#include"ProductFinnhubCompanyPeer.h"
#include"ProductFinnhubCompanyBasicFinancial.h"
#include"ProductFinnhubCompanyInsiderTransaction.h"
#include"ProductFinnhubCompanyInsiderSentiment.h"
#include"ProductFinnhubCryptoDayLine.h"
#include"ProductFinnhubForexDayLine.h"
#include"ProductFinnhubStockDayLine.h"

#include"ProductFinnhubMarketStatus.h"
#include"ProductFinnhubMarketHoliday.h"

#include"ProductFinnhubStockPriceQuote.h"
#include"ProductFinnhubStockEstimatesEPSSurprise.h"

#include"ProductFinnhubStockSymbol.h"
#include"ProductFinnhubCryptoSymbol.h"
#include"ProductFinnhubForexSymbol.h"

#include"ProductFinnhubForexExchange.h"
#include"ProductFinnhubCryptoExchange.h"

#include"ProductFinnhubEconomicCountryList.h"
#include"ProductFinnhubEconomicCalendar.h"

import FireBird.WebProduct.Dummy;

import FireBird.WebProduct.Virtual;
#include"VirtualMarket.h"

#include "ProductFinnhubSECFilings.h"
module FireBird.Factory.Finnhub; 

import FireBird.FinnhubInquiryType;

CVirtualProductWebDataPtr CFinnhubFactory::CreateProduct(CVirtualMarketPtr pMarket, int iInquireType) {
	CVirtualProductWebDataPtr p = nullptr;

	switch (iInquireType) {
	case COMPANY_PROFILE_: // Premium 免费账户无法读取此信息，sandbox模式能读取，但数据是错误的，只能用于测试。
		p = make_shared<CProductFinnhubCompanyProfile>();
		break;
	case COMPANY_PROFILE_CONCISE_:
		p = make_shared<CProductFinnhubCompanyProfileConcise>();
		break;
	case STOCK_SYMBOLS_:
		p = make_shared<CProductFinnhubStockSymbol>();
		break;
	case MARKET_STATUS_:
		p = make_shared<CProductFinnhubMarketStatus>();
		break;
	case MARKET_HOLIDAY_:
		p = make_shared<CProductFinnhubMarketHoliday>();
		break;
	case COMPANY_EXECUTIVE_: // Premium
		break;
	case MARKET_NEWS_:
		break;
	case COMPANY_NEWS_:
		p = make_shared<CProductFinnhubCompanyNews>();
		break;
	case PRESS_RELEASE_: // Premium
		break;
	case NEWS_SENTIMENT_:
		break;
	case PEERS_:
		p = make_shared<CProductFinnhubCompanyPeer>();
		break;
	case BASIC_FINANCIALS_:
		p = make_shared<CProductFinnhubCompanyBasicFinancial>();
		break;
	case INSIDER_TRANSACTION_:
		p = make_shared<CProductFinnhubCompanyInsiderTransaction>();
		break;
	case INSIDER_SENTIMENT_:
		p = make_shared<CProductFinnhubCompanyInsiderSentiment>();
		break;
	case OWNERSHIP_: // Premium
		break;
	case FUND_OWNERSHIP_: // Premium
		break;
	case FINANCIAL_: // Premium
		break;
	case FINANCIAL_AS_REPORTED_:
		break;
	case SEC_FILINGS_:
		p = make_shared<CProductFinnhubSECFilings>();
		break;
	case INTERNATIONAL_FILINGS_: // Premium
		break;
	case SEC_SENTIMENT_ANALYSIS_: // Premium
		break;
	case SIMILARITY_INDEX_: // Premium
		break;
	case IPO_CALENDAR_:
		break;
	case DIVIDENDS_: // Premium
		break;
	case STOCK_ESTIMATES_RECOMMENDATION_TRENDS_:
		break;
	case STOCK_ESTIMATES_PRICE_TARGET_:
		break;
	case STOCK_ESTIMATES_UPGRADE_DOWNGRADE_: // Premium
		break;
	case STOCK_ESTIMATES_REVENUE_ESTIMATES_: // Premium
		break;
	case STOCK_ESTIMATES_EPS_ESTIMATES_:// Premium
		break;
	case STOCK_ESTIMATES_EPS_SURPRISE_:
		p = make_shared<CProductFinnhubStockEstimatesEPSSurprise>();
		break;
	case STOCK_ESTIMATES_EARNING_CALENDAR_:
		break;
	case STOCK_PRICE_QUOTE_:
		p = make_shared<CProductFinnhubStockPriceQuote>();
		break;
	case STOCK_PRICE_CANDLES_:
		p = make_shared<CProductFinnhubStockDayLine>();
		break;
	case STOCK_PRICE_TICK_DATA_: // Premium
		break;
	case STOCK_PRICE_LAST_BID_ASK_: // Premium
		break;
	case STOCK_PRICE_SPLITS_:
		break;
	case FOREX_EXCHANGE_:
		p = make_shared<CProductFinnhubForexExchange>();
		break;
	case FOREX_SYMBOLS_:
		p = make_shared<CProductFinnhubForexSymbol>();
		break;
	case FOREX_CANDLES_:
		p = make_shared<CProductFinnhubForexDayLine>();
		break;
	case FOREX_ALL_RATES_:
		break;
	case CRYPTO_EXCHANGE_:
		p = make_shared<CProductFinnhubCryptoExchange>();
		break;
	case CRYPTO_SYMBOLS_:
		p = make_shared<CProductFinnhubCryptoSymbol>();
		break;
	case CRYPTO_CANDLES_:
		p = make_shared<CProductFinnhubCryptoDayLine>();
		break;
	case ECONOMIC_COUNTRY_LIST_:
		p = make_shared<CProductFinnhubEconomicCountryList>();
		break;
	case ECONOMIC_CALENDAR_:
		p = make_shared<CProductFinnhubEconomicCalendar>();
		break;
	default:
		p = make_shared<CProductDummy>();
		TRACE("未处理指令%d\n", iInquireType);
		gl_systemMessage.PushErrorMessage(_T("Finnhub product未实现"));
		break;
	}
	p->SetMarket(pMarket);
	p->SetInquireType(iInquireType);

	return p;
}
