#include"pch.h"

#include"TimeConvert.h"
#include"SystemConfiguration.h"

#include"FinnhubInquiryType.h"
#include"FinnhubInaccessibleExchange.h"

#include "FinnhubDataSource.h"

#include"WorldMarket.h"

CFinnhubDataSource::CFinnhubDataSource(void) {
	// 无需（也无法）每日更新的变量放在这里
	m_fUpdateEPSSurprise = true;
	m_lCurrentUpdateEPSSurprisePos = 0;
	m_lCurrentUpdateDayLinePos = 0; // 由于证券代码总数有二十万之多，无法在一天之内更新完，故不再重置此索引。

	m_strInquiryFunction = _T(""); // finnhub有各种数据，故其前缀由数据申请函数每次设置，不同的前缀申请不同的数据。
	m_strParam = _T("");
	m_strSuffix = _T("&token=");
	m_strInquiryToken = _T("");
	m_lInquiringNumber = 1; // Finnhub实时数据查询数量默认值

	CFinnhubDataSource::ConfigureSession();

	CFinnhubDataSource::Reset();
}

bool CFinnhubDataSource::Reset(void) {
	CVirtualDataSource::Reset();

	// Finnhub各申请网络数据标识，每日需要重置。
	m_fUpdateCountryList = true;
	m_fUpdateSymbol = true; // 每日需要更新代码
	m_fUpdateStockProfile = true;
	m_fUpdateCompanyNews = true;
	m_fUpdateCompanyPriceMetrics = true;
	m_fUpdateStockBasicFinancial = true;
	m_fUpdateStockDayLine = true;
	m_fUpdateForexExchange = true;
	m_fUpdateForexSymbol = true;
	m_fUpdateForexDayLine = true;
	m_fUpdateCryptoExchange = true;
	m_fUpdateCryptoSymbol = true;
	m_fUpdateCryptoDayLine = true;
	m_fUpdateEconomicCalendar = true;

	m_fUpdatePeer = true;
	m_fUpdateInsiderTransaction = true;
	m_fUpdateInsiderSentiment = true;

	if (GetDayOfWeek() == 6) {
		// 每周的星期六更新一次EPSSurprise
		m_lCurrentUpdateEPSSurprisePos = 0;
		SetUpdateEPSSurprise(true);
	}

	return true;
}

