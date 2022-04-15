#include "pch.h"

#include"globedef.h"
#include"WorldStock.h"
#include"FinnhubInquiryType.h"

#include"ProductFinnhubCompanyProfile.h"
#include"ProductFinnhubCompanyProfileConcise.h"
#include"ProductFinnhubCompanyPeer.h"
#include"ProductFinnhubCompanyBasicFinancial.h"
#include"ProductFinnhubCompanyInsiderTransaction.h"
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

#include "FinnhubFactory.h"

CWebSourceDataProductPtr CFinnhubFactory::CreateProduct(CVirtualMarket* pMarket, int iIndex) {
	CWebSourceDataProductPtr p = nullptr;

	switch (iIndex) {
	case __COMPANY_PROFILE__: // Premium 免费账户无法读取此信息，sandbox模式能读取，但数据是错误的，只能用于测试。
		p = make_shared<CProductFinnhubCompanyProfile>();
		break;
	case __COMPANY_PROFILE_CONCISE__:
		p = make_shared<CProductFinnhubCompanyProfileConcise>();
		break;
	case  __STOCK_SYMBOLS__:
		p = make_shared<CProductFinnhubStockSymbol>();
		break;
	case __COMPANY_EXECTIVE__: // Premium
		break;
	case __MARKET_NEWS__:
		break;
	case __COMPANY_NEWS__:
		break;
	case __PRESS_RELEASE__: // Premium
		break;
	case __NEWS_SENTIMENT__:
		break;
	case __PEERS__:
		p = make_shared<CProductFinnhubCompanyPeer>();
		break;
	case __BASIC_FINANCIALS__:
		p = make_shared<CProductFinnhubCompanyBasicFinancial>();
		break;
	case __INSIDER_TRANSACTION__:
		p = make_shared<CProductFinnhubCompanyInsiderTransaction>();
		break;
	case __OWNERSHIP__: // Premium
		break;
	case __FUND_OWNERSHIP__: // jPremium
		break;
	case __FINANCIAL__: // Premium
		break;
	case __FINAICIAL_AS_REPORTED__:
		break;
	case __SEC_FILINGS__:
		break;
	case __INTERNATIONAL_FILINGS__: // Premium
		break;
	case __SEC_SENTIMENT_ANALYSIS__: // Premium
		break;
	case __SIMILARITY_INDEX__: // Premium
		break;
	case __IPO_CALENDAR__:
		break;
	case __DIVIDENDS__: // Premium
		break;
	case __STOCK_ESTIMATES_RECOMMENDATION_TRENDS__:
		break;
	case __STOCK_ESTIMATES_PRICE_TARGET__:
		break;
	case __STOCK_ESTIMATES_UPGRADE_DOWNGRADE__: // Premium
		break;
	case __STOCK_ESTIMATES_REVENUE_EXTIMATES__: // Premium
		break;
	case __STOCK_ESTIMATES_EPS_EXTIMATES__:// Premium
		break;
	case __STOCK_ESTIMATES_EPS_SURPRISE__:
		p = make_shared<CProductFinnhubStockEstimatesEPSSurprise>();
		break;
	case __STOCK_ESTIMATES_EARNING_CALENDAR__:
		break;
	case __STOCK_PRICE_QUOTE__:
		p = make_shared<CProductFinnhubStockPriceQuote>();
		break;
	case __STOCK_PRICE_CANDLES__:
		p = make_shared<CProductFinnhubStockDayLine>();
		break;
	case __STOCK_PRICE_TICK_DATA__: // Premium
		break;
	case __STOCK_PRICE_LAST_BID_ASK__: // Premium
		break;
	case __STOCK_PRICE_SPLITS__:
		break;
	case __FOREX_EXCHANGE__:
		p = make_shared<CProductFinnhubForexExchange>();
		break;
	case __FOREX_SYMBOLS__:
		p = make_shared<CProductFinnhubForexSymbol>();
		break;
	case __FOREX_CANDLES__:
		p = make_shared<CProductFinnhubForexDayLine>();
		break;
	case __FOREX_ALL_RATES__:
		break;
	case __CRYPTO_EXCHANGE__:
		p = make_shared<CProductFinnhubCryptoExchange>();
		break;
	case __CRYPTO_SYMBOLS__:
		p = make_shared<CProductFinnhubCryptoSymbol>();
		break;
	case __CRYPTO_CANDLES__:
		p = make_shared<CProductFinnhubCryptoDayLine>();
		break;
	case __ECONOMIC_COUNTRY_LIST__:
		p = make_shared<CProductFinnhubEconomicCountryList>();
		break;
	case __ECONOMIC_CALENDAR__:
		p = make_shared<CProductFinnhubEconomicCalendar>();
		break;
	default:
		TRACE("未处理指令%d\n", iIndex);
		gl_systemMessage.PushErrorMessage(_T("Finnhub product未实现"));
		break;
	}
	p->SetMarket(pMarket);

	return p;
}