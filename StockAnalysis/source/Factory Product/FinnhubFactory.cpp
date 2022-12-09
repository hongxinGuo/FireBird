#include "pch.h"

#include"WorldStock.h"
#include"FinnhubInquiryType.h"

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

#include"ProductFinnhubStockPriceQuote.h"
#include"ProductFinnhubStockEstimatesEPSSurprise.h"

#include"ProductFinnhubStockSymbol.h"
#include"ProductFinnhubCryptoSymbol.h"
#include"ProductFinnhubForexSymbol.h"

#include"ProductFinnhubForexExchange.h"
#include"ProductFinnhubCryptoExchange.h"

#include"ProductFinnhubEconomicCountryList.h"
#include"ProductFinnhubEconomicCalendar.h"

#include"ProductDummy.h"

#include "FinnhubFactory.h"

CVirtualProductWebDataPtr CFinnhubFactory::CreateProduct(CVirtualMarket* pMarket, int iIndex) {
	CVirtualProductWebDataPtr p = nullptr;

	switch (iIndex) {
	case _COMPANY_PROFILE_: // Premium 免费账户无法读取此信息，sandbox模式能读取，但数据是错误的，只能用于测试。
		p = make_shared<CProductFinnhubCompanyProfile>();
		break;
	case _COMPANY_PROFILE_CONCISE_:
		p = make_shared<CProductFinnhubCompanyProfileConcise>();
		break;
	case  _STOCK_SYMBOLS_:
		p = make_shared<CProductFinnhubStockSymbol>();
		break;
	case _COMPANY_EXECTIVE_: // Premium
		break;
	case _MARKET_NEWS_:
		break;
	case _COMPANY_NEWS_:
		p = make_shared<CProductFinnhubCompanyNews>();
		break;
	case _PRESS_RELEASE_: // Premium
		break;
	case _NEWS_SENTIMENT_:
		break;
	case _PEERS_:
		p = make_shared<CProductFinnhubCompanyPeer>();
		break;
	case _BASIC_FINANCIALS_:
		p = make_shared<CProductFinnhubCompanyBasicFinancial>();
		break;
	case _INSIDER_TRANSACTION_:
		p = make_shared<CProductFinnhubCompanyInsiderTransaction>();
		break;
	case _INSIDER_SENTIMENT_:
		p = make_shared<CProductFinnhubCompanyInsiderSentiment>();
		break;
	case _OWNERSHIP_: // Premium
		break;
	case _FUND_OWNERSHIP_: // jPremium
		break;
	case _FINANCIAL_: // Premium
		break;
	case _FINAICIAL_AS_REPORTED_:
		break;
	case _SEC_FILINGS_:
		break;
	case _INTERNATIONAL_FILINGS_: // Premium
		break;
	case _SEC_SENTIMENT_ANALYSIS_: // Premium
		break;
	case _SIMILARITY_INDEX_: // Premium
		break;
	case _IPO_CALENDAR_:
		break;
	case _DIVIDENDS_: // Premium
		break;
	case _STOCK_ESTIMATES_RECOMMENDATION_TRENDS_:
		break;
	case _STOCK_ESTIMATES_PRICE_TARGET_:
		break;
	case _STOCK_ESTIMATES_UPGRADE_DOWNGRADE_: // Premium
		break;
	case _STOCK_ESTIMATES_REVENUE_EXTIMATES_: // Premium
		break;
	case _STOCK_ESTIMATES_EPS_EXTIMATES_:// Premium
		break;
	case _STOCK_ESTIMATES_EPS_SURPRISE_:
		p = make_shared<CProductFinnhubStockEstimatesEPSSurprise>();
		break;
	case _STOCK_ESTIMATES_EARNING_CALENDAR_:
		break;
	case _STOCK_PRICE_QUOTE_:
		p = make_shared<CProductFinnhubStockPriceQuote>();
		break;
	case _STOCK_PRICE_CANDLES_:
		p = make_shared<CProductFinnhubStockDayLine>();
		break;
	case _STOCK_PRICE_TICK_DATA_: // Premium
		break;
	case _STOCK_PRICE_LAST_BID_ASK_: // Premium
		break;
	case _STOCK_PRICE_SPLITS_:
		break;
	case _FOREX_EXCHANGE_:
		p = make_shared<CProductFinnhubForexExchange>();
		break;
	case _FOREX_SYMBOLS_:
		p = make_shared<CProductFinnhubForexSymbol>();
		break;
	case _FOREX_CANDLES_:
		p = make_shared<CProductFinnhubForexDayLine>();
		break;
	case _FOREX_ALL_RATES_:
		break;
	case _CRYPTO_EXCHANGE_:
		p = make_shared<CProductFinnhubCryptoExchange>();
		break;
	case _CRYPTO_SYMBOLS_:
		p = make_shared<CProductFinnhubCryptoSymbol>();
		break;
	case _CRYPTO_CANDLES_:
		p = make_shared<CProductFinnhubCryptoDayLine>();
		break;
	case _ECONOMIC_COUNTRY_LIST_:
		p = make_shared<CProductFinnhubEconomicCountryList>();
		break;
	case _ECONOMIC_CALENDAR_:
		p = make_shared<CProductFinnhubEconomicCalendar>();
		break;
	default:
		p = make_shared<CProductDummy>();
		TRACE("未处理指令%d\n", iIndex);
		gl_systemMessage.PushErrorMessage(_T("Finnhub product未实现"));
		break;
	}
	p->SetMarket(pMarket);
	p->SetProductType(iIndex);

	return p;
}