//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// finnhub.io��Ϊ����û�ʹ�õ������𽥼��٣���Ҫ����һ�µ����ݿ����洢�����ݵ��������״̬��
///
/// 20231126��20231127��stock DayLine��Crypto DayLine������ѡ�
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
	{ _T("API limit reached. Please try again later. Remaining Limit: 0"), ERROR_FINNHUB_REACH_MAX_API_LIMIT__ }, // http״̬�룺200
	{ _T(""), ERROR_FINNHUB_INQUIRE_RATE_TOO_HIGH__ }
};

CFinnhubDataSource::CFinnhubDataSource() {
	ASSERT(gl_systemConfiguration.IsInitialized());
	// ���裨Ҳ�޷���ÿ�ո��µı�����������

	m_strInquiryFunction = _T(""); // finnhub�и������ݣ�����ǰ׺���������뺯��ÿ�����ã���ͬ��ǰ׺���벻ͬ�����ݡ�
	m_strParam = _T("");
	m_strSuffix = _T("&token=");
	m_strInquiryToken = _T("");
	m_lInquiringNumber = 1; // Finnhubʵʱ���ݲ�ѯ����Ĭ��ֵ
	m_iMaxNormalInquireTime = 1000;

	CFinnhubDataSource::ConfigureInternetOption();

	CFinnhubDataSource::Reset();
}

bool CFinnhubDataSource::Reset() {
	// Finnhub�������������ݱ�ʶ��ÿ����Ҫ���á�Ŀǰ��21�
	m_fUpdateCountryList = true;
	m_fUpdateSymbol = true; // ÿ����Ҫ���´���
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
// �����ԣ��������е�timeoutʱ�䣬���������жϺ�ȴ���ʱ�䡣��������ݣ����۽��շ��͵��ٶȶ����������ᴥ����ʱ�����༴�������timeout��
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
		case ERROR_FINNHUB_NO_RIGHT_TO_ACCESS__:// ��Ȩ����
			m_pCurrentProduct->SetReceivedDataStatus(NO_ACCESS_RIGHT_);
			if (m_pCurrentProduct->CheckInaccessible()) {
				// ���ϵͳ������Ȩ��ѯ��������, Ŀǰ�������ϵͳ��Ϣ�б���
				string s = _T("No right to access: ");
				s += m_pCurrentProduct->GetInquiry();
				s += _T(",  Exchange = ");
				s += m_pCurrentProduct->GetInquiringExchange();
				gl_systemMessage.PushInnerSystemInformationMessage(s);
			}
			break;
		case ERROR_FINNHUB_MISSING_API_KEY__: // ȱ��API key
			gl_systemMessage.PushErrorMessage(_T("finnhub missing API key"));
			break;
		case ERROR_FINNHUB_REACH_MAX_API_LIMIT__: // 
		case ERROR_FINNHUB_INQUIRE_RATE_TOO_HIGH__:// ����Ƶ�ʳ���
			// ���Ͳ�ѯƵ��200ms��
			// todo �������ֻ��ϵͳ����Ƶ�ʳ�������ϵͳ�������������
			i = gl_systemConfiguration.GetWorldMarketFinnhubInquiryTime().count();
			gl_systemConfiguration.SetWorldMarketFinnhubInquiryTime(i + 200);
			break;
		case ERROR_FINNHUB_NOT_HANDLED__: // error not handled
			ReportErrorNotHandled(error);
			break;
		default: // ȱʡ��֧��Ӧ�ó���
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
// ��ϵͳ���������ȣ�����GetTickCount()��ȷ����ǰʱ�䡣
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CFinnhubDataSource::GenerateInquiryMessage(long lCurrentTime) {
	const auto llTickCount = GetTickCount();

	if (gl_systemConfiguration.IsWebBusy()) return false; // �����������ʱ��������finnhub�����ݡ�
	if (llTickCount <= m_PrevInquireTimePoint + gl_systemConfiguration.GetWorldMarketFinnhubInquiryTime()) return false;

	m_PrevInquireTimePoint = llTickCount;
	ASSERT(!IsInquiring());
	// Ensure we are not in the market reset window before proceeding
	ASSERT(lCurrentTime <= GetPrevTime(gl_systemConfiguration.GetWorldMarketResettingTime(), 0, 10, 0)
		|| lCurrentTime >= GetNextTime(gl_systemConfiguration.GetWorldMarketResettingTime(), 0, 5, 0)); // �����г�ʱ���������������Ϣ��
	if (GenerateEconomicCalendar()) return true; // ��һ�����뾭������������ϢΪpremium��ʹ�ô���Ϣ�������˻����ͣ���ѻ����շѣ���
	if (GenerateCountryList()) return true;
	// Finnhub���ṩStock Exchange������ʹ��Ԥ���ṩ�Ĺ�Ʊ�������б�
	if (GenerateForexExchange()) return true;
	if (GenerateCryptoExchange()) return true;
	if (GenerateMarketStatus()) return true;
	if (GenerateMarketHoliday()) return true;
	if (GenerateCompanySymbol()) return true; // ��һ���������������뵱��֤ȯ����
	if (GenerateForexSymbol()) return true;
	if (GenerateCryptoSymbol()) return true;

	if (!gl_pWorldMarket->IsSystemReady()) return false; // proceeding when world market ready

	// ����Finnhub������Ϣ�����񣬽�Ҫ����������Ա�֤���г�ʱ���賿ʮ���Ӻ�ִ�С������ܹ���֤�������г�ʱ����ִ�в�ѯ����
	if (GenerateCompanyProfileConcise()) return true;
	if (GenerateCompanyNews()) return true;
	if (GenerateCompanyBasicFinancial()) return true;
	if (GeneratePeer()) return true;
	if (GenerateInsiderTransaction()) return true;
	if (GenerateInsiderSentiment()) return true;
	if (GenerateCryptoDayLine()) return true; // Crypto dayLine20231127��ֻ���ڸ����û�ʹ��
	if (GenerateStockDayLine()) return true; // Stock dayLine20231127��ֻ���ڸ����û�ʹ��
	if (GenerateForexDayLine()) return true; // Forex dayLineĿǰֻ���ڸ����û�ʹ��
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
		std::string str = _T("����������:");
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
		std::string str = _T("���:");
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
		std::string str = _T("��˾����:");
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
		std::string str = _T("��������:");
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
		std::string str = _T("����:");
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
		std::string str = _T("�ڲ�����:");
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
		std::string str = _T("�ڲ���������:");
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
		std::string str = _T("Forex���ߣ�");
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
		std::string str = _T("Crypto���ߣ�");
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
