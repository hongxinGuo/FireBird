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
#include "WebData.h"
#include"WorldMarket.h"

static auto s_isAccessible = [](int inquireType, const std::string& exchangeCode) {
	return !gl_finnhubInaccessibleExchange.HaveExchange(inquireType, exchangeCode);
};
static auto s_setIndex = [](auto& product, long pos) { product->SetIndex(pos); };

map<string, enum_ErrorMessageData> mapFinnhubErrorMap{
	{ _T("You don't have access to this resource."), ERROR_FINNHUB_NO_RIGHT_TO_ACCESS__ },
	{ _T("Please use an API key."), ERROR_FINNHUB_MISSING_API_KEY__ },
	{ _T("API limit reached. Please try again later. Remaining Limit: 0"), ERROR_FINNHUB_REACH_MAX_API_LIMIT__ }, // http状态码：200
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
	m_internetOption.option_send_timeout = 2000;
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
				string s = _T("No right to access: ");
				s += m_pCurrentProduct->GetInquiry();
				s += _T(",  Exchange = ");
				s += m_pCurrentProduct->GetInquiringExchange();
				gl_systemMessage.PushInnerSystemInformationMessage(s);
			}
			break;
		case ERROR_FINNHUB_MISSING_API_KEY__: // 缺少API key
			gl_systemMessage.PushErrorMessage(_T("finnhub missing API key"));
			break;
		case ERROR_FINNHUB_REACH_MAX_API_LIMIT__: // 
		case ERROR_FINNHUB_INQUIRE_RATE_TOO_HIGH__:// 申请频率超高
			// 降低查询频率200ms。
			// todo 这里最好只向系统报告频率超出，由系统决定如何修正。
			i = gl_systemConfiguration.GetWorldMarketFinnhubInquiryTime().count();
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
	const auto llTickCount = GetTickCount();

	if (gl_systemConfiguration.IsWebBusy()) return false; // 网络出现问题时，不申请finnhub各数据。
	if (llTickCount <= m_PrevInquireTimePoint + gl_systemConfiguration.GetWorldMarketFinnhubInquiryTime()) return false;

	m_PrevInquireTimePoint = llTickCount;
	ASSERT(!IsInquiring());
	// Ensure we are not in the market reset window before proceeding
	ASSERT(lCurrentTime <= GetPrevTime(gl_systemConfiguration.GetWorldMarketResettingTime(), 0, 10, 0)
		|| lCurrentTime >= GetNextTime(gl_systemConfiguration.GetWorldMarketResettingTime(), 0, 5, 0)); // 重启市场时不允许接收网络信息。
	if (GenerateEconomicCalendar()) return true; // 第一步申请经济日历。此信息为premium，使用此信息来决定账户类型（免费还是收费）。
	if (GenerateCountryList()) return true;
	// Finnhub不提供Stock Exchange名单，使用预先提供的股票交易所列表。
	if (GenerateForexExchange()) return true;
	if (GenerateCryptoExchange()) return true;
	if (GenerateMarketStatus()) return true;
	if (GenerateMarketHoliday()) return true;
	if (GenerateCompanySymbol()) return true; // 第一个动作，首先申请当日证券代码
	if (GenerateForexSymbol()) return true;
	if (GenerateCryptoSymbol()) return true;

	if (!gl_pWorldMarket->IsSystemReady()) return false; // proceeding when world market ready

	// 申请Finnhub网络信息的任务，皆要放置在这里，以保证在市场时间凌晨十分钟后执行。这样能够保证在重启市场时不会执行查询任务
	if (GenerateCompanyProfileConcise()) return true;
	if (GenerateCompanyNews()) return true;
	if (GenerateCompanyBasicFinancial()) return true;
	if (GeneratePeer()) return true;
	if (GenerateInsiderTransaction()) return true;
	if (GenerateInsiderSentiment()) return true;
	if (GenerateCryptoDayLine()) return true; // Crypto dayLine20231127后只限于付费用户使用
	if (GenerateStockDayLine()) return true; // Stock dayLine20231127后只限于付费用户使用
	if (GenerateForexDayLine()) return true; // Forex dayLine目前只限于付费用户使用
	if (GenerateEPSSurprise()) return true;
	if (GenerateSECFilings()) return true;
	if (IsUpdateStockDayLine()) {
		//InquireRTQuote()) return true;
	}

	ASSERT(!IsInquiring());
	if (!m_fFinnhubDataInquiryFinished) {
		gl_systemMessage.PushInformationMessage(_T("finnhub data inquiry finished"));
		gl_systemMessage.SetCurrentFinnhubFunction(_T("finished"));
		m_fFinnhubDataInquiryFinished = true;
	}
	return false;
}

