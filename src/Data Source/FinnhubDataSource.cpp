//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// finnhub.io中为免费用户使用的数据逐渐减少，需要设置一新的数据库来存储各数据的免费与否的状态。
///
/// 20231126：20231127后，stock DayLine和Crypto DayLine不再免费。
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include"SystemConfiguration.h"

#include"FinnhubInquiryType.h"
#include"FinnhubInaccessibleExchange.h"

#include "FinnhubDataSource.h"

#include "TimeConvert.h"
#include"WorldMarket.h"

map<string, enum_ErrorMessageData> mapFinnhubErrorMap{
	{ _T("You don't have access to this resource."), ERROR_FINNHUB_NO_RIGHT_TO_ACCESS__ },
	{ _T("Please use an API key."), ERROR_FINNHUB_MISSING_API_KEY__ },
	{ _T(""), ERROR_FINNHUB_INQUIRE_RATE_TOO_HIGH__ }
};

CFinnhubDataSource::CFinnhubDataSource() {
	ASSERT(gl_systemConfiguration.IsInitialized());
	// 无需（也无法）每日更新的变量放在这里

	m_strInquiryFunction = _T(""); // finnhub有各种数据，故其前缀由数据申请函数每次设置，不同的前缀申请不同的数据。
	m_strParam = _T("");
	m_strSuffix = _T("&token=");
	m_strInquiryToken = _T("");
	m_lInquiringNumber = 1; // Finnhub实时数据查询数量默认值
	m_iMaxNormalInquireTime = 1000;

	CFinnhubDataSource::ConfigureInternetOption();

	CFinnhubDataSource::Reset();
}

