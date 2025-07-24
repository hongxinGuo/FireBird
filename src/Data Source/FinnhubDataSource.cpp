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

	m_lCurrentRTDataQuotePos = 0;
	m_lCurrentForexExchangePos = 0;
	m_lCurrentCryptoExchangePos = 0;

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

/*
bool CFinnhubDataSource::GenerateCountryList() {
	ASSERT(!IsInquiring());
	if (IsUpdateCountryList()) {
		const CVirtualProductWebDataPtr product = m_FinnhubFactory.CreateProduct(gl_pWorldMarket, ECONOMIC_COUNTRY_LIST_);
		StoreInquiry(product);
		SetInquiring(true);
		gl_systemMessage.SetCurrentFinnhubFunction(_T("Finnhub country List"));
		return true;
	}
	return false;
}
*/

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

/*
bool CFinnhubDataSource::GenerateMarketStatus() {
	const auto lExchangeSize = gl_dataContainerStockExchange.Size();
	bool fHaveInquiry = false;
	constexpr int iInquireType = MARKET_STATUS_;

	ASSERT(!IsInquiring());
	if (IsUpdateMarketStatus()) {
		CStockExchangePtr pExchange;
		bool fFound = false;
		size_t lCurrentStockExchangePos;
		for (lCurrentStockExchangePos = 0; lCurrentStockExchangePos < lExchangeSize; lCurrentStockExchangePos++) {
			if (gl_dataContainerStockExchange.GetItem(lCurrentStockExchangePos)->IsUpdateMarketStatus()) {
				pExchange = gl_dataContainerStockExchange.GetItem(lCurrentStockExchangePos);
				if (!gl_finnhubInaccessibleExchange.HaveExchange(iInquireType, pExchange->GetExchangeCode())) {
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
			string str = _T("Market status: ");
			str += pExchange->GetExchangeCode();
			gl_systemMessage.SetCurrentFinnhubFunction(str);
		}
		else {
			fHaveInquiry = false;
			SetUpdateMarketStatus(false);
			const string str = "Finnhub market status finished";
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}
*/

bool CFinnhubDataSource::GenerateMarketStatus() {
	// Lambda to check if update is needed
	auto isUpdateNeeded = [this]() { return IsUpdateMarketStatus(); };
	auto isUpdateItemNeeded = [](const auto& item) { return item->IsUpdateMarketStatus(); };
	auto isAccessible = [](int inquireType, const std::string& exchangeCode) {
		return !gl_finnhubInaccessibleExchange.HaveExchange(inquireType, exchangeCode);
	};
	auto createProduct = [this](int inquireType) { return m_FinnhubFactory.CreateProduct(gl_pWorldMarket, inquireType); };
	auto setIndex = [](auto& product, long pos) { product->SetIndex(pos); };
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
		isAccessible,
		createProduct,
		setIndex,
		setMessage,
		setUpdateFlag,
		finishedMsg
	);
}

/*
bool CFinnhubDataSource::GenerateMarketHoliday() {
	const auto lExchangeSize = gl_dataContainerStockExchange.Size();
	bool fHaveInquiry = false;
	constexpr int iInquireType = MARKET_HOLIDAY_;

	ASSERT(!IsInquiring());
	if (IsUpdateMarketHoliday()) {
		CStockExchangePtr pExchange;
		bool fFound = false;
		long lCurrentStockExchangePos;
		for (lCurrentStockExchangePos = 0; lCurrentStockExchangePos < lExchangeSize; lCurrentStockExchangePos++) {
			if (gl_dataContainerStockExchange.GetItem(lCurrentStockExchangePos)->IsUpdateMarketHoliday()) {
				pExchange = gl_dataContainerStockExchange.GetItem(lCurrentStockExchangePos);
				if (!gl_finnhubInaccessibleExchange.HaveExchange(iInquireType, pExchange->GetExchangeCode())) {
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
			string str = _T("Market holiday: ");
			str += pExchange->GetExchangeCode();
			gl_systemMessage.SetCurrentFinnhubFunction(str);
		}
		else {
			fHaveInquiry = false;
			SetUpdateMarketHoliday(false);
			const string str = "Finnhub market holiday finished";
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}
*/