bool CFinnhubDataSource::GenerateCountryList() {
	auto isUpdateNeeded = [this]() { return IsUpdateCountryList(); };
	auto createProduct = [this](int inquireType) { return m_FinnhubFactory.CreateProduct(gl_pWorldMarket, inquireType); };
	return GenerateSimpleInquiry(
		ECONOMIC_COUNTRY_LIST_,
		isUpdateNeeded,
		createProduct,
		[] { gl_systemMessage.SetCurrentFinnhubFunction(_T("Finnhub country List")); }
	);
}

bool CFinnhubDataSource::GenerateMarketStatus() {
	// Lambda to check if update is needed
	auto isUpdateNeeded = [this]() { return IsUpdateMarketStatus(); };
	auto isUpdateItemNeeded = [](const auto& item) { return item->IsUpdateMarketStatus(); };
	auto createProduct = [this](int inquireType) { return m_FinnhubFactory.CreateProduct(gl_pWorldMarket, inquireType); };
	auto setMessage = [](const auto& item) {
		std::string str = _T("Market status: ");
		str += item->GetExchangeCode();
		gl_systemMessage.SetCurrentFinnhubFunction(str);
	};
	auto setUpdateFlag = [this](bool flag) { SetUpdateMarketStatus(flag); };
	const std::string finishedMsg = "Finnhub market status finished";

	return GenerateInquiryIterateWithAccessCheck(
		gl_dataContainerStockExchange,
		MARKET_STATUS_,
		isUpdateNeeded,
		isUpdateItemNeeded,
		s_isAccessible,
		createProduct,
		s_setIndex,
		setMessage,
		setUpdateFlag,
		finishedMsg
	);
}

bool CFinnhubDataSource::GenerateMarketHoliday() {
	auto isUpdateNeeded = [this]() { return IsUpdateMarketHoliday(); };
	auto isUpdateItemNeeded = [](const auto& item) { return item->IsUpdateMarketHoliday(); };
	auto createProduct = [this](int inquireType) { return m_FinnhubFactory.CreateProduct(gl_pWorldMarket, inquireType); };
	auto setMessage = [](const auto& item) {
		std::string str = _T("Market holiday: ");
		str += item->GetExchangeCode();
		gl_systemMessage.SetCurrentFinnhubFunction(str);
	};
	auto setUpdateFlag = [this](bool flag) { SetUpdateMarketHoliday(flag); };
	const std::string finishedMsg = "Finnhub market holiday finished";

	return GenerateInquiryIterateWithAccessCheck(
		gl_dataContainerStockExchange,
		MARKET_HOLIDAY_,
		isUpdateNeeded,
		isUpdateItemNeeded,
		s_isAccessible,
		createProduct,
		s_setIndex,
		setMessage,
		setUpdateFlag,
		finishedMsg
	);
}

bool CFinnhubDataSource::GenerateCompanySymbol() {
	auto isUpdateNeeded = [this]() { return IsUpdateSymbol(); };
	auto isUpdateItemNeeded = [](const auto& item) { return item->IsUpdateStockSymbol(); };
	auto createProduct = [this](int inquireType) { return m_FinnhubFactory.CreateProduct(gl_pWorldMarket, inquireType); };
	auto setMessage = [](const auto& item) {
		std::string str = _T("交易所代码:");
		str += item->GetExchangeCode();
		gl_systemMessage.SetCurrentFinnhubFunction(str);
	};
	auto setUpdateFlag = [this](bool flag) { SetUpdateSymbol(flag); };
	const std::string finishedMsg = "Finnhub exchange's stock updated";

	return GenerateInquiryIterateWithAccessCheck(
		gl_dataContainerStockExchange,
		STOCK_SYMBOLS_,
		isUpdateNeeded,
		isUpdateItemNeeded,
		s_isAccessible,
		createProduct,
		s_setIndex,
		setMessage,
		setUpdateFlag,
		finishedMsg
	);
}

