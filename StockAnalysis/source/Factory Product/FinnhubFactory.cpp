#include "pch.h"

#include"globedef.h"
#include"WorldMarket.h"

#include"FinnhubCompanyProfileConcise.h"
#include"FinnhubCompanyPeer.h"
#include"FinnhubCompanyInsiderTransaction.h"
#include"FinnhubCryptoDayLine.h"
#include"FinnhubForexDayLine.h"
#include"FinnhubStockDayLine.h"

#include"FinnhubCompanySymbolProduct.h"
#include"FinnhubCryptoSymbolProduct.h"
#include"FinnhubForexSymbolProduct.h"

#include"FinnhubForexExchange.h"
#include"FinnhubCryptoExchange.h"

#include"FinnhubEconomicCountryList.h"

#include "FinnhubFactory.h"

CWebSourceDataProductPtr CFinnhubFactory::CreateProduct(int iIndex) {
	CWebSourceDataProductPtr p = nullptr;

	switch (iIndex) {
	case __COMPANY_PROFILE__: // Premium 免费账户无法读取此信息，sandbox模式能读取，但数据是错误的，只能用于测试。
		break;
	case __COMPANY_PROFILE_CONCISE__:
		p = make_shared<CFinnhubCompanyProfileConcise>();
		break;
	case  __STOCK_SYMBOLS__:
		p = make_shared<CFinnhubCompanySymbolProduct>();
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
		p = make_shared<CFinnhubCompanyPeer>();
		break;
	case __BASIC_FINANCIALS__:
		break;
	case __INSIDER_TRANSACTION__:
		p = make_shared<CFinnhubCompanyInsiderTransaction>();
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
		break;
	case __STOCK_ESTIMATES_EARNING_CALENDAR__:
		break;
	case __STOCK_PRICE_QUOTE__:
		break;
	case __STOCK_PRICE_CANDLES__:
		p = make_shared<CFinnhubStockDayLine>();
		break;
	case __STOCK_PRICE_TICK_DATA__: // Premium
		break;
	case __STOCK_PRICE_LAST_BID_ASK__: // Premium
		break;
	case __STOCK_PRICE_SPLITS__:
		break;
	case __FOREX_EXCHANGE__:
		p = make_shared<CFinnhubForexExchange>();
		break;
	case __FOREX_SYMBOLS__:
		p = make_shared<CFinnhubForexSymbolProduct>();
		break;
	case __FOREX_CANDLES__:
		p = make_shared<CFinnhubForexDayLine>();
		break;
	case __FOREX_ALL_RATES__:
		break;
	case __CRYPTO_EXCHANGE__:
		p = make_shared<CFinnhubCryptoExchange>();
		break;
	case __CRYPTO_SYMBOLS__:
		p = make_shared<CFinnhubCryptoSymbolProduct>();
		break;
	case __CRYPTO_CANDLES__:
		p = make_shared<CFinnhubCryptoDayLine>();
		break;
	case __ECONOMIC_COUNTRY_LIST__:
		p = make_shared<CFinnhubEconomicCountryList>();
		break;
	case __ECONOMIC_CALENDAR__:
		break;
	default:
		TRACE("未处理指令%d\n", iIndex);
		gl_systemMessage.PushErrorMessage(_T("Finnhub product未实现"));
		break;
	}
	ASSERT(p != nullptr);

	return p;
}