void CFinnhubDataSource::UpdateStatus(void) {
	switch (m_pCurrentProduct->GetProductType()) {
	case COMPANY_PROFILE_: // Premium 免费账户无法读取此信息，sandbox模式能读取，但数据是错误的，只能用于测试。
		break;
	case COMPANY_PROFILE_CONCISE_: break;
	case STOCK_SYMBOLS_: break;
	case COMPANY_EXECUTIVE_: // Premium
		break;
	case MARKET_NEWS_: break;
	case COMPANY_NEWS_: break;
	case PRESS_RELEASE_: // Premium
		break;
	case NEWS_SENTIMENT_: break;
	case PEERS_: break;
	case BASIC_FINANCIALS_: break;
	case INSIDER_TRANSACTION_: break;
	case INSIDER_SENTIMENT_: break;
	case OWNERSHIP_: // Premium
		break;
	case FUND_OWNERSHIP_: // jPremium
		break;
	case FINANCIAL_: // Premium
		break;
	case FINANCIAL_AS_REPORTED_: break;
	case SEC_FILINGS_: break;
	case INTERNATIONAL_FILINGS_: // Premium
		break;
	case SEC_SENTIMENT_ANALYSIS_: // Premium
		break;
	case SIMILARITY_INDEX_: // Premium
		break;
	case IPO_CALENDAR_: break;
	case DIVIDENDS_: // Premium
		break;
	case STOCK_ESTIMATES_RECOMMENDATION_TRENDS_: break;
	case STOCK_ESTIMATES_PRICE_TARGET_: break;
	case STOCK_ESTIMATES_UPGRADE_DOWNGRADE_: // Premium
		break;
	case STOCK_ESTIMATES_REVENUE_ESTIMATES_: // Premium
		break;
	case STOCK_ESTIMATES_EPS_ESTIMATES_: // Premium
		break;
	case STOCK_ESTIMATES_EPS_SURPRISE_: break;
	case STOCK_ESTIMATES_EARNING_CALENDAR_: break;
	case STOCK_PRICE_QUOTE_: break;
	case STOCK_PRICE_CANDLES_: break;
	case STOCK_PRICE_TICK_DATA_: // Premium
		break;
	case STOCK_PRICE_LAST_BID_ASK_: // Premium
		break;
	case STOCK_PRICE_SPLITS_: break;
	case FOREX_EXCHANGE_: m_fUpdateForexExchange = false;
		break;
	case FOREX_SYMBOLS_: break;
	case FOREX_CANDLES_: break;
	case FOREX_ALL_RATES_: break;
	case CRYPTO_EXCHANGE_: m_fUpdateCryptoExchange = false;
		break;
	case CRYPTO_SYMBOLS_: break;
	case CRYPTO_CANDLES_: break;
	case ECONOMIC_COUNTRY_LIST_: m_fUpdateCountryList = false;
		break;
	case ECONOMIC_CALENDAR_: m_fUpdateEconomicCalendar = false;
		if (m_pCurrentProduct->IsNoRightToAccess()) {
			gl_systemConfiguration.ChangeFinnhubAccountTypeToFree();
		}
		break;
	default: TRACE("未处理指令%d\n", m_pCurrentProduct->GetProductType());
		gl_systemMessage.PushErrorMessage(_T("Finnhub product未实现"));
		break;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 由系统调度器调度，采用GetTickCount64()来确定当前时间。
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CFinnhubDataSource::GenerateInquiryMessage(const long lCurrentTime) {
	static long long sllLastTimeTickCount = 0;
	static bool sbWebErrorOccurred = false;
	const long long llTickCount = GetTickCount64();

	if (gl_systemStatus.IsWebBusy()) return false; // 网络出现问题时，不申请finnhub各数据。
	if (!sbWebErrorOccurred) {
		if (IsWebError()) {
			sbWebErrorOccurred = true;
			sllLastTimeTickCount += 300000; // 如果出现错误，则延迟五分钟再重新申请。
		}
	}

	if (llTickCount > (sllLastTimeTickCount + gl_systemConfiguration.GetWorldMarketFinnhubInquiryTime())) {
		sbWebErrorOccurred = false; // 申请时清除错误标识
		if (!IsInquiring()) {
			InquireFinnhub(lCurrentTime);
			if (IsInquiring()) sllLastTimeTickCount = llTickCount;
		}
	}
	return true;
}

bool CFinnhubDataSource::InquireFinnhub(const long lCurrentTime) {
	ASSERT(!IsInquiring());
	if (((lCurrentTime < 165700) || (lCurrentTime > 170500))) {
		// 下午五时重启系统，故而此时不允许接收网络信息。
		InquireEconomicCalendar(); // 第一步申请经济日历。此信息为premium，使用此信息来决定账户类型（免费还是收费）。
		InquireCountryList();
		InquireForexExchange();
		InquireCryptoExchange();
		InquireCompanySymbol(); // 第一个动作，首先申请当日证券代码
		InquireForexSymbol();
		InquireCryptoSymbol();

		// 申请Finnhub网络信息的任务，皆要放置在这里，以保证在市场时间凌晨十分钟后执行。这样能够保证在重启市场时不会执行查询任务
		if (gl_pWorldMarket->IsSystemReady()) {
			InquireCompanyProfileConcise();
			InquireCompanyNews(); // 由于mysql字符集的问题，导致有些数据无法存储， 目前未找到解决的方法。故暂时不执行此查询
			InquireCompanyBasicFinancial();
			InquirePeer();
			InquireInsiderTransaction();
			InquireInsiderSentiment();
			InquireCryptoDayLine();
			InquireStockDayLine();
			InquireForexDayLine(); // Forex dayLine目前只限于付费用户使用
			//InquireEPSSurprise(); // 这个现在没什么用，暂时停止更新。
			if (IsUpdateStockDayLine()) {
				//InquireRTQuote();
			}
		}
		return true;
	}
	return false;
}

bool CFinnhubDataSource::InquireCountryList(void) {
	if (!IsInquiring() && IsUpdateCountryList()) {
		const CVirtualProductWebDataPtr product = m_FinnhubFactory.CreateProduct(gl_pWorldMarket.get(), ECONOMIC_COUNTRY_LIST_);
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
	constexpr int iInquiryType = STOCK_SYMBOLS_;

	if (!IsInquiring() && IsUpdateSymbol()) {
		CFinnhubStockExchangePtr pExchange;
		bool fFound = false;
		long lCurrentStockExchangePos;
		if (!m_fInquiringFinnhubStockSymbol) {
			gl_systemMessage.PushInformationMessage(_T("Inquiring finnhub stock symbol..."));
			m_fInquiringFinnhubStockSymbol = true;
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
			gl_pWorldMarket->SetCurrentFunction(_T("Finnhub交易所代码:") + pExchange->m_strCode);
			SetInquiring(true);
			//TRACE("申请%s交易所证券代码\n", pExchange->m_strCode.GetBuffer());
		}
		else {
			m_fInquiringFinnhubStockSymbol = false;
			fHaveInquiry = false;
			SetUpdateSymbol(false);
			const CString str = "Finnhub交易所代码数据查询完毕";
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}

bool CFinnhubDataSource::InquireCompanyProfileConcise(void) {
	const auto lStockSetSize = gl_pWorldMarket->GetStockSize();
	bool fHaveInquiry = false;
	constexpr int iInquiryType = COMPANY_PROFILE_CONCISE_;

	ASSERT(gl_pWorldMarket->IsSystemReady());
	if (!IsInquiring() && IsUpdateStockProfile()) {
		long lCurrentProfilePos;
		bool fFound = false;
		if (!m_fInquiringFinnhubStockProfile) {
			gl_systemMessage.PushInformationMessage(_T("Inquiring finnhub stock profile..."));
			m_fInquiringFinnhubStockProfile = true;
		}
		for (lCurrentProfilePos = 0; lCurrentProfilePos < lStockSetSize; lCurrentProfilePos++) {
			if (const auto pStock = gl_pWorldMarket->GetStock(lCurrentProfilePos); pStock->IsUpdateCompanyProfile()) {
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
			gl_pWorldMarket->SetCurrentFunction(_T("简介:") + gl_pWorldMarket->GetStock(lCurrentProfilePos)->GetSymbol());
			//TRACE("更新%s简介\n", m_vWorldStock.at(m_lCurrentProfilePos)->m_strSymbol.GetBuffer());
			SetInquiring(true);
			fHaveInquiry = true;
		}
		else {
			m_fInquiringFinnhubStockProfile = false;
			SetUpdateStockProfile(false);
			const CString str = "Finnhub股票简介更新完毕";
			gl_systemMessage.PushInformationMessage(str);
			fHaveInquiry = false;
		}
	}
	return fHaveInquiry;
}

/// <summary>
/// 公司新闻。目前finnhub.io只提供北美公司的新闻
/// </summary>
bool CFinnhubDataSource::InquireCompanyNews(void) {
	const auto lStockSetSize = gl_pWorldMarket->GetStockSize();
	bool fHaveInquiry = false;
	constexpr int iInquiryType = COMPANY_NEWS_;

	ASSERT(gl_pWorldMarket->IsSystemReady());
	if (!IsInquiring() && IsUpdateCompanyNews()) {
		long lCurrentCompanyNewsPos;
		bool fFound = false;
		if (!m_fInquiringFinnhubCompanyNews) {
			gl_systemMessage.PushInformationMessage(_T("Inquiring finnhub company news..."));
			m_fInquiringFinnhubCompanyNews = true;
		}
		for (lCurrentCompanyNewsPos = 0; lCurrentCompanyNewsPos < lStockSetSize; lCurrentCompanyNewsPos++) {
			const CWorldStockPtr pStock = gl_pWorldMarket->GetStock(lCurrentCompanyNewsPos);
			if (pStock->IsUpdateCompanyNews()) {
				if (!gl_finnhubInaccessibleExchange.IsInaccessible(iInquiryType, pStock->GetExchangeCode())) {
					// 目前只处理美国市场
					fFound = true;
					break;
				}
			}
		}
		if (fFound) {
			const CVirtualProductWebDataPtr product = m_FinnhubFactory.CreateProduct(gl_pWorldMarket.get(), iInquiryType);
			product->SetIndex(lCurrentCompanyNewsPos);
			StoreInquiry(product);
			gl_pWorldMarket->SetCurrentFunction(_T("公司新闻:") + gl_pWorldMarket->GetStock(lCurrentCompanyNewsPos)->GetSymbol());
			//TRACE("更新%s公司新闻\n", m_vWorldStock.at(m_lCurrentProfilePos)->m_strSymbol.GetBuffer());
			SetInquiring(true);
			fHaveInquiry = true;
		}
		else {
			m_fInquiringFinnhubCompanyNews = false;
			SetUpdateCompanyNews(false);
			const CString str = "Finnhub公司新闻更新完毕";
			gl_systemMessage.PushInformationMessage(str);
			fHaveInquiry = false;
		}
	}
	return fHaveInquiry;
}

bool CFinnhubDataSource::InquireCompanyBasicFinancial(void) {
	const auto lStockSetSize = gl_pWorldMarket->GetStockSize();
	bool fHaveInquiry = false;
	constexpr int iInquiryType = BASIC_FINANCIALS_;

	ASSERT(gl_pWorldMarket->IsSystemReady());
	if (!IsInquiring() && IsUpdateStockBasicFinancial()) {
		bool fFound = false;
		long lCurrentBasicFinancialsPos;
		if (!m_fInquiringFinnhubCompanyBasicFinancial) {
			gl_systemMessage.PushInformationMessage(_T("Inquiring finnhub stock basic financials..."));
			m_fInquiringFinnhubCompanyBasicFinancial = true;
		}
		for (lCurrentBasicFinancialsPos = 0; lCurrentBasicFinancialsPos < lStockSetSize; lCurrentBasicFinancialsPos++) {
			const CWorldStockPtr pStock = gl_pWorldMarket->GetStock(lCurrentBasicFinancialsPos);
			if (pStock->IsUpdateBasicFinancial()) {
				if (!gl_finnhubInaccessibleExchange.IsInaccessible(iInquiryType, pStock->GetExchangeCode())) {
					fFound = true;
					break;
				}
			}
		}
		if (fFound) {
			const CVirtualProductWebDataPtr product = m_FinnhubFactory.CreateProduct(gl_pWorldMarket.get(), iInquiryType);
			product->SetIndex(lCurrentBasicFinancialsPos);
			StoreInquiry(product);
			gl_pWorldMarket->SetCurrentFunction(_T("基本财务:") + gl_pWorldMarket->GetStock(lCurrentBasicFinancialsPos)->GetSymbol());
			//TRACE("更新%s简介\n", m_vWorldStock.at(m_lCurrentProfilePos)->m_strSymbol.GetBuffer());
			SetInquiring(true);
			fHaveInquiry = true;
		}
		else {
			m_fInquiringFinnhubCompanyBasicFinancial = false;
			SetUpdateStockBasicFinancial(false);
			const CString str = "Finnhub股票基本财务情况更新完毕";
			gl_systemMessage.PushInformationMessage(str);
			fHaveInquiry = false;
		}
	}
	return fHaveInquiry;
}

bool CFinnhubDataSource::InquireStockDayLine(void) {
	const auto lStockSetSize = gl_pWorldMarket->GetStockSize();
	bool fHaveInquiry = false;
	constexpr int iInquiryType = STOCK_PRICE_CANDLES_;

	ASSERT(gl_pWorldMarket->IsSystemReady());
	if (!IsInquiring() && IsUpdateStockDayLine()) {
		CWorldStockPtr pStock;
		bool fFound = false;
		if (!m_fInquiringFinnhubStockDayLine) {
			gl_systemMessage.PushInformationMessage(_T("Inquiring finnhub stock day line..."));
			m_fInquiringFinnhubStockDayLine = true;
		}
		for (m_lCurrentUpdateDayLinePos = 0; m_lCurrentUpdateDayLinePos < lStockSetSize; m_lCurrentUpdateDayLinePos++) {
			pStock = gl_pWorldMarket->GetStock(m_lCurrentUpdateDayLinePos);
			if (pStock->IsDayLineNeedUpdate()) {
				// 目前免费账户只能下载美国市场的股票日线。
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
			gl_pWorldMarket->SetCurrentFunction(_T("日线:") + pStock->GetSymbol());
			//TRACE("申请%s日线数据\n", pStock->GetSymbol().GetBuffer());
		}
		else {
			m_fInquiringFinnhubStockDayLine = false;
			fHaveInquiry = false;
			SetUpdateStockDayLine(false);
			m_lCurrentUpdateDayLinePos = 0; // 重置此索引。所有的日线数据更新一次所需时间要超过24小时，故保持更新即可。
			TRACE("Finnhub日线更新完毕，从新开始更新\n");
			const CString str = "US Market日线历史数据更新完毕";
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}

bool CFinnhubDataSource::InquireInsiderTransaction(void) {
	const auto lStockSetSize = gl_pWorldMarket->GetStockSize();
	bool fHaveInquiry = false;
	constexpr int iInquiryType = INSIDER_TRANSACTION_;

	ASSERT(gl_pWorldMarket->IsSystemReady());
	if (!IsInquiring() && IsUpdateInsiderTransaction()) {
		long lCurrentUpdateInsiderTransactionPos;
		CWorldStockPtr pStock;
		bool fFound = false;
		if (!m_fInquiringFinnhubStockInsiderTransaction) {
			gl_systemMessage.PushInformationMessage(_T("Inquiring finnhub stock insider transaction..."));
			m_fInquiringFinnhubStockInsiderTransaction = true;
		}
		for (lCurrentUpdateInsiderTransactionPos = 0; lCurrentUpdateInsiderTransactionPos < lStockSetSize; lCurrentUpdateInsiderTransactionPos++) {
			pStock = gl_pWorldMarket->GetStock(lCurrentUpdateInsiderTransactionPos);
			if (pStock->IsUpdateInsiderTransaction()) {
				// 目前免费账户只能下载美国市场的股票日线。
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
			gl_pWorldMarket->SetCurrentFunction(_T("内部交易:") + pStock->GetSymbol());
			//TRACE("申请%s 内部交易数据\n", pStock->GetSymbol().GetBuffer());
		}
		else {
			m_fInquiringFinnhubStockInsiderTransaction = false;
			fHaveInquiry = false;
			SetUpdateInsiderTransaction(false);
			const CString str = "US Market Insider Transaction数据更新完毕";
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}

bool CFinnhubDataSource::InquireInsiderSentiment(void) {
	const auto lStockSetSize = gl_pWorldMarket->GetStockSize();
	bool fHaveInquiry = false;
	constexpr int iInquiryType = INSIDER_SENTIMENT_;

	ASSERT(gl_pWorldMarket->IsSystemReady());
	if (!IsInquiring() && IsUpdateInsiderSentiment()) {
		long lCurrentUpdateInsiderSentimentPos;
		CWorldStockPtr pStock;
		bool fFound = false;
		if (!m_fInquiringFinnhubStockInsiderSentiment) {
			gl_systemMessage.PushInformationMessage(_T("Inquiring finnhub stock insider sentiment..."));
			m_fInquiringFinnhubStockInsiderSentiment = true;
		}
		for (lCurrentUpdateInsiderSentimentPos = 0; lCurrentUpdateInsiderSentimentPos < lStockSetSize; lCurrentUpdateInsiderSentimentPos++) {
			pStock = gl_pWorldMarket->GetStock(lCurrentUpdateInsiderSentimentPos);
			if (pStock->IsUpdateInsiderSentiment()) {
				if (!gl_finnhubInaccessibleExchange.IsInaccessible(iInquiryType, pStock->GetExchangeCode())) {
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
			gl_pWorldMarket->SetCurrentFunction(_T("内部交易情绪:") + pStock->GetSymbol());
			//TRACE("申请%s 内部交易情绪数据\n", pStock->GetSymbol().GetBuffer());
		}
		else {
			m_fInquiringFinnhubStockInsiderSentiment = false;
			fHaveInquiry = false;
			SetUpdateInsiderSentiment(false);
			TRACE("FinnhubInsider Sentiment更新完毕\n");
			const CString str = "US Market Insider Sentiment数据更新完毕";
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}

bool CFinnhubDataSource::InquireRTQuote(void) {
	if (!IsInquiring()) {
		static long s_lCurrentRTDataQuotePos = 0;
		ASSERT(gl_pWorldMarket->IsSystemReady());
		const CVirtualProductWebDataPtr product = m_FinnhubFactory.CreateProduct(gl_pWorldMarket.get(), STOCK_PRICE_QUOTE_);
		product->SetIndex(s_lCurrentRTDataQuotePos);
		StoreInquiry(product);
		SetInquiring(true);
		s_lCurrentRTDataQuotePos++;
		if (s_lCurrentRTDataQuotePos == gl_pWorldMarket->GetStockSize()) s_lCurrentRTDataQuotePos = 0;
		gl_pWorldMarket->SetCurrentFunction(_T("Finnhub stock RT: ") + gl_pWorldMarket->GetStock(s_lCurrentRTDataQuotePos)->GetSymbol());
		TRACE("申请%s实时数据\n", gl_pWorldMarket->GetStock(s_lCurrentRTDataQuotePos)->GetSymbol().GetBuffer());
	}
	return true;
}

bool CFinnhubDataSource::InquirePeer(void) {
	const auto lStockSetSize = gl_pWorldMarket->GetStockSize();
	bool fHaveInquiry = false;
	constexpr int iInquiryType = PEERS_;

	ASSERT(gl_pWorldMarket->IsSystemReady());
	if (!IsInquiring() && IsUpdatePeer()) {
		long lCurrentUpdatePeerPos;
		bool fFound = false;
		if (!m_fInquiringFinnhubStockPeer) {
			gl_systemMessage.PushInformationMessage(_T("Inquiring finnhub stock peer..."));
			m_fInquiringFinnhubStockPeer = true;
		}
		for (lCurrentUpdatePeerPos = 0; lCurrentUpdatePeerPos < lStockSetSize; lCurrentUpdatePeerPos++) {
			const CWorldStockPtr pStock = gl_pWorldMarket->GetStock(lCurrentUpdatePeerPos);
			if (pStock->IsUpdatePeer()) {
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
			//TRACE("申请%s Peer数据\n", GetStock(lCurrentUpdatePeerPos)->GetSymbol().GetBuffer());
		}
		else {
			m_fInquiringFinnhubStockPeer = false;
			fHaveInquiry = false;
			SetUpdatePeer(false);
			const CString str = "Finnhub Peer Updated";
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}

bool CFinnhubDataSource::InquireEconomicCalendar(void) {
	constexpr int iInquiryType = ECONOMIC_CALENDAR_;

	if (!IsInquiring() && IsUpdateEconomicCalendar()) {
		const CVirtualProductWebDataPtr product = m_FinnhubFactory.CreateProduct(gl_pWorldMarket.get(), iInquiryType);
		StoreInquiry(product);
		SetInquiring(true);
		gl_pWorldMarket->SetCurrentFunction(_T("Finnhub ecomomic calendar updated"));
		gl_systemMessage.PushInformationMessage(_T("Inquiring Finnhub ecomomic calendar"));
		return true;
	}
	return false;
}

bool CFinnhubDataSource::InquireEPSSurprise(void) {
	const auto lStockSetSize = gl_pWorldMarket->GetStockSize();
	bool fHaveInquiry = false;
	constexpr int iInquiryType = STOCK_ESTIMATES_EPS_SURPRISE_;

	ASSERT(gl_pWorldMarket->IsSystemReady());
	if (!IsInquiring() && IsUpdateEPSSurprise()) {
		bool fFound = false;
		for (m_lCurrentUpdateEPSSurprisePos = 0; m_lCurrentUpdateEPSSurprisePos < lStockSetSize; m_lCurrentUpdateEPSSurprisePos++) {
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
			TRACE("申请%s EPS Surprise数据\n", gl_pWorldMarket->GetStock(m_lCurrentUpdateEPSSurprisePos)->GetSymbol().GetBuffer());
		}
		else {
			SetUpdateEPSSurprise(false);
			TRACE("Finnhub EPS Surprise更新完毕\n");
			const CString str = "Finnhub EPS Surprise Updated";
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}

bool CFinnhubDataSource::InquireForexExchange(void) {
	if (!IsInquiring() && IsUpdateForexExchange()) {
		StoreInquiry(m_FinnhubFactory.CreateProduct(gl_pWorldMarket.get(), FOREX_EXCHANGE_));
		SetInquiring(true);
		gl_pWorldMarket->SetCurrentFunction(_T("Finnhub forex exchange"));
		gl_systemMessage.PushInformationMessage(_T("Inquiring Finnhub forex exchange"));
		return true;
	}
	return false;
}

bool CFinnhubDataSource::InquireForexSymbol(void) {
	if (!IsInquiring() && IsUpdateForexSymbol()) {
		static long s_lCurrentForexExchangePos = 0;
		const CVirtualProductWebDataPtr product = m_FinnhubFactory.CreateProduct(gl_pWorldMarket.get(), FOREX_SYMBOLS_);
		product->SetIndex(s_lCurrentForexExchangePos++);
		StoreInquiry(product);
		SetInquiring(true);
		if (s_lCurrentForexExchangePos >= gl_pWorldMarket->GetForexExchangeSize()) {
			SetUpdateForexSymbol(false);
			s_lCurrentForexExchangePos = 0;
			gl_pWorldMarket->SetCurrentFunction(_T("Finnhub forex symbol"));
			gl_systemMessage.PushInformationMessage(_T("Inquiring Finnhub Forex symbols..."));
		}
		return true;
	}
	return false;
}

bool CFinnhubDataSource::InquireForexDayLine(void) {
	const auto lStockSetSize = gl_pWorldMarket->GetForexSymbolSize();
	bool fHaveInquiry = false;
	constexpr int iInquiryType = FOREX_CANDLES_;

	ASSERT(gl_pWorldMarket->IsSystemReady());
	if (!IsInquiring() && IsUpdateForexDayLine()) {
		CForexSymbolPtr pForexSymbol = nullptr;
		long lCurrentUpdateForexDayLinePos;
		bool fFound = false;
		if (!m_fInquiringFinnhubForexDayLine) {
			gl_systemMessage.PushInformationMessage(_T("Inquiring finnhub forex day line..."));
			m_fInquiringFinnhubForexDayLine = true;
		}
		for (lCurrentUpdateForexDayLinePos = 0; lCurrentUpdateForexDayLinePos < lStockSetSize; lCurrentUpdateForexDayLinePos ++) {
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
			gl_pWorldMarket->SetCurrentFunction(_T("Finnhub Forex日线：") + pForexSymbol->GetSymbol());
			SetInquiring(true);
			gl_pWorldMarket->SetCurrentFunction(_T("日线:") + pForexSymbol->GetSymbol());
			TRACE("申请%s日线数据\n", pForexSymbol->GetSymbol().GetBuffer());
		}
		else {
			m_fInquiringFinnhubForexDayLine = false;
			SetUpdateForexDayLine(false);
			TRACE("Finnhub Forex日线更新完毕\n");
			const CString str = "Forex DayLine Updated";
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}

bool CFinnhubDataSource::InquireCryptoExchange(void) {
	if (!IsInquiring() && IsUpdateCryptoExchange()) {
		StoreInquiry(m_FinnhubFactory.CreateProduct(gl_pWorldMarket.get(), CRYPTO_EXCHANGE_));
		SetInquiring(true);
		gl_pWorldMarket->SetCurrentFunction(_T("Finnhub crypto exchange"));
		gl_systemMessage.PushInformationMessage(_T("Inquiring Finnhub crypto exchange"));
		return true;
	}
	return false;
}

bool CFinnhubDataSource::InquireCryptoSymbol(void) {
	if (!IsInquiring() && IsUpdateCryptoSymbol()) {
		static long s_lCurrentCryptoExchangePos = 0;
		const CVirtualProductWebDataPtr product = m_FinnhubFactory.CreateProduct(gl_pWorldMarket.get(), CRYPTO_SYMBOLS_);
		product->SetIndex(s_lCurrentCryptoExchangePos++);
		StoreInquiry(product);
		SetInquiring(true);
		if (s_lCurrentCryptoExchangePos >= gl_pWorldMarket->GetCryptoExchangeSize()) {
			SetUpdateCryptoSymbol(false);
			gl_pWorldMarket->SetCurrentFunction(_T("Finnhub crypto synmbol"));
			gl_systemMessage.PushInformationMessage(_T("Inquiring Finnhub Crypto symbols"));
			s_lCurrentCryptoExchangePos = 0;
		}
		return true;
	}
	return false;
}

bool CFinnhubDataSource::InquireCryptoDayLine(void) {
	const auto lStockSetSize = gl_pWorldMarket->GetFinnhubCryptoSymbolSize();
	bool fHaveInquiry = false;
	constexpr int iInquiryType = CRYPTO_CANDLES_;

	ASSERT(gl_pWorldMarket->IsSystemReady());
	if (!IsInquiring() && IsUpdateCryptoDayLine()) {
		CFinnhubCryptoSymbolPtr pCryptoSymbol;
		long lCurrentUpdateCryptoDayLinePos;
		bool fFound = false;
		if (!m_fInquiringFinnhubCryptoDayLine) {
			gl_systemMessage.PushInformationMessage(_T("Inquiring finnhub crypto day line..."));
			m_fInquiringFinnhubCryptoDayLine = true;
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
			const CVirtualProductWebDataPtr product = m_FinnhubFactory.CreateProduct(gl_pWorldMarket.get(), iInquiryType);
			product->SetIndex(lCurrentUpdateCryptoDayLinePos);
			StoreInquiry(product);
			gl_pWorldMarket->SetCurrentFunction(_T("Finnhub Crypto日线：") + pCryptoSymbol->GetSymbol());
			SetInquiring(true);
			gl_pWorldMarket->SetCurrentFunction(_T("日线:") + pCryptoSymbol->GetSymbol());
			//TRACE("申请%s日线数据\n", pCryptoSymbol->GetSymbol().GetBuffer());
		}
		else {
			m_fInquiringFinnhubCryptoDayLine = false;
			SetUpdateCryptoDayLine(false);
			TRACE("Finnhub Crypto日线更新完毕\n");
			const CString str = "Crypto DayLine Updated";
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}

void CFinnhubDataSource::ConfigureSession(void) {
	ASSERT(m_pSession != nullptr);
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 15000); // 设置连接超时时间为15秒。 正常情况下Finnhub.io数据接收时间不超过5秒。
	m_pSession->SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, 15000); // 设置接收超时时间为15秒
	m_pSession->SetOption(INTERNET_OPTION_SEND_TIMEOUT, 1000); // 设置发送超时时间为1秒
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1); // 1次重试
}

/////////////////////////////////////////////////////////////////////////////////////
//
//
// 使用nlohmann json解析。
//
////////////////////////////////////////////////////////////////////////////////////
bool CFinnhubDataSource::ParseData(CWebDataPtr pWebData) {
	return pWebData->CreateNlohmannJson(0, 0);
}