bool CFinnhubDataSource::GenerateMarketHoliday() {
	auto isUpdateNeeded = [this]() { return IsUpdateMarketHoliday(); };
	auto isUpdateItemNeeded = [](const auto& item) { return item->IsUpdateMarketHoliday(); };
	auto isAccessible = [](int inquireType, const std::string& exchangeCode) {
		return !gl_finnhubInaccessibleExchange.HaveExchange(inquireType, exchangeCode);
	};
	auto createProduct = [this](int inquireType) { return m_FinnhubFactory.CreateProduct(gl_pWorldMarket, inquireType); };
	auto setIndex = [](auto& product, long pos) { product->SetIndex(pos); };
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
		isAccessible,
		createProduct,
		setIndex,
		setMessage,
		setUpdateFlag,
		finishedMsg
	);
}

/*
bool CFinnhubDataSource::GenerateCompanySymbol() {
	const auto lExchangeSize = gl_dataContainerStockExchange.Size();
	bool fHaveInquiry = false;
	constexpr int iInquireType = STOCK_SYMBOLS_;

	ASSERT(!IsInquiring());
	if (IsUpdateSymbol()) {
		CStockExchangePtr pExchange;
		bool fFound = false;
		long lCurrentStockExchangePos;
		for (lCurrentStockExchangePos = 0; lCurrentStockExchangePos < lExchangeSize; lCurrentStockExchangePos++) {
			if (gl_dataContainerStockExchange.GetItem(lCurrentStockExchangePos)->IsUpdateStockSymbol()) {
				pExchange = gl_dataContainerStockExchange.GetItem(lCurrentStockExchangePos);
				if (!gl_finnhubInaccessibleExchange.HaveExchange(iInquireType, pExchange->GetExchangeCode())) {
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
			string str = _T("����������:");
			str += pExchange->GetExchangeCode();
			gl_systemMessage.SetCurrentFinnhubFunction(str);
		}
		else {
			fHaveInquiry = false;
			SetUpdateSymbol(false);
			const string str = "Finnhub exchange's stock updated";
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}*/

bool CFinnhubDataSource::GenerateCompanySymbol() {
	auto isUpdateNeeded = [this]() { return IsUpdateSymbol(); };
	auto isUpdateItemNeeded = [](const auto& item) { return item->IsUpdateStockSymbol(); };
	auto isAccessible = [](int inquireType, const std::string& exchangeCode) {
		return !gl_finnhubInaccessibleExchange.HaveExchange(inquireType, exchangeCode);
	};
	auto createProduct = [this](int inquireType) { return m_FinnhubFactory.CreateProduct(gl_pWorldMarket, inquireType); };
	auto setIndex = [](auto& product, long pos) { product->SetIndex(pos); };
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
		isAccessible,
		createProduct,
		setIndex,
		setMessage,
		setUpdateFlag,
		finishedMsg
	);
}

/*
bool CFinnhubDataSource::GenerateCompanyProfileConcise() {
	const auto lStockSetSize = gl_dataContainerFinnhubStock.Size();
	bool fHaveInquiry = false;
	constexpr int iInquireType = COMPANY_PROFILE_CONCISE_;

	ASSERT(gl_pWorldMarket->IsSystemReady());
	ASSERT(!IsInquiring());
	if (IsUpdateStockProfile()) {
		long lCurrentProfilePos;
		bool fFound = false;
		for (lCurrentProfilePos = 0; lCurrentProfilePos < lStockSetSize; lCurrentProfilePos++) {
			if (const auto pStock = gl_dataContainerFinnhubStock.GetItem(lCurrentProfilePos); pStock->IsUpdateCompanyProfile()) {
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
			string str = _T("���:");
			str += gl_dataContainerFinnhubStock.GetItem(lCurrentProfilePos)->GetSymbol();
			gl_systemMessage.SetCurrentFinnhubFunction(str);
		}
		else {
			SetUpdateStockProfile(false);
			const string str = "Finnhub company profile basic updated";
			gl_systemMessage.PushInformationMessage(str);
			fHaveInquiry = false;
		}
	}
	return fHaveInquiry;
}*/

bool CFinnhubDataSource::GenerateCompanyProfileConcise() {
	auto isUpdateNeeded = [this]() { return IsUpdateStockProfile(); };
	auto isUpdateItemNeeded = [](const auto& item) { return item->IsUpdateCompanyProfile(); };
	auto isAccessible = [](int inquireType, const std::string& exchangeCode) {
		return !gl_finnhubInaccessibleExchange.HaveExchange(inquireType, exchangeCode);
	};
	auto createProduct = [this](int inquireType) { return m_FinnhubFactory.CreateProduct(gl_pWorldMarket, inquireType); };
	auto setIndex = [](auto& product, long pos) { product->SetIndex(pos); };
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
		isAccessible,
		createProduct,
		setIndex,
		setMessage,
		setUpdateFlag,
		finishedMsg
	);
}