bool CFinnhubDataSource::GenerateCompanyProfileConcise() {
	auto isUpdateNeeded = [this]() { return IsUpdateStockProfile(); };
	auto isUpdateItemNeeded = [](const auto& item) { return item->IsUpdateCompanyProfile(); };
	auto createProduct = [this](int inquireType) { return m_FinnhubFactory.CreateProduct(gl_pWorldMarket, inquireType); };
	auto setMessage = [](const auto& item) {
		std::string str = _T("简介:");
		str += item->GetSymbol();
		gl_systemMessage.SetCurrentFinnhubFunction(str);
	};
	auto setUpdateFlag = [this](bool flag) { SetUpdateStockProfile(flag); };
	const std::string finishedMsg = "Finnhub company profile basic updated";

	return GenerateInquiryIterateWithAccessCheck(
		gl_dataContainerFinnhubStock,
		COMPANY_PROFILE_CONCISE_,
		isUpdateNeeded,
		isUpdateItemNeeded,
		s_isAccessible,
		createProduct,
		s_setIndex,
		setMessage,
		setUpdateFlag,
		finishedMsg
	);
}

bool CFinnhubDataSource::GenerateCompanyNews() {
	auto isUpdateNeeded = [this]() { return IsUpdateCompanyNews(); };
	auto isUpdateItemNeeded = [](const auto& item) { return item->IsUpdateCompanyNews(); };
	auto createProduct = [this](int inquireType) { return m_FinnhubFactory.CreateProduct(gl_pWorldMarket, inquireType); };
	auto setMessage = [](const auto& item) {
		std::string str = _T("公司新闻:");
		str += item->GetSymbol();
		gl_systemMessage.SetCurrentFinnhubFunction(str);
	};
	auto setUpdateFlag = [this](bool flag) { SetUpdateCompanyNews(flag); };
	const std::string finishedMsg = "Finnhub company news updated";

	return GenerateInquiryIterateWithAccessCheck(
		gl_dataContainerFinnhubStock,
		COMPANY_NEWS_,
		isUpdateNeeded,
		isUpdateItemNeeded,
		s_isAccessible,
		createProduct,
		s_setIndex,
		setMessage,
		setUpdateFlag,
		finishedMsg
	);
}

bool CFinnhubDataSource::GenerateCompanyBasicFinancial() {
	auto isUpdateNeeded = [this]() { return IsUpdateStockBasicFinancial(); };
	auto isUpdateItemNeeded = [](const auto& item) { return item->IsUpdateBasicFinancial(); };
	auto createProduct = [this](int inquireType) { return m_FinnhubFactory.CreateProduct(gl_pWorldMarket, inquireType); };
	auto setMessage = [](const auto& item) {
		std::string str = _T("基本财务:");
		str += item->GetSymbol();
		gl_systemMessage.SetCurrentFinnhubFunction(str);
	};
	auto setUpdateFlag = [this](bool flag) { SetUpdateStockBasicFinancial(flag); };
	const std::string finishedMsg = "Finnhub basic financial updated";

	return GenerateInquiryIterateWithAccessCheck(
		gl_dataContainerFinnhubStock,
		BASIC_FINANCIALS_,
		isUpdateNeeded,
		isUpdateItemNeeded,
		s_isAccessible,
		createProduct,
		s_setIndex,
		setMessage,
		setUpdateFlag,
		finishedMsg
	);
}

bool CFinnhubDataSource::GenerateStockDayLine() {
	auto isUpdateNeeded = [this]() { return IsUpdateStockDayLine(); };
	auto isUpdateItemNeeded = [](const auto& item) { return item->IsUpdateDayLine(); };
	auto createProduct = [this](int inquireType) { return m_FinnhubFactory.CreateProduct(gl_pWorldMarket, inquireType); };
	auto setMessage = [](const auto& item) {
		std::string str = _T("日线:");
		str += item->GetSymbol();
		gl_systemMessage.SetCurrentFinnhubFunction(str);
	};
	auto setUpdateFlag = [this](bool flag) { SetUpdateStockDayLine(flag); };
	const std::string finishedMsg = "Finnhub dayline updated";

	return GenerateInquiryIterateWithAccessCheck(
		gl_dataContainerFinnhubStock,
		STOCK_PRICE_CANDLES_,
		isUpdateNeeded,
		isUpdateItemNeeded,
		s_isAccessible,
		createProduct,
		s_setIndex,
		setMessage,
		setUpdateFlag,
		finishedMsg
	);
}

