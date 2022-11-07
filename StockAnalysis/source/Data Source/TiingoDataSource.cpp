#include"pch.h"

#include "TiingoDataSource.h"
#include"FinnhubInquiryType.h"

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
		gl_systemMessage.PushErrorMessage(_T("Tiingo productδʵ��"));
		//ASSERT(0);
		break;
	}
	return true;
}

bool CTiingoDataSource::Inquire(long lCurrentTime) {
	static int s_iCountTiingoLimit = 80; // ��֤ÿ80��ִ��һ�Σ���8��ÿ�Σ�.Tiingo����˻��ٶ�����ΪÿСʱ500�Σ� ÿ����9�Σ���ÿ��8�뼴�ɡ�

	if (--s_iCountTiingoLimit < 0) {
		if (!IsInquiring()) {
			InquireTiingo();
		}
		s_iCountTiingoLimit = gl_systemConfigeration.GetWorldMarketTiingoInquiryTime() / 100;
		if (m_pWebInquiry->IsWebError()) {
			m_pWebInquiry->SetWebError(false);
			s_iCountTiingoLimit = 6000; // ������ִ�����ÿ10������������һ�Ρ�
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
	if (!IsStockSymbolUpdated()) {
		CProductWebSourceDataPtr p = m_TiingoFactory.CreateProduct(gl_pWorldMarket.get(), __STOCK_SYMBOLS__);
		m_qProduct.push(p);
		SetInquiring(true);
		gl_pWorldMarket->SetCurrentFunction(_T("Tiingo stock synmbol"));
		gl_systemMessage.PushInformationMessage(_T("Tiingo stock symbol�Ѹ���"));

		return true;
	}
	return false;
}

bool CTiingoDataSource::InquireCryptoSymbol(void) {
	if (!IsCryptoSymbolUpdated()) {
		CProductWebSourceDataPtr p = m_TiingoFactory.CreateProduct(gl_pWorldMarket.get(), __CRYPTO_SYMBOLS__);
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
	long lStockSetSize = gl_pWorldMarket->GetChoicedStockSize();
	bool fHaveInquiry = false;

	ASSERT(gl_pWorldMarket->IsSystemReady());
	if (!IsDayLineUpdated()) {
		for (long lCurrentUpdateDayLinePos = 0; lCurrentUpdateDayLinePos < lStockSetSize; lCurrentUpdateDayLinePos++) {
			if (gl_pWorldMarket->GetChoicedStock(lCurrentUpdateDayLinePos)->IsDayLineNeedUpdate()) {
				pStock = gl_pWorldMarket->GetChoicedStock(lCurrentUpdateDayLinePos);
				fFound = true;
				break;
			}
		}
		if (fFound) {
			fHaveInquiry = true;
			CProductWebSourceDataPtr p = m_TiingoFactory.CreateProduct(gl_pWorldMarket.get(), __STOCK_PRICE_CANDLES__);
			p->SetIndex(gl_pWorldMarket->GetStockIndex(pStock->GetSymbol()));
			m_qProduct.push(p);
			gl_pWorldMarket->SetCurrentFunction(_T("Tiingo Stock���ߣ�") + pStock->GetSymbol());
			SetInquiring(true);
			pStock->SetDayLineNeedUpdate(false);
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