#include "FinnhubDataSource.h"
#include"pch.h"

#include"TimeConvert.h"

#include"FinnhubInquiryType.h"

#include "FinnhubDataSource.h"

#include"WorldMarket.h"

CFinnhubDataSource::CFinnhubDataSource(void) {
	// ���裨Ҳ�޷���ÿ�ո��µı�����������
	m_fEPSSurpriseUpdated = true;
	m_lCurrentUpdateEPSSurprisePos = 0;
	m_lCurrentUpdateDayLinePos = 0; // ����֤ȯ���������ж�ʮ��֮�࣬�޷���һ��֮�ڸ����꣬�ʲ������ô�������

	Reset();
}

CFinnhubDataSource::~CFinnhubDataSource(void) {
}

bool CFinnhubDataSource::Reset(void) {
	CDataSource::Reset();

	// Finnhub�������������ݱ�ʶ��ÿ����Ҫ���á�
	m_fCountryListUpdated = false;
	m_fSymbolUpdated = false; // ÿ����Ҫ���´���
	m_fStockProfileUpdated = false;
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

	if (GetDayOfWeek() == 6) { // ÿ�ܵ�����������һ��EPSSurprise
		m_lCurrentUpdateEPSSurprisePos = 0;
		SetEPSSurpriseUpdated(false);
	}

	return true;
}

bool CFinnhubDataSource::UpdateStatus(void) {
	switch (m_pCurrentProduct->GetProductType()) {
	case __COMPANY_PROFILE__: // Premium ����˻��޷���ȡ����Ϣ��sandboxģʽ�ܶ�ȡ���������Ǵ���ģ�ֻ�����ڲ��ԡ�
		m_fStockProfileUpdated = true;
		break;
	case __COMPANY_PROFILE_CONCISE__:
		m_fStockProfileUpdated = true;
		break;
	case  __STOCK_SYMBOLS__:
		break;
	case __COMPANY_EXECTIVE__: // Premium
		break;
	case __MARKET_NEWS__:
		break;
	case __COMPANY_NEWS__:
		break;
	case __PRESS_RELEASE__: // Premium
		break;
	case __NEWS_SENTIMENT__:
		break;
	case __PEERS__:
		m_fPeerUpdated = true;
		break;
	case __BASIC_FINANCIALS__:
		break;
	case __INSIDER_TRANSACTION__:
		break;
	case __INSIDER_SENTIMENT__:
		break;
	case __OWNERSHIP__: // Premium
		break;
	case __FUND_OWNERSHIP__: // jPremium
		break;
	case __FINANCIAL__: // Premium
		break;
	case __FINAICIAL_AS_REPORTED__:
		break;
	case __SEC_FILINGS__:
		break;
	case __INTERNATIONAL_FILINGS__: // Premium
		break;
	case __SEC_SENTIMENT_ANALYSIS__: // Premium
		break;
	case __SIMILARITY_INDEX__: // Premium
		break;
	case __IPO_CALENDAR__:
		break;
	case __DIVIDENDS__: // Premium
		break;
	case __STOCK_ESTIMATES_RECOMMENDATION_TRENDS__:
		break;
	case __STOCK_ESTIMATES_PRICE_TARGET__:
		break;
	case __STOCK_ESTIMATES_UPGRADE_DOWNGRADE__: // Premium
		break;
	case __STOCK_ESTIMATES_REVENUE_EXTIMATES__: // Premium
		break;
	case __STOCK_ESTIMATES_EPS_EXTIMATES__:// Premium
		break;
	case __STOCK_ESTIMATES_EPS_SURPRISE__:
		break;
	case __STOCK_ESTIMATES_EARNING_CALENDAR__:
		break;
	case __STOCK_PRICE_QUOTE__:
		break;
	case __STOCK_PRICE_CANDLES__:
		break;
	case __STOCK_PRICE_TICK_DATA__: // Premium
		break;
	case __STOCK_PRICE_LAST_BID_ASK__: // Premium
		break;
	case __STOCK_PRICE_SPLITS__:
		break;
	case __FOREX_EXCHANGE__:
		m_fForexExchangeUpdated = true;
		break;
	case __FOREX_SYMBOLS__:
		break;
	case __FOREX_CANDLES__:
		break;
	case __FOREX_ALL_RATES__:
		break;
	case __CRYPTO_EXCHANGE__:
		m_fCryptoExchangeUpdated = true;
		break;
	case __CRYPTO_SYMBOLS__:
		break;
	case __CRYPTO_CANDLES__:
		break;
	case __ECONOMIC_COUNTRY_LIST__:
		m_fCountryListUpdated = true;
		break;
	case __ECONOMIC_CALENDAR__:
		m_fEconomicCalendarUpdated = true;
		break;
	default:
		TRACE("δ����ָ��%d\n", m_pCurrentProduct->GetProductType());
		gl_systemMessage.PushErrorMessage(_T("Finnhub productδʵ��"));
		break;
	}
	return true;
}

