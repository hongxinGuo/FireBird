#include"pch.h"

#include"TimeConvert.h"

#include"FinnhubInquiryType.h"
#include"FinnhubInaccessibleExchange.h"

#include "FinnhubDataSource.h"

#include"WorldMarket.h"

CFinnhubDataSource::CFinnhubDataSource(void) {
	// ���裨Ҳ�޷���ÿ�ո��µı�����������
	m_fEPSSurpriseUpdated = true;
	m_lCurrentUpdateEPSSurprisePos = 0;
	m_lCurrentUpdateDayLinePos = 0; // ����֤ȯ���������ж�ʮ��֮�࣬�޷���һ��֮�ڸ����꣬�ʲ������ô�������

	Reset();
}

bool CFinnhubDataSource::Reset(void) {
	CVirtualDataSource::Reset();

	// Finnhub�������������ݱ�ʶ��ÿ����Ҫ���á�
	m_fCountryListUpdated = false;
	m_fSymbolUpdated = false; // ÿ����Ҫ���´���
	m_fStockProfileUpdated = false;
	m_fCompanyNewsUpdated = false;
	m_fStockBasicFinancialUpdated = false;
	m_fDayLineUpdated = false;
	m_fForexExchangeUpdated = false;
	m_fForexSymbolUpdated = false;
	m_fForexDayLineUpdated = false;
	m_fCryptoExchangeUpdated = false;
	m_fCryptoSymbolUpdated = false;
	m_fCryptoDayLineUpdated = false;
	m_fEconomicCalendarUpdated = false;

	m_fPeerUpdated = false;
	m_fInsiderTransactionUpdated = false;
	m_fInsiderSentimentUpdated = false;

	if (GetDayOfWeek() == 6) {
		// ÿ�ܵ�����������һ��EPSSurprise
		m_lCurrentUpdateEPSSurprisePos = 0;
		SetEPSSurpriseUpdated(false);
	}

	return true;
}

