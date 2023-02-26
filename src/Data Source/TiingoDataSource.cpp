#include"pch.h"

#include"FinnhubInquiryType.h"

#include "TiingoDataSource.h"

#include"WorldMarket.h"

CTiingoDataSource::CTiingoDataSource(void) {
	m_strInquiryFunction = _T(""); // Tiingo有各种数据，故其前缀由数据申请函数每次设置，不同的前缀申请不同的数据。
	m_strParam = _T("");
	m_strSuffix = _T("");
	m_strSuffix = _T("&token=");
	m_strInquiryToken = _T("");
	m_lInquiringNumber = 1; // Tiingo实时数据查询数量默认值

	CTiingoDataSource::ConfigureSession();
	CTiingoDataSource::Reset();
}

bool CTiingoDataSource::Reset(void) {
	m_fUpdateStockSymbol = true;
	m_fUpdateCryptoSymbol = true;
	m_fUpdateDayLine = true;

	return true;
}

void CTiingoDataSource::UpdateStatus(void) {
	switch (m_pCurrentProduct->GetProductType()) {
	case STOCK_SYMBOLS_:
		m_fUpdateStockSymbol = false;
		break;
	case STOCK_PRICE_CANDLES_:
		break;
	case COMPANY_PROFILE_:
	case COMPANY_PROFILE_CONCISE_:
	case COMPANY_EXECUTIVE_: // Premium
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
		m_fUpdateCryptoSymbol = false;
		break;
	case CRYPTO_CANDLES_:
	case ECONOMIC_COUNTRY_LIST_:
	case ECONOMIC_CALENDAR_:
		break;
	default:
		// error. not implement yet.
		gl_systemMessage.PushErrorMessage(_T("Tiingo product未实现"));
	//ASSERT(0);
		break;
	}
}

bool CTiingoDataSource::GenerateInquiryMessage(const long lCurrentTime) {
	const long long llTickCount = GetTickCount();

	if (llTickCount > (m_llLastTimeTickCount + gl_systemConfiguration.GetWorldMarketTiingoInquiryTime())) {
		if (IsWebError()) {
			m_llLastTimeTickCount += 300000; // 如果出现错误，则延迟5分钟再重新申请。
		}
		else {
			m_llLastTimeTickCount = llTickCount;
		}

		if (!IsInquiring()) {
			InquireTiingo();
			if (IsInquiring()) {
				return true;
			}
		}
	}
	return false;
}

bool CTiingoDataSource::InquireTiingo(void) {
	ASSERT(!IsInquiring());
	if (gl_pWorldMarket->IsSystemReady()) {
		InquireCompanySymbol();
		InquireCryptoSymbol();
		InquireDayLine();
		return true;
	}
	return false;
}

bool CTiingoDataSource::InquireCompanySymbol(void) {
	if (!IsInquiring() && IsUpdateStockSymbol()) {
		const CVirtualProductWebDataPtr p = m_TiingoFactory.CreateProduct(gl_pWorldMarket.get(), STOCK_SYMBOLS_);
		m_qProduct.push(p);
		SetInquiring(true);
		//gl_pWorldMarket->SetCurrentFunction(_T("Tiingo stock symbol"));
		gl_systemMessage.PushInformationMessage(_T("Tiingo stock symbol已更新"));
		return true;
	}
	return false;
}

bool CTiingoDataSource::InquireCryptoSymbol(void) {
	if (!IsInquiring() && IsUpdateCryptoSymbol()) {
		CVirtualProductWebDataPtr p = m_TiingoFactory.CreateProduct(gl_pWorldMarket.get(), CRYPTO_SYMBOLS_);
		m_qProduct.push(p);
		SetInquiring(true);
		//gl_pWorldMarket->SetCurrentFunction(_T("Tiingo crypto symbol"));
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
	const auto lStockSetSize = gl_pWorldMarket->GetChosenStockSize();
	bool fHaveInquiry = false;

	ASSERT(gl_pWorldMarket->IsSystemReady());
	if (!IsInquiring() && IsUpdateDayLine()) {
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
			//gl_pWorldMarket->SetCurrentFunction(_T("Tiingo Stock日线：") + pStock->GetSymbol());
			SetInquiring(true);
			TRACE("申请Tiingo %s日线数据\n", pStock->GetSymbol().GetBuffer());
		}
		else {
			SetUpdateDayLine(false);
			const CString str = "美国市场自选股票日线历史数据更新完毕";
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}

void CTiingoDataSource::ConfigureSession(void) {
	ASSERT(m_pSession != nullptr);
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 120000); // 设置连接超时时间为120秒
	m_pSession->SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, 120000); // 设置接收超时时间为120秒
	m_pSession->SetOption(INTERNET_OPTION_SEND_TIMEOUT, 500); // 设置发送超时时间为2秒
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1); // 2次重试
}

/////////////////////////////////////////////////////////////////////////////////////
//
//
// 使用nlohmann json解析。
//
////////////////////////////////////////////////////////////////////////////////////
bool CTiingoDataSource::ParseData(CWebDataPtr pWebData) {
	return pWebData->CreateJson(0, 0);
}