bool CFinnhubDataSource::GenerateInsiderTransaction() {
	auto isUpdateNeeded = [this]() { return IsUpdateInsiderTransaction(); };
	auto isUpdateItemNeeded = [](const auto& item) { return item->IsUpdateInsiderTransaction(); };
	auto createProduct = [this](int inquireType) { return m_FinnhubFactory.CreateProduct(gl_pWorldMarket, inquireType); };
	auto setMessage = [](const auto& item) {
		std::string str = _T("内部交易:");
		str += item->GetSymbol();
		gl_systemMessage.SetCurrentFinnhubFunction(str);
	};
	auto setUpdateFlag = [this](bool flag) { SetUpdateInsiderTransaction(flag); };
	const std::string finishedMsg = "Finnhub Insider Transaction updated";

	return GenerateInquiryIterateWithAccessCheck(
		gl_dataContainerFinnhubStock,
		INSIDER_TRANSACTION_,
		isUpdateNeeded,
		isUpdateItemNeeded,
		s_isAccessible,
		createProduct,
		s_setIndex,
		setMessage,
		setUpdateFlag,
		finishedMsg
	);
}

bool CFinnhubDataSource::GenerateInsiderSentiment() {
	auto isUpdateNeeded = [this]() { return IsUpdateInsiderSentiment(); };
	auto isUpdateItemNeeded = [](const auto& item) { return item->IsUpdateInsiderSentiment(); };
	auto createProduct = [this](int inquireType) { return m_FinnhubFactory.CreateProduct(gl_pWorldMarket, inquireType); };
	auto setMessage = [](const auto& item) {
		std::string str = _T("内部交易情绪:");
		str += item->GetSymbol();
		gl_systemMessage.SetCurrentFinnhubFunction(str);
	};
	auto setUpdateFlag = [this](bool flag) { SetUpdateInsiderSentiment(flag); };
	const std::string finishedMsg = "Finnhub Insider Sentiment updated";

	return GenerateInquiryIterateWithAccessCheck(
		gl_dataContainerFinnhubStock,
		INSIDER_SENTIMENT_,
		isUpdateNeeded,
		isUpdateItemNeeded,
		s_isAccessible,
		createProduct,
		s_setIndex,
		setMessage,
		setUpdateFlag,
		finishedMsg
	);
}

bool CFinnhubDataSource::GenerateRTQuote() {
	static size_t s_lCurrentRTDataQuotePos = 0;
	ASSERT(!IsInquiring());
	ASSERT(gl_pWorldMarket->IsSystemReady());
	const CVirtualProductWebDataPtr product = m_FinnhubFactory.CreateProduct(gl_pWorldMarket, STOCK_PRICE_QUOTE_);
	product->SetIndex(s_lCurrentRTDataQuotePos);
	StoreInquiry(product);
	SetInquiring(true);
	s_lCurrentRTDataQuotePos++;
	if (s_lCurrentRTDataQuotePos == gl_dataContainerFinnhubStock.Size()) s_lCurrentRTDataQuotePos = 0;
	string str = _T("stock RT: ");
	str += gl_dataContainerFinnhubStock.GetItem(s_lCurrentRTDataQuotePos)->GetSymbol();
	gl_systemMessage.SetCurrentFinnhubFunction(str);
	return true;
}

bool CFinnhubDataSource::GeneratePeer() {
	auto isUpdateNeeded = [this]() { return IsUpdatePeer(); };
	auto isUpdateItemNeeded = [](const auto& item) { return item->IsUpdatePeer(); };
	auto createProduct = [this](int inquireType) { return m_FinnhubFactory.CreateProduct(gl_pWorldMarket, inquireType); };
	auto setMessage = [](const auto& item) {
		std::string str = _T("Peer:");
		str += item->GetSymbol();
		gl_systemMessage.SetCurrentFinnhubFunction(str);
	};
	auto setUpdateFlag = [this](bool flag) { SetUpdatePeer(flag); };
	const std::string finishedMsg = "Finnhub Peer Updated";

	return GenerateInquiryIterateWithAccessCheck(
		gl_dataContainerFinnhubStock,
		PEERS_,
		isUpdateNeeded,
		isUpdateItemNeeded,
		s_isAccessible,
		createProduct,
		s_setIndex,
		setMessage,
		setUpdateFlag,
		finishedMsg
	);
}

bool CFinnhubDataSource::GenerateEconomicCalendar() {
	auto isUpdateNeeded = [this]() { return IsUpdateEconomicCalendar(); };
	auto createProduct = [this](int inquireType) { return m_FinnhubFactory.CreateProduct(gl_pWorldMarket, inquireType); };
	return GenerateSimpleInquiry(
		ECONOMIC_CALENDAR_,
		isUpdateNeeded,
		createProduct,
		[] { gl_systemMessage.SetCurrentFinnhubFunction(_T("updating economic calendar")); }
	);
}

