#include "pch.h"

#include"FinnhubInquiryType.h"

#include "TiingoFactory.h"

#include"ProductTiingoCompanyProfile.h"
#include"ProductTiingoStockSymbol.h"
#include"ProductTiingoStockDayLine.h"

#include"ProductTiingoCryptoSymbol.h"

#include"ProductDummy.h"

CVirtualProductWebDataPtr CTiingoFactory::CreateProduct(CVirtualMarket* pMarket, int iIndex) {
	CVirtualProductWebDataPtr p = nullptr;

	switch (iIndex) {
	case _STOCK_SYMBOLS_:
		p = make_shared<CProductTiingoStockSymbol>();
		break;
	case _STOCK_PRICE_CANDLES_:
		p = make_shared<CProductTiingoStockDayLine>();
		break;
	case _COMPANY_PROFILE_:
	case _COMPANY_PROFILE_CONCISE_:
	case _COMPANY_EXECTIVE_: // Premium
	case _MARKET_NEWS_:
	case _COMPANY_NEWS_:
	case _PRESS_RELEASE_: // Premium
	case _NEWS_SENTIMENT_:
	case _PEERS_:
	case _BASIC_FINANCIALS_:
	case _INSIDER_TRANSACTION_:
	case _OWNERSHIP_: // Premium
	case _FUND_OWNERSHIP_: // jPremium
	case _FINANCIAL_: // Premium
	case _FINANCIAL_AS_REPORTED_:
	case _SEC_FILINGS_:
	case _INTERNATIONAL_FILINGS_: // Premium
	case _SEC_SENTIMENT_ANALYSIS_: // Premium
	case _SIMILARITY_INDEX_: // Premium
	case _IPO_CALENDAR_:
	case _DIVIDENDS_: // Premium
	case _STOCK_ESTIMATES_RECOMMENDATION_TRENDS_:
	case _STOCK_ESTIMATES_PRICE_TARGET_:
	case _STOCK_ESTIMATES_UPGRADE_DOWNGRADE_: // Premium
	case _STOCK_ESTIMATES_REVENUE_EXTIMATES_: // Premium
	case _STOCK_ESTIMATES_EPS_EXTIMATES_: // Premium
	case _STOCK_ESTIMATES_EPS_SURPRISE_:
	case _STOCK_ESTIMATES_EARNING_CALENDAR_:
	case _STOCK_PRICE_QUOTE_:
	case _STOCK_PRICE_TICK_DATA_: // Premium
	case _STOCK_PRICE_LAST_BID_ASK_: // Premium
	case _STOCK_PRICE_SPLITS_:
	case _FOREX_EXCHANGE_:
	case _FOREX_SYMBOLS_:
	case _FOREX_CANDLES_:
	case _FOREX_ALL_RATES_:
	case _CRYPTO_EXCHANGE_:
	case _CRYPTO_SYMBOLS_:
		p = make_shared<CProductTiingoCryptoSymbol>();
		break;
	case _CRYPTO_CANDLES_:
	case _ECONOMIC_COUNTRY_LIST_:
	case _ECONOMIC_CALENDAR_:
	default:
		// not impliment yet.
		p = make_shared<CProductDummy>();
		gl_systemMessage.PushErrorMessage(_T("Tiingo productÎ´ÊµÏÖ"));
		break;
	}
	p->SetMarket(pMarket);
	p->SetProductType(iIndex);

	return p;
}
