#include"pch.h"

#include"FinnhubInquiryType.h"

#include "TiingoDataSource.h"

#include"WorldMarket.h"

CTiingoDataSource::CTiingoDataSource(void) {
	m_pWebInquiry = nullptr;
	Reset();
}

CTiingoDataSource::~CTiingoDataSource(void) {
}

bool CTiingoDataSource::Reset(void) {
	CVirtualDataSource::Reset();

	m_fStockSymbolUpdated = false;
	m_fCryptoSymbolUpdated = false;
	m_fDayLineUpdated = false;

	return true;
}

bool CTiingoDataSource::UpdateStatus(void)
{
	switch (m_pCurrentProduct->GetProductType()) {
	case __STOCK_SYMBOLS__:
		m_fStockSymbolUpdated = true;
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
		m_fCryptoSymbolUpdated = true;
		break;
	case __CRYPTO_CANDLES__:
	case __ECONOMIC_COUNTRY_LIST__:
	case __ECONOMIC_CALENDAR__:
	default:
		// error. not impliment yet.
		gl_systemMessage.PushErrorMessage(_T("Tiingo product未实现"));
		//ASSERT(0);
		break;
	}
	return true;
}

bool CTiingoDataSource::Inquire(long lCurrentTime) {
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
	else {
		if (llTickCount > (sllLastTimeTickCount + gl_systemConfigeration.GetWorldMarketFinnhubInquiryTime())) {
			sbWebError = false;
		}
	}

	if (llTickCount > (sllLastTimeTickCount + gl_systemConfigeration.GetWorldMarketTiingoInquiryTime())) {
		if (!IsInquiring()) {
			InquireTiingo();
		}
		if (IsInquiring()) {
			sllLastTimeTickCount = llTickCount;
		}
	}
	return true;
}

bool CTiingoDataSource::InquireTiingo(void) {
	ASSERT(!IsInquiring());
	if (gl_pWorldMarket->IsSystemReady()) {
		InquireCompanySymbol();
		InquireCryptoSymbol();
		// 由于Tiingo规定每月只能查询500个代码，故测试成功后即暂时不使用。
		InquireDayLine(); // 初步测试完毕。
		return true;
	}
	return false;
}

bool CTiingoDataSource::InquireCompanySymbol(void) {
	if (!IsInquiring() && !IsStockSymbolUpdated()) {
		CVirtualProductWebDataPtr p = m_TiingoFactory.CreateProduct(gl_pWorldMarket.get(), __STOCK_SYMBOLS__);
		m_qProduct.push(p);
		SetInquiring(true);
		gl_pWorldMarket->SetCurrentFunction(_T("Tiingo stock synmbol"));
		gl_systemMessage.PushInformationMessage(_T("Tiingo stock symbol已更新"));

		return true;
	}
	return false;
}

bool CTiingoDataSource::InquireCryptoSymbol(void) {
	if (!IsInquiring() && !IsCryptoSymbolUpdated()) {
		CVirtualProductWebDataPtr p = m_TiingoFactory.CreateProduct(gl_pWorldMarket.get(), __CRYPTO_SYMBOLS__);
		m_qProduct.push(p);
		SetInquiring(true);
		gl_pWorldMarket->SetCurrentFunction(_T("Tiingo crypto synmbol"));
		gl_systemMessage.PushInformationMessage(_T("Tiingo crypto symbol已更新"));

		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// Tiingo的下载日线数据与Finnhub的日线下载函数，只允许同时运行其中之一。
// 目前此功能只用于下载自选股票的日线数据。
//
//
//////////////////////////////////////////////////////////////////////////////////////////
bool CTiingoDataSource::InquireDayLine(void) {
	bool fFound = false;
	CWorldStockPtr pStock;
	CString str = _T("");
	long lStockSetSize = gl_pWorldMarket->GetChoicedStockSize();
	bool fHaveInquiry = false;

	ASSERT(gl_pWorldMarket->IsSystemReady());
	if (!IsInquiring() && !IsDayLineUpdated()) {
		for (long lCurrentUpdateDayLinePos = 0; lCurrentUpdateDayLinePos < lStockSetSize; lCurrentUpdateDayLinePos++) {
			if (gl_pWorldMarket->GetChoicedStock(lCurrentUpdateDayLinePos)->IsDayLineNeedUpdate()) {
				pStock = gl_pWorldMarket->GetChoicedStock(lCurrentUpdateDayLinePos);
				fFound = true;
				break;
			}
		}
		if (fFound) {
			fHaveInquiry = true;
			CVirtualProductWebDataPtr p = m_TiingoFactory.CreateProduct(gl_pWorldMarket.get(), __STOCK_PRICE_CANDLES__);
			p->SetIndex(gl_pWorldMarket->GetStockIndex(pStock->GetSymbol()));
			m_qProduct.push(p);
			gl_pWorldMarket->SetCurrentFunction(_T("Tiingo Stock日线：") + pStock->GetSymbol());
			SetInquiring(true);
			pStock->SetDayLineNeedUpdate(false);
			TRACE("申请Tiingo %s日线数据\n", pStock->GetSymbol().GetBuffer());
		}
		else {
			SetDayLineUpdated(true);
			TRACE("Tiingo日线更新完毕\n");
			str = _T("美国市场自选股票日线历史数据更新完毕");
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}