bool CFinnhubDataSource::GenerateEPSSurprise() {
	auto isUpdateNeeded = [this]() { return IsUpdateEPSSurprise(); };
	auto isUpdateItemNeeded = [](const auto& item) { return item->IsUpdateEPSSurprise(); };
	auto createProduct = [this](int inquireType) { return m_FinnhubFactory.CreateProduct(gl_pWorldMarket, inquireType); };
	auto setMessage = [](const auto& item) {
		std::string str = _T("EPS surprise:");
		str += item->GetSymbol();
		gl_systemMessage.SetCurrentFinnhubFunction(str);
	};
	auto setUpdateFlag = [this](bool flag) { SetUpdateEPSSurprise(flag); };
	const std::string finishedMsg = "Finnhub EPS Surprise Updated";

	return GenerateInquiryIterateWithAccessCheck(
		gl_dataContainerFinnhubStock,
		STOCK_ESTIMATES_EPS_SURPRISE_,
		isUpdateNeeded,
		isUpdateItemNeeded,
		s_isAccessible,
		createProduct,
		s_setIndex,
		setMessage,
		setUpdateFlag,
		finishedMsg
	);
}

bool CFinnhubDataSource::GenerateSECFilings() {
	auto isUpdateNeeded = [this]() { return IsUpdateSECFilings(); };
	auto isUpdateItemNeeded = [](const auto& item) { return item->IsUpdateSECFilings(); };
	auto createProduct = [this](int inquireType) { return m_FinnhubFactory.CreateProduct(gl_pWorldMarket, inquireType); };
	auto setMessage = [](const auto& item) {
		std::string str = _T("SEC Filings:");
		str += item->GetSymbol();
		gl_systemMessage.SetCurrentFinnhubFunction(str);
	};
	auto setUpdateFlag = [this](bool flag) { SetUpdateSECFilings(flag); };
	const std::string finishedMsg = "Finnhub SEC Filings Updated";

	return GenerateInquiryIterateWithAccessCheck(
		gl_dataContainerFinnhubStock,
		SEC_FILINGS_,
		isUpdateNeeded,
		isUpdateItemNeeded,
		s_isAccessible,
		createProduct,
		s_setIndex,
		setMessage,
		setUpdateFlag,
		finishedMsg
	);
}

bool CFinnhubDataSource::GenerateForexExchange() {
	auto isUpdateNeeded = [this]() { return IsUpdateForexExchange(); };
	auto createProduct = [this](int inquireType) { return m_FinnhubFactory.CreateProduct(gl_pWorldMarket, inquireType); };
	return GenerateSimpleInquiry(
		FOREX_EXCHANGE_,
		isUpdateNeeded,
		createProduct,
		[] { gl_systemMessage.SetCurrentFinnhubFunction(_T("forex exchange")); }
	);
}

bool CFinnhubDataSource::GenerateForexSymbol() {
	static size_t s_lCurrentForexExchangePos = 0;
	ASSERT(!IsInquiring());
	if (IsUpdateForexSymbol()) {
		const CVirtualProductWebDataPtr product = m_FinnhubFactory.CreateProduct(gl_pWorldMarket, FOREX_SYMBOLS_);
		product->SetIndex(s_lCurrentForexExchangePos);
		StoreInquiry(product);
		SetInquiring(true);
		string str = _T("forex symbol: ");
		str += gl_dataContainerFinnhubForexExchange.GetItem(s_lCurrentForexExchangePos);
		gl_systemMessage.SetCurrentFinnhubFunction(str);
		if (++s_lCurrentForexExchangePos >= gl_dataContainerFinnhubForexExchange.Size()) {
			SetUpdateForexSymbol(false);
			s_lCurrentForexExchangePos = 0;
			gl_systemMessage.PushInformationMessage(_T("Finnhub Forex symbols updated"));
		}
		return true;
	}
	return false;
}

/*
bool CFinnhubDataSource::GenerateForexSymbol() {
	auto isUpdateNeeded = [this]() { return IsUpdateForexSymbol(); };
	auto isUpdateItemNeeded = [](const auto& item) { return true; };
	auto isAccessible = [](int inquireType, const std::string& exchangeCode) { return true; };
	auto createProduct = [this](int inquireType) { return m_FinnhubFactory.CreateProduct(gl_pWorldMarket, inquireType); };
	auto setIndex = [](auto& product, long pos) { product->SetIndex(pos); };
	auto setMessage = [](const auto& item) {
		std::string str = _T("forex symbol: ");
		str += item;
		gl_systemMessage.SetCurrentFinnhubFunction(str);
	};
	auto setUpdateFlag = [this](bool flag) { SetUpdateForexSymbol(flag); };
	const std::string finishedMsg = "Finnhub Forex symbols updated";

	return GenerateInquiryIterateWithAccessCheck(
		gl_dataContainerFinnhubForexExchange,
		FOREX_SYMBOLS_,
		isUpdateNeeded,
		isUpdateItemNeeded,
		isAccessible,
		createProduct,
		setIndex,
		setMessage,
		setUpdateFlag,
		finishedMsg
	);
}*/