/*
/// <summary>
/// ��˾���š�Ŀǰfinnhub.ioֻ�ṩ������˾������
/// </summary>
bool CFinnhubDataSource::GenerateCompanyNews() {
	const auto lStockSetSize = gl_dataContainerFinnhubStock.Size();
	bool fHaveInquiry = false;
	constexpr int iInquireType = COMPANY_NEWS_;

	ASSERT(gl_pWorldMarket->IsSystemReady());
	ASSERT(!IsInquiring());
	if (IsUpdateCompanyNews()) {
		long lCurrentCompanyNewsPos;
		bool fFound = false;
		for (lCurrentCompanyNewsPos = 0; lCurrentCompanyNewsPos < lStockSetSize; lCurrentCompanyNewsPos++) {
			const CFinnhubStockPtr pStock = gl_dataContainerFinnhubStock.GetItem(lCurrentCompanyNewsPos);
			if (pStock->IsUpdateCompanyNews()) {
				if (!gl_finnhubInaccessibleExchange.HaveExchange(iInquireType, pStock->GetExchangeCode())) {
					// Ŀǰֻ���������г�
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
			string str = _T("��˾����:");
			str += gl_dataContainerFinnhubStock.GetItem(lCurrentCompanyNewsPos)->GetSymbol();
			gl_systemMessage.SetCurrentFinnhubFunction(str);
		}
		else {
			SetUpdateCompanyNews(false);
			const string str = "Finnhub company news updated";
			gl_systemMessage.PushInformationMessage(str);
			fHaveInquiry = false;
		}
	}
	return fHaveInquiry;
}*/

bool CFinnhubDataSource::GenerateCompanyNews() {
	auto isUpdateNeeded = [this]() { return IsUpdateCompanyNews(); };
	auto isUpdateItemNeeded = [](const auto& item) { return item->IsUpdateCompanyNews(); };
	auto isAccessible = [](int inquireType, const std::string& exchangeCode) {
		return !gl_finnhubInaccessibleExchange.HaveExchange(inquireType, exchangeCode);
	};
	auto createProduct = [this](int inquireType) { return m_FinnhubFactory.CreateProduct(gl_pWorldMarket, inquireType); };
	auto setIndex = [](auto& product, long pos) { product->SetIndex(pos); };
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
		isAccessible,
		createProduct,
		setIndex,
		setMessage,
		setUpdateFlag,
		finishedMsg
	);
}

/*
bool CFinnhubDataSource::GenerateCompanyBasicFinancial() {
	const auto lStockSetSize = gl_dataContainerFinnhubStock.Size();
	bool fHaveInquiry = false;
	constexpr int iInquireType = BASIC_FINANCIALS_;

	ASSERT(gl_pWorldMarket->IsSystemReady());
	ASSERT(!IsInquiring());
	if (IsUpdateStockBasicFinancial()) {
		bool fFound = false;
		long lCurrentBasicFinancialsPos;
		for (lCurrentBasicFinancialsPos = 0; lCurrentBasicFinancialsPos < lStockSetSize; lCurrentBasicFinancialsPos++) {
			const CFinnhubStockPtr pStock = gl_dataContainerFinnhubStock.GetItem(lCurrentBasicFinancialsPos);
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
			string str = _T("��������:");
			str += gl_dataContainerFinnhubStock.GetItem(lCurrentBasicFinancialsPos)->GetSymbol();
			gl_systemMessage.SetCurrentFinnhubFunction(str);
		}
		else {
			SetUpdateStockBasicFinancial(false);
			const string str = "Finnhub basic financial updated";
			gl_systemMessage.PushInformationMessage(str);
			fHaveInquiry = false;
		}
	}
	return fHaveInquiry;
}*/

