#include"pch.h"

#include"FinnhubInquiryType.h"

#include "QuandlDataSource.h"

#include"WorldMarket.h"

CQuandlDataSource::CQuandlDataSource(void) { Reset(); }

bool CQuandlDataSource::Reset(void) {
	CVirtualDataSource::Reset();
	return true;
}

bool CQuandlDataSource::UpdateStatus(void) {
	switch (m_pCurrentProduct->GetProductType()) {
	case STOCK_SYMBOLS_:
		break;
	case STOCK_PRICE_CANDLES_:
		break;
	case COMPANY_PROFILE_:
	case COMPANY_PROFILE_CONCISE_:
	case COMPANY_EXECTIVE_: // Premium
	case MARKET_NEWS_:
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
		break;
	case CRYPTO_CANDLES_:
	case ECONOMIC_COUNTRY_LIST_:
	case ECONOMIC_CALENDAR_:
	default:
		// error. not implement yet.
		gl_systemMessage.PushErrorMessage(_T("Quandl product未实现"));
	//ASSERT(0);
		break;
	}
	return true;
}

bool CQuandlDataSource::Inquire(long lCurrentTime) {
	static long long sllLastTimeTickCount = 0;
	static bool sbWebError = false;

	const long long llTickCount = GetTickCount64();
	if (!sbWebError) {
		if (m_pWebInquiry->IsWebError()) {
			sbWebError = true;
			sllLastTimeTickCount += 300000; // 如果出现错误，则延迟5分钟再重新申请。
		}
	}

	if (llTickCount > (sllLastTimeTickCount + gl_systemConfiguration.GetWorldMarketQuandlInquiryTime())) {
		sbWebError = false;
		if (!IsInquiring()) {
		}
		if (IsInquiring()) sllLastTimeTickCount = llTickCount;
	}
	return true;
}
