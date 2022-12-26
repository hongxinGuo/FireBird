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
	case _STOCK_SYMBOLS_:
		break;
	case _STOCK_PRICE_CANDLES_:
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
		break;
	case _CRYPTO_SYMBOLS_:
		break;
	case _CRYPTO_CANDLES_:
	case _ECONOMIC_COUNTRY_LIST_:
	case _ECONOMIC_CALENDAR_:
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