bool CFinnhubDataSource::GenerateCompanyBasicFinancial() {
	auto isUpdateNeeded = [this]() { return IsUpdateStockBasicFinancial(); };
	auto isUpdateItemNeeded = [](const auto& item) { return item->IsUpdateBasicFinancial(); };
	auto isAccessible = [](int inquireType, const std::string& exchangeCode) {
		return !gl_finnhubInaccessibleExchange.HaveExchange(inquireType, exchangeCode);
	};
	auto createProduct = [this](int inquireType) { return m_FinnhubFactory.CreateProduct(gl_pWorldMarket, inquireType); };
	auto setIndex = [](auto& product, long pos) { product->SetIndex(pos); };
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
		isAccessible,
		createProduct,
		setIndex,
		setMessage,
		setUpdateFlag,
		finishedMsg
	);
}

/*
bool CFinnhubDataSource::GenerateStockDayLine() {
	const auto lStockSetSize = gl_dataContainerFinnhubStock.Size();
	bool fHaveInquiry = false;
	constexpr int iInquireType = STOCK_PRICE_CANDLES_;

	ASSERT(gl_pWorldMarket->IsSystemReady());
	ASSERT(!IsInquiring());
	if (IsUpdateStockDayLine()) {
		CFinnhubStockPtr pStock;
		bool fFound = false;
		long lCurrentUpdateDayLinePos;
		for (lCurrentUpdateDayLinePos = 0; lCurrentUpdateDayLinePos < lStockSetSize; lCurrentUpdateDayLinePos++) {
			pStock = gl_dataContainerFinnhubStock.GetItem(lCurrentUpdateDayLinePos);
			if (pStock->IsUpdateDayLine()) { // Ŀǰ����˻�ֻ�����������г��Ĺ�Ʊ���ߡ�
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
			string str = _T("����:");
			str += pStock->GetSymbol();
			gl_systemMessage.SetCurrentFinnhubFunction(str);
		}
		else {
			fHaveInquiry = false;
			SetUpdateStockDayLine(false);
			//lCurrentUpdateDayLinePos = 0; // ���ô����������е��������ݸ���һ������ʱ��Ҫ����24Сʱ���ʱ��ָ��¼��ɡ�
			//TRACE("Finnhub���߸�����ϣ����¿�ʼ����\n");
			const string str = "Finnhub dayline updated";
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}*/

bool CFinnhubDataSource::GenerateStockDayLine() {
	auto isUpdateNeeded = [this]() { return IsUpdateStockDayLine(); };
	auto isUpdateItemNeeded = [](const auto& item) { return item->IsUpdateDayLine(); };
	auto isAccessible = [](int inquireType, const std::string& exchangeCode) {
		return !gl_finnhubInaccessibleExchange.HaveExchange(inquireType, exchangeCode);
	};
	auto createProduct = [this](int inquireType) { return m_FinnhubFactory.CreateProduct(gl_pWorldMarket, inquireType); };
	auto setIndex = [](auto& product, long pos) { product->SetIndex(pos); };
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
		isAccessible,
		createProduct,
		setIndex,
		setMessage,
		setUpdateFlag,
		finishedMsg
	);
}

/*
bool CFinnhubDataSource::GenerateInsiderTransaction() {
	const auto lStockSetSize = gl_dataContainerFinnhubStock.Size();
	bool fHaveInquiry = false;
	constexpr int iInquireType = INSIDER_TRANSACTION_;

	ASSERT(gl_pWorldMarket->IsSystemReady());
	ASSERT(!IsInquiring());
	if (IsUpdateInsiderTransaction()) {
		long lCurrentUpdateInsiderTransactionPos;
		CFinnhubStockPtr pStock;
		bool fFound = false;
		for (lCurrentUpdateInsiderTransactionPos = 0; lCurrentUpdateInsiderTransactionPos < lStockSetSize; lCurrentUpdateInsiderTransactionPos++) {
			pStock = gl_dataContainerFinnhubStock.GetItem(lCurrentUpdateInsiderTransactionPos);
			if (pStock->IsUpdateInsiderTransaction()) {	// Ŀǰ����˻�ֻ�����������г����ڲ��������ݡ�
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
			string str = _T("�ڲ�����:");
			str += pStock->GetSymbol();
			gl_systemMessage.SetCurrentFinnhubFunction(str);
		}
		else {
			fHaveInquiry = false;
			SetUpdateInsiderTransaction(false);
			const string str = "Finnhub Insider Transaction updated";
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}*/