bool CFinnhubDataSource::UpdateStatus(void) {
	switch (m_pCurrentProduct->GetProductType()) {
	case _COMPANY_PROFILE_: // Premium ����˻��޷���ȡ����Ϣ��sandboxģʽ�ܶ�ȡ���������Ǵ���ģ�ֻ�����ڲ��ԡ�
		m_fStockProfileUpdated = true;
		break;
	case _COMPANY_PROFILE_CONCISE_:
		m_fStockProfileUpdated = true;
		break;
	case _STOCK_SYMBOLS_:
		break;
	case _COMPANY_EXECTIVE_: // Premium
		break;
	case _MARKET_NEWS_:
		break;
	case _COMPANY_NEWS_:
		break;
	case _PRESS_RELEASE_: // Premium
		break;
	case _NEWS_SENTIMENT_:
		break;
	case _PEERS_:
		m_fPeerUpdated = true;
		break;
	case _BASIC_FINANCIALS_:
		break;
	case _INSIDER_TRANSACTION_:
		break;
	case _INSIDER_SENTIMENT_:
		break;
	case _OWNERSHIP_: // Premium
		break;
	case _FUND_OWNERSHIP_: // jPremium
		break;
	case _FINANCIAL_: // Premium
		break;
	case _FINANCIAL_AS_REPORTED_:
		break;
	case _SEC_FILINGS_:
		break;
	case _INTERNATIONAL_FILINGS_: // Premium
		break;
	case _SEC_SENTIMENT_ANALYSIS_: // Premium
		break;
	case _SIMILARITY_INDEX_: // Premium
		break;
	case _IPO_CALENDAR_:
		break;
	case _DIVIDENDS_: // Premium
		break;
	case _STOCK_ESTIMATES_RECOMMENDATION_TRENDS_:
		break;
	case _STOCK_ESTIMATES_PRICE_TARGET_:
		break;
	case _STOCK_ESTIMATES_UPGRADE_DOWNGRADE_: // Premium
		break;
	case _STOCK_ESTIMATES_REVENUE_EXTIMATES_: // Premium
		break;
	case _STOCK_ESTIMATES_EPS_EXTIMATES_: // Premium
		break;
	case _STOCK_ESTIMATES_EPS_SURPRISE_:
		break;
	case _STOCK_ESTIMATES_EARNING_CALENDAR_:
		break;
	case _STOCK_PRICE_QUOTE_:
		break;
	case _STOCK_PRICE_CANDLES_:
		break;
	case _STOCK_PRICE_TICK_DATA_: // Premium
		break;
	case _STOCK_PRICE_LAST_BID_ASK_: // Premium
		break;
	case _STOCK_PRICE_SPLITS_:
		break;
	case _FOREX_EXCHANGE_:
		m_fForexExchangeUpdated = true;
		break;
	case _FOREX_SYMBOLS_:
		break;
	case _FOREX_CANDLES_:
		break;
	case _FOREX_ALL_RATES_:
		break;
	case _CRYPTO_EXCHANGE_:
		m_fCryptoExchangeUpdated = true;
		break;
	case _CRYPTO_SYMBOLS_:
		break;
	case _CRYPTO_CANDLES_:
		break;
	case _ECONOMIC_COUNTRY_LIST_:
		m_fCountryListUpdated = true;
		break;
	case _ECONOMIC_CALENDAR_:
		m_fEconomicCalendarUpdated = true;
		break;
	default:
		TRACE("δ����ָ��%d\n", m_pCurrentProduct->GetProductType());
		gl_systemMessage.PushErrorMessage(_T("Finnhub productδʵ��"));
		break;
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ��ϵͳ���������ȣ�����GetTickCount64()��ȷ����ǰʱ�䡣
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CFinnhubDataSource::Inquire(long lCurrentTime) {
	static long long sllLastTimeTickCount = 0;
	static bool sbWebErrorOccurred = false;
	const long long llTickCount = GetTickCount64();;

	if (!sbWebErrorOccurred) {
		if (m_pWebInquiry->IsWebError()) {
			sbWebErrorOccurred = true;
			sllLastTimeTickCount += 300000; // ������ִ������ӳ�5�������������롣
		}
	}

	if (llTickCount > (sllLastTimeTickCount + gl_systemConfiguration.GetWorldMarketFinnhubInquiryTime())) {
		sbWebErrorOccurred = false; // ����ʱ��������ʶ
		if (!IsInquiring()) {
			InquireFinnhub(lCurrentTime);
			if (IsInquiring()) sllLastTimeTickCount = llTickCount;
		}
	}
	return true;
}

bool CFinnhubDataSource::InquireFinnhub(long lCurrentTime) {
	ASSERT(!IsInquiring());
	if (((lCurrentTime < 165700) || (lCurrentTime > 170500))) {
		// ������ʱ����ϵͳ���ʶ���ʱ���������������Ϣ��
		InquireCountryList();
		InquireForexExchange();
		InquireCryptoExchange();
		InquireCompanySymbol(); // ��һ���������������뵱��֤ȯ����
		InquireForexSymbol();
		InquireCryptoSymbol();
		InquireEconomicCalendar();

		// ����Finnhub������Ϣ�����񣬽�Ҫ����������Ա�֤���г�ʱ���賿ʮ���Ӻ�ִ�С������ܹ���֤�������г�ʱ����ִ�в�ѯ����
		if (gl_pWorldMarket->IsSystemReady()) {
			InquireCompanyNews(); // ����mysql�ַ��������⣬������Щ�����޷��洢�� Ŀǰδ�ҵ�����ķ���������ʱ��ִ�д˲�ѯ
			InquireCompanyProfileConcise();
			InquireCompanyBasicFinancial();
			InquirePeer();
			InquireInsiderTransaction();
			InquireInsiderSentiment();
			InquireCryptoDayLine();
			InquireStockDayLine();
			InquireForexDayLine(); // Forex dayLineĿǰֻ���ڸ����û�ʹ��
			//InquireEPSSurprise(); // �������ûʲô�ã���ʱֹͣ���¡�
			if (!IsStockDayLineUpdated()) {
				//InquireRTQuote();
			}
		}
		return true;
	}
	return false;
}

bool CFinnhubDataSource::InquireCountryList(void) {
	if (!IsInquiring() && !IsCountryListUpdated()) {
		const CVirtualProductWebDataPtr product = m_FinnhubFactory.
		CreateProduct(gl_pWorldMarket.get(), _ECONOMIC_COUNTRY_LIST_);
		StoreInquiry(product);
		gl_pWorldMarket->SetCurrentFunction(_T("Finnhub country List"));
		SetInquiring(true);
		gl_systemMessage.PushInformationMessage(_T("Inquiring Finnhub economic country List"));
		return true;
	}
	return false;
}

bool CFinnhubDataSource::InquireCompanySymbol(void) {
	const long lExchangeSize = gl_pWorldMarket->GetStockExchangeSize();
	bool fHaveInquiry = false;
	constexpr int iInquiryType = _STOCK_SYMBOLS_;

	if (!IsInquiring() && !IsSymbolUpdated()) {
		CFinnhubStockExchangePtr pExchange;
		static bool s_fInquiringFinnhubStockSymbol = false;
		bool fFound = false;
		long lCurrentStockExchangePos = 0;
		if (!s_fInquiringFinnhubStockSymbol) {
			gl_systemMessage.PushInformationMessage(_T("Inquiring finnhub stock symbol..."));
			s_fInquiringFinnhubStockSymbol = true;
		}
		for (lCurrentStockExchangePos = 0; lCurrentStockExchangePos < lExchangeSize; lCurrentStockExchangePos++) {
			if (!gl_pWorldMarket->GetStockExchange(lCurrentStockExchangePos)->IsUpdated()) {
				pExchange = gl_pWorldMarket->GetStockExchange(lCurrentStockExchangePos);
				if (!gl_finnhubInaccessibleExchange.IsInaccessible(iInquiryType, pExchange->m_strCode)) {
					fFound = true;
					break;
				}
			}
		}
		if (fFound) {
			fHaveInquiry = true;
			const auto product = m_FinnhubFactory.CreateProduct(gl_pWorldMarket.get(), iInquiryType);
			product->SetIndex(lCurrentStockExchangePos);
			StoreInquiry(product);
			gl_pWorldMarket->SetCurrentFunction(_T("Finnhub����������:") + pExchange->m_strCode);
			SetInquiring(true);
			//TRACE("����%s������֤ȯ����\n", pExchange->m_strCode.GetBuffer());
		}
		else {
			s_fInquiringFinnhubStockSymbol = false;
			fHaveInquiry = false;
			SetSymbolUpdated(true);
			TRACE("Finnhub�������������ݲ�ѯ���\n");
			const CString str = "Finnhub�������������ݲ�ѯ���";
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}

bool CFinnhubDataSource::InquireCompanyProfileConcise(void) {
	const long lStockSetSize = gl_pWorldMarket->GetStockSize();
	bool fHaveInquiry = false;
	constexpr int iInquiryType = _COMPANY_PROFILE_CONCISE_;

	ASSERT(gl_pWorldMarket->IsSystemReady());
	if (!IsInquiring() && !IsStockProfileUpdated()) {
		long lCurrentProfilePos;
		bool fFound = false;
		static bool s_fInquiringFinnhubStockProfile = false;
		if (!s_fInquiringFinnhubStockProfile) {
			gl_systemMessage.PushInformationMessage(_T("Inquiring finnhub stock profile..."));
			s_fInquiringFinnhubStockProfile = true;
		}
		for (lCurrentProfilePos = 0; lCurrentProfilePos < lStockSetSize; lCurrentProfilePos++) {
			if (const auto pStock = gl_pWorldMarket->GetStock(lCurrentProfilePos); !pStock->IsCompanyProfileUpdated()) {
				if (!gl_finnhubInaccessibleExchange.IsInaccessible(iInquiryType, pStock->GetExchangeCode())) {
					fFound = true;
					break;
				}
			}
		}
		if (fFound) {
			const auto product = m_FinnhubFactory.CreateProduct(gl_pWorldMarket.get(), iInquiryType);
			product->SetIndex(lCurrentProfilePos);
			StoreInquiry(product);
			gl_pWorldMarket->SetCurrentFunction(_T("���:") + gl_pWorldMarket->GetStock(lCurrentProfilePos)->GetSymbol());
			//TRACE("����%s���\n", m_vWorldStock.at(m_lCurrentProfilePos)->m_strSymbol.GetBuffer());
			SetInquiring(true);
			fHaveInquiry = true;
		}
		else {
			s_fInquiringFinnhubStockProfile = false;
			SetStockProfileUpdated(true);
			TRACE("Finnhub��Ʊ���������\n");
			const CString str = "Finnhub��Ʊ���������";
			gl_systemMessage.PushInformationMessage(str);
			fHaveInquiry = false;
		}
	}
	return fHaveInquiry;
}

/// <summary>
/// ��˾���š�Ŀǰfinnhub.ioֻ�ṩ������˾������
/// </summary>
/// <param name=""></param>
/// <returns></returns>
bool CFinnhubDataSource::InquireCompanyNews(void) {
	const long lStockSetSize = gl_pWorldMarket->GetStockSize();
	bool fHaveInquiry = false;
	constexpr int iInquiryType = _COMPANY_NEWS_;

	ASSERT(gl_pWorldMarket->IsSystemReady());
	if (!IsInquiring() && !IsCompanyNewsUpdated()) {
		long lCurrentCompanyNewsPos;
		bool fFound = false;
		static bool s_fInquiringFinnhubCompanyNews = false;
		if (!s_fInquiringFinnhubCompanyNews) {
			gl_systemMessage.PushInformationMessage(_T("Inquiring finnhub company news..."));
			s_fInquiringFinnhubCompanyNews = true;
		}
		for (lCurrentCompanyNewsPos = 0; lCurrentCompanyNewsPos < lStockSetSize; lCurrentCompanyNewsPos++) {
			const CWorldStockPtr pStock = gl_pWorldMarket->GetStock(lCurrentCompanyNewsPos);
			if (!pStock->IsCompanyNewsUpdated()) {
				if (!gl_finnhubInaccessibleExchange.IsInaccessible(iInquiryType, pStock->GetExchangeCode())) {
					// Ŀǰֻ���������г�
					fFound = true;
					break;
				}
			}
		}
		if (fFound) {
			const CVirtualProductWebDataPtr product = m_FinnhubFactory.CreateProduct(gl_pWorldMarket.get(), iInquiryType);
			product->SetIndex(lCurrentCompanyNewsPos);
			StoreInquiry(product);
			gl_pWorldMarket->SetCurrentFunction(_T("��˾����:") + gl_pWorldMarket->GetStock(lCurrentCompanyNewsPos)->GetSymbol());
			//TRACE("����%s��˾����\n", m_vWorldStock.at(m_lCurrentProfilePos)->m_strSymbol.GetBuffer());
			SetInquiring(true);
			fHaveInquiry = true;
		}
		else {
			s_fInquiringFinnhubCompanyNews = false;
			SetCompanyNewsUpdated(true);
			TRACE("Finnhub��˾���Ÿ������\n");
			const CString str = "Finnhub��˾���Ÿ������";
			gl_systemMessage.PushInformationMessage(str);
			fHaveInquiry = false;
		}
	}
	return fHaveInquiry;
}

bool CFinnhubDataSource::InquireCompanyBasicFinancial(void) {
	const long lStockSetSize = gl_pWorldMarket->GetStockSize();
	bool fHaveInquiry = false;
	CVirtualProductWebDataPtr product = nullptr;
	constexpr int iInquiryType = _BASIC_FINANCIALS_;

	ASSERT(gl_pWorldMarket->IsSystemReady());
	if (!IsInquiring() && !IsStockBasicFinancialUpdated()) {
		static bool s_fInquiringFinnhubCompanyBasicFinancial = false;
		bool fFound = false;
		long lCurrentBasicFinancialsPos;
		if (!s_fInquiringFinnhubCompanyBasicFinancial) {
			gl_systemMessage.PushInformationMessage(_T("Inquiring finnhub stock basic financials..."));
			s_fInquiringFinnhubCompanyBasicFinancial = true;
		}
		for (lCurrentBasicFinancialsPos = 0; lCurrentBasicFinancialsPos < lStockSetSize; lCurrentBasicFinancialsPos++) {
			const CWorldStockPtr pStock = gl_pWorldMarket->GetStock(lCurrentBasicFinancialsPos);
			if (!pStock->IsBasicFinancialUpdated()) {
				if (!gl_finnhubInaccessibleExchange.IsInaccessible(iInquiryType, pStock->GetExchangeCode())) {
					fFound = true;
					break;
				}
			}
		}
		if (fFound) {
			product = m_FinnhubFactory.CreateProduct(gl_pWorldMarket.get(), iInquiryType);
			product->SetIndex(lCurrentBasicFinancialsPos);
			StoreInquiry(product);
			gl_pWorldMarket->SetCurrentFunction(
				_T("��������:") + gl_pWorldMarket->GetStock(iInquiryType)->GetSymbol());
			//TRACE("����%s���\n", m_vWorldStock.at(m_lCurrentProfilePos)->m_strSymbol.GetBuffer());
			SetInquiring(true);
			fHaveInquiry = true;
		}
		else {
			s_fInquiringFinnhubCompanyBasicFinancial = false;
			SetStockBasicFinancialUpdated(true);
			TRACE("Finnhub��Ʊ������������������\n");
			const CString str = "Finnhub��Ʊ������������������";
			gl_systemMessage.PushInformationMessage(str);
			fHaveInquiry = false;
		}
	}
	return fHaveInquiry;
}

bool CFinnhubDataSource::InquireStockDayLine(void) {
	const long lStockSetSize = gl_pWorldMarket->GetStockSize();
	bool fHaveInquiry = false;
	constexpr int iInquiryType = _STOCK_PRICE_CANDLES_;

	ASSERT(gl_pWorldMarket->IsSystemReady());
	if (!IsInquiring() && !IsStockDayLineUpdated()) {
		CWorldStockPtr pStock;
		bool fFound = false;
		static bool s_fInquiringFinnhubStockDayLine = false;
		if (!s_fInquiringFinnhubStockDayLine) {
			gl_systemMessage.PushInformationMessage(_T("Inquiring finnhub stock day line..."));
			s_fInquiringFinnhubStockDayLine = true;
		}
		for (m_lCurrentUpdateDayLinePos = 0; m_lCurrentUpdateDayLinePos < lStockSetSize; m_lCurrentUpdateDayLinePos++) {
			pStock = gl_pWorldMarket->GetStock(m_lCurrentUpdateDayLinePos);
			if (pStock->IsDayLineNeedUpdate()) {
				// Ŀǰ����˻�ֻ�����������г��Ĺ�Ʊ���ߡ�
				if (!gl_finnhubInaccessibleExchange.IsInaccessible(iInquiryType, pStock->GetExchangeCode())) {
					fFound = true;
					break;
				}
			}
		}
		if (fFound) {
			fHaveInquiry = true;
			const auto product = m_FinnhubFactory.CreateProduct(gl_pWorldMarket.get(), iInquiryType);
			product->SetIndex(m_lCurrentUpdateDayLinePos);
			StoreInquiry(product);
			SetInquiring(true);
			gl_pWorldMarket->SetCurrentFunction(_T("����:") + pStock->GetSymbol());
			//TRACE("����%s��������\n", pStock->GetSymbol().GetBuffer());
		}
		else {
			s_fInquiringFinnhubStockDayLine = false;
			fHaveInquiry = false;
			SetDayLineUpdated(true);
			m_lCurrentUpdateDayLinePos = 0; // ���ô����������е��������ݸ���һ������ʱ��Ҫ����24Сʱ���ʱ��ָ��¼��ɡ�
			TRACE("Finnhub���߸�����ϣ����¿�ʼ����\n");
			const CString str = "US Market������ʷ���ݸ������";
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}

bool CFinnhubDataSource::InquireInsiderTransaction(void) {
	long lStockSetSize = gl_pWorldMarket->GetStockSize();
	bool fHaveInquiry = false;
	constexpr int iInquiryType = _INSIDER_TRANSACTION_;

	ASSERT(gl_pWorldMarket->IsSystemReady());
	if (!IsInquiring() && !IsInsiderTransactionUpdated()) {
		long lCurrentUpdateInsiderTransactionPos = 0;
		CWorldStockPtr pStock;
		bool fFound = false;
		static bool s_fInquiringFinnhubStockInsiderTransaction = false;
		if (!s_fInquiringFinnhubStockInsiderTransaction) {
			gl_systemMessage.PushInformationMessage(_T("Inquiring finnhub stock insider transaction..."));
			s_fInquiringFinnhubStockInsiderTransaction = true;
		}
		for (lCurrentUpdateInsiderTransactionPos = 0; lCurrentUpdateInsiderTransactionPos < lStockSetSize;
		     lCurrentUpdateInsiderTransactionPos++) {
			pStock = gl_pWorldMarket->GetStock(lCurrentUpdateInsiderTransactionPos);
			if (pStock->IsInsiderTransactionNeedUpdate()) {
				// Ŀǰ����˻�ֻ�����������г��Ĺ�Ʊ���ߡ�
				if (!gl_finnhubInaccessibleExchange.IsInaccessible(iInquiryType, pStock->GetExchangeCode())) {
					fFound = true;
					break;
				}
			}
		}
		if (fFound) {
			fHaveInquiry = true;
			const CVirtualProductWebDataPtr product = m_FinnhubFactory.CreateProduct(gl_pWorldMarket.get(), iInquiryType);
			product->SetIndex(lCurrentUpdateInsiderTransactionPos);
			StoreInquiry(product);
			SetInquiring(true);
			gl_pWorldMarket->SetCurrentFunction(_T("�ڲ�����:") + pStock->GetSymbol());
			//TRACE("����%s �ڲ���������\n", pStock->GetSymbol().GetBuffer());
		}
		else {
			s_fInquiringFinnhubStockInsiderTransaction = false;
			fHaveInquiry = false;
			SetInsiderTransactionUpdated(true);
			TRACE("FinnhubInsider Transaction�������\n");
			const CString str = "US Market Insider Transaction���ݸ������";
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}

bool CFinnhubDataSource::InquireInsiderSentiment(void) {
	long lStockSetSize = gl_pWorldMarket->GetStockSize();
	bool fHaveInquiry = false;
	constexpr int iInquiryType = _INSIDER_SENTIMENT_;

	ASSERT(gl_pWorldMarket->IsSystemReady());
	if (!IsInquiring() && !IsInsiderSentimentUpdated()) {
		long lCurrentUpdateInsiderSentimentPos = 0;
		CWorldStockPtr pStock;
		bool fFound = false;
		static bool s_fInquiringFinnhubStockInsiderSentiment = false;
		if (!s_fInquiringFinnhubStockInsiderSentiment) {
			gl_systemMessage.PushInformationMessage(_T("Inquiring finnhub stock insider sentiment..."));
			s_fInquiringFinnhubStockInsiderSentiment = true;
		}
		for (lCurrentUpdateInsiderSentimentPos = 0; lCurrentUpdateInsiderSentimentPos < lStockSetSize;
		     lCurrentUpdateInsiderSentimentPos++) {
			pStock = gl_pWorldMarket->GetStock(lCurrentUpdateInsiderSentimentPos);
			if (!gl_finnhubInaccessibleExchange.IsInaccessible(iInquiryType, pStock->GetExchangeCode())) {
				if (pStock->IsInsiderSentimentNeedUpdate()) {
					fFound = true;
					break;
				}
			}
		}
		if (fFound) {
			fHaveInquiry = true;
			const CVirtualProductWebDataPtr product = m_FinnhubFactory.CreateProduct(gl_pWorldMarket.get(), iInquiryType);
			product->SetIndex(lCurrentUpdateInsiderSentimentPos);
			StoreInquiry(product);
			SetInquiring(true);
			gl_pWorldMarket->SetCurrentFunction(_T("�ڲ���������:") + pStock->GetSymbol());
			//TRACE("����%s �ڲ�������������\n", pStock->GetSymbol().GetBuffer());
		}
		else {
			s_fInquiringFinnhubStockInsiderSentiment = false;
			fHaveInquiry = false;
			SetInsiderSentimentUpdated(true);
			TRACE("FinnhubInsider Sentiment�������\n");
			const CString str = "US Market Insider Sentiment���ݸ������";
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}

bool CFinnhubDataSource::InquireRTQuote(void) {
	if (!IsInquiring()) {
		static long s_lCurrentRTDataQuotePos = 0;
		ASSERT(gl_pWorldMarket->IsSystemReady());
		const CVirtualProductWebDataPtr product = m_FinnhubFactory.CreateProduct(gl_pWorldMarket.get(), _STOCK_PRICE_QUOTE_);
		product->SetIndex(s_lCurrentRTDataQuotePos);
		StoreInquiry(product);
		SetInquiring(true);
		s_lCurrentRTDataQuotePos++;
		if (s_lCurrentRTDataQuotePos == gl_pWorldMarket->GetStockSize()) s_lCurrentRTDataQuotePos = 0;
		gl_pWorldMarket->SetCurrentFunction(
			_T("Finnhub stock RT: ") + gl_pWorldMarket->GetStock(s_lCurrentRTDataQuotePos)->GetSymbol());
		TRACE("����%sʵʱ����\n", gl_pWorldMarket->GetStock(s_lCurrentRTDataQuotePos)->GetSymbol().GetBuffer());
	}
	return true;
}

bool CFinnhubDataSource::InquirePeer(void) {
	CWorldStockPtr pStock = nullptr;
	long lStockSetSize = gl_pWorldMarket->GetStockSize();
	bool fHaveInquiry = false;
	constexpr int iInquiryType = _PEERS_;

	ASSERT(gl_pWorldMarket->IsSystemReady());
	if (!IsInquiring() && !IsPeerUpdated()) {
		long lCurrentUpdatePeerPos = 0;
		bool fFound = false;
		static bool s_fInquiringFinnhubStockPeer = false;
		if (!s_fInquiringFinnhubStockPeer) {
			gl_systemMessage.PushInformationMessage(_T("Inquiring finnhub stock peer..."));
			s_fInquiringFinnhubStockPeer = true;
		}
		for (lCurrentUpdatePeerPos = 0; lCurrentUpdatePeerPos < lStockSetSize; lCurrentUpdatePeerPos++) {
			pStock = gl_pWorldMarket->GetStock(lCurrentUpdatePeerPos);
			if (!pStock->IsPeerUpdated()) {
				if (!gl_finnhubInaccessibleExchange.IsInaccessible(iInquiryType, pStock->GetExchangeCode())) {
					fFound = true;
					break;
				}
			}
		}
		if (fFound) {
			fHaveInquiry = true;
			CVirtualProductWebDataPtr product = m_FinnhubFactory.CreateProduct(gl_pWorldMarket.get(), iInquiryType);
			product->SetIndex(lCurrentUpdatePeerPos);
			StoreInquiry(product);
			SetInquiring(true);
			gl_pWorldMarket->SetCurrentFunction(_T("Peer:") + gl_pWorldMarket->GetStock(lCurrentUpdatePeerPos)->GetSymbol());
			//TRACE("����%s Peer����\n", GetStock(lCurrentUpdatePeerPos)->GetSymbol().GetBuffer());
		}
		else {
			s_fInquiringFinnhubStockPeer = false;
			fHaveInquiry = false;
			SetPeerUpdated(true);
			TRACE("Finnhub Peers�������\n");
			const CString str = "Finnhub Peer Updated";
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}

bool CFinnhubDataSource::InquireEconomicCalendar(void) {
	constexpr int iInquiryType = _ECONOMIC_CALENDAR_;

	if (!IsInquiring() && !IsEconomicCalendarUpdated()) {
		if (!gl_finnhubInaccessibleExchange.IsInaccessible(iInquiryType, _T("ALL"))) {
			const CVirtualProductWebDataPtr product = m_FinnhubFactory.CreateProduct(gl_pWorldMarket.get(), iInquiryType);
			StoreInquiry(product);
			SetInquiring(true);
		}
		else { SetEconomicCalendarUpdated(true); }
		gl_pWorldMarket->SetCurrentFunction(_T("Finnhub ecomomic calendar"));
		return true;
	}
	return false;
}

bool CFinnhubDataSource::InquireEPSSurprise(void) {
	long lStockSetSize = gl_pWorldMarket->GetStockSize();
	bool fHaveInquiry = false;
	constexpr int iInquiryType = _STOCK_ESTIMATES_EPS_SURPRISE_;

	ASSERT(gl_pWorldMarket->IsSystemReady());
	if (!IsInquiring() && !IsEPSSurpriseUpdated()) {
		bool fFound = false;
		for (m_lCurrentUpdateEPSSurprisePos = 0; m_lCurrentUpdateEPSSurprisePos < lStockSetSize;
		     m_lCurrentUpdateEPSSurprisePos++) {
			const CWorldStockPtr pStock = gl_pWorldMarket->GetStock(m_lCurrentUpdateEPSSurprisePos);
			if (!pStock->IsEPSSurpriseUpdated()) {
				if (!gl_finnhubInaccessibleExchange.IsInaccessible(iInquiryType, pStock->GetExchangeCode())) {
					fFound = true;
					break;
				}
			}
		}
		if (fFound) {
			fHaveInquiry = true;
			const CVirtualProductWebDataPtr product = m_FinnhubFactory.CreateProduct(gl_pWorldMarket.get(), iInquiryType);
			product->SetIndex(m_lCurrentUpdateEPSSurprisePos);
			StoreInquiry(product);
			SetInquiring(true);
			gl_pWorldMarket->GetStock(m_lCurrentUpdateEPSSurprisePos)->SetEPSSurpriseUpdated(true);
			TRACE("����%s EPS Surprise����\n", gl_pWorldMarket->GetStock(m_lCurrentUpdateEPSSurprisePos)->GetSymbol().GetBuffer());
		}
		else {
			SetEPSSurpriseUpdated(true);
			TRACE("Finnhub EPS Surprise�������\n");
			const CString str = "Finnhub EPS Surprise Updated";
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}

bool CFinnhubDataSource::InquireForexExchange(void) {
	if (!IsInquiring() && !IsForexExchangeUpdated()) {
		StoreInquiry(m_FinnhubFactory.CreateProduct(gl_pWorldMarket.get(), _FOREX_EXCHANGE_));
		SetInquiring(true);
		gl_pWorldMarket->SetCurrentFunction(_T("Finnhub forex exchange"));
		gl_systemMessage.PushInformationMessage(_T("Inquiring Finnhub forex exchange"));
		return true;
	}
	return false;
}

bool CFinnhubDataSource::InquireForexSymbol(void) {
	if (!IsInquiring() && !IsForexSymbolUpdated()) {
		static long s_lCurrentForexExchangePos = 0;
		const CVirtualProductWebDataPtr product = m_FinnhubFactory.CreateProduct(gl_pWorldMarket.get(), _FOREX_SYMBOLS_);
		product->SetIndex(s_lCurrentForexExchangePos++);
		StoreInquiry(product);
		SetInquiring(true);
		if (s_lCurrentForexExchangePos >= gl_pWorldMarket->GetForexExchangeSize()) {
			SetForexSymbolUpdated(true);
			s_lCurrentForexExchangePos = 0;
			gl_pWorldMarket->SetCurrentFunction(_T("Finnhub forex symbol"));
			gl_systemMessage.PushInformationMessage(_T("Inquiring Finnhub Forex symbols..."));
		}
		return true;
	}
	return false;
}

bool CFinnhubDataSource::InquireForexDayLine(void) {
	const long lStockSetSize = gl_pWorldMarket->GetForexSymbolSize();
	bool fHaveInquiry = false;
	constexpr int iInquiryType = _FOREX_CANDLES_;

	ASSERT(gl_pWorldMarket->IsSystemReady());
	if (!IsInquiring() && !IsForexDayLineUpdated()) {
		CForexSymbolPtr pForexSymbol = nullptr;
		long lCurrentUpdateForexDayLinePos;
		bool fFound = false;
		static bool s_fInquiringFinnhubForexDayLine = false;
		if (!s_fInquiringFinnhubForexDayLine) {
			gl_systemMessage.PushInformationMessage(_T("Inquiring finnhub forex day line..."));
			s_fInquiringFinnhubForexDayLine = true;
		}
		for (lCurrentUpdateForexDayLinePos = 0; lCurrentUpdateForexDayLinePos < lStockSetSize; lCurrentUpdateForexDayLinePos
		     ++) {
			pForexSymbol = gl_pWorldMarket->GetForexSymbol(lCurrentUpdateForexDayLinePos);
			if (pForexSymbol->IsDayLineNeedUpdate()) {
				if (!gl_finnhubInaccessibleExchange.IsInaccessible(iInquiryType, pForexSymbol->GetExchangeCode())) {
					fFound = true;
					break;
				}
			}
		}
		if (fFound) {
			fHaveInquiry = true;
			const CVirtualProductWebDataPtr product = m_FinnhubFactory.CreateProduct(gl_pWorldMarket.get(), iInquiryType);
			product->SetIndex(lCurrentUpdateForexDayLinePos);
			StoreInquiry(product);
			gl_pWorldMarket->SetCurrentFunction(_T("Finnhub Forex���ߣ�") + pForexSymbol->GetSymbol());
			SetInquiring(true);
			gl_pWorldMarket->SetCurrentFunction(_T("����:") + pForexSymbol->GetSymbol());
			TRACE("����%s��������\n", pForexSymbol->GetSymbol().GetBuffer());
		}
		else {
			s_fInquiringFinnhubForexDayLine = false;
			SetForexDayLineUpdated(true);
			TRACE("Finnhub Forex���߸������\n");
			const CString str = "Forex DayLine Updated";
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}

bool CFinnhubDataSource::InquireCryptoExchange(void) {
	if (!IsInquiring() && !IsCryptoExchangeUpdated()) {
		StoreInquiry(m_FinnhubFactory.CreateProduct(gl_pWorldMarket.get(), _CRYPTO_EXCHANGE_));
		SetInquiring(true);
		gl_pWorldMarket->SetCurrentFunction(_T("Finnhub crypto exchange"));
		gl_systemMessage.PushInformationMessage(_T("Inquiring Finnhub crypto exchange"));
		return true;
	}
	return false;
}

bool CFinnhubDataSource::InquireCryptoSymbol(void) {
	if (!IsInquiring() && !IsCryptoSymbolUpdated()) {
		static long s_lCurrentCryptoExchangePos = 0;
		const CVirtualProductWebDataPtr product = m_FinnhubFactory.CreateProduct(gl_pWorldMarket.get(), _CRYPTO_SYMBOLS_);
		product->SetIndex(s_lCurrentCryptoExchangePos++);
		StoreInquiry(product);
		SetInquiring(true);
		if (s_lCurrentCryptoExchangePos >= gl_pWorldMarket->GetCryptoExchangeSize()) {
			SetCryptoSymbolUpdated(true);
			gl_pWorldMarket->SetCurrentFunction(_T("Finnhub crypto synmbol"));
			gl_systemMessage.PushInformationMessage(_T("Inquiring Finnhub Crypto sysbols"));
			s_lCurrentCryptoExchangePos = 0;
		}
		return true;
	}
	return false;
}

bool CFinnhubDataSource::InquireCryptoDayLine(void) {
	const long lStockSetSize = gl_pWorldMarket->GetFinnhubCryptoSymbolSize();
	bool fHaveInquiry = false;
	constexpr int iInquiryType = _CRYPTO_CANDLES_;

	ASSERT(gl_pWorldMarket->IsSystemReady());
	if (!IsInquiring() && !IsCryptoDayLineUpdated()) {
		CFinnhubCryptoSymbolPtr pCryptoSymbol;
		long lCurrentUpdateCryptoDayLinePos;
		bool fFound = false;
		static bool s_fInquiringFinnhubCryptoDayLine = false;
		if (!s_fInquiringFinnhubCryptoDayLine) {
			gl_systemMessage.PushInformationMessage(_T("Inquiring finnhub forex day line..."));
			s_fInquiringFinnhubCryptoDayLine = true;
		}
		for (lCurrentUpdateCryptoDayLinePos = 0; lCurrentUpdateCryptoDayLinePos < lStockSetSize;
		     lCurrentUpdateCryptoDayLinePos++) {
			pCryptoSymbol = gl_pWorldMarket->GetFinnhubCryptoSymbol(lCurrentUpdateCryptoDayLinePos);
			if (pCryptoSymbol->IsDayLineNeedUpdate()) {
				if (!gl_finnhubInaccessibleExchange.IsInaccessible(iInquiryType, pCryptoSymbol->GetExchangeCode())) {
					fFound = true;
					break;
				}
			}
		}
		if (fFound) {
			fHaveInquiry = true;
			const CVirtualProductWebDataPtr product = m_FinnhubFactory.CreateProduct(gl_pWorldMarket.get(), iInquiryType);
			product->SetIndex(lCurrentUpdateCryptoDayLinePos);
			StoreInquiry(product);
			gl_pWorldMarket->SetCurrentFunction(_T("Finnhub Crypto���ߣ�") + pCryptoSymbol->GetSymbol());
			SetInquiring(true);
			gl_pWorldMarket->SetCurrentFunction(_T("����:") + pCryptoSymbol->GetSymbol());
			//TRACE("����%s��������\n", pCryptoSymbol->GetSymbol().GetBuffer());
		}
		else {
			s_fInquiringFinnhubCryptoDayLine = false;
			SetCryptoDayLineUpdated(true);
			TRACE("Finnhub Crypto���߸������\n");
			const CString str = "Crypto DayLine Updated";
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}
