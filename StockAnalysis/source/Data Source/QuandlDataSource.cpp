#include"pch.h"

#include"FinnhubInquiryType.h"

#include "QuandlDataSource.h"

#include"WorldMarket.h"

CQuandlDataSource::CQuandlDataSource(void) {
	Reset();
}

CQuandlDataSource::~CQuandlDataSource(void) {
}

bool CQuandlDataSource::Reset(void) {
	CVirtualDataSource::Reset();
	return true;
}

bool CQuandlDataSource::UpdateStatus(void)
{
	switch (m_pCurrentProduct->GetProductType()) {
	case __STOCK_SYMBOLS__:
		break;
	case __STOCK_PRICE_CANDLES__:
		break;
	case __COMPANY_PROFILE__:
	case __COMPANY_PROFILE_CONCISE__:
	case __COMPANY_EXECTIVE__: // Premium
	case __MARKET_NEWS__:
	case __COMPANY_NEWS__:
	case __PRESS_RELEASE__: // Premium
	case __NEWS_SENTIMENT__:
	case __PEERS__:
	case __BASIC_FINANCIALS__:
	case __INSIDER_TRANSACTION__:
	case __OWNERSHIP__: // Premium
	case __FUND_OWNERSHIP__: // jPremium
	case __FINANCIAL__: // Premium
	case __FINAICIAL_AS_REPORTED__:
	case __SEC_FILINGS__:
	case __INTERNATIONAL_FILINGS__: // Premium
	case __SEC_SENTIMENT_ANALYSIS__: // Premium
	case __SIMILARITY_INDEX__: // Premium
	case __IPO_CALENDAR__:
	case __DIVIDENDS__: // Premium
	case __STOCK_ESTIMATES_RECOMMENDATION_TRENDS__:
	case __STOCK_ESTIMATES_PRICE_TARGET__:
	case __STOCK_ESTIMATES_UPGRADE_DOWNGRADE__: // Premium
	case __STOCK_ESTIMATES_REVENUE_EXTIMATES__: // Premium
	case __STOCK_ESTIMATES_EPS_EXTIMATES__:// Premium
	case __STOCK_ESTIMATES_EPS_SURPRISE__:
	case __STOCK_ESTIMATES_EARNING_CALENDAR__:
	case __STOCK_PRICE_QUOTE__:
	case __STOCK_PRICE_TICK_DATA__: // Premium
	case __STOCK_PRICE_LAST_BID_ASK__: // Premium
	case __STOCK_PRICE_SPLITS__:
	case __FOREX_EXCHANGE__:
	case __FOREX_SYMBOLS__:
	case __FOREX_CANDLES__:
	case __FOREX_ALL_RATES__:
	case __CRYPTO_EXCHANGE__:
		break;
	case __CRYPTO_SYMBOLS__:
		break;
	case __CRYPTO_CANDLES__:
	case __ECONOMIC_COUNTRY_LIST__:
	case __ECONOMIC_CALENDAR__:
	default:
		// error. not impliment yet.
		gl_systemMessage.PushErrorMessage(_T("Quandl product未实现"));
		//ASSERT(0);
		break;
	}
	return true;
}

bool CQuandlDataSource::Inquire(long lCurrentTime) {
	static long long sllLastTimeTickCount = 0;
	static bool sbWebError = false;
	long long llTickCount = 0;

	llTickCount = GetTickCount64();
	if (!sbWebError) {
		if (m_pWebInquiry->IsWebError()) {
			sbWebError = true;
			sllLastTimeTickCount += 300000; // 如果出现错误，则延迟5分钟再重新申请。
		}
	}

	if (llTickCount > (sllLastTimeTickCount + gl_systemConfigeration.GetWorldMarketQuandlInquiryTime())) {
		sbWebError = false;
		if (!IsInquiring()) {
		}
		if (IsInquiring()) sllLastTimeTickCount = llTickCount;
	}
	return true;
}