bool CFinnhubDataSource::GenerateInsiderTransaction() {
	auto isUpdateNeeded = [this]() { return IsUpdateInsiderTransaction(); };
	auto isUpdateItemNeeded = [](const auto& item) { return item->IsUpdateInsiderTransaction(); };
	auto isAccessible = [](int inquireType, const std::string& exchangeCode) {
		return !gl_finnhubInaccessibleExchange.HaveExchange(inquireType, exchangeCode);
	};
	auto createProduct = [this](int inquireType) { return m_FinnhubFactory.CreateProduct(gl_pWorldMarket, inquireType); };
	auto setIndex = [](auto& product, long pos) { product->SetIndex(pos); };
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
		isAccessible,
		createProduct,
		setIndex,
		setMessage,
		setUpdateFlag,
		finishedMsg
	);
}

/*
bool CFinnhubDataSource::GenerateInsiderSentiment() {
	const auto lStockSetSize = gl_dataContainerFinnhubStock.Size();
	bool fHaveInquiry = false;
	constexpr int iInquireType = INSIDER_SENTIMENT_;

	ASSERT(gl_pWorldMarket->IsSystemReady());
	ASSERT(!IsInquiring());
	if (IsUpdateInsiderSentiment()) {
		long lCurrentUpdateInsiderSentimentPos;
		CFinnhubStockPtr pStock;
		bool fFound = false;
		for (lCurrentUpdateInsiderSentimentPos = 0; lCurrentUpdateInsiderSentimentPos < lStockSetSize; lCurrentUpdateInsiderSentimentPos++) {
			pStock = gl_dataContainerFinnhubStock.GetItem(lCurrentUpdateInsiderSentimentPos);
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
			string str = _T("�ڲ���������:");
			str += pStock->GetSymbol();
			gl_systemMessage.SetCurrentFinnhubFunction(str);
		}
		else {
			fHaveInquiry = false;
			SetUpdateInsiderSentiment(false);
			//TRACE("FinnhubInsider Sentiment�������\n");
			const string str = "Finnhub Insider Sentiment updated";
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}*/

bool CFinnhubDataSource::GenerateInsiderSentiment() {
	auto isUpdateNeeded = [this]() { return IsUpdateInsiderSentiment(); };
	auto isUpdateItemNeeded = [](const auto& item) { return item->IsUpdateInsiderSentiment(); };
	auto isAccessible = [](int inquireType, const std::string& exchangeCode) {
		return !gl_finnhubInaccessibleExchange.HaveExchange(inquireType, exchangeCode);
	};
	auto createProduct = [this](int inquireType) { return m_FinnhubFactory.CreateProduct(gl_pWorldMarket, inquireType); };
	auto setIndex = [](auto& product, long pos) { product->SetIndex(pos); };
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
		isAccessible,
		createProduct,
		setIndex,
		setMessage,
		setUpdateFlag,
		finishedMsg
	);
}

bool CFinnhubDataSource::GenerateRTQuote() {
	ASSERT(!IsInquiring());
	ASSERT(gl_pWorldMarket->IsSystemReady());
	const CVirtualProductWebDataPtr product = m_FinnhubFactory.CreateProduct(gl_pWorldMarket, STOCK_PRICE_QUOTE_);
	product->SetIndex(m_lCurrentRTDataQuotePos);
	StoreInquiry(product);
	SetInquiring(true);
	m_lCurrentRTDataQuotePos++;
	if (m_lCurrentRTDataQuotePos == gl_dataContainerFinnhubStock.Size()) m_lCurrentRTDataQuotePos = 0;
	string str = _T("stock RT: ");
	str += gl_dataContainerFinnhubStock.GetItem(m_lCurrentRTDataQuotePos)->GetSymbol();
	gl_systemMessage.SetCurrentFinnhubFunction(str);
	return true;
}

/*
bool CFinnhubDataSource::GeneratePeer() {
	const auto lStockSetSize = gl_dataContainerFinnhubStock.Size();
	bool fHaveInquiry = false;
	constexpr int iInquireType = PEERS_;

	ASSERT(gl_pWorldMarket->IsSystemReady());
	ASSERT(!IsInquiring());
	if (IsUpdatePeer()) {
		long lCurrentUpdatePeerPos;
		bool fFound = false;
		for (lCurrentUpdatePeerPos = 0; lCurrentUpdatePeerPos < lStockSetSize; lCurrentUpdatePeerPos++) {
			const CFinnhubStockPtr pStock = gl_dataContainerFinnhubStock.GetItem(lCurrentUpdatePeerPos);
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
			string str = _T("Peer:");
			str += gl_dataContainerFinnhubStock.GetItem(lCurrentUpdatePeerPos)->GetSymbol();
			gl_systemMessage.SetCurrentFinnhubFunction(str);
		}
		else {
			fHaveInquiry = false;
			SetUpdatePeer(false);
			const string str = "Finnhub Peer Updated";
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}*/