bool CFinnhubDataSource::Inquiry(long lCurrentTime) {
	static int s_iCountfinnhubLimit = 100; // Finnhub.ioÿ1.2����������һ�Σ��Է�ֹ����Ƶ�ʹ��ߵ��������ʼֵ��Ϊ��������10���ſ�ʼ��
	int i = 0;
	if (--s_iCountfinnhubLimit < 0) {
		if (!IsInquiring() && !m_pWebInquiry->IsWebError()) InquiryFinnhub(lCurrentTime);
		if (IsInquiring()) {
			i = gl_systemConfigeration.GetWorldMarketFinnhubInquiryTime();
			s_iCountfinnhubLimit = gl_systemConfigeration.GetWorldMarketFinnhubInquiryTime() / 100; // ����������������ݣ������ü��������Ա�������һ�Ρ�
		}
	}
	return true;
}

bool CFinnhubDataSource::InquiryFinnhub(long lCurrentTime) {
	if (((lCurrentTime < 165700) || (lCurrentTime > 170500))) { // ������ʱ����ϵͳ���ʶ���ʱ���������������Ϣ��
		InquiryCountryList();
		InquiryForexExchange();
		InquiryCryptoExchange();
		InquiryCompanySymbol(); // ��һ���������������뵱��֤ȯ����
		InquiryForexSymbol();
		InquiryCryptoSymbol();
		//InquiryEconomicCalendar();

		// ����Finnhub������Ϣ�����񣬽�Ҫ����������Ա�֤���г�ʱ���賿ʮ���Ӻ�ִ�С������ܹ���֤�������г�ʱ����ִ�в�ѯ����
		if (gl_pWorldMarket->IsSystemReady()) {
			InquiryCompanyProfileConcise();
			InquiryCompanyBasicFinancial();
			InquiryPeer();
			InquiryInsiderTransaction();
			InquiryInsiderSentiment();
			InquiryCryptoDayLine();
			InquiryStockDayLine();
			//InquiryForexDayLine(); // Forex daylineĿǰֻ���ڸ����û�ʹ��
			//InquiryEPSSurprise(); // �������ûʲô�ã���ʱֹͣ���¡�
			if (!IsStockDayLineUpdated()) {
				//InquiryRTQuote();
			}
		}
		return true;
	}
	return false;
}

bool CFinnhubDataSource::InquiryCountryList(void) {
	if (!IsCountryListUpdated() && !IsInquiring()) {
		CProductWebSourceDataPtr p = m_FinnhubFactory.CreateProduct(gl_pWorldMarket.get(), __ECONOMIC_COUNTRY_LIST__);
		StoreInquiry(p);
		gl_pWorldMarket->SetCurrentFunction(_T("Finnhub country List"));
		SetInquiring(true);
		gl_systemMessage.PushInformationMessage(_T("Inquiring Finnhub economic country List"));
		return true;
	}
	return false;
}

