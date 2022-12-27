#include"pch.h"

#include"FinnhubInquiryType.h"

#include "TiingoDataSource.h"

#include"WorldMarket.h"

CTiingoDataSource::CTiingoDataSource(void) { Reset(); }

bool CTiingoDataSource::Reset(void) {
	CVirtualDataSource::Reset();

	m_fStockSymbolUpdated = false;
	m_fCryptoSymbolUpdated = false;
	m_fDayLineUpdated = false;

	return true;
}

bool CTiingoDataSource::UpdateStatus(void) {
	switch (m_pCurrentProduct->GetProductType()) {
	case STOCK_SYMBOLS_:
		m_fStockSymbolUpdated = true;
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
		m_fCryptoSymbolUpdated = true;
		break;
	case CRYPTO_CANDLES_:
	case ECONOMIC_COUNTRY_LIST_:
	case ECONOMIC_CALENDAR_:
	default:
		// error. not implement yet.
		gl_systemMessage.PushErrorMessage(_T("Tiingo product未实现"));
	//ASSERT(0);
		break;
	}
	return true;
}

bool CTiingoDataSource::Inquire(const long lCurrentTime) {
	static long long sllLastTimeTickCount = 0;
	static bool sbWebErrorOccurred = false;
	const long long llTickCount = GetTickCount64();

	if (!sbWebErrorOccurred) {
		if (m_pWebInquiry->IsWebError()) {
			sbWebErrorOccurred = true;
			sllLastTimeTickCount += 300000; // 如果出现错误，则延迟5分钟再重新申请。
		}
	}

	if (llTickCount > (sllLastTimeTickCount + gl_systemConfiguration.GetWorldMarketTiingoInquiryTime())) {
		sbWebErrorOccurred = false; // 申请时清除错误标识
		if (!IsInquiring()) { InquireTiingo(); }
		if (IsInquiring()) { sllLastTimeTickCount = llTickCount; }
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
		CVirtualProductWebDataPtr p = m_TiingoFactory.CreateProduct(gl_pWorldMarket.get(), STOCK_SYMBOLS_);
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
		CVirtualProductWebDataPtr p = m_TiingoFactory.CreateProduct(gl_pWorldMarket.get(), CRYPTO_SYMBOLS_);
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
	CString str = _T("");
	const long lStockSetSize = gl_pWorldMarket->GetChosenStockSize();
	bool fHaveInquiry = false;

	ASSERT(gl_pWorldMarket->IsSystemReady());
	if (!IsInquiring() && !IsDayLineUpdated()) {
		CWorldStockPtr pStock;
		bool fFound = false;
		for (long lCurrentUpdateDayLinePos = 0; lCurrentUpdateDayLinePos < lStockSetSize; lCurrentUpdateDayLinePos++) {
			if (gl_pWorldMarket->GetChosenStock(lCurrentUpdateDayLinePos)->IsDayLineNeedUpdate()) {
				pStock = gl_pWorldMarket->GetChosenStock(lCurrentUpdateDayLinePos);
				fFound = true;
				break;
			}
		}
		if (fFound) {
			fHaveInquiry = true;
			CVirtualProductWebDataPtr p = m_TiingoFactory.CreateProduct(gl_pWorldMarket.get(), STOCK_PRICE_CANDLES_);
			p->SetIndex(gl_pWorldMarket->GetStockIndex(pStock->GetSymbol()));
			m_qProduct.push(p);
			gl_pWorldMarket->SetCurrentFunction(_T("Tiingo Stock日线：") + pStock->GetSymbol());
			SetInquiring(true);
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