bool CFinnhubDataSource::GeneratePeer() {
	auto isUpdateNeeded = [this]() { return IsUpdatePeer(); };
	auto isUpdateItemNeeded = [](const auto& item) { return item->IsUpdatePeer(); };
	auto isAccessible = [](int inquireType, const std::string& exchangeCode) {
		return !gl_finnhubInaccessibleExchange.HaveExchange(inquireType, exchangeCode);
	};
	auto createProduct = [this](int inquireType) { return m_FinnhubFactory.CreateProduct(gl_pWorldMarket, inquireType); };
	auto setIndex = [](auto& product, long pos) { product->SetIndex(pos); };
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
		isAccessible,
		createProduct,
		setIndex,
		setMessage,
		setUpdateFlag,
		finishedMsg
	);
}

/*
bool CFinnhubDataSource::GenerateEconomicCalendar() {
	constexpr int iInquireType = ECONOMIC_CALENDAR_;

	ASSERT(!IsInquiring());
	if (IsUpdateEconomicCalendar()) {
		const CVirtualProductWebDataPtr product = m_FinnhubFactory.CreateProduct(gl_pWorldMarket, iInquireType);
		StoreInquiry(product);
		SetInquiring(true);
		gl_systemMessage.SetCurrentFinnhubFunction(_T("updating economic calendar"));
		return true;
	}
	return false;
} */

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

/*
bool CFinnhubDataSource::GenerateEPSSurprise() {
	const auto lStockSetSize = gl_dataContainerFinnhubStock.Size();
	bool fHaveInquiry = false;
	constexpr int iInquireType = STOCK_ESTIMATES_EPS_SURPRISE_;

	ASSERT(gl_pWorldMarket->IsSystemReady());
	ASSERT(!IsInquiring());
	if (IsUpdateEPSSurprise()) {
		CFinnhubStockPtr pStock;
		bool fFound = false;
		for (m_lCurrentUpdateEPSSurprisePos = 0; m_lCurrentUpdateEPSSurprisePos < lStockSetSize; m_lCurrentUpdateEPSSurprisePos++) {
			pStock = gl_dataContainerFinnhubStock.GetItem(m_lCurrentUpdateEPSSurprisePos);
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
			gl_dataContainerFinnhubStock.GetItem(m_lCurrentUpdateEPSSurprisePos)->SetUpdateEPSSurprise(false);
			string str = _T("EPS surprise:");
			str += pStock->GetSymbol();
			gl_systemMessage.SetCurrentFinnhubFunction(str);
		}
		else {
			m_lCurrentUpdateEPSSurprisePos = 0;
			fHaveInquiry = false;
			SetUpdateEPSSurprise(false);
			const string str = "Finnhub EPS Surprise Updated";
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
} */

bool CFinnhubDataSource::GenerateEPSSurprise() {
	auto isUpdateNeeded = [this]() { return IsUpdateEPSSurprise(); };
	auto isUpdateItemNeeded = [](const auto& item) { return item->IsUpdateEPSSurprise(); };
	auto isAccessible = [](int inquireType, const std::string& exchangeCode) {
		return !gl_finnhubInaccessibleExchange.HaveExchange(inquireType, exchangeCode);
	};
	auto createProduct = [this](int inquireType) { return m_FinnhubFactory.CreateProduct(gl_pWorldMarket, inquireType); };
	auto setIndex = [](auto& product, long pos) { product->SetIndex(pos); };
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
		isAccessible,
		createProduct,
		setIndex,
		setMessage,
		setUpdateFlag,
		finishedMsg
	);
}

