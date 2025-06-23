#include"pch.h"

#include"FinnhubInquiryType.h"
#include "TiingoDataSource.h"

#include "TiingoInaccessibleStock.h"
#include "WebData.h"
#include"WorldMarket.h"

map<string, enum_ErrorMessageData> mapTiingoErrorMap{
	{ _T("You do not have permission to access the News API"), ERROR_TIINGO_NO_RIGHT_TO_ACCESS__ }, // http״̬�룺403
	{ _T("Error: resampleFreq must be in 'Min' or 'Hour' only"), ERROR_TIINGO_FREQUENCY__ },
	{ _T("You have run over your 500 symbol look up for this month. Please upgrade at https://api.tiingo.com/pricing to have your limits increased."), ERROR_TIINGO_REACH_MAX_SYMBOL_LIMIT__ }, // http״̬�룺200
	{ _T("Error: You have run over your monthly bandwidth allocation. Please upgrade at https://api.tiingo.com/pricing to have your limits increased."), ERROR_TIINGO_REACH_MAX_BANDWIDTH_LIMIT__ }, // http״̬�룺429
	{ _T("API limit reached.please try again later.Remaining limit:0"), ERROR_TIINGO_REACH_MAX_API_LIMIT__ }, // http״̬�룺200
	{ _T("Not found."), ERROR_TIINGO_NOT_FOUND__ },
	{ _T(""), ERROR_TIINGO_INQUIRE_RATE_TOO_HIGH__ }
};

CTiingoDataSource::CTiingoDataSource() {
	ASSERT(gl_systemConfiguration.IsInitialized());
	m_strInquiryFunction = _T(""); // Tiingo�и������ݣ�����ǰ׺���������뺯��ÿ�����ã���ͬ��ǰ׺���벻ͬ�����ݡ�
	m_strParam = _T("");
	m_strSuffix = _T("");
	m_strSuffix = _T("&token=");
	m_strInquiryToken = _T("");
	m_lInquiringNumber = 1; // Tiingoʵʱ���ݲ�ѯ����Ĭ��ֵ

	CTiingoDataSource::ConfigureInternetOption();
	CTiingoDataSource::Reset();
}

bool CTiingoDataSource::Reset() {
	// Ŀǰ������
	m_fUpdateMarketNews = true;
	m_fUpdateFundamentalDefinitions = true;
	m_fUpdateStockSymbol = true;
	m_fUpdateCryptoSymbol = true;
	m_fUpdateStockDailyMeta = false; // ���¹�˾Meta�����������
	m_fUpdateDayLine = false; // ��������������������
	m_fUpdateFinancialState = true;
	m_fUpdateIEXTopOFBook = true;

	return true;
}

void CTiingoDataSource::ConfigureInternetOption() {
	m_internetOption.option_connect_timeout = 12000;
	m_internetOption.option_receive_timeout = 12000;
	m_internetOption.option_data_receive_timeout = 12000;
	m_internetOption.option_send_timeout = 1000;
	m_internetOption.option_connect_retries = 1;
}