bool CFinnhubDataSource::Reset() {
	// Finnhub各申请网络数据标识，每日需要重置。目前共21项。
	m_fUpdateCountryList = true;
	m_fUpdateSymbol = true; // 每日需要更新代码
	m_fUpdateMarketStatus = true;
	m_fUpdateMarketHoliday = true;
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
	m_fUpdateEPSSurprise = true;
	m_fUpdateSECFilings = true;

	m_lCurrentRTDataQuotePos = 0;
	m_lCurrentForexExchangePos = 0;
	m_lCurrentCryptoExchangePos = 0;

	m_fFinnhubDataInquiryFinished = false;

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 经测试，这里所有的timeout时间，都是数据中断后等待的时间。如果有数据，无论接收发送的速度多慢，都不会触发计时器，亦即不会出现timeout。
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CFinnhubDataSource::ConfigureInternetOption() {
	m_internetOption.option_connect_timeout = 15000;
	m_internetOption.option_receive_timeout = 10000;
	m_internetOption.option_data_receive_timeout = 10000;
	m_internetOption.option_send_timeout = 1000;
	m_internetOption.option_connect_retries = 1;
}

enum_ErrorMessageData CFinnhubDataSource::IsAErrorMessageData(const CWebDataPtr& pWebData) {
	ASSERT(m_pCurrentProduct != nullptr);

	m_eErrorMessageData = ERROR_NO_ERROR__;
	if (m_dwHTTPStatusCode == 200) return m_eErrorMessageData; // OK? return no error

	json js;
	pWebData->CreateJson(js);

	try {
		string error = js.at(_T("error"));
		int i;
		CString s;
		try {
			m_eErrorMessageData = mapFinnhubErrorMap.at(error);
		} catch (exception&) {
			m_eErrorMessageData = ERROR_FINNHUB_NOT_HANDLED__;
		}
		switch (m_eErrorMessageData) {
		case ERROR_FINNHUB_NO_RIGHT_TO_ACCESS__:// 无权申请
			m_pCurrentProduct->SetReceivedDataStatus(NO_ACCESS_RIGHT_);
			if (m_pCurrentProduct->CheckInaccessible()) {
				// 如果系统报告无权查询此类数据, 目前先在软件系统消息中报告
				gl_systemMessage.PushInnerSystemInformationMessage(_T("No right to access: ") + m_pCurrentProduct->GetInquiry() + _T(",  Exchange = ") + m_pCurrentProduct->GetInquiringExchange());
			}
			break;
		case ERROR_FINNHUB_MISSING_API_KEY__: // 缺少API key
			gl_systemMessage.PushErrorMessage(_T("finnhub missing API key"));
			break;
		case ERROR_FINNHUB_INQUIRE_RATE_TOO_HIGH__:// 申请频率超高
			// 降低查询频率200ms。
			// todo 这里最好只向系统报告频率超出，由系统决定如何修正。
			i = gl_systemConfiguration.GetWorldMarketFinnhubInquiryTime();
			gl_systemConfiguration.SetWorldMarketFinnhubInquiryTime(i + 200);
			break;
		case ERROR_FINNHUB_NOT_HANDLED__: // error not handled
			ReportErrorNotHandled(error);
			break;
		default: // 缺省分支不应该出现
			ASSERT(false);
			break;
		}
	} catch (json::exception&) { // no error. do nothing
		m_eErrorMessageData = ERROR_NO_ERROR__;
	}
	return m_eErrorMessageData;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 由系统调度器调度，采用GetTickCount()来确定当前时间。
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CFinnhubDataSource::GenerateInquiryMessage(long lCurrentTime) {
	const long long llTickCount = GetTickCount();

	if (gl_systemConfiguration.IsWebBusy()) return false; // 网络出现问题时，不申请finnhub各数据。
	if (llTickCount > (m_llLastTimeTickCount + gl_systemConfiguration.GetWorldMarketFinnhubInquiryTime())) {
		m_llLastTimeTickCount = llTickCount;

		if (!IsInquiring()) {
			ASSERT(!HaveInquiry());
			Inquire(lCurrentTime);
			if (IsInquiring()) {
				ASSERT(HaveInquiry());
				return true;
			}
		}
	}
	return false;
}

void CFinnhubDataSource::Inquire(const long lCurrentTime) {
	ASSERT(!IsInquiring());
	const long resettingTime = gl_systemConfiguration.GetWorldMarketResettingTime();
	ASSERT(lCurrentTime <= GetPrevTime(resettingTime, 0, 10, 0) || lCurrentTime >= GetNextTime(resettingTime, 0, 5, 0)); // 重启市场时不允许接收网络信息。
	InquireEconomicCalendar(); // 第一步申请经济日历。此信息为premium，使用此信息来决定账户类型（免费还是收费）。
	InquireCountryList();
	// Finnhub不提供Stock Exchange名单，使用预先提供的股票交易所列表。
	InquireForexExchange();
	InquireCryptoExchange();
	InquireMarketStatus();
	InquireMarketHoliday();
	InquireCompanySymbol(); // 第一个动作，首先申请当日证券代码
	InquireForexSymbol();
	InquireCryptoSymbol();

	// 申请Finnhub网络信息的任务，皆要放置在这里，以保证在市场时间凌晨十分钟后执行。这样能够保证在重启市场时不会执行查询任务
	if (gl_pWorldMarket->IsSystemReady()) {
		InquireCompanyProfileConcise();
		InquireCompanyNews();
		InquireCompanyBasicFinancial();
		InquirePeer();
		InquireInsiderTransaction();
		InquireInsiderSentiment();
		InquireCryptoDayLine(); // Crypto dayLine20231127后只限于付费用户使用
		InquireStockDayLine(); // Stock dayLine20231127后只限于付费用户使用
		InquireForexDayLine(); // Forex dayLine目前只限于付费用户使用
		InquireEPSSurprise();
		InquireSECFilings();
		if (IsUpdateStockDayLine()) {
			//InquireRTQuote();
		}
		if (!IsInquiring()) {
			if (!m_fFinnhubDataInquiryFinished) {
				gl_systemMessage.PushInformationMessage(_T("finnhub data inquiry finished"));
				m_fFinnhubDataInquiryFinished = true;
			}
		}
	}
}

bool CFinnhubDataSource::InquireCountryList() {
	if (!IsInquiring() && IsUpdateCountryList()) {
		const CVirtualProductWebDataPtr product = m_FinnhubFactory.CreateProduct(gl_pWorldMarket, ECONOMIC_COUNTRY_LIST_);
		StoreInquiry(product);
		SetInquiring(true);
		gl_pWorldMarket->SetCurrentFinnhubFunction(_T("Finnhub country List"));
		gl_systemMessage.PushInformationMessage(_T("Inquiring Finnhub economic country List"));
		return true;
	}
	return false;
}

bool CFinnhubDataSource::InquireMarketStatus() {
	const long lExchangeSize = gl_dataContainerFinnhubStockExchange.Size();
	bool fHaveInquiry = false;
	constexpr int iInquireType = MARKET_STATUS_;

	if (!IsInquiring() && IsUpdateMarketStatus()) {
		CFinnhubStockExchangePtr pExchange;
		bool fFound = false;
		long lCurrentStockExchangePos;
		if (!m_fInquiringFinnhubMarketStatus) {
			gl_systemMessage.PushInformationMessage(_T("Inquiring finnhub MarketStatus..."));
			m_fInquiringFinnhubMarketStatus = true;
		}
		for (lCurrentStockExchangePos = 0; lCurrentStockExchangePos < lExchangeSize; lCurrentStockExchangePos++) {
			if (!gl_dataContainerFinnhubStockExchange.GetExchange(lCurrentStockExchangePos)->IsMarketStatusUpdated()) {
				pExchange = gl_dataContainerFinnhubStockExchange.GetExchange(lCurrentStockExchangePos);
				if (!gl_finnhubInaccessibleExchange.HaveExchange(iInquireType, pExchange->m_strCode)) {
					fFound = true;
					break;
				}
			}
		}
		if (fFound) {
			fHaveInquiry = true;
			const auto product = m_FinnhubFactory.CreateProduct(gl_pWorldMarket, iInquireType);
			product->SetIndex(lCurrentStockExchangePos);
			StoreInquiry(product);
			SetInquiring(true);
			gl_pWorldMarket->SetCurrentFinnhubFunction(_T("Market status: ") + pExchange->m_strCode);
		}
		else {
			m_fInquiringFinnhubMarketStatus = false;
			fHaveInquiry = false;
			SetUpdateMarketStatus(false);
			const CString str = "Inquire finnhub MarketStatus finished";
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}

bool CFinnhubDataSource::InquireMarketHoliday() {
	const long lExchangeSize = gl_dataContainerFinnhubStockExchange.Size();
	bool fHaveInquiry = false;
	constexpr int iInquireType = MARKET_HOLIDAY_;

	if (!IsInquiring() && IsUpdateMarketHoliday()) {
		CFinnhubStockExchangePtr pExchange;
		bool fFound = false;
		long lCurrentStockExchangePos;
		if (!m_fInquiringFinnhubMarketHoliday) {
			gl_systemMessage.PushInformationMessage(_T("Inquiring finnhub market holiday..."));
			m_fInquiringFinnhubMarketHoliday = true;
		}
		for (lCurrentStockExchangePos = 0; lCurrentStockExchangePos < lExchangeSize; lCurrentStockExchangePos++) {
			if (!gl_dataContainerFinnhubStockExchange.GetExchange(lCurrentStockExchangePos)->IsMarketHolidayUpdated()) {
				pExchange = gl_dataContainerFinnhubStockExchange.GetExchange(lCurrentStockExchangePos);
				if (!gl_finnhubInaccessibleExchange.HaveExchange(iInquireType, pExchange->m_strCode)) {
					fFound = true;
					break;
				}
			}
		}
		if (fFound) {
			fHaveInquiry = true;
			const auto product = m_FinnhubFactory.CreateProduct(gl_pWorldMarket, iInquireType);
			product->SetIndex(lCurrentStockExchangePos);
			StoreInquiry(product);
			SetInquiring(true);
			gl_pWorldMarket->SetCurrentFinnhubFunction(_T("Market holiday: ") + pExchange->m_strCode);
		}
		else {
			m_fInquiringFinnhubMarketHoliday = false;
			fHaveInquiry = false;
			SetUpdateMarketHoliday(false);
			const CString str = "Inquire finnhub market holiday finished";
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}

bool CFinnhubDataSource::InquireCompanySymbol() {
	const long lExchangeSize = gl_dataContainerFinnhubStockExchange.Size();
	bool fHaveInquiry = false;
	constexpr int iInquireType = STOCK_SYMBOLS_;

	if (!IsInquiring() && IsUpdateSymbol()) {
		CFinnhubStockExchangePtr pExchange;
		bool fFound = false;
		long lCurrentStockExchangePos;
		if (!m_fInquiringFinnhubStockSymbol) {
			gl_systemMessage.PushInformationMessage(_T("Inquiring finnhub stock symbol..."));
			m_fInquiringFinnhubStockSymbol = true;
		}
		for (lCurrentStockExchangePos = 0; lCurrentStockExchangePos < lExchangeSize; lCurrentStockExchangePos++) {
			if (!gl_dataContainerFinnhubStockExchange.GetExchange(lCurrentStockExchangePos)->IsStockSymbolUpdated()) {
				pExchange = gl_dataContainerFinnhubStockExchange.GetExchange(lCurrentStockExchangePos);
				if (!gl_finnhubInaccessibleExchange.HaveExchange(iInquireType, pExchange->m_strCode)) {
					fFound = true;
					break;
				}
			}
		}
		if (fFound) {
			fHaveInquiry = true;
			const auto product = m_FinnhubFactory.CreateProduct(gl_pWorldMarket, iInquireType);
			product->SetIndex(lCurrentStockExchangePos);
			StoreInquiry(product);
			SetInquiring(true);
			gl_pWorldMarket->SetCurrentFinnhubFunction(_T("交易所代码:") + pExchange->m_strCode);
			//TRACE("申请%s交易所证券代码\n", pExchange->m_strCode.GetBuffer());
		}
		else {
			m_fInquiringFinnhubStockSymbol = false;
			fHaveInquiry = false;
			SetUpdateSymbol(false);
			const CString str = "Finnhub exchanges updated";
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}

bool CFinnhubDataSource::InquireCompanyProfileConcise() {
	const auto lStockSetSize = gl_dataContainerFinnhubStock.Size();
	bool fHaveInquiry = false;
	constexpr int iInquireType = COMPANY_PROFILE_CONCISE_;

	ASSERT(gl_pWorldMarket->IsSystemReady());
	if (!IsInquiring() && IsUpdateStockProfile()) {
		long lCurrentProfilePos;
		bool fFound = false;
		if (!m_fInquiringFinnhubStockProfile) {
			gl_systemMessage.PushInformationMessage(_T("Inquiring finnhub stock profile..."));
			m_fInquiringFinnhubStockProfile = true;
		}
		for (lCurrentProfilePos = 0; lCurrentProfilePos < lStockSetSize; lCurrentProfilePos++) {
			if (const auto pStock = gl_dataContainerFinnhubStock.GetStock(lCurrentProfilePos); pStock->IsUpdateCompanyProfile()) {
				if (!gl_finnhubInaccessibleExchange.HaveExchange(iInquireType, pStock->GetExchangeCode())) {
					fFound = true;
					break;
				}
			}
		}
		if (fFound) {
			const auto product = m_FinnhubFactory.CreateProduct(gl_pWorldMarket, iInquireType);
			product->SetIndex(lCurrentProfilePos);
			StoreInquiry(product);
			SetInquiring(true);
			fHaveInquiry = true;
			gl_pWorldMarket->SetCurrentFinnhubFunction(_T("简介:") + gl_dataContainerFinnhubStock.GetStock(lCurrentProfilePos)->GetSymbol());
			//TRACE("更新%s简介\n", m_vWorldStock.at(m_lCurrentProfilePos)->m_strSymbol.GetBuffer());
		}
		else {
			m_fInquiringFinnhubStockProfile = false;
			SetUpdateStockProfile(false);
			const CString str = "Finnhub company profile basic updated";
			gl_systemMessage.PushInformationMessage(str);
			fHaveInquiry = false;
		}
	}
	return fHaveInquiry;
}

/// <summary>
/// 公司新闻。目前finnhub.io只提供北美公司的新闻
/// </summary>
bool CFinnhubDataSource::InquireCompanyNews() {
	const auto lStockSetSize = gl_dataContainerFinnhubStock.Size();
	bool fHaveInquiry = false;
	constexpr int iInquireType = COMPANY_NEWS_;

	ASSERT(gl_pWorldMarket->IsSystemReady());
	if (!IsInquiring() && IsUpdateCompanyNews()) {
		long lCurrentCompanyNewsPos;
		bool fFound = false;
		if (!m_fInquiringFinnhubCompanyNews) {
			gl_systemMessage.PushInformationMessage(_T("Inquiring finnhub company news..."));
			m_fInquiringFinnhubCompanyNews = true;
		}
		for (lCurrentCompanyNewsPos = 0; lCurrentCompanyNewsPos < lStockSetSize; lCurrentCompanyNewsPos++) {
			const CFinnhubStockPtr pStock = gl_dataContainerFinnhubStock.GetStock(lCurrentCompanyNewsPos);
			if (pStock->IsUpdateCompanyNews()) {
				if (!gl_finnhubInaccessibleExchange.HaveExchange(iInquireType, pStock->GetExchangeCode())) {
					// 目前只处理美国市场
					fFound = true;
					break;
				}
			}
		}
		if (fFound) {
			const CVirtualProductWebDataPtr product = m_FinnhubFactory.CreateProduct(gl_pWorldMarket, iInquireType);
			product->SetIndex(lCurrentCompanyNewsPos);
			StoreInquiry(product);
			SetInquiring(true);
			fHaveInquiry = true;
			gl_pWorldMarket->SetCurrentFinnhubFunction(_T("公司新闻:") + gl_dataContainerFinnhubStock.GetStock(lCurrentCompanyNewsPos)->GetSymbol());
			//TRACE("更新%s公司新闻\n", m_vWorldStock.at(m_lCurrentProfilePos)->m_strSymbol.GetBuffer());
		}
		else {
			m_fInquiringFinnhubCompanyNews = false;
			SetUpdateCompanyNews(false);
			const CString str = "Finnhub company news updated";
			gl_systemMessage.PushInformationMessage(str);
			fHaveInquiry = false;
		}
	}
	return fHaveInquiry;
}

bool CFinnhubDataSource::InquireCompanyBasicFinancial() {
	const auto lStockSetSize = gl_dataContainerFinnhubStock.Size();
	bool fHaveInquiry = false;
	constexpr int iInquireType = BASIC_FINANCIALS_;

	ASSERT(gl_pWorldMarket->IsSystemReady());
	if (!IsInquiring() && IsUpdateStockBasicFinancial()) {
		bool fFound = false;
		long lCurrentBasicFinancialsPos;
		if (!m_fInquiringFinnhubCompanyBasicFinancial) {
			gl_systemMessage.PushInformationMessage(_T("Inquiring finnhub stock basic financials..."));
			m_fInquiringFinnhubCompanyBasicFinancial = true;
		}
		for (lCurrentBasicFinancialsPos = 0; lCurrentBasicFinancialsPos < lStockSetSize; lCurrentBasicFinancialsPos++) {
			const CFinnhubStockPtr pStock = gl_dataContainerFinnhubStock.GetStock(lCurrentBasicFinancialsPos);
			if (pStock->IsUpdateBasicFinancial()) {
				if (!gl_finnhubInaccessibleExchange.HaveExchange(iInquireType, pStock->GetExchangeCode())) {
					fFound = true;
					break;
				}
			}
		}
		if (fFound) {
			const CVirtualProductWebDataPtr product = m_FinnhubFactory.CreateProduct(gl_pWorldMarket, iInquireType);
			product->SetIndex(lCurrentBasicFinancialsPos);
			StoreInquiry(product);
			SetInquiring(true);
			fHaveInquiry = true;
			gl_pWorldMarket->SetCurrentFinnhubFunction(_T("基本财务:") + gl_dataContainerFinnhubStock.GetStock(lCurrentBasicFinancialsPos)->GetSymbol());
			//TRACE("更新%s简介\n", m_vWorldStock.at(m_lCurrentProfilePos)->m_strSymbol.GetBuffer());
		}
		else {
			m_fInquiringFinnhubCompanyBasicFinancial = false;
			SetUpdateStockBasicFinancial(false);
			const CString str = "Finnhub basic financial updated";
			gl_systemMessage.PushInformationMessage(str);
			fHaveInquiry = false;
		}
	}
	return fHaveInquiry;
}

bool CFinnhubDataSource::InquireStockDayLine() {
	const auto lStockSetSize = gl_dataContainerFinnhubStock.Size();
	bool fHaveInquiry = false;
	constexpr int iInquireType = STOCK_PRICE_CANDLES_;

	ASSERT(gl_pWorldMarket->IsSystemReady());
	if (!IsInquiring() && IsUpdateStockDayLine()) {
		CFinnhubStockPtr pStock;
		bool fFound = false;
		long lCurrentUpdateDayLinePos;
		if (!m_fInquiringFinnhubStockDayLine) {
			gl_systemMessage.PushInformationMessage(_T("Inquiring finnhub stock day line..."));
			m_fInquiringFinnhubStockDayLine = true;
		}
		for (lCurrentUpdateDayLinePos = 0; lCurrentUpdateDayLinePos < lStockSetSize; lCurrentUpdateDayLinePos++) {
			pStock = gl_dataContainerFinnhubStock.GetStock(lCurrentUpdateDayLinePos);
			if (pStock->IsUpdateDayLine()) {
				// 目前免费账户只能下载美国市场的股票日线。
				if (!gl_finnhubInaccessibleExchange.HaveExchange(iInquireType, pStock->GetExchangeCode())) {
					fFound = true;
					break;
				}
			}
		}
		if (fFound) {
			fHaveInquiry = true;
			const auto product = m_FinnhubFactory.CreateProduct(gl_pWorldMarket, iInquireType);
			product->SetIndex(lCurrentUpdateDayLinePos);
			StoreInquiry(product);
			SetInquiring(true);
			gl_pWorldMarket->SetCurrentFinnhubFunction(_T("日线:") + pStock->GetSymbol());
			//TRACE("申请%s日线数据\n", pStock->GetSymbol().GetBuffer());
		}
		else {
			m_fInquiringFinnhubStockDayLine = false;
			fHaveInquiry = false;
			SetUpdateStockDayLine(false);
			//lCurrentUpdateDayLinePos = 0; // 重置此索引。所有的日线数据更新一次所需时间要超过24小时，故保持更新即可。
			//TRACE("Finnhub日线更新完毕，从新开始更新\n");
			const CString str = "US Market dayline updated";
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}

bool CFinnhubDataSource::InquireInsiderTransaction() {
	const auto lStockSetSize = gl_dataContainerFinnhubStock.Size();
	bool fHaveInquiry = false;
	constexpr int iInquireType = INSIDER_TRANSACTION_;

	ASSERT(gl_pWorldMarket->IsSystemReady());
	if (!IsInquiring() && IsUpdateInsiderTransaction()) {
		long lCurrentUpdateInsiderTransactionPos;
		CFinnhubStockPtr pStock;
		bool fFound = false;
		if (!m_fInquiringFinnhubStockInsiderTransaction) {
			gl_systemMessage.PushInformationMessage(_T("Inquiring finnhub stock insider transaction..."));
			m_fInquiringFinnhubStockInsiderTransaction = true;
		}
		for (lCurrentUpdateInsiderTransactionPos = 0; lCurrentUpdateInsiderTransactionPos < lStockSetSize; lCurrentUpdateInsiderTransactionPos++) {
			pStock = gl_dataContainerFinnhubStock.GetStock(lCurrentUpdateInsiderTransactionPos);
			if (pStock->IsUpdateInsiderTransaction()) {
				// 目前免费账户只能下载美国市场的内部交易数据。
				if (!gl_finnhubInaccessibleExchange.HaveExchange(iInquireType, pStock->GetExchangeCode())) {
					fFound = true;
					break;
				}
			}
		}
		if (fFound) {
			fHaveInquiry = true;
			const CVirtualProductWebDataPtr product = m_FinnhubFactory.CreateProduct(gl_pWorldMarket, iInquireType);
			product->SetIndex(lCurrentUpdateInsiderTransactionPos);
			StoreInquiry(product);
			SetInquiring(true);
			gl_pWorldMarket->SetCurrentFinnhubFunction(_T("内部交易:") + pStock->GetSymbol());
			//TRACE("申请%s 内部交易数据\n", pStock->GetSymbol().GetBuffer());
		}
		else {
			m_fInquiringFinnhubStockInsiderTransaction = false;
			fHaveInquiry = false;
			SetUpdateInsiderTransaction(false);
			const CString str = "US Market Insider Transaction updated";
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}

bool CFinnhubDataSource::InquireInsiderSentiment() {
	const auto lStockSetSize = gl_dataContainerFinnhubStock.Size();
	bool fHaveInquiry = false;
	constexpr int iInquireType = INSIDER_SENTIMENT_;

	ASSERT(gl_pWorldMarket->IsSystemReady());
	if (!IsInquiring() && IsUpdateInsiderSentiment()) {
		long lCurrentUpdateInsiderSentimentPos;
		CFinnhubStockPtr pStock;
		bool fFound = false;
		if (!m_fInquiringFinnhubStockInsiderSentiment) {
			gl_systemMessage.PushInformationMessage(_T("Inquiring finnhub stock insider sentiment..."));
			m_fInquiringFinnhubStockInsiderSentiment = true;
		}
		for (lCurrentUpdateInsiderSentimentPos = 0; lCurrentUpdateInsiderSentimentPos < lStockSetSize; lCurrentUpdateInsiderSentimentPos++) {
			pStock = gl_dataContainerFinnhubStock.GetStock(lCurrentUpdateInsiderSentimentPos);
			if (pStock->IsUpdateInsiderSentiment()) {
				if (!gl_finnhubInaccessibleExchange.HaveExchange(iInquireType, pStock->GetExchangeCode())) {
					fFound = true;
					break;
				}
			}
		}
		if (fFound) {
			fHaveInquiry = true;
			const CVirtualProductWebDataPtr product = m_FinnhubFactory.CreateProduct(gl_pWorldMarket, iInquireType);
			product->SetIndex(lCurrentUpdateInsiderSentimentPos);
			StoreInquiry(product);
			SetInquiring(true);
			gl_pWorldMarket->SetCurrentFinnhubFunction(_T("内部交易情绪:") + pStock->GetSymbol());
		}
		else {
			m_fInquiringFinnhubStockInsiderSentiment = false;
			fHaveInquiry = false;
			SetUpdateInsiderSentiment(false);
			//TRACE("FinnhubInsider Sentiment更新完毕\n");
			const CString str = "US Market Insider Sentiment updated";
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}

bool CFinnhubDataSource::InquireRTQuote() {
	if (!IsInquiring()) {
		ASSERT(gl_pWorldMarket->IsSystemReady());
		const CVirtualProductWebDataPtr product = m_FinnhubFactory.CreateProduct(gl_pWorldMarket, STOCK_PRICE_QUOTE_);
		product->SetIndex(m_lCurrentRTDataQuotePos);
		StoreInquiry(product);
		SetInquiring(true);
		m_lCurrentRTDataQuotePos++;
		if (m_lCurrentRTDataQuotePos == gl_dataContainerFinnhubStock.Size()) m_lCurrentRTDataQuotePos = 0;
		gl_pWorldMarket->SetCurrentFinnhubFunction(_T("stock RT: ") + gl_dataContainerFinnhubStock.GetStock(m_lCurrentRTDataQuotePos)->GetSymbol());
		//TRACE("申请%s实时数据\n", gl_dataContainerFinnhubStock.GetStock(m_lCurrentRTDataQuotePos)->GetSymbol().GetBuffer());
	}
	return true;
}

bool CFinnhubDataSource::InquirePeer() {
	const auto lStockSetSize = gl_dataContainerFinnhubStock.Size();
	bool fHaveInquiry = false;
	constexpr int iInquireType = PEERS_;

	ASSERT(gl_pWorldMarket->IsSystemReady());
	if (!IsInquiring() && IsUpdatePeer()) {
		long lCurrentUpdatePeerPos;
		bool fFound = false;
		if (!m_fInquiringFinnhubStockPeer) {
			gl_systemMessage.PushInformationMessage(_T("Inquiring finnhub stock peer..."));
			m_fInquiringFinnhubStockPeer = true;
		}
		for (lCurrentUpdatePeerPos = 0; lCurrentUpdatePeerPos < lStockSetSize; lCurrentUpdatePeerPos++) {
			const CFinnhubStockPtr pStock = gl_dataContainerFinnhubStock.GetStock(lCurrentUpdatePeerPos);
			if (pStock->IsUpdatePeer()) {
				if (!gl_finnhubInaccessibleExchange.HaveExchange(iInquireType, pStock->GetExchangeCode())) {
					fFound = true;
					break;
				}
			}
		}
		if (fFound) {
			fHaveInquiry = true;
			const CVirtualProductWebDataPtr product = m_FinnhubFactory.CreateProduct(gl_pWorldMarket, iInquireType);
			product->SetIndex(lCurrentUpdatePeerPos);
			StoreInquiry(product);
			SetInquiring(true);
			gl_pWorldMarket->SetCurrentFinnhubFunction(_T("Peer:") + gl_dataContainerFinnhubStock.GetStock(lCurrentUpdatePeerPos)->GetSymbol());
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

bool CFinnhubDataSource::InquireEconomicCalendar() {
	constexpr int iInquireType = ECONOMIC_CALENDAR_;

	if (!IsInquiring() && IsUpdateEconomicCalendar()) {
		const CVirtualProductWebDataPtr product = m_FinnhubFactory.CreateProduct(gl_pWorldMarket, iInquireType);
		StoreInquiry(product);
		SetInquiring(true);
		gl_pWorldMarket->SetCurrentFinnhubFunction(_T("economic calendar updated"));
		gl_systemMessage.PushInformationMessage(_T("Inquiring Finnhub economic calendar"));
		return true;
	}
	return false;
}

bool CFinnhubDataSource::InquireEPSSurprise() {
	const auto lStockSetSize = gl_dataContainerFinnhubStock.Size();
	bool fHaveInquiry = false;
	constexpr int iInquireType = STOCK_ESTIMATES_EPS_SURPRISE_;

	ASSERT(gl_pWorldMarket->IsSystemReady());
	if (!IsInquiring() && IsUpdateEPSSurprise()) {
		CFinnhubStockPtr pStock;
		bool fFound = false;
		if (!m_fInquiringFinnhubStockEPSSurprise) {
			gl_systemMessage.PushInformationMessage(_T("Inquiring finnhub stock EPS surprise..."));
			m_fInquiringFinnhubStockEPSSurprise = true;
		}
		for (m_lCurrentUpdateEPSSurprisePos = 0; m_lCurrentUpdateEPSSurprisePos < lStockSetSize; m_lCurrentUpdateEPSSurprisePos++) {
			pStock = gl_dataContainerFinnhubStock.GetStock(m_lCurrentUpdateEPSSurprisePos);
			if (pStock->IsUpdateEPSSurprise()) {
				if (!gl_finnhubInaccessibleExchange.HaveExchange(iInquireType, pStock->GetExchangeCode())) {
					fFound = true;
					break;
				}
			}
		}
		if (fFound) {
			fHaveInquiry = true;
			const CVirtualProductWebDataPtr product = m_FinnhubFactory.CreateProduct(gl_pWorldMarket, iInquireType);
			product->SetIndex(m_lCurrentUpdateEPSSurprisePos);
			StoreInquiry(product);
			SetInquiring(true);
			gl_dataContainerFinnhubStock.GetStock(m_lCurrentUpdateEPSSurprisePos)->SetUpdateEPSSurprise(false);
			gl_pWorldMarket->SetCurrentFinnhubFunction(_T("EPS surprise:") + pStock->GetSymbol());
			//TRACE("申请%s EPS Surprise数据\n", gl_dataContainerFinnhubStock.GetStock(m_lCurrentUpdateEPSSurprisePos)->GetSymbol().GetBuffer());
		}
		else {
			m_fInquiringFinnhubStockEPSSurprise = false;
			m_lCurrentUpdateEPSSurprisePos = 0;
			fHaveInquiry = false;
			SetUpdateEPSSurprise(false);
			TRACE("Finnhub EPS Surprise更新完毕\n");
			const CString str = "Finnhub EPS Surprise Updated";
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}

bool CFinnhubDataSource::InquireSECFilings() {
	const auto lStockSetSize = gl_dataContainerFinnhubStock.Size();
	bool fHaveInquiry = false;
	constexpr int iInquireType = SEC_FILINGS_;

	ASSERT(gl_pWorldMarket->IsSystemReady());
	if (!IsInquiring() && IsUpdateSECFilings()) {
		CFinnhubStockPtr pStock;
		bool fFound = false;
		if (!m_fInquiringFinnhubStockSECFilings) {
			gl_systemMessage.PushInformationMessage(_T("Inquiring finnhub stock SEC Filings..."));
			m_fInquiringFinnhubStockSECFilings = true;
		}
		for (m_lCurrentUpdateSECFilingsPos = 0; m_lCurrentUpdateSECFilingsPos < lStockSetSize; m_lCurrentUpdateSECFilingsPos++) {
			pStock = gl_dataContainerFinnhubStock.GetStock(m_lCurrentUpdateSECFilingsPos);
			if (!pStock->IsSECFilingsUpdated()) {
				if (!gl_finnhubInaccessibleExchange.HaveExchange(iInquireType, pStock->GetExchangeCode())) {
					fFound = true;
					break;
				}
			}
		}
		if (fFound) {
			fHaveInquiry = true;
			const CVirtualProductWebDataPtr product = m_FinnhubFactory.CreateProduct(gl_pWorldMarket, iInquireType);
			product->SetIndex(m_lCurrentUpdateSECFilingsPos);
			StoreInquiry(product);
			SetInquiring(true);
			gl_dataContainerFinnhubStock.GetStock(m_lCurrentUpdateSECFilingsPos)->SetSECFilingsUpdated(true);
			gl_pWorldMarket->SetCurrentFinnhubFunction(_T("SEC Filings:") + pStock->GetSymbol());
		}
		else {
			m_fInquiringFinnhubStockSECFilings = false;
			m_lCurrentUpdateSECFilingsPos = 0;
			fHaveInquiry = false;
			SetUpdateSECFilings(false);
			TRACE("Finnhub SEC Filings 更新完毕\n");
			const CString str = "Finnhub SEC Filings Updated";
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}

bool CFinnhubDataSource::InquireForexExchange() {
	if (!IsInquiring() && IsUpdateForexExchange()) {
		StoreInquiry(m_FinnhubFactory.CreateProduct(gl_pWorldMarket, FOREX_EXCHANGE_));
		SetInquiring(true);
		gl_pWorldMarket->SetCurrentFinnhubFunction(_T("forex exchange"));
		gl_systemMessage.PushInformationMessage(_T("Inquiring Finnhub forex exchange"));
		return true;
	}
	return false;
}

bool CFinnhubDataSource::InquireForexSymbol() {
	if (!IsInquiring() && IsUpdateForexSymbol()) {
		const CVirtualProductWebDataPtr product = m_FinnhubFactory.CreateProduct(gl_pWorldMarket, FOREX_SYMBOLS_);
		product->SetIndex(m_lCurrentForexExchangePos);
		StoreInquiry(product);
		SetInquiring(true);
		gl_pWorldMarket->SetCurrentFinnhubFunction(_T("forex symbol: ") + gl_dataContainerFinnhubForexExchange.GetExchange(m_lCurrentForexExchangePos));
		if (++m_lCurrentForexExchangePos >= gl_dataContainerFinnhubForexExchange.Size()) {
			SetUpdateForexSymbol(false);
			m_lCurrentForexExchangePos = 0;
			gl_systemMessage.PushInformationMessage(_T("Finnhub Forex symbols updated"));
		}
		return true;
	}
	return false;
}

bool CFinnhubDataSource::InquireForexDayLine() {
	const auto lStockSetSize = gl_dataFinnhubForexSymbol.Size();
	bool fHaveInquiry = false;
	constexpr int iInquireType = FOREX_CANDLES_;

	ASSERT(gl_pWorldMarket->IsSystemReady());
	if (!IsInquiring() && IsUpdateForexDayLine()) {
		CForexSymbolPtr pForexSymbol = nullptr;
		long lCurrentUpdateForexDayLinePos;
		bool fFound = false;
		if (!m_fInquiringFinnhubForexDayLine) {
			gl_systemMessage.PushInformationMessage(_T("Inquiring finnhub forex day line..."));
			m_fInquiringFinnhubForexDayLine = true;
		}
		for (lCurrentUpdateForexDayLinePos = 0; lCurrentUpdateForexDayLinePos < lStockSetSize; lCurrentUpdateForexDayLinePos++) {
			pForexSymbol = gl_dataFinnhubForexSymbol.GetSymbol(lCurrentUpdateForexDayLinePos);
			if (pForexSymbol->IsUpdateDayLine()) {
				if (!gl_finnhubInaccessibleExchange.HaveExchange(iInquireType, pForexSymbol->GetExchangeCode())) {
					fFound = true;
					break;
				}
			}
		}
		if (fFound) {
			fHaveInquiry = true;
			const CVirtualProductWebDataPtr product = m_FinnhubFactory.CreateProduct(gl_pWorldMarket, iInquireType);
			product->SetIndex(lCurrentUpdateForexDayLinePos);
			StoreInquiry(product);
			SetInquiring(true);
			gl_pWorldMarket->SetCurrentFinnhubFunction(_T("Forex日线：") + pForexSymbol->GetSymbol());
			//TRACE("申请%s日线数据\n", pForexSymbol->GetSymbol().GetBuffer());
		}
		else {
			m_fInquiringFinnhubForexDayLine = false;
			SetUpdateForexDayLine(false);
			//TRACE("Finnhub Forex日线更新完毕\n");
			const CString str = "Forex DayLine Updated";
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}

bool CFinnhubDataSource::InquireCryptoExchange() {
	if (!IsInquiring() && IsUpdateCryptoExchange()) {
		StoreInquiry(m_FinnhubFactory.CreateProduct(gl_pWorldMarket, CRYPTO_EXCHANGE_));
		SetInquiring(true);
		gl_pWorldMarket->SetCurrentFinnhubFunction(_T("crypto exchange"));
		gl_systemMessage.PushInformationMessage(_T("Inquiring Finnhub crypto exchange"));
		return true;
	}
	return false;
}

bool CFinnhubDataSource::InquireCryptoSymbol() {
	if (!IsInquiring() && IsUpdateCryptoSymbol()) {
		const CVirtualProductWebDataPtr product = m_FinnhubFactory.CreateProduct(gl_pWorldMarket, CRYPTO_SYMBOLS_);
		product->SetIndex(m_lCurrentCryptoExchangePos);
		StoreInquiry(product);
		SetInquiring(true);
		CString str = _T("crypto symbol: ");
		str += gl_dataContainerFinnhubCryptoExchange.GetExchange(m_lCurrentCryptoExchangePos).c_str();
		gl_pWorldMarket->SetCurrentFinnhubFunction(str);
		if (++m_lCurrentCryptoExchangePos >= gl_dataContainerFinnhubCryptoExchange.Size()) {
			SetUpdateCryptoSymbol(false);
			m_lCurrentCryptoExchangePos = 0;
			gl_systemMessage.PushInformationMessage(_T("Finnhub Crypto symbols updated"));
		}
		return true;
	}
	return false;
}

bool CFinnhubDataSource::InquireCryptoDayLine() {
	const auto lStockSetSize = gl_dataFinnhubCryptoSymbol.Size();
	bool fHaveInquiry = false;
	constexpr int iInquireType = CRYPTO_CANDLES_;

	ASSERT(gl_pWorldMarket->IsSystemReady());
	if (!IsInquiring() && IsUpdateCryptoDayLine()) {
		CFinnhubCryptoPtr pCryptoSymbol;
		long lCurrentUpdateCryptoDayLinePos;
		bool fFound = false;
		if (!m_fInquiringFinnhubCryptoDayLine) {
			gl_systemMessage.PushInformationMessage(_T("Inquiring finnhub crypto day line..."));
			m_fInquiringFinnhubCryptoDayLine = true;
		}
		for (lCurrentUpdateCryptoDayLinePos = 0; lCurrentUpdateCryptoDayLinePos < lStockSetSize; lCurrentUpdateCryptoDayLinePos++) {
			pCryptoSymbol = gl_dataFinnhubCryptoSymbol.GetSymbol(lCurrentUpdateCryptoDayLinePos);
			if (pCryptoSymbol->IsUpdateDayLine()) {
				if (!gl_finnhubInaccessibleExchange.HaveExchange(iInquireType, pCryptoSymbol->GetExchangeCode())) {
					fFound = true;
					break;
				}
			}
		}
		if (fFound) {
			fHaveInquiry = true;
			const CVirtualProductWebDataPtr product = m_FinnhubFactory.CreateProduct(gl_pWorldMarket, iInquireType);
			product->SetIndex(lCurrentUpdateCryptoDayLinePos);
			StoreInquiry(product);
			SetInquiring(true);
			gl_pWorldMarket->SetCurrentFinnhubFunction(_T("Crypto日线：") + pCryptoSymbol->GetSymbol());
			//TRACE("申请%s日线数据\n", pCryptoSymbol->GetSymbol().GetBuffer());
		}
		else {
			m_fInquiringFinnhubCryptoDayLine = false;
			SetUpdateCryptoDayLine(false);
			//TRACE("Finnhub Crypto日线更新完毕\n");
			const CString str = "Crypto DayLine Updated";
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}