/*
bool CFinnhubDataSource::GenerateSECFilings() {
	const auto lStockSetSize = gl_dataContainerFinnhubStock.Size();
	bool fHaveInquiry = false;
	constexpr int iInquireType = SEC_FILINGS_;

	ASSERT(gl_pWorldMarket->IsSystemReady());
	ASSERT(!IsInquiring());
	if (IsUpdateSECFilings()) {
		CFinnhubStockPtr pStock;
		bool fFound = false;
		for (m_lCurrentUpdateSECFilingsPos = 0; m_lCurrentUpdateSECFilingsPos < lStockSetSize; m_lCurrentUpdateSECFilingsPos++) {
			pStock = gl_dataContainerFinnhubStock.GetItem(m_lCurrentUpdateSECFilingsPos);
			if (!pStock->IsUpdateSECFilings()) {
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
			string str = _T("SEC Filings:");
			str += pStock->GetSymbol();
			gl_systemMessage.SetCurrentFinnhubFunction(str);
		}
		else {
			m_lCurrentUpdateSECFilingsPos = 0;
			fHaveInquiry = false;
			SetUpdateSECFilings(false);
			const string str = "Finnhub SEC Filings Updated";
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}*/

bool CFinnhubDataSource::GenerateSECFilings() {
	auto isUpdateNeeded = [this]() { return IsUpdateSECFilings(); };
	auto isUpdateItemNeeded = [](const auto& item) { return item->IsUpdateSECFilings(); };
	auto isAccessible = [](int inquireType, const std::string& exchangeCode) {
		return !gl_finnhubInaccessibleExchange.HaveExchange(inquireType, exchangeCode);
	};
	auto createProduct = [this](int inquireType) { return m_FinnhubFactory.CreateProduct(gl_pWorldMarket, inquireType); };
	auto setIndex = [](auto& product, long pos) { product->SetIndex(pos); };
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
		isAccessible,
		createProduct,
		setIndex,
		setMessage,
		setUpdateFlag,
		finishedMsg
	);
}

/*
bool CFinnhubDataSource::GenerateForexExchange() {
	ASSERT(!IsInquiring());
	if (IsUpdateForexExchange()) {
		StoreInquiry(m_FinnhubFactory.CreateProduct(gl_pWorldMarket, FOREX_EXCHANGE_));
		SetInquiring(true);
		gl_systemMessage.SetCurrentFinnhubFunction(_T("forex exchange"));
		return true;
	}
	return false;
}*/

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
	ASSERT(!IsInquiring());
	if (IsUpdateForexSymbol()) {
		const CVirtualProductWebDataPtr product = m_FinnhubFactory.CreateProduct(gl_pWorldMarket, FOREX_SYMBOLS_);
		product->SetIndex(m_lCurrentForexExchangePos);
		StoreInquiry(product);
		SetInquiring(true);
		string str = _T("forex symbol: ");
		str += gl_dataContainerFinnhubForexExchange.GetItem(m_lCurrentForexExchangePos);
		gl_systemMessage.SetCurrentFinnhubFunction(str);
		if (++m_lCurrentForexExchangePos >= gl_dataContainerFinnhubForexExchange.Size()) {
			SetUpdateForexSymbol(false);
			m_lCurrentForexExchangePos = 0;
			gl_systemMessage.PushInformationMessage(_T("Finnhub Forex symbols updated"));
		}
		return true;
	}
	return false;
}

bool CFinnhubDataSource::GenerateCryptoSymbol() {
	ASSERT(!IsInquiring());
	if (IsUpdateCryptoSymbol()) {
		const CVirtualProductWebDataPtr product = m_FinnhubFactory.CreateProduct(gl_pWorldMarket, CRYPTO_SYMBOLS_);
		product->SetIndex(m_lCurrentCryptoExchangePos);
		StoreInquiry(product);
		SetInquiring(true);
		string str = _T("crypto symbol: ");
		str += gl_dataContainerFinnhubCryptoExchange.GetItem(m_lCurrentCryptoExchangePos);
		gl_systemMessage.SetCurrentFinnhubFunction(str);
		if (++m_lCurrentCryptoExchangePos >= gl_dataContainerFinnhubCryptoExchange.Size()) {
			SetUpdateCryptoSymbol(false);
			m_lCurrentCryptoExchangePos = 0;
			gl_systemMessage.PushInformationMessage(_T("Finnhub Crypto symbols updated"));
		}
		return true;
	}
	return false;
}