enum_ErrorMessageData CTiingoDataSource::IsAErrorMessageData(const CWebDataPtr& pWebData) {
	ASSERT(m_pCurrentProduct != nullptr);

	string s2;
	string str;
	string_view strView;
	long l;

	m_eErrorMessageData = ERROR_NO_ERROR__;
	// ��һ��switch�����json���ݸ�ʽ�Ĵ���
	switch (m_dwHTTPStatusCode) {
	case 200:
		if (gl_systemConfiguration.IsPaidTypeTiingoAccount()) return m_eErrorMessageData; // �����˻�ֱ�ӷ���
		if (pWebData->GetBufferLength() == 137) { // ����Ϊ��json��ʽ����
			strView = pWebData->GetStringView(0, 75); // ֻʹ��ǰ75���ַ�
			if (strView.compare(_T("You have run over your 500 symbol look up for this month. Please upgrade at")) == 0) { 	// �ﵽ��������
				gl_systemMessage.PushInnerSystemInformationMessage(_T("Tiingo symbol reach 500"));
				s2 = strView;
				gl_warnLogger->warn("{}", s2);
				return ERROR_TIINGO_REACH_MAX_SYMBOL_LIMIT__;
			}
		}
		break;
	case 400: // bad request
		if (pWebData->GetBufferLength() == 62) {
			strView = pWebData->GetStringView(0, 62);
			if (strView.compare(_T("[\"Error: Endpoint only available for US and US - listed Stocks\"]")) == 0) { 	// ��������Ʊ���ṩ��������
				gl_systemMessage.PushInnerSystemInformationMessage(_T("Error: Endpoint only available for US and US - listed Stocks\"]"));
				s2 = strView;
				gl_warnLogger->warn("{}", s2);
				return ERROR_TIINGO_ENDPOINT_ONLY_FOR_US_LISTED_STOCK__;
			}
		}
		break;
	case 403:
		if (pWebData->GetBufferLength() == 34) {
			strView = pWebData->GetStringView(0, 34);
			if (strView.compare(_T("{\"detail\":\"Please supply a token\"}")) == 0) { 	// ��Ҫ����
				gl_systemMessage.PushErrorMessage(_T("Tiingo missing API key"));
				m_pCurrentProduct->SetReceivedDataStatus(NO_ACCESS_RIGHT_);
				s2 = strView;
				gl_warnLogger->warn("{}", s2);
				return ERROR_TIINGO_MISSING_API_KEY__;
			}
		}
		if (pWebData->GetBufferLength() == 62) {
			strView = pWebData->GetStringView(0, 62);
			if (strView.compare(_T("{\"detail\":\"You do not have permission to access the News API\"}")) == 0) { 	// ��������Ʊ���ṩ��������
				gl_systemMessage.PushInnerSystemInformationMessage(_T("Tiingo free account no right to access News API"));
				m_pCurrentProduct->SetReceivedDataStatus(NO_ACCESS_RIGHT_);
				s2 = strView;
				gl_warnLogger->warn("{}", s2);
				return ERROR_TIINGO_NO_RIGHT_TO_ACCESS__;
			}
		}
		if (pWebData->GetBufferLength() == 216) {
			strView = pWebData->GetStringView(0, 216);
			if (strView.compare(_T("{\"detail\":\"Error: Free and Power plans are limited to the DOW 30. If you would like access to all supported tickers, then please E-mail support@tiingo.com to get the Fundamental Data API added as an add-on service.\"}")) == 0) { 	// ��������Ʊ���ṩ��������
				gl_systemMessage.PushInnerSystemInformationMessage(_T("Tiingo Add-on permission needed"));
				m_pCurrentProduct->SetReceivedDataStatus(NO_ACCESS_RIGHT_);
				gl_systemConfiguration.SetTiingoAccountAddOnPaid(false);
				s2 = strView;
				gl_warnLogger->warn("{}", s2);
				return ERROR_TIINGO_ADD_ON_PERMISSION_NEEDED__;
			}
		}

		l = pWebData->GetBufferLength() > 30 ? 30 : pWebData->GetBufferLength();
		strView = pWebData->GetStringView(0, l); //
		s2 = strView;
		str = _T("Warning: Tiingo no handled ");
		str += s2.c_str();
		str += _T("  ") + m_pCurrentProduct->GetInquiry();
		gl_systemMessage.PushInnerSystemInformationMessage(str);
		gl_warnLogger->warn("{}", str);
		return ERROR_TIINGO_NOT_HANDLED__;
	case 404:
		if (pWebData->GetBufferLength() == 23) {
			strView = pWebData->GetStringView(0, 23); // 
			if (strView.compare(_T("{\"detail\":\"Not found.\"}")) == 0) { 	//
				str = _T("Warning: Tiingo symbol not exist: ");
				str += m_pCurrentProduct->GetInquiry();
				gl_systemMessage.PushInnerSystemInformationMessage(str);
				gl_warnLogger->warn("{}", str);
				return ERROR_TIINGO_NOT_FOUND__;
			}
		}
		str = _T("No right to access: ") + m_pCurrentProduct->GetInquiry() + _T(",  Exchange = ") + m_pCurrentProduct->GetInquiringExchange();
		gl_systemMessage.PushInnerSystemInformationMessage(str);
		gl_warnLogger->warn("{}", str);
		return ERROR_TIINGO_NO_RIGHT_TO_ACCESS__; // Ŀǰ��������������Ȩ��ʱ���ش������404��
	case 429:
		if (pWebData->GetBufferLength() == 152) {
			strView = pWebData->GetStringView(0, 152); // 
			if (strView.compare(_T("{\"detail\":\"Error: You have run over your monthly bandwidth allocation. Please upgrade at https://api.tiingo.com/pricing to have your limits increased.\"}")) == 0) { 	//
				str = _T("Warning: Tiingo run over your monthly bandwidth");
				gl_systemMessage.PushInnerSystemInformationMessage(str);
				gl_warnLogger->warn("{}", str);
				m_pCurrentProduct->SetReceivedDataStatus(ERROR_TIINGO_REACH_MAX_BANDWIDTH_LIMIT__);
				return ERROR_TIINGO_REACH_MAX_BANDWIDTH_LIMIT__;
			}
		}
		break;
	default:
		l = pWebData->GetBufferLength() > 30 ? 30 : pWebData->GetBufferLength();
		strView = pWebData->GetStringView(0, l); //
		s2 = strView;
		str = _T("Warning: Tiingo no handled ");
		str += s2.c_str();
		str += _T("  ") + m_pCurrentProduct->GetInquiry();
		gl_systemMessage.PushInnerSystemInformationMessage(str);
		gl_warnLogger->warn("{}", str);
		return ERROR_TIINGO_NOT_HANDLED__;
	}

	json js;
	if (!pWebData->CreateJson(js)) { // ��json��ʽ����Ӧ�ó��֡�
		m_eErrorMessageData = ERROR_NO_ERROR__;
		return m_eErrorMessageData; // ��ʱ������ȷ
	}

	try {
		string_view sView;
		size_t iStringViewLength;
		long long statusCode;
		string error = js.at(_T("detail"));
		int i;
		try {
			m_eErrorMessageData = mapTiingoErrorMap.at(error);
		} catch (exception&) {
			m_eErrorMessageData = ERROR_TIINGO_NOT_HANDLED__;
		}

		switch (m_eErrorMessageData) {
		case ERROR_TIINGO_NO_RIGHT_TO_ACCESS__:// ��Ȩ����
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
		case ERROR_TIINGO_MISSING_API_KEY__: // ȱ��API key
			gl_systemMessage.PushErrorMessage(_T("Tiingo missing API key"));
			m_pCurrentProduct->SetReceivedDataStatus(NO_ACCESS_RIGHT_);
			break;
		case ERROR_TIINGO_REACH_MAX_BANDWIDTH_LIMIT__: // �þ���ÿ������ʹ���� HTTP status code: 429
			ASSERT(m_dwHTTPStatusCode == 429);
			gl_systemMessage.PushErrorMessage(_T("Tiingo run over monthly bandwidth allocation"));
			m_pCurrentProduct->SetReceivedDataStatus(ERROR_TIINGO_REACH_MAX_BANDWIDTH_LIMIT__);
			break;
		case ERROR_TIINGO_INQUIRE_RATE_TOO_HIGH__:// ����Ƶ�ʳ���
			// ���Ͳ�ѯƵ��200ms��
			// todo �������ֻ��ϵͳ����Ƶ�ʳ�������ϵͳ�������������
			i = gl_systemConfiguration.GetWorldMarketTiingoInquiryTime().count();
			gl_systemConfiguration.SetWorldMarketTiingoInquiryTime(i + 200);
			break;
		case ERROR_TIINGO_NOT_FOUND__:
			m_pCurrentProduct->SetReceivedDataStatus(NO_ACCESS_RIGHT_);
			gl_systemMessage.PushInnerSystemInformationMessage(_T("Tiingo symbol not exist"));
			break;
		case ERROR_TIINGO_NOT_HANDLED__: // error not handled
			if (pWebData->GetBufferLength() > 50) iStringViewLength = 50;
			else iStringViewLength = pWebData->GetBufferLength();
			sView = pWebData->GetStringView(0, iStringViewLength);
			s2 = sView;
			statusCode = m_dwHTTPStatusCode.load();
			gl_warnLogger->warn("TiingoDataSource Error status not handled http status code = {} message = {}", statusCode, s2.c_str());
			ReportErrorNotHandled(error);
			break;
		default: // ȱʡ�����Ӧ�ó���
			ASSERT(false);
			break;
		}
	} catch (json::exception&) { // no error. do nothing
		m_eErrorMessageData = ERROR_NO_ERROR__;
	}
	return m_eErrorMessageData;
}