bool CFinnhubDataSource::GenerateCryptoSymbol() {
	static size_t s_lCurrentCryptoExchangePos = 0
	ASSERT(!IsInquiring());
	if (IsUpdateCryptoSymbol()) {
		const CVirtualProductWebDataPtr product = m_FinnhubFactory.CreateProduct(gl_pWorldMarket, CRYPTO_SYMBOLS_);
		product->SetIndex(s_lCurrentCryptoExchangePos);
		StoreInquiry(product);
		SetInquiring(true);
		string str = _T("crypto symbol: ");
		str += gl_dataContainerFinnhubCryptoExchange.GetItem(s_lCurrentCryptoExchangePos);
		gl_systemMessage.SetCurrentFinnhubFunction(str);
		if (++s_lCurrentCryptoExchangePos >= gl_dataContainerFinnhubCryptoExchange.Size()) {
			SetUpdateCryptoSymbol(false);
			s_lCurrentCryptoExchangePos = 0;
			gl_systemMessage.PushInformationMessage(_T("Finnhub Crypto symbols updated"));
		}
		return true;
	}
	return false;
}

bool CFinnhubDataSource::GenerateForexDayLine() {
	auto isUpdateNeeded = [this]() { return IsUpdateForexDayLine(); };
	auto isUpdateItemNeeded = [](const auto& item) { return item->IsUpdateDayLine(); };
	auto createProduct = [this](int inquireType) { return m_FinnhubFactory.CreateProduct(gl_pWorldMarket, inquireType); };
	auto setMessage = [](const auto& item) {
		std::string str = _T("Forex日线：");
		str += item->GetSymbol();
		gl_systemMessage.SetCurrentFinnhubFunction(str);
	};
	auto setUpdateFlag = [this](bool flag) { SetUpdateForexDayLine(flag); };
	const std::string finishedMsg = "Finnhub Forex DayLine Updated";

	return GenerateInquiryIterateWithAccessCheck(
		gl_dataFinnhubForexSymbol,
		FOREX_CANDLES_,
		isUpdateNeeded,
		isUpdateItemNeeded,
		s_isAccessible,
		createProduct,
		s_setIndex,
		setMessage,
		setUpdateFlag,
		finishedMsg
	);
}

bool CFinnhubDataSource::GenerateCryptoExchange() {
	auto isUpdateNeeded = [this]() { return IsUpdateCryptoExchange(); };
	auto createProduct = [this](int inquireType) { return m_FinnhubFactory.CreateProduct(gl_pWorldMarket, inquireType); };
	return GenerateSimpleInquiry(
		CRYPTO_EXCHANGE_,
		isUpdateNeeded,
		createProduct,
		[] { gl_systemMessage.SetCurrentFinnhubFunction(_T("crypto exchange")); }
	);
}

bool CFinnhubDataSource::GenerateCryptoDayLine() {
	auto isUpdateNeeded = [this]() { return IsUpdateCryptoDayLine(); };
	auto isUpdateItemNeeded = [](const auto& item) { return item->IsUpdateDayLine(); };
	auto createProduct = [this](int inquireType) { return m_FinnhubFactory.CreateProduct(gl_pWorldMarket, inquireType); };
	auto setMessage = [](const auto& item) {
		std::string str = _T("Crypto日线：");
		str += item->GetSymbol();
		gl_systemMessage.SetCurrentFinnhubFunction(str);
	};
	auto setUpdateFlag = [this](bool flag) { SetUpdateCryptoDayLine(flag); };
	const std::string finishedMsg = "Finnhub Crypto DayLine Updated";

	return GenerateInquiryIterateWithAccessCheck(
		gl_dataFinnhubCryptoSymbol,
		CRYPTO_CANDLES_,
		isUpdateNeeded,
		isUpdateItemNeeded,
		s_isAccessible,
		createProduct,
		s_setIndex,
		setMessage,
		setUpdateFlag,
		finishedMsg
	);
}