bool CFinnhubDataSource::InquiryCompanySymbol(void) {
	static bool s_fInquiringFinnhubStockSymbol = false;
	bool fFound = false;
	CFinnhubStockExchangePtr pExchange;
	CString str = _T("");
	long lExchangeSize = gl_pWorldMarket->GetStockExchangeSize();
	bool fHaveInquiry = false;
	CProductWebSourceDataPtr p = nullptr;
	long lCurrentStockExchangePos = 0;

	if (!IsSymbolUpdated() && !IsInquiring()) {
		if (!s_fInquiringFinnhubStockSymbol) {
			gl_systemMessage.PushInformationMessage(_T("Inquiring finnhub stock symbol..."));
			s_fInquiringFinnhubStockSymbol = true;
		}
		for (lCurrentStockExchangePos = 0; lCurrentStockExchangePos < lExchangeSize; lCurrentStockExchangePos++) {
			if (!gl_pWorldMarket->GetStockExchange(lCurrentStockExchangePos)->IsUpdated()) {
				pExchange = gl_pWorldMarket->GetStockExchange(lCurrentStockExchangePos);
				fFound = true;
				break;
			}
		}
		if (fFound) {
			fHaveInquiry = true;
			p = m_FinnhubFactory.CreateProduct(gl_pWorldMarket.get(), __STOCK_SYMBOLS__);
			p->SetIndex(lCurrentStockExchangePos);
			StoreInquiry(p);
			gl_pWorldMarket->SetCurrentFunction(_T("Finnhub����������:") + pExchange->m_strCode);
			SetInquiring(true);
			pExchange->SetUpdated(true);
			//TRACE("����%s������֤ȯ����\n", pExchange->m_strCode.GetBuffer());
		}
		else {
			s_fInquiringFinnhubStockSymbol = false;
			fHaveInquiry = false;
			SetSymbolUpdated(true);
			TRACE("Finnhub�������������ݲ�ѯ���\n");
			str = _T("Finnhub�������������ݲ�ѯ���");
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}

bool CFinnhubDataSource::InquiryCompanyProfileConcise(void) {
	static bool s_fInquiringFinnhubStockProfile = false;
	bool fFound = false;
	long lStockSetSize = gl_pWorldMarket->GetStockSize();
	CString str = _T("");
	bool fHaveInquiry = false;
	CProductWebSourceDataPtr p = nullptr;
	long lCurrentProfilePos;

	ASSERT(gl_pWorldMarket->IsSystemReady());
	if (!IsStockProfileUpdated() && !IsInquiring()) {
		if (!s_fInquiringFinnhubStockProfile) {
			gl_systemMessage.PushInformationMessage(_T("Inquiring finnhub stock profile..."));
			s_fInquiringFinnhubStockProfile = true;
		}
		for (lCurrentProfilePos = 0; lCurrentProfilePos < lStockSetSize; lCurrentProfilePos++) {
			if (!gl_pWorldMarket->GetStock(lCurrentProfilePos)->IsProfileUpdated()) {
				fFound = true;
				break;
			}
		}
		if (fFound) {
			p = m_FinnhubFactory.CreateProduct(gl_pWorldMarket.get(), __COMPANY_PROFILE_CONCISE__);
			p->SetIndex(lCurrentProfilePos);
			StoreInquiry(p);
			gl_pWorldMarket->SetCurrentFunction(_T("���:") + gl_pWorldMarket->GetStock(lCurrentProfilePos)->GetSymbol());
			//TRACE("����%s���\n", m_vWorldStock.at(m_lCurrentProfilePos)->m_strSymbol.GetBuffer());
			SetInquiring(true);
			fHaveInquiry = true;
		}
		else {
			s_fInquiringFinnhubStockProfile = false;
			SetStockProfileUpdated(true);
			TRACE("Finnhub��Ʊ���������\n");
			str = _T("Finnhub��Ʊ���������");
			gl_systemMessage.PushInformationMessage(str);
			fHaveInquiry = false;
		}
	}
	return fHaveInquiry;
}

bool CFinnhubDataSource::InquiryCompanyBasicFinancial(void) {
	static bool s_fInquiringFinnhubCompanyBasicFinancial = false;
	bool fFound = false;
	long lStockSetSize = gl_pWorldMarket->GetStockSize();
	CString str = _T("");
	bool fHaveInquiry = false;
	CProductWebSourceDataPtr p = nullptr;
	long lCurrentBasicFinncialsPos;

	ASSERT(gl_pWorldMarket->IsSystemReady());
	if (!IsStockBasicFinancialUpdated() && !IsInquiring()) {
		if (!s_fInquiringFinnhubCompanyBasicFinancial) {
			gl_systemMessage.PushInformationMessage(_T("Inquiring finnhub stock basic financials..."));
			s_fInquiringFinnhubCompanyBasicFinancial = true;
		}
		for (lCurrentBasicFinncialsPos = 0; lCurrentBasicFinncialsPos < lStockSetSize; lCurrentBasicFinncialsPos++) {
			if (!gl_pWorldMarket->GetStock(lCurrentBasicFinncialsPos)->IsBasicFinancialUpdated()) {
				fFound = true;
				break;
			}
		}
		if (fFound) {
			p = m_FinnhubFactory.CreateProduct(gl_pWorldMarket.get(), __BASIC_FINANCIALS__);
			p->SetIndex(lCurrentBasicFinncialsPos);
			StoreInquiry(p);
			gl_pWorldMarket->SetCurrentFunction(_T("��������:") + gl_pWorldMarket->GetStock(lCurrentBasicFinncialsPos)->GetSymbol());
			//TRACE("����%s���\n", m_vWorldStock.at(m_lCurrentProfilePos)->m_strSymbol.GetBuffer());
			SetInquiring(true);
			fHaveInquiry = true;
		}
		else {
			s_fInquiringFinnhubCompanyBasicFinancial = false;
			SetStockBasicFinancialUpdated(true);
			TRACE("Finnhub��Ʊ������������������\n");
			str = _T("Finnhub��Ʊ������������������");
			gl_systemMessage.PushInformationMessage(str);
			fHaveInquiry = false;
		}
	}
	return fHaveInquiry;
}

bool CFinnhubDataSource::InquiryStockDayLine(void) {
	static bool s_fInquiringFinnhubStockDayLine = false;
	bool fFound = false;
	CWorldStockPtr pStock;
	CString str = _T("");
	long lStockSetSize = gl_pWorldMarket->GetStockSize();
	bool fHaveInquiry = false;
	CProductWebSourceDataPtr p = nullptr;

	ASSERT(gl_pWorldMarket->IsSystemReady());
	if (!IsStockDayLineUpdated() && !IsInquiring()) {
		if (!s_fInquiringFinnhubStockDayLine) {
			gl_systemMessage.PushInformationMessage(_T("Inquiring finnhub stock day line..."));
			s_fInquiringFinnhubStockDayLine = true;
		}
		for (m_lCurrentUpdateDayLinePos = 0; m_lCurrentUpdateDayLinePos < lStockSetSize; m_lCurrentUpdateDayLinePos++) {
			pStock = gl_pWorldMarket->GetStock(m_lCurrentUpdateDayLinePos);
			if (pStock->IsUSMarket() && pStock->IsDayLineNeedUpdate()) { // Ŀǰ����˻�ֻ�����������г��Ĺ�Ʊ���ߡ�
				fFound = true;
				break;
			}
		}
		if (fFound) {
			fHaveInquiry = true;
			p = m_FinnhubFactory.CreateProduct(gl_pWorldMarket.get(), __STOCK_PRICE_CANDLES__);
			p->SetIndex(m_lCurrentUpdateDayLinePos);
			StoreInquiry(p);
			SetInquiring(true);
			pStock->SetDayLineNeedUpdate(false);
			gl_pWorldMarket->SetCurrentFunction(_T("����:") + pStock->GetSymbol());
			//TRACE("����%s��������\n", pStock->GetSymbol().GetBuffer());
		}
		else {
			s_fInquiringFinnhubStockDayLine = false;
			fHaveInquiry = false;
			SetDayLineUpdated(true);
			m_lCurrentUpdateDayLinePos = 0; // ���ô����������е��������ݸ���һ������ʱ��Ҫ����24Сʱ���ʱ��ָ��¼��ɡ�
			TRACE("Finnhub���߸�����ϣ����¿�ʼ����\n");
			str = _T("US Market������ʷ���ݸ������");
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}

bool CFinnhubDataSource::InquiryInsiderTransaction(void) {
	static bool s_fInquiringFinnhubStockInsiderTransaction = false;
	bool fFound = false;
	CWorldStockPtr pStock;
	CString str = _T("");
	long lStockSetSize = gl_pWorldMarket->GetStockSize();
	bool fHaveInquiry = false;
	CProductWebSourceDataPtr p = nullptr;
	long lCurrentUpdateInsiderTransactionPos = 0;

	ASSERT(gl_pWorldMarket->IsSystemReady());
	if (!IsInsiderTransactionUpdated() && !IsInquiring()) {
		if (!s_fInquiringFinnhubStockInsiderTransaction) {
			gl_systemMessage.PushInformationMessage(_T("Inquiring finnhub stock insider transaction..."));
			s_fInquiringFinnhubStockInsiderTransaction = true;
		}
		for (lCurrentUpdateInsiderTransactionPos = 0; lCurrentUpdateInsiderTransactionPos < lStockSetSize; lCurrentUpdateInsiderTransactionPos++) {
			pStock = gl_pWorldMarket->GetStock(lCurrentUpdateInsiderTransactionPos);
			if (pStock->IsUSMarket()) {
				if (pStock->IsInsiderTransactionNeedUpdate()) { // Ŀǰ����˻�ֻ�����������г��Ĺ�Ʊ���ߡ�
					fFound = true;
					break;
				}
			}
		}
		if (fFound) {
			fHaveInquiry = true;
			p = m_FinnhubFactory.CreateProduct(gl_pWorldMarket.get(), __INSIDER_TRANSACTION__);
			p->SetIndex(lCurrentUpdateInsiderTransactionPos);
			StoreInquiry(p);
			SetInquiring(true);
			gl_pWorldMarket->SetCurrentFunction(_T("�ڲ�����:") + pStock->GetSymbol());
			//TRACE("����%s �ڲ���������\n", pStock->GetSymbol().GetBuffer());
		}
		else {
			s_fInquiringFinnhubStockInsiderTransaction = false;
			fHaveInquiry = false;
			SetInsiderTransactionUpdated(true);
			TRACE("FinnhubInsider Transaction�������\n");
			str = _T("US Market Insider Transaction���ݸ������");
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}

bool CFinnhubDataSource::InquiryInsiderSentiment(void) {
	static bool s_fInquiringFinnhubStockInsiderSentiment = false;
	bool fFound = false;
	CWorldStockPtr pStock;
	CString str = _T("");
	long lStockSetSize = gl_pWorldMarket->GetStockSize();
	bool fHaveInquiry = false;
	CProductWebSourceDataPtr p = nullptr;
	long lCurrentUpdateInsiderSentimentPos = 0;

	ASSERT(gl_pWorldMarket->IsSystemReady());
	if (!IsInsiderSentimentUpdated() && !IsInquiring()) {
		if (!s_fInquiringFinnhubStockInsiderSentiment) {
			gl_systemMessage.PushInformationMessage(_T("Inquiring finnhub stock insider sentiment..."));
			s_fInquiringFinnhubStockInsiderSentiment = true;
		}
		for (lCurrentUpdateInsiderSentimentPos = 0; lCurrentUpdateInsiderSentimentPos < lStockSetSize; lCurrentUpdateInsiderSentimentPos++) {
			pStock = gl_pWorldMarket->GetStock(lCurrentUpdateInsiderSentimentPos);
			if (pStock->IsUSMarket()) {
				if (pStock->IsInsiderSentimentNeedUpdate()) {
					fFound = true;
					break;
				}
			}
		}
		if (fFound) {
			fHaveInquiry = true;
			p = m_FinnhubFactory.CreateProduct(gl_pWorldMarket.get(), __INSIDER_SENTIMENT__);
			p->SetIndex(lCurrentUpdateInsiderSentimentPos);
			StoreInquiry(p);
			SetInquiring(true);
			gl_pWorldMarket->SetCurrentFunction(_T("�ڲ���������:") + pStock->GetSymbol());
			//TRACE("����%s �ڲ�������������\n", pStock->GetSymbol().GetBuffer());
		}
		else {
			s_fInquiringFinnhubStockInsiderSentiment = false;
			fHaveInquiry = false;
			SetInsiderSentimentUpdated(true);
			TRACE("FinnhubInsider Sentiment�������\n");
			str = _T("US Market Insider Sentiment���ݸ������");
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}

bool CFinnhubDataSource::InquiryRTQuote(void) {
	static long s_lCurrentRTDataQuotePos = 0;
	CProductWebSourceDataPtr p = nullptr;

	ASSERT(gl_pWorldMarket->IsSystemReady());
	if (!IsInquiring()) {
		p = m_FinnhubFactory.CreateProduct(gl_pWorldMarket.get(), __STOCK_PRICE_QUOTE__);
		p->SetIndex(s_lCurrentRTDataQuotePos);
		StoreInquiry(p);
		SetInquiring(true);
		s_lCurrentRTDataQuotePos++;
		if (s_lCurrentRTDataQuotePos == gl_pWorldMarket->GetStockSize()) s_lCurrentRTDataQuotePos = 0;
		gl_pWorldMarket->SetCurrentFunction(_T("Finnhub stock RT: ") + gl_pWorldMarket->GetStock(s_lCurrentRTDataQuotePos)->GetSymbol());
		TRACE("����%sʵʱ����\n", gl_pWorldMarket->GetStock(s_lCurrentRTDataQuotePos)->GetSymbol().GetBuffer());
		return true;
	}
	return false;
}

bool CFinnhubDataSource::InquiryPeer(void) {
	static bool s_fInquiringFinnhubStockPeer = false;
	bool fFound = false;
	CWorldStockPtr pStock;
	CString str = _T("");
	long lStockSetSize = gl_pWorldMarket->GetStockSize();
	bool fHaveInquiry = false;
	CProductWebSourceDataPtr p = nullptr;
	long lCurrentUpdatePeerPos = 0;

	ASSERT(gl_pWorldMarket->IsSystemReady());
	if (!IsPeerUpdated() && !IsInquiring()) {
		if (!s_fInquiringFinnhubStockPeer) {
			gl_systemMessage.PushInformationMessage(_T("Inquiring finnhub stock peer..."));
			s_fInquiringFinnhubStockPeer = true;
		}
		for (lCurrentUpdatePeerPos = 0; lCurrentUpdatePeerPos < lStockSetSize; lCurrentUpdatePeerPos++) {
			if (!gl_pWorldMarket->GetStock(lCurrentUpdatePeerPos)->IsPeerUpdated()) {
				pStock = gl_pWorldMarket->GetStock(lCurrentUpdatePeerPos);
				fFound = true;
				break;
			}
		}
		if (fFound) {
			fHaveInquiry = true;
			p = m_FinnhubFactory.CreateProduct(gl_pWorldMarket.get(), __PEERS__);
			p->SetIndex(lCurrentUpdatePeerPos);
			StoreInquiry(p);
			SetInquiring(true);
			gl_pWorldMarket->SetCurrentFunction(_T("Peer:") + gl_pWorldMarket->GetStock(lCurrentUpdatePeerPos)->GetSymbol());
			//TRACE("����%s Peer����\n", GetStock(lCurrentUpdatePeerPos)->GetSymbol().GetBuffer());
		}
		else {
			s_fInquiringFinnhubStockPeer = false;
			fHaveInquiry = false;
			SetPeerUpdated(true);
			TRACE("Finnhub Peers�������\n");
			str = _T("Finnhub Peer Updated");
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}

bool CFinnhubDataSource::InquiryEconomicCalendar(void) {
	CProductWebSourceDataPtr p = nullptr;

	if (!IsEconomicCalendarUpdated() && !IsInquiring()) {
		p = m_FinnhubFactory.CreateProduct(gl_pWorldMarket.get(), __ECONOMIC_CALENDAR__);
		StoreInquiry(p);
		SetInquiring(true);
		gl_pWorldMarket->SetCurrentFunction(_T("Finnhub ecomomic calendar"));
		return true;
	}
	return false;
}

bool CFinnhubDataSource::InquiryEPSSurprise(void) {
	bool fFound = false;
	CWorldStockPtr pStock;
	CString str = _T("");
	long lStockSetSize = gl_pWorldMarket->GetStockSize();
	bool fHaveInquiry = false;

	ASSERT(gl_pWorldMarket->IsSystemReady());
	if (!IsEPSSurpriseUpdated() && !IsInquiring()) {
		for (m_lCurrentUpdateEPSSurprisePos = 0; m_lCurrentUpdateEPSSurprisePos < lStockSetSize; m_lCurrentUpdateEPSSurprisePos++) {
			if (!gl_pWorldMarket->GetStock(m_lCurrentUpdateEPSSurprisePos)->IsEPSSurpriseUpdated()) {
				pStock = gl_pWorldMarket->GetStock(m_lCurrentUpdateEPSSurprisePos);
				fFound = true;
				break;
			}
		}
		if (fFound) {
			fHaveInquiry = true;
			CProductWebSourceDataPtr p = m_FinnhubFactory.CreateProduct(gl_pWorldMarket.get(), __STOCK_ESTIMATES_EPS_SURPRISE__);
			p->SetIndex(m_lCurrentUpdateEPSSurprisePos);
			StoreInquiry(p);
			SetInquiring(true);
			gl_pWorldMarket->GetStock(m_lCurrentUpdateEPSSurprisePos)->SetEPSSurpriseUpdated(true);
			TRACE("����%s EPS Surprise����\n", gl_pWorldMarket->GetStock(m_lCurrentUpdateEPSSurprisePos)->GetSymbol().GetBuffer());
		}
		else {
			SetEPSSurpriseUpdated(true);
			TRACE("Finnhub EPS Surprise�������\n");
			str = _T("Finnhub EPS Surprise Updated");
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}

bool CFinnhubDataSource::InquiryForexExchange(void) {
	if (!IsForexExchangeUpdated() && !IsInquiring()) {
		StoreInquiry(m_FinnhubFactory.CreateProduct(gl_pWorldMarket.get(), __FOREX_EXCHANGE__));
		SetInquiring(true);
		gl_pWorldMarket->SetCurrentFunction(_T("Finnhub forex exchange"));
		gl_systemMessage.PushInformationMessage(_T("Inquiring Finnhub forex exchange"));
		return true;
	}
	return false;
}

bool CFinnhubDataSource::InquiryForexSymbol(void) {
	static long s_lCurrentForexExchangePos = 0;
	CProductWebSourceDataPtr p = nullptr;

	if (!IsForexSymbolUpdated() && !IsInquiring()) {
		p = m_FinnhubFactory.CreateProduct(gl_pWorldMarket.get(), __FOREX_SYMBOLS__);
		p->SetIndex(s_lCurrentForexExchangePos++);
		StoreInquiry(p);
		SetInquiring(true);
		if (s_lCurrentForexExchangePos >= gl_pWorldMarket->GetForexExchangeSize()) {
			SetForexSymbolUpdated(true);
			s_lCurrentForexExchangePos = 0;
			gl_pWorldMarket->SetCurrentFunction(_T("Finnhub forex synmbol"));
			gl_systemMessage.PushInformationMessage(_T("Inquiring Finnhub Forex sysbols..."));
		}
		return true;
	}
	return false;
}

bool CFinnhubDataSource::InquiryForexDayLine(void) {
	static bool s_fInquiringFinnhubForexDayLine = false;
	bool fFound = false;
	CForexSymbolPtr pForexSymbol;
	CString str = _T("");
	const long lStockSetSize = gl_pWorldMarket->GetForexSymbolSize();
	bool fHaveInquiry = false;
	CProductWebSourceDataPtr p = nullptr;
	long lCurrentUpdateForexDayLinePos = 0;

	ASSERT(gl_pWorldMarket->IsSystemReady());
	if (!IsForexDayLineUpdated() && !IsInquiring()) {
		if (!s_fInquiringFinnhubForexDayLine) {
			gl_systemMessage.PushInformationMessage(_T("Inquiring finnhub forex day line..."));
			s_fInquiringFinnhubForexDayLine = true;
		}
		for (lCurrentUpdateForexDayLinePos = 0; lCurrentUpdateForexDayLinePos < lStockSetSize; lCurrentUpdateForexDayLinePos++) {
			if (gl_pWorldMarket->GetForexSymbol(lCurrentUpdateForexDayLinePos)->IsDayLineNeedUpdate()) {
				pForexSymbol = gl_pWorldMarket->GetForexSymbol(lCurrentUpdateForexDayLinePos);
				fFound = true;
				break;
			}
		}
		if (fFound) {
			fHaveInquiry = true;
			p = m_FinnhubFactory.CreateProduct(gl_pWorldMarket.get(), __FOREX_CANDLES__);
			p->SetIndex(lCurrentUpdateForexDayLinePos);
			StoreInquiry(p);
			gl_pWorldMarket->SetCurrentFunction(_T("Finnhub Forex���ߣ�") + pForexSymbol->GetSymbol());
			SetInquiring(true);
			pForexSymbol->SetDayLineNeedUpdate(false);
			gl_pWorldMarket->SetCurrentFunction(_T("����:") + pForexSymbol->GetSymbol());
			TRACE("����%s��������\n", pForexSymbol->GetSymbol().GetBuffer());
		}
		else {
			s_fInquiringFinnhubForexDayLine = false;
			SetForexDayLineUpdated(true);
			TRACE("Finnhub Forex���߸������\n");
			str = _T("Forex DayLine Updated");
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}

bool CFinnhubDataSource::InquiryCryptoExchange(void) {
	if (!IsCryptoExchangeUpdated() && !IsInquiring()) {
		StoreInquiry(m_FinnhubFactory.CreateProduct(gl_pWorldMarket.get(), __CRYPTO_EXCHANGE__));
		SetInquiring(true);
		gl_pWorldMarket->SetCurrentFunction(_T("Finnhub crypto exchange"));
		gl_systemMessage.PushInformationMessage(_T("Inquiring Finnhub crypto exchange"));
		return true;
	}
	return false;
}

bool CFinnhubDataSource::InquiryCryptoSymbol(void) {
	static long s_lCurrentCryptoExchangePos = 0;
	CProductWebSourceDataPtr p = nullptr;

	if (!IsCryptoSymbolUpdated() && !IsInquiring()) {
		p = m_FinnhubFactory.CreateProduct(gl_pWorldMarket.get(), __CRYPTO_SYMBOLS__);
		p->SetIndex(s_lCurrentCryptoExchangePos++);
		StoreInquiry(p);
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

bool CFinnhubDataSource::InquiryCryptoDayLine(void) {
	static bool s_fInquiringFinnhubCryptoDayLine = false;
	bool fFound = false;
	CFinnhubCryptoSymbolPtr pCryptoSymbol;
	CString str = _T("");
	const long lStockSetSize = gl_pWorldMarket->GetFinnhubCryptoSymbolSize();
	bool fHaveInquiry = false;
	CProductWebSourceDataPtr p = nullptr;
	long lCurrentUpdateCryptoDayLinePos = 0;

	ASSERT(gl_pWorldMarket->IsSystemReady());
	if (!IsCryptoDayLineUpdated() && !IsInquiring()) {
		if (!s_fInquiringFinnhubCryptoDayLine) {
			gl_systemMessage.PushInformationMessage(_T("Inquiring finnhub forex day line..."));
			s_fInquiringFinnhubCryptoDayLine = true;
		}
		for (lCurrentUpdateCryptoDayLinePos = 0; lCurrentUpdateCryptoDayLinePos < lStockSetSize; lCurrentUpdateCryptoDayLinePos++) {
			if (gl_pWorldMarket->GetFinnhubCryptoSymbol(lCurrentUpdateCryptoDayLinePos)->IsDayLineNeedUpdate()) {
				pCryptoSymbol = gl_pWorldMarket->GetFinnhubCryptoSymbol(lCurrentUpdateCryptoDayLinePos);
				fFound = true;
				break;
			}
		}
		if (fFound) {
			fHaveInquiry = true;
			p = m_FinnhubFactory.CreateProduct(gl_pWorldMarket.get(), __CRYPTO_CANDLES__);
			p->SetIndex(lCurrentUpdateCryptoDayLinePos);
			StoreInquiry(p);
			gl_pWorldMarket->SetCurrentFunction(_T("Finnhub Crypto���ߣ�") + pCryptoSymbol->GetSymbol());
			SetInquiring(true);
			pCryptoSymbol->SetDayLineNeedUpdate(false);
			gl_pWorldMarket->SetCurrentFunction(_T("����:") + pCryptoSymbol->GetSymbol());
			//TRACE("����%s��������\n", pCryptoSymbol->GetSymbol().GetBuffer());
		}
		else {
			s_fInquiringFinnhubCryptoDayLine = false;
			SetCryptoDayLineUpdated(true);
			TRACE("Finnhub Crypto���߸������\n");
			str = _T("Crypto DayLine Updated");
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}