bool CTiingoDataSource::GenerateInquiryMessage(const long lCurrentTime) {
	const auto llTickCount = GetTickCount();

	if (llTickCount <= (m_PrevInquireTimePoint + gl_systemConfiguration.GetWorldMarketTiingoInquiryTime())) return false;

	m_PrevInquireTimePoint = llTickCount;
	ASSERT(!IsInquiring());
	ASSERT(lCurrentTime <= GetPrevTime(gl_systemConfiguration.GetWorldMarketResettingTime(), 0, 10, 0)
		|| lCurrentTime >= GetNextTime(gl_systemConfiguration.GetWorldMarketResettingTime(), 0, 5, 0)); // �����г�ʱ���������������Ϣ��
	if (GenerateMarketNews()) return true; // Note �������λ�ڵ�һλ�������ж�tiingo�˻������͡�
	if (GenerateFundamentalDefinition()) return true;
	if (GenerateCompanySymbol()) return true;
	if (GenerateCryptoSymbol()) return true;
	if (GenerateIEXTopOfBook(lCurrentTime)) return true; // Note �������ݰ������й�Ʊ�ļ�ʱ��Ϣ������������Ϊʵʱ����ʹ�á�
	if (GenerateStockDailyMeta()) return true;
	if (GenerateDayLine()) return true; // ������������Ҫλ�ڰ��������������Ŀ֮�ס�
	if (GenerateFinancialState()) return true;

	ASSERT(!IsInquiring());
	gl_systemMessage.SetCurrentTiingoFunction(_T("idling"));
	return false;
}

bool CTiingoDataSource::GenerateMarketNews() {
	ASSERT(!IsInquiring());
	if (IsUpdateMarketNews()) {
		const CVirtualProductWebDataPtr p = m_TiingoFactory.CreateProduct(gl_pWorldMarket, MARKET_NEWS_);
		StoreInquiry(p);
		SetInquiring(true);
		gl_systemMessage.SetCurrentTiingoFunction(_T("Market news"));
		return true;
	}
	return false;
}

bool CTiingoDataSource::GenerateFundamentalDefinition() {
	ASSERT(!IsInquiring());
	if (IsUpdateFundamentalDefinition()) {
		const CVirtualProductWebDataPtr p = m_TiingoFactory.CreateProduct(gl_pWorldMarket, TIINGO_FUNDAMENTAL_DEFINITION_);
		StoreInquiry(p);
		SetInquiring(true);
		gl_systemMessage.SetCurrentTiingoFunction(_T("Fundamental Definition"));
		return true;
	}
	return false;
}

bool CTiingoDataSource::GenerateCompanySymbol() {
	ASSERT(!IsInquiring());
	if (IsUpdateStockSymbol()) {
		const CVirtualProductWebDataPtr p = m_TiingoFactory.CreateProduct(gl_pWorldMarket, STOCK_SYMBOLS_);
		StoreInquiry(p);
		SetInquiring(true);
		gl_systemMessage.SetCurrentTiingoFunction(_T("Stock symbol"));
		return true;
	}
	return false;
}

