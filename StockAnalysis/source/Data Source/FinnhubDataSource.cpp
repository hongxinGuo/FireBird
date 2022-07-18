#include "FinnhubDataSource.h"
#include"pch.h"

#include"TimeConvert.h"

#include"FinnhubInquiryType.h"

#include "FinnhubDataSource.h"

#include"WorldMarket.h"

CFinnhubDataSource::CFinnhubDataSource(void) {
	// 无需（也无法）每日更新的变量放在这里
	m_fEPSSurpriseUpdated = true;
	m_lCurrentUpdateEPSSurprisePos = 0;
	m_lCurrentUpdateDayLinePos = 0; // 由于证券代码总数有二十万之多，无法在一天之内更新完，故不再重置此索引。

	Reset();
}

CFinnhubDataSource::~CFinnhubDataSource(void) {
}

bool CFinnhubDataSource::Reset(void) {
	CDataSource::Reset();

	// Finnhub各申请网络数据标识，每日需要重置。
	m_fCountryListUpdated = false;
	m_fSymbolUpdated = false; // 每日需要更新代码
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

	if (GetDayOfWeek() == 6) { // 每周的星期六更新一次EPSSurprise
		m_lCurrentUpdateEPSSurprisePos = 0;
		SetEPSSurpriseUpdated(false);
	}

	return true;
}

bool CFinnhubDataSource::UpdateStatus(void) {
	switch (m_pCurrentProduct->GetProductType()) {
	case __COMPANY_PROFILE__: // Premium 免费账户无法读取此信息，sandbox模式能读取，但数据是错误的，只能用于测试。
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
		TRACE("未处理指令%d\n", m_pCurrentProduct->GetProductType());
		gl_systemMessage.PushErrorMessage(_T("Finnhub product未实现"));
		break;
	}
	return true;
}

bool CFinnhubDataSource::Inquiry(long lCurrentTime) {
	static int s_iCountfinnhubLimit = 100; // Finnhub.io每1.2秒左右申请一次，以防止出现频率过高的情况。初始值设为程序启动10秒后才开始。
	int i = 0;
	if (--s_iCountfinnhubLimit < 0) {
		if (!IsInquiring() && !m_pWebInquiry->IsWebError()) InquiryFinnhub(lCurrentTime);
		if (IsInquiring()) {
			i = gl_systemConfigeration.GetWorldMarketFinnhubInquiryTime();
			s_iCountfinnhubLimit = gl_systemConfigeration.GetWorldMarketFinnhubInquiryTime() / 100; // 如果申请了网络数据，则重置计数器，以便申请下一次。
		}
	}
	return true;
}