/*
bool CFinnhubDataSource::GenerateForexDayLine() {
	const auto lStockSetSize = gl_dataFinnhubForexSymbol.Size();
	bool fHaveInquiry = false;
	constexpr int iInquireType = FOREX_CANDLES_;

	ASSERT(gl_pWorldMarket->IsSystemReady());
	ASSERT(!IsInquiring());
	if (IsUpdateForexDayLine()) {
		CForexSymbolPtr pForexSymbol = nullptr;
		long lCurrentUpdateForexDayLinePos;
		bool fFound = false;
		for (lCurrentUpdateForexDayLinePos = 0; lCurrentUpdateForexDayLinePos < lStockSetSize; lCurrentUpdateForexDayLinePos++) {
			pForexSymbol = gl_dataFinnhubForexSymbol.GetItem(lCurrentUpdateForexDayLinePos);
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
			string str = _T("Forex���ߣ�");
			str += pForexSymbol->GetSymbol();
			gl_systemMessage.SetCurrentFinnhubFunction(str);
		}
		else {
			SetUpdateForexDayLine(false);
			//TRACE("Finnhub Forex���߸������\n");
			const string str = "Finnhub Forex DayLine Updated";
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}*/

bool CFinnhubDataSource::GenerateForexDayLine() {
	auto isUpdateNeeded = [this]() { return IsUpdateForexDayLine(); };
	auto isUpdateItemNeeded = [](const auto& item) { return item->IsUpdateDayLine(); };
	auto isAccessible = [](int inquireType, const std::string& exchangeCode) {
		return !gl_finnhubInaccessibleExchange.HaveExchange(inquireType, exchangeCode);
	};
	auto createProduct = [this](int inquireType) { return m_FinnhubFactory.CreateProduct(gl_pWorldMarket, inquireType); };
	auto setIndex = [](auto& product, long pos) { product->SetIndex(pos); };
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
		isAccessible,
		createProduct,
		setIndex,
		setMessage,
		setUpdateFlag,
		finishedMsg
	);
}

/*
bool CFinnhubDataSource::GenerateCryptoExchange() {
	ASSERT(!IsInquiring());
	if (IsUpdateCryptoExchange()) {
		StoreInquiry(m_FinnhubFactory.CreateProduct(gl_pWorldMarket, CRYPTO_EXCHANGE_));
		SetInquiring(true);
		gl_systemMessage.SetCurrentFinnhubFunction(_T("crypto exchange"));
		return true;
	}
	return false;
}*/

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

/*
bool CFinnhubDataSource::GenerateCryptoDayLine() {
	const auto lStockSetSize = gl_dataFinnhubCryptoSymbol.Size();
	bool fHaveInquiry = false;
	constexpr int iInquireType = CRYPTO_CANDLES_;

	ASSERT(gl_pWorldMarket->IsSystemReady());
	ASSERT(!IsInquiring());
	if (IsUpdateCryptoDayLine()) {
		CFinnhubCryptoPtr pCryptoSymbol;
		long lCurrentUpdateCryptoDayLinePos;
		bool fFound = false;
		for (lCurrentUpdateCryptoDayLinePos = 0; lCurrentUpdateCryptoDayLinePos < lStockSetSize; lCurrentUpdateCryptoDayLinePos++) {
			pCryptoSymbol = gl_dataFinnhubCryptoSymbol.GetItem(lCurrentUpdateCryptoDayLinePos);
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
			string str = _T("Crypto���ߣ�");
			str += pCryptoSymbol->GetSymbol();
			gl_systemMessage.SetCurrentFinnhubFunction(str);
		}
		else {
			SetUpdateCryptoDayLine(false);
			//TRACE("Finnhub Crypto���߸������\n");
			const string str = "Finnhub Crypto DayLine Updated";
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}*/

bool CFinnhubDataSource::GenerateCryptoDayLine() {
	auto isUpdateNeeded = [this]() { return IsUpdateCryptoDayLine(); };
	auto isUpdateItemNeeded = [](const auto& item) { return item->IsUpdateDayLine(); };
	auto isAccessible = [](int inquireType, const std::string& exchangeCode) {
		return !gl_finnhubInaccessibleExchange.HaveExchange(inquireType, exchangeCode);
	};
	auto createProduct = [this](int inquireType) { return m_FinnhubFactory.CreateProduct(gl_pWorldMarket, inquireType); };
	auto setIndex = [](auto& product, long pos) { product->SetIndex(pos); };
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
		isAccessible,
		createProduct,
		setIndex,
		setMessage,
		setUpdateFlag,
		finishedMsg
	);
}