bool CTiingoDataSource::GenerateCryptoSymbol() {
	ASSERT(!IsInquiring());
	if (IsUpdateCryptoSymbol()) {
		const CVirtualProductWebDataPtr p = m_TiingoFactory.CreateProduct(gl_pWorldMarket, CRYPTO_SYMBOLS_);
		StoreInquiry(p);
		SetInquiring(true);
		gl_systemMessage.SetCurrentTiingoFunction(_T("Crypto symbol"));
		return true;
	}
	return false;
}

bool CTiingoDataSource::GenerateIEXTopOfBook(long lCurrentTime) {
	ASSERT(!IsInquiring());
	if (IsUpdateIEXTopOfBook()) {
		const CVirtualProductWebDataPtr p = m_TiingoFactory.CreateProduct(gl_pWorldMarket, TIINGO_IEX_TOP_OF_BOOK_);
		StoreInquiry(p);
		SetInquiring(true);
		gl_systemMessage.SetCurrentTiingoFunction(_T("IEX top of book"));

		SetUpdateIEXTopOfBook(false); // �������������յ�������
		return true;
	}
	return false;
}

bool CTiingoDataSource::GenerateStockDailyMeta() {
	bool fHaveInquiry = false;
	size_t lStockSetSize = gl_dataContainerTiingoStock.Size();
	constexpr int iInquireType = TIINGO_STOCK_DAILY_META__;

	ASSERT(!IsInquiring());
	if (IsUpdateStockDailyMeta()) {
		long lCurrentUpdatePos;
		bool fFound = false;
		CTiingoStockPtr pTiingoStock;
		for (lCurrentUpdatePos = 0; lCurrentUpdatePos < lStockSetSize; lCurrentUpdatePos++) {
			pTiingoStock = gl_dataContainerTiingoStock.GetStock(lCurrentUpdatePos);
			if (pTiingoStock->IsUpdateStockDailyMeta()) {
				if (gl_systemConfiguration.IsPaidTypeTiingoAccount()) { // ����Ǹ����˻��Ļ����������й�Ʊ
					fFound = true;
					break;
				}
				if (gl_dataContainerTiingoNewSymbol.IsSymbol(pTiingoStock->GetSymbol())) { // ����˻�ֻ���±����³��ֵ��¹�Ʊ����֤ÿ���´��벻����500����
					fFound = true;
					break;
				}
				pTiingoStock->SetUpdateStockDailyMeta(false); // ����˻������¾ɹ�Ʊ
			}
		}
		if (fFound) {
			fHaveInquiry = true;
			const CVirtualProductWebDataPtr p = m_TiingoFactory.CreateProduct(gl_pWorldMarket, iInquireType);
			p->SetIndex(lCurrentUpdatePos);
			StoreInquiry(p);
			string s = _T("daily meta: ");
			s += pTiingoStock->GetSymbol();
			gl_systemMessage.SetCurrentTiingoFunction(s);
			SetInquiring(true);
		}
		else {
			gl_systemMessage.SetCurrentTiingoFunction(_T(""));
			SetUpdateStockDailyMeta(false);
			const string str = "Tiingo stock daily meta updated";
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Tiingo����������������Finnhub���������غ�����ֻ����ͬʱ��������֮һ��
// 
// �����˻������������ߣ�����˻���������ѡ��Ʊ�����ߡ�
// Note �������г�����ʱ��ͬʱ�������˻��Ƿ��Ǹ��ѵġ����������˻�����ͬʱ�رճ���ѡ��Ʊ����������߲�ѯ����
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CTiingoDataSource::GenerateDayLine() {
	bool fHaveInquiry = false;
	size_t lStockSetSize = gl_dataContainerTiingoStock.Size();
	constexpr int iInquireType = STOCK_PRICE_CANDLES_;

	ASSERT(!IsInquiring());
	if (IsUpdateDayLine()) {
		long lCurrentUpdatePos;
		CTiingoStockPtr pTiingoStock;
		bool fFound = false;
		for (lCurrentUpdatePos = 0; lCurrentUpdatePos < lStockSetSize; lCurrentUpdatePos++) {
			pTiingoStock = gl_dataContainerTiingoStock.GetStock(lCurrentUpdatePos);
			if (pTiingoStock->IsUpdateDayLine()) {
				ASSERT(pTiingoStock->IsActive()); // ��Ծ��Ʊ��
				if (gl_systemConfiguration.IsPaidTypeTiingoAccount()) { // �����˻��������й�Ʊ������
					fFound = true;
					break;
				}
				if (gl_dataContainerTiingoNewSymbol.IsSymbol(pTiingoStock->GetSymbol())) { // ����˻�ֻ�����¹�Ʊ������
					fFound = true;
					break;
				}
				pTiingoStock->SetUpdateDayLine(false); // ����˻�ʱ�������¾ɹ�Ʊ������
			}
		}
		if (fFound) {
			fHaveInquiry = true;
			const CVirtualProductWebDataPtr p = m_TiingoFactory.CreateProduct(gl_pWorldMarket, iInquireType);
			p->SetIndex(lCurrentUpdatePos);
			StoreInquiry(p);
			SetInquiring(true);
			string s = _T("Day line: ");
			s += pTiingoStock->GetSymbol();
			gl_systemMessage.SetCurrentTiingoFunction(s);
		}
		else {
			gl_systemMessage.SetCurrentTiingoFunction(_T(""));
			SetUpdateDayLine(false);
			const string str = "Tiingo stock dayLine Updated";
			gl_systemMessage.PushInformationMessage(str);
			if (gl_systemConfiguration.IsPaidTypeTiingoAccount()) {
				// Note �ݲ��Զ�������������
				//gl_pWorldMarket->AddTask(WORLD_MARKET_TIINGO_PROCESS_DAYLINE__, GetNextTime(gl_pWorldMarket->GetMarketTime(), 0, 2, 0));
			}
		}
	}
	return fHaveInquiry;
}

bool CTiingoDataSource::GenerateFinancialState() {
	bool fHaveInquiry = false;
	size_t lStockSetSize = gl_dataContainerTiingoStock.Size();
	constexpr int iInquireType = TIINGO_FINANCIAL_STATEMENT_;

	ASSERT(!IsInquiring());
	if (IsUpdateFinancialState()) {
		long lCurrentUpdatePos;
		bool fFound = false;
		CTiingoStockPtr pTiingoStock;
		for (lCurrentUpdatePos = 0; lCurrentUpdatePos < lStockSetSize; lCurrentUpdatePos++) {
			pTiingoStock = gl_dataContainerTiingoStock.GetStock(lCurrentUpdatePos);
			if (pTiingoStock->IsUpdateFinancialState()) {
				if (gl_systemConfiguration.IsTiingoAccountAddOnPaid()) {
					fFound = true;
					break;
				}
				pTiingoStock->SetUpdateFinancialState(false); // ����˻�������
			}
		}
		if (fFound) {
			fHaveInquiry = true;
			const CVirtualProductWebDataPtr p = m_TiingoFactory.CreateProduct(gl_pWorldMarket, iInquireType);
			p->SetIndex(lCurrentUpdatePos);
			StoreInquiry(p);
			string s = _T("Financial statement: ");
			s += pTiingoStock->GetSymbol();
			gl_systemMessage.SetCurrentTiingoFunction(s);
			SetInquiring(true);
		}
		else {
			gl_systemMessage.SetCurrentTiingoFunction(_T(""));
			SetUpdateFinancialState(false);
			const string str = "Tiingo financial statements Updated";
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}