bool CFinnhubDataSource::InquiryFinnhub(long lCurrentTime) {
	if (((lCurrentTime < 165700) || (lCurrentTime > 170500))) { // 下午五时重启系统，故而此时不允许接收网络信息。
		InquiryCountryList();
		InquiryForexExchange();
		InquiryCryptoExchange();
		InquiryCompanySymbol(); // 第一个动作，首先申请当日证券代码
		InquiryForexSymbol();
		InquiryCryptoSymbol();
		//InquiryEconomicCalendar();

		// 申请Finnhub网络信息的任务，皆要放置在这里，以保证在市场时间凌晨十分钟后执行。这样能够保证在重启市场时不会执行查询任务
		if (gl_pWorldMarket->IsSystemReady()) {
			InquiryCompanyProfileConcise();
			InquiryCompanyBasicFinancial();
			InquiryPeer();
			InquiryInsiderTransaction();
			InquiryInsiderSentiment();
			InquiryCryptoDayLine();
			InquiryStockDayLine();
			//InquiryForexDayLine(); // Forex dayline目前只限于付费用户使用
			//InquiryEPSSurprise(); // 这个现在没什么用，暂时停止更新。
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
			gl_pWorldMarket->SetCurrentFunction(_T("Finnhub交易所代码:") + pExchange->m_strCode);
			SetInquiring(true);
			pExchange->SetUpdated(true);
			//TRACE("申请%s交易所证券代码\n", pExchange->m_strCode.GetBuffer());
		}
		else {
			s_fInquiringFinnhubStockSymbol = false;
			fHaveInquiry = false;
			SetSymbolUpdated(true);
			TRACE("Finnhub交易所代码数据查询完毕\n");
			str = _T("Finnhub交易所代码数据查询完毕");
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
			gl_pWorldMarket->SetCurrentFunction(_T("简介:") + gl_pWorldMarket->GetStock(lCurrentProfilePos)->GetSymbol());
			//TRACE("更新%s简介\n", m_vWorldStock.at(m_lCurrentProfilePos)->m_strSymbol.GetBuffer());
			SetInquiring(true);
			fHaveInquiry = true;
		}
		else {
			s_fInquiringFinnhubStockProfile = false;
			SetStockProfileUpdated(true);
			TRACE("Finnhub股票简介更新完毕\n");
			str = _T("Finnhub股票简介更新完毕");
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
			gl_pWorldMarket->SetCurrentFunction(_T("基本财务:") + gl_pWorldMarket->GetStock(lCurrentBasicFinncialsPos)->GetSymbol());
			//TRACE("更新%s简介\n", m_vWorldStock.at(m_lCurrentProfilePos)->m_strSymbol.GetBuffer());
			SetInquiring(true);
			fHaveInquiry = true;
		}
		else {
			s_fInquiringFinnhubCompanyBasicFinancial = false;
			SetStockBasicFinancialUpdated(true);
			TRACE("Finnhub股票基本财务情况更新完毕\n");
			str = _T("Finnhub股票基本财务情况更新完毕");
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
			if (pStock->IsUSMarket() && pStock->IsDayLineNeedUpdate()) { // 目前免费账户只能下载美国市场的股票日线。
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
			gl_pWorldMarket->SetCurrentFunction(_T("日线:") + pStock->GetSymbol());
			//TRACE("申请%s日线数据\n", pStock->GetSymbol().GetBuffer());
		}
		else {
			s_fInquiringFinnhubStockDayLine = false;
			fHaveInquiry = false;
			SetDayLineUpdated(true);
			m_lCurrentUpdateDayLinePos = 0; // 重置此索引。所有的日线数据更新一次所需时间要超过24小时，故保持更新即可。
			TRACE("Finnhub日线更新完毕，从新开始更新\n");
			str = _T("US Market日线历史数据更新完毕");
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
				if (pStock->IsInsiderTransactionNeedUpdate()) { // 目前免费账户只能下载美国市场的股票日线。
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
			gl_pWorldMarket->SetCurrentFunction(_T("内部交易:") + pStock->GetSymbol());
			//TRACE("申请%s 内部交易数据\n", pStock->GetSymbol().GetBuffer());
		}
		else {
			s_fInquiringFinnhubStockInsiderTransaction = false;
			fHaveInquiry = false;
			SetInsiderTransactionUpdated(true);
			TRACE("FinnhubInsider Transaction更新完毕\n");
			str = _T("US Market Insider Transaction数据更新完毕");
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
			gl_pWorldMarket->SetCurrentFunction(_T("内部交易情绪:") + pStock->GetSymbol());
			//TRACE("申请%s 内部交易情绪数据\n", pStock->GetSymbol().GetBuffer());
		}
		else {
			s_fInquiringFinnhubStockInsiderSentiment = false;
			fHaveInquiry = false;
			SetInsiderSentimentUpdated(true);
			TRACE("FinnhubInsider Sentiment更新完毕\n");
			str = _T("US Market Insider Sentiment数据更新完毕");
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
		TRACE("申请%s实时数据\n", gl_pWorldMarket->GetStock(s_lCurrentRTDataQuotePos)->GetSymbol().GetBuffer());
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
			//TRACE("申请%s Peer数据\n", GetStock(lCurrentUpdatePeerPos)->GetSymbol().GetBuffer());
		}
		else {
			s_fInquiringFinnhubStockPeer = false;
			fHaveInquiry = false;
			SetPeerUpdated(true);
			TRACE("Finnhub Peers更新完毕\n");
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
			TRACE("申请%s EPS Surprise数据\n", gl_pWorldMarket->GetStock(m_lCurrentUpdateEPSSurprisePos)->GetSymbol().GetBuffer());
		}
		else {
			SetEPSSurpriseUpdated(true);
			TRACE("Finnhub EPS Surprise更新完毕\n");
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
			gl_pWorldMarket->SetCurrentFunction(_T("Finnhub Forex日线：") + pForexSymbol->GetSymbol());
			SetInquiring(true);
			pForexSymbol->SetDayLineNeedUpdate(false);
			gl_pWorldMarket->SetCurrentFunction(_T("日线:") + pForexSymbol->GetSymbol());
			TRACE("申请%s日线数据\n", pForexSymbol->GetSymbol().GetBuffer());
		}
		else {
			s_fInquiringFinnhubForexDayLine = false;
			SetForexDayLineUpdated(true);
			TRACE("Finnhub Forex日线更新完毕\n");
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
			gl_pWorldMarket->SetCurrentFunction(_T("Finnhub Crypto日线：") + pCryptoSymbol->GetSymbol());
			SetInquiring(true);
			pCryptoSymbol->SetDayLineNeedUpdate(false);
			gl_pWorldMarket->SetCurrentFunction(_T("日线:") + pCryptoSymbol->GetSymbol());
			//TRACE("申请%s日线数据\n", pCryptoSymbol->GetSymbol().GetBuffer());
		}
		else {
			s_fInquiringFinnhubCryptoDayLine = false;
			SetCryptoDayLineUpdated(true);
			TRACE("Finnhub Crypto日线更新完毕\n");
			str = _T("Crypto DayLine Updated");
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}