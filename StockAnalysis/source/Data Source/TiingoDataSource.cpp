#include"pch.h"

#include"FinnhubInquiryType.h"

#include "TiingoDataSource.h"

#include"WorldMarket.h"

CTiingoDataSource::CTiingoDataSource(void) {
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
	case _STOCK_SYMBOLS_:
		m_fStockSymbolUpdated = true;
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
	case _FINAICIAL_AS_REPORTED_:
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
	case _STOCK_ESTIMATES_EPS_EXTIMATES_:// Premium
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
		m_fCryptoSymbolUpdated = true;
		break;
	case _CRYPTO_CANDLES_:
	case _ECONOMIC_COUNTRY_LIST_:
	case _ECONOMIC_CALENDAR_:
	default:
		// error. not impliment yet.
		gl_systemMessage.PushErrorMessage(_T("Tiingo productδʵ��"));
		//ASSERT(0);
		break;
	}
	return true;
}

bool CTiingoDataSource::Inquire(long lCurrentTime) {
	static long long sllLastTimeTickCount = 0;
	static bool sbWebErrorOccured = false;
	long long llTickCount = GetTickCount64();

	if (!sbWebErrorOccured) {
		if (m_pWebInquiry->IsWebError()) {
			sbWebErrorOccured = true;
			sllLastTimeTickCount += 300000; // ������ִ������ӳ�5�������������롣
		}
	}

	if (llTickCount > (sllLastTimeTickCount + gl_systemConfigeration.GetWorldMarketTiingoInquiryTime())) {
		sbWebErrorOccured = false; // ����ʱ��������ʶ
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
		// ����Tiingo�涨ÿ��ֻ�ܲ�ѯ500�����룬�ʲ��Գɹ�����ʱ��ʹ�á�
		InquireDayLine(); // ����������ϡ�
		return true;
	}
	return false;
}

bool CTiingoDataSource::InquireCompanySymbol(void) {
	if (!IsInquiring() && !IsStockSymbolUpdated()) {
		CVirtualProductWebDataPtr p = m_TiingoFactory.CreateProduct(gl_pWorldMarket.get(), _STOCK_SYMBOLS_);
		m_qProduct.push(p);
		SetInquiring(true);
		gl_pWorldMarket->SetCurrentFunction(_T("Tiingo stock synmbol"));
		gl_systemMessage.PushInformationMessage(_T("Tiingo stock symbol�Ѹ���"));

		return true;
	}
	return false;
}

bool CTiingoDataSource::InquireCryptoSymbol(void) {
	if (!IsInquiring() && !IsCryptoSymbolUpdated()) {
		CVirtualProductWebDataPtr p = m_TiingoFactory.CreateProduct(gl_pWorldMarket.get(), _CRYPTO_SYMBOLS_);
		m_qProduct.push(p);
		SetInquiring(true);
		gl_pWorldMarket->SetCurrentFunction(_T("Tiingo crypto synmbol"));
		gl_systemMessage.PushInformationMessage(_T("Tiingo crypto symbol�Ѹ���"));

		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// Tiingo����������������Finnhub���������غ�����ֻ����ͬʱ��������֮һ��
// Ŀǰ�˹���ֻ����������ѡ��Ʊ���������ݡ�
//
//
//////////////////////////////////////////////////////////////////////////////////////////
bool CTiingoDataSource::InquireDayLine(void) {
	bool fFound = false;
	CWorldStockPtr pStock;
	CString str = _T("");
	long lStockSetSize = gl_pWorldMarket->GetChosenStockSize();
	bool fHaveInquiry = false;

	ASSERT(gl_pWorldMarket->IsSystemReady());
	if (!IsInquiring() && !IsDayLineUpdated()) {
		for (long lCurrentUpdateDayLinePos = 0; lCurrentUpdateDayLinePos < lStockSetSize; lCurrentUpdateDayLinePos++) {
			if (gl_pWorldMarket->GetChosenStock(lCurrentUpdateDayLinePos)->IsDayLineNeedUpdate()) {
				pStock = gl_pWorldMarket->GetChosenStock(lCurrentUpdateDayLinePos);
				fFound = true;
				break;
			}
		}
		if (fFound) {
			fHaveInquiry = true;
			CVirtualProductWebDataPtr p = m_TiingoFactory.CreateProduct(gl_pWorldMarket.get(), _STOCK_PRICE_CANDLES_);
			p->SetIndex(gl_pWorldMarket->GetStockIndex(pStock->GetSymbol()));
			m_qProduct.push(p);
			gl_pWorldMarket->SetCurrentFunction(_T("Tiingo Stock���ߣ�") + pStock->GetSymbol());
			SetInquiring(true);
			TRACE("����Tiingo %s��������\n", pStock->GetSymbol().GetBuffer());
		}
		else {
			SetDayLineUpdated(true);
			TRACE("Tiingo���߸������\n");
			str = _T("�����г���ѡ��Ʊ������ʷ���ݸ������");
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}