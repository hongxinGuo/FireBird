#include"pch.h"

#include"TimeConvert.h"

#include"FinnhubInquiryType.h"
#include"FinnhubInaccessibleExchange.h"

#include "FinnhubDataSource.h"

#include"WorldMarket.h"

CFinnhubDataSource::CFinnhubDataSource(void) {
	m_pWebInquiry = nullptr;

	// ���裨Ҳ�޷���ÿ�ո��µı�����������
	m_fEPSSurpriseUpdated = true;
	m_lCurrentUpdateEPSSurprisePos = 0;
	m_lCurrentUpdateDayLinePos = 0; // ����֤ȯ���������ж�ʮ��֮�࣬�޷���һ��֮�ڸ����꣬�ʲ������ô�������

	Reset();
}

CFinnhubDataSource::~CFinnhubDataSource(void) {
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

/*
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ��ϵͳ���������ȣ���Լ100�������һ�Ρ�
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CFinnhubDataSource::Inquire(long lCurrentTime) {
	static int s_iCountfinnhubLimit = 100; // Finnhub.ioÿ1.2����������һ�Σ��Է�ֹ����Ƶ�ʹ��ߵ��������ʼֵ��Ϊ��������10���ſ�ʼ��
	if (--s_iCountfinnhubLimit < 0) {
		if (!IsInquiring() && !m_pWebInquiry->IsWebError()) {
			InquireFinnhub(lCurrentTime);
		}
		if (IsInquiring()) {
			s_iCountfinnhubLimit = gl_systemConfigeration.GetWorldMarketFinnhubInquiryTime() / 100; // ����������������ݣ������ü��������Ա�������һ�Ρ�
		}
	}
	return true;
}
*/

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ��ϵͳ���������ȣ�����GetTickCount64()��ȷ����ǰʱ�䡣
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CFinnhubDataSource::Inquire(long lCurrentTime) {
	static long long sllLastTimeTickCount = 0;
	static bool sbWebError = false;
	long long llTickCount = 0;

	llTickCount = GetTickCount64();
	if (!sbWebError) {
		if (m_pWebInquiry->IsWebError()) {
			sbWebError = true;
			sllLastTimeTickCount += 300000; // ������ִ������ӳ�5�������������롣
		}
	}
	else {
		if (llTickCount > (sllLastTimeTickCount + gl_systemConfigeration.GetWorldMarketFinnhubInquiryTime())) {
			sbWebError = false;
		}
	}

	if (llTickCount > (sllLastTimeTickCount + gl_systemConfigeration.GetWorldMarketFinnhubInquiryTime())) {
		if (!IsInquiring()) {
			InquireFinnhub(lCurrentTime);
		}
		if (IsInquiring()) {
			sllLastTimeTickCount = llTickCount;
		}
	}
	return true;
}

bool CFinnhubDataSource::InquireFinnhub(long lCurrentTime) {
	ASSERT(!IsInquiring());
	if (((lCurrentTime < 165700) || (lCurrentTime > 170500))) { // ������ʱ����ϵͳ���ʶ���ʱ���������������Ϣ��
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
			InquireForexDayLine(); // Forex daylineĿǰֻ���ڸ����û�ʹ��
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
		CVirtualProductWebDataPtr product = m_FinnhubFactory.CreateProduct(gl_pWorldMarket.get(), __ECONOMIC_COUNTRY_LIST__);
		StoreInquiry(product);
		gl_pWorldMarket->SetCurrentFunction(_T("Finnhub country List"));
		SetInquiring(true);
		gl_systemMessage.PushInformationMessage(_T("Inquiring Finnhub economic country List"));
		return true;
	}
	return false;
}

bool CFinnhubDataSource::InquireCompanySymbol(void) {
	static bool s_fInquiringFinnhubStockSymbol = false;
	bool fFound = false;
	CFinnhubStockExchangePtr pExchange;
	CString str = _T("");
	long lExchangeSize = gl_pWorldMarket->GetStockExchangeSize();
	bool fHaveInquiry = false;
	CVirtualProductWebDataPtr product = nullptr;
	long lCurrentStockExchangePos = 0;
	int iInquiryType = __STOCK_SYMBOLS__;

	if (!IsInquiring() && !IsSymbolUpdated()) {
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
			product = m_FinnhubFactory.CreateProduct(gl_pWorldMarket.get(), iInquiryType);
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
			str = _T("Finnhub�������������ݲ�ѯ���");
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}

bool CFinnhubDataSource::InquireCompanyProfileConcise(void) {
	static bool s_fInquiringFinnhubStockProfile = false;
	bool fFound = false;
	long lStockSetSize = gl_pWorldMarket->GetStockSize();
	CString str = _T("");
	bool fHaveInquiry = false;
	CVirtualProductWebDataPtr product = nullptr;
	long lCurrentProfilePos;
	CWorldStockPtr pStock = nullptr;
	int iInquiryType = __COMPANY_PROFILE_CONCISE__;

	ASSERT(gl_pWorldMarket->IsSystemReady());
	if (!IsInquiring() && !IsStockProfileUpdated()) {
		if (!s_fInquiringFinnhubStockProfile) {
			gl_systemMessage.PushInformationMessage(_T("Inquiring finnhub stock profile..."));
			s_fInquiringFinnhubStockProfile = true;
		}
		for (lCurrentProfilePos = 0; lCurrentProfilePos < lStockSetSize; lCurrentProfilePos++) {
			pStock = gl_pWorldMarket->GetStock(lCurrentProfilePos);
			if (!pStock->IsCompanyProfileUpdated()) {
				if (!gl_finnhubInaccessibleExchange.IsInaccessible(iInquiryType, pStock->GetExchangeCode())) {
					fFound = true;
					break;
				}
			}
		}
		if (fFound) {
			product = m_FinnhubFactory.CreateProduct(gl_pWorldMarket.get(), iInquiryType);
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
			str = _T("Finnhub��Ʊ���������");
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
	static bool s_fInquiringFinnhubCompanyNews = false;
	bool fFound = false;
	long lStockSetSize = gl_pWorldMarket->GetStockSize();
	CString str = _T("");
	bool fHaveInquiry = false;
	CVirtualProductWebDataPtr product = nullptr;
	long lCurrentCompanyNewsPos;

	ASSERT(gl_pWorldMarket->IsSystemReady());
	if (!IsInquiring() && !IsCompanyNewsUpdated()) {
		if (!s_fInquiringFinnhubCompanyNews) {
			gl_systemMessage.PushInformationMessage(_T("Inquiring finnhub company news..."));
			s_fInquiringFinnhubCompanyNews = true;
		}
		for (lCurrentCompanyNewsPos = 0; lCurrentCompanyNewsPos < lStockSetSize; lCurrentCompanyNewsPos++) {
			CWorldStockPtr pStock = gl_pWorldMarket->GetStock(lCurrentCompanyNewsPos);
			if ((pStock->IsUSMarket()) && (!pStock->IsCompanyNewsUpdated())) { // Ŀǰֻ���������г�
				fFound = true;
				break;
			}
		}
		if (fFound) {
			product = m_FinnhubFactory.CreateProduct(gl_pWorldMarket.get(), __COMPANY_NEWS__);
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
			str = _T("Finnhub��˾���Ÿ������");
			gl_systemMessage.PushInformationMessage(str);
			fHaveInquiry = false;
		}
	}
	return fHaveInquiry;
}

bool CFinnhubDataSource::InquireCompanyBasicFinancial(void) {
	static bool s_fInquiringFinnhubCompanyBasicFinancial = false;
	bool fFound = false;
	long lStockSetSize = gl_pWorldMarket->GetStockSize();
	CString str = _T("");
	bool fHaveInquiry = false;
	CVirtualProductWebDataPtr product = nullptr;
	long lCurrentBasicFinncialsPos;
	CWorldStockPtr pStock = nullptr;
	int iInquiryType = __BASIC_FINANCIALS__;

	ASSERT(gl_pWorldMarket->IsSystemReady());
	if (!IsInquiring() && !IsStockBasicFinancialUpdated()) {
		if (!s_fInquiringFinnhubCompanyBasicFinancial) {
			gl_systemMessage.PushInformationMessage(_T("Inquiring finnhub stock basic financials..."));
			s_fInquiringFinnhubCompanyBasicFinancial = true;
		}
		for (lCurrentBasicFinncialsPos = 0; lCurrentBasicFinncialsPos < lStockSetSize; lCurrentBasicFinncialsPos++) {
			pStock = gl_pWorldMarket->GetStock(lCurrentBasicFinncialsPos);
			if (!pStock->IsBasicFinancialUpdated()) {
				if (!gl_finnhubInaccessibleExchange.IsInaccessible(iInquiryType, pStock->GetExchangeCode())) {
					fFound = true;
					break;
				}
			}
		}
		if (fFound) {
			product = m_FinnhubFactory.CreateProduct(gl_pWorldMarket.get(), iInquiryType);
			product->SetIndex(lCurrentBasicFinncialsPos);
			StoreInquiry(product);
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

bool CFinnhubDataSource::InquireStockDayLine(void) {
	static bool s_fInquiringFinnhubStockDayLine = false;
	bool fFound = false;
	CWorldStockPtr pStock;
	CString str = _T("");
	long lStockSetSize = gl_pWorldMarket->GetStockSize();
	bool fHaveInquiry = false;
	CVirtualProductWebDataPtr product = nullptr;
	int iInquiryType = __STOCK_PRICE_CANDLES__;

	ASSERT(gl_pWorldMarket->IsSystemReady());
	if (!IsInquiring() && !IsStockDayLineUpdated()) {
		if (!s_fInquiringFinnhubStockDayLine) {
			gl_systemMessage.PushInformationMessage(_T("Inquiring finnhub stock day line..."));
			s_fInquiringFinnhubStockDayLine = true;
		}
		for (m_lCurrentUpdateDayLinePos = 0; m_lCurrentUpdateDayLinePos < lStockSetSize; m_lCurrentUpdateDayLinePos++) {
			pStock = gl_pWorldMarket->GetStock(m_lCurrentUpdateDayLinePos);
			if (pStock->IsDayLineNeedUpdate()) { // Ŀǰ����˻�ֻ�����������г��Ĺ�Ʊ���ߡ�
				if (!gl_finnhubInaccessibleExchange.IsInaccessible(iInquiryType, pStock->GetExchangeCode())) {
					fFound = true;
					break;
				}
			}
		}
		if (fFound) {
			fHaveInquiry = true;
			product = m_FinnhubFactory.CreateProduct(gl_pWorldMarket.get(), iInquiryType);
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
			str = _T("US Market������ʷ���ݸ������");
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}

bool CFinnhubDataSource::InquireInsiderTransaction(void) {
	static bool s_fInquiringFinnhubStockInsiderTransaction = false;
	bool fFound = false;
	CWorldStockPtr pStock = nullptr;
	CString str = _T("");
	long lStockSetSize = gl_pWorldMarket->GetStockSize();
	bool fHaveInquiry = false;
	CVirtualProductWebDataPtr product = nullptr;
	long lCurrentUpdateInsiderTransactionPos = 0;
	int iInquiryType = __INSIDER_TRANSACTION__;

	ASSERT(gl_pWorldMarket->IsSystemReady());
	if (!IsInquiring() && !IsInsiderTransactionUpdated()) {
		if (!s_fInquiringFinnhubStockInsiderTransaction) {
			gl_systemMessage.PushInformationMessage(_T("Inquiring finnhub stock insider transaction..."));
			s_fInquiringFinnhubStockInsiderTransaction = true;
		}
		for (lCurrentUpdateInsiderTransactionPos = 0; lCurrentUpdateInsiderTransactionPos < lStockSetSize; lCurrentUpdateInsiderTransactionPos++) {
			pStock = gl_pWorldMarket->GetStock(lCurrentUpdateInsiderTransactionPos);
			if (pStock->IsInsiderTransactionNeedUpdate()) { // Ŀǰ����˻�ֻ�����������г��Ĺ�Ʊ���ߡ�
				if (!gl_finnhubInaccessibleExchange.IsInaccessible(iInquiryType, pStock->GetExchangeCode())) {
					fFound = true;
					break;
				}
			}
		}
		if (fFound) {
			fHaveInquiry = true;
			product = m_FinnhubFactory.CreateProduct(gl_pWorldMarket.get(), iInquiryType);
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
			str = _T("US Market Insider Transaction���ݸ������");
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}

bool CFinnhubDataSource::InquireInsiderSentiment(void) {
	static bool s_fInquiringFinnhubStockInsiderSentiment = false;
	bool fFound = false;
	CWorldStockPtr pStock = nullptr;
	CString str = _T("");
	long lStockSetSize = gl_pWorldMarket->GetStockSize();
	bool fHaveInquiry = false;
	CVirtualProductWebDataPtr product = nullptr;
	long lCurrentUpdateInsiderSentimentPos = 0;
	int iInquiryType = __INSIDER_SENTIMENT__;

	ASSERT(gl_pWorldMarket->IsSystemReady());
	if (!IsInquiring() && !IsInsiderSentimentUpdated()) {
		if (!s_fInquiringFinnhubStockInsiderSentiment) {
			gl_systemMessage.PushInformationMessage(_T("Inquiring finnhub stock insider sentiment..."));
			s_fInquiringFinnhubStockInsiderSentiment = true;
		}
		for (lCurrentUpdateInsiderSentimentPos = 0; lCurrentUpdateInsiderSentimentPos < lStockSetSize; lCurrentUpdateInsiderSentimentPos++) {
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
			product = m_FinnhubFactory.CreateProduct(gl_pWorldMarket.get(), iInquiryType);
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
			str = _T("US Market Insider Sentiment���ݸ������");
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}

bool CFinnhubDataSource::InquireRTQuote(void) {
	static long s_lCurrentRTDataQuotePos = 0;
	CVirtualProductWebDataPtr product = nullptr;

	if (!IsInquiring()) {
		ASSERT(gl_pWorldMarket->IsSystemReady());
		product = m_FinnhubFactory.CreateProduct(gl_pWorldMarket.get(), __STOCK_PRICE_QUOTE__);
		product->SetIndex(s_lCurrentRTDataQuotePos);
		StoreInquiry(product);
		SetInquiring(true);
		s_lCurrentRTDataQuotePos++;
		if (s_lCurrentRTDataQuotePos == gl_pWorldMarket->GetStockSize()) s_lCurrentRTDataQuotePos = 0;
		gl_pWorldMarket->SetCurrentFunction(_T("Finnhub stock RT: ") + gl_pWorldMarket->GetStock(s_lCurrentRTDataQuotePos)->GetSymbol());
		TRACE("����%sʵʱ����\n", gl_pWorldMarket->GetStock(s_lCurrentRTDataQuotePos)->GetSymbol().GetBuffer());
	}
	return true;
}

bool CFinnhubDataSource::InquirePeer(void) {
	static bool s_fInquiringFinnhubStockPeer = false;
	bool fFound = false;
	CWorldStockPtr pStock = nullptr;
	CString str = _T("");
	long lStockSetSize = gl_pWorldMarket->GetStockSize();
	bool fHaveInquiry = false;
	CVirtualProductWebDataPtr product = nullptr;
	long lCurrentUpdatePeerPos = 0;
	int iInquiryType = __PEERS__;

	ASSERT(gl_pWorldMarket->IsSystemReady());
	if (!IsInquiring() && !IsPeerUpdated()) {
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
			product = m_FinnhubFactory.CreateProduct(gl_pWorldMarket.get(), iInquiryType);
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
			str = _T("Finnhub Peer Updated");
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}

bool CFinnhubDataSource::InquireEconomicCalendar(void) {
	CVirtualProductWebDataPtr product = nullptr;
	int iInquiryType = __ECONOMIC_CALENDAR__;

	if (!IsInquiring() && !IsEconomicCalendarUpdated()) {
		if (!gl_finnhubInaccessibleExchange.IsInaccessible(iInquiryType, _T("ALL"))) {
			product = m_FinnhubFactory.CreateProduct(gl_pWorldMarket.get(), iInquiryType);
			StoreInquiry(product);
			SetInquiring(true);
		}
		else {
			SetEconomicCalendarUpdated(true);
		}
		gl_pWorldMarket->SetCurrentFunction(_T("Finnhub ecomomic calendar"));
		return true;
	}
	return false;
}

bool CFinnhubDataSource::InquireEPSSurprise(void) {
	bool fFound = false;
	CWorldStockPtr pStock = nullptr;
	CString str = _T("");
	long lStockSetSize = gl_pWorldMarket->GetStockSize();
	bool fHaveInquiry = false;
	int iInquiryType = __STOCK_ESTIMATES_EPS_SURPRISE__;

	ASSERT(gl_pWorldMarket->IsSystemReady());
	if (!IsInquiring() && !IsEPSSurpriseUpdated()) {
		for (m_lCurrentUpdateEPSSurprisePos = 0; m_lCurrentUpdateEPSSurprisePos < lStockSetSize; m_lCurrentUpdateEPSSurprisePos++) {
			pStock = gl_pWorldMarket->GetStock(m_lCurrentUpdateEPSSurprisePos);
			if (!pStock->IsEPSSurpriseUpdated()) {
				if (!gl_finnhubInaccessibleExchange.IsInaccessible(iInquiryType, pStock->GetExchangeCode())) {
					fFound = true;
					break;
				}
			}
		}
		if (fFound) {
			fHaveInquiry = true;
			CVirtualProductWebDataPtr product = m_FinnhubFactory.CreateProduct(gl_pWorldMarket.get(), iInquiryType);
			product->SetIndex(m_lCurrentUpdateEPSSurprisePos);
			StoreInquiry(product);
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

bool CFinnhubDataSource::InquireForexExchange(void) {
	if (!IsInquiring() && !IsForexExchangeUpdated()) {
		StoreInquiry(m_FinnhubFactory.CreateProduct(gl_pWorldMarket.get(), __FOREX_EXCHANGE__));
		SetInquiring(true);
		gl_pWorldMarket->SetCurrentFunction(_T("Finnhub forex exchange"));
		gl_systemMessage.PushInformationMessage(_T("Inquiring Finnhub forex exchange"));
		return true;
	}
	return false;
}

bool CFinnhubDataSource::InquireForexSymbol(void) {
	static long s_lCurrentForexExchangePos = 0;
	CVirtualProductWebDataPtr product = nullptr;

	if (!IsInquiring() && !IsForexSymbolUpdated()) {
		product = m_FinnhubFactory.CreateProduct(gl_pWorldMarket.get(), __FOREX_SYMBOLS__);
		product->SetIndex(s_lCurrentForexExchangePos++);
		StoreInquiry(product);
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

bool CFinnhubDataSource::InquireForexDayLine(void) {
	static bool s_fInquiringFinnhubForexDayLine = false;
	bool fFound = false;
	CForexSymbolPtr pForexSymbol = nullptr;
	CString str = _T("");
	const long lStockSetSize = gl_pWorldMarket->GetForexSymbolSize();
	bool fHaveInquiry = false;
	CVirtualProductWebDataPtr product = nullptr;
	long lCurrentUpdateForexDayLinePos = 0;
	int iInquiryType = __FOREX_CANDLES__;

	ASSERT(gl_pWorldMarket->IsSystemReady());
	if (!IsInquiring() && !IsForexDayLineUpdated()) {
		if (!s_fInquiringFinnhubForexDayLine) {
			gl_systemMessage.PushInformationMessage(_T("Inquiring finnhub forex day line..."));
			s_fInquiringFinnhubForexDayLine = true;
		}
		for (lCurrentUpdateForexDayLinePos = 0; lCurrentUpdateForexDayLinePos < lStockSetSize; lCurrentUpdateForexDayLinePos++) {
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
			product = m_FinnhubFactory.CreateProduct(gl_pWorldMarket.get(), iInquiryType);
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
			str = _T("Forex DayLine Updated");
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}

bool CFinnhubDataSource::InquireCryptoExchange(void) {
	if (!IsInquiring() && !IsCryptoExchangeUpdated()) {
		StoreInquiry(m_FinnhubFactory.CreateProduct(gl_pWorldMarket.get(), __CRYPTO_EXCHANGE__));
		SetInquiring(true);
		gl_pWorldMarket->SetCurrentFunction(_T("Finnhub crypto exchange"));
		gl_systemMessage.PushInformationMessage(_T("Inquiring Finnhub crypto exchange"));
		return true;
	}
	return false;
}

bool CFinnhubDataSource::InquireCryptoSymbol(void) {
	static long s_lCurrentCryptoExchangePos = 0;
	CVirtualProductWebDataPtr product = nullptr;

	if (!IsInquiring() && !IsCryptoSymbolUpdated()) {
		product = m_FinnhubFactory.CreateProduct(gl_pWorldMarket.get(), __CRYPTO_SYMBOLS__);
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
	static bool s_fInquiringFinnhubCryptoDayLine = false;
	bool fFound = false;
	CFinnhubCryptoSymbolPtr pCryptoSymbol = nullptr;
	CString str = _T("");
	const long lStockSetSize = gl_pWorldMarket->GetFinnhubCryptoSymbolSize();
	bool fHaveInquiry = false;
	CVirtualProductWebDataPtr product = nullptr;
	long lCurrentUpdateCryptoDayLinePos = 0;
	int iInquiryType = __CRYPTO_CANDLES__;

	ASSERT(gl_pWorldMarket->IsSystemReady());
	if (!IsInquiring() && !IsCryptoDayLineUpdated()) {
		if (!s_fInquiringFinnhubCryptoDayLine) {
			gl_systemMessage.PushInformationMessage(_T("Inquiring finnhub forex day line..."));
			s_fInquiringFinnhubCryptoDayLine = true;
		}
		for (lCurrentUpdateCryptoDayLinePos = 0; lCurrentUpdateCryptoDayLinePos < lStockSetSize; lCurrentUpdateCryptoDayLinePos++) {
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
			product = m_FinnhubFactory.CreateProduct(gl_pWorldMarket.get(), iInquiryType);
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
			str = _T("Crypto DayLine Updated");
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}