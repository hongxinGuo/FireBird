#include "pch.h"

#include"SystemData.h"
import FireBird.System.Message;
import FireBird.WebProduct.Virtual;
#include"VirtualMarket.h"

//#include"ProductTiingoStockProfile.h"
#include"ProductTiingoStockProfile.h"
#include"ProductTiingoStockDayLine.h"

#include"ProductTiingoCryptoSymbol.h"

#include"ProductDummy.h"
#include "ProductTiingoFinancialState.h"
#include "ProductTiingoFundamentalDefinition.h"
#include "ProductTiingoIEXTopOFBook.h"
#include "ProductTiingoMarketNews.h"
#include "ProductTiingoStockDailyMeta.h"
#include "TiingoIEXTopOFBook.h"
import FireBird.FinnhubInquiryType;
module FireBird.Factory.Tiingo; 



CVirtualProductWebDataPtr CTiingoFactory::CreateProduct(CVirtualMarketPtr pMarket, int iInquireType) {
	CVirtualProductWebDataPtr p = nullptr;

	switch (iInquireType) {
	case STOCK_SYMBOLS_:
		p = make_shared<CProductTiingoStockProfile>();
		break;
	case STOCK_PRICE_CANDLES_:
		p = make_shared<CProductTiingoStockDayLine>();
		break;
	case COMPANY_PROFILE_:
	case COMPANY_PROFILE_CONCISE_:
	case COMPANY_EXECUTIVE_: // Premium
	case MARKET_NEWS_: // Premium
		p = make_shared<CProductTiingoMarketNews>();
		break;
	case COMPANY_NEWS_:
	case PRESS_RELEASE_: // Premium
	case NEWS_SENTIMENT_:
	case PEERS_:
	case BASIC_FINANCIALS_:
	case INSIDER_TRANSACTION_:
	case OWNERSHIP_: // Premium
	case FUND_OWNERSHIP_: // jPremium
	case FINANCIAL_: // Premium
	case FINANCIAL_AS_REPORTED_:
	case SEC_FILINGS_:
	case INTERNATIONAL_FILINGS_: // Premium
	case SEC_SENTIMENT_ANALYSIS_: // Premium
	case SIMILARITY_INDEX_: // Premium
	case IPO_CALENDAR_:
	case DIVIDENDS_: // Premium
	case STOCK_ESTIMATES_RECOMMENDATION_TRENDS_:
	case STOCK_ESTIMATES_PRICE_TARGET_:
	case STOCK_ESTIMATES_UPGRADE_DOWNGRADE_: // Premium
	case STOCK_ESTIMATES_REVENUE_ESTIMATES_: // Premium
	case STOCK_ESTIMATES_EPS_ESTIMATES_: // Premium
	case STOCK_ESTIMATES_EPS_SURPRISE_:
	case STOCK_ESTIMATES_EARNING_CALENDAR_:
	case STOCK_PRICE_QUOTE_:
	case STOCK_PRICE_TICK_DATA_: // Premium
	case STOCK_PRICE_LAST_BID_ASK_: // Premium
	case STOCK_PRICE_SPLITS_:
	case FOREX_EXCHANGE_:
	case FOREX_SYMBOLS_:
	case FOREX_CANDLES_:
	case FOREX_ALL_RATES_:
	case CRYPTO_EXCHANGE_:
		break;
	case CRYPTO_SYMBOLS_:
		p = make_shared<CProductTiingoCryptoSymbol>();
		break;
	case CRYPTO_CANDLES_:
	case ECONOMIC_COUNTRY_LIST_:
	case ECONOMIC_CALENDAR_:
		break;
	case TIINGO_FUNDAMENTAL_DEFINITION_:
		p = make_shared<CProductTiingoFundamentalDefinition>();
		break;
	case TIINGO_STOCK_DAILY_META__:
		p = make_shared<CProductTiingoStockDailyMeta>();
		break;
	case TIINGO_FINANCIAL_STATEMENT_:
		p = make_shared<CProductTiingoFinancialState>();
		break;
	case TIINGO_IEX_TOP_OF_BOOK_:
		p = make_shared<CProductTiingoIEXTopOfBook>();
		break;
	default:
		p = make_shared<CProductDummy>();
		gl_systemMessage.PushErrorMessage(_T("Tiingo product未实现"));
		break;
	}
	if (p == nullptr) {
		p = make_shared<CProductDummy>();
		gl_systemMessage.PushErrorMessage(_T("Tiingo product未实现"));
	}
	p->SetMarket(pMarket);
	p->SetInquireType(iInquireType);

	return p;
}
