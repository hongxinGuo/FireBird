#include"pch.h"

#include"FinnhubInquiryType.h"
#include "TiingoDataSource.h"

#include "TiingoInaccessibleStock.h"
#include "TimeConvert.h"
#include"WorldMarket.h"

map<string, enum_ErrorMessageData> mapTiingoErrorMap{
	{ _T("You do not have permission to access the News API"), ERROR_TIINGO_NO_RIGHT_TO_ACCESS__ }, // http״̬�룺403
	{ _T("Please supply a token"), ERROR_TIINGO_MISSING_API_KEY__ },
	{ _T("Error: Free and Power plans are limited to the DOW 30. If you would like access to all supported tickers, then please E-mail support@tiingo.com to get the Fundamental Data API added as an add-on service."), ERROR_TIINGO_ADD_ON_PERMISSION_NEEDED__ }, // http״̬�룺400
	{ _T("Error: resampleFreq must be in 'Min' or 'Hour' only"), ERROR_TIINGO_FREQUENCY__ },
	{ _T("You have run over your 500 symbol look up for this month. Please upgrade at https://api.tiingo.com/pricing to have your limits increased."), ERROR_TIINGO_REACH_MAX_SYMBOL_LIMIT__ }, // http״̬�룺200
	{ _T(""), ERROR_TIINGO_INQUIRE_RATE_TOO_HIGH__ }
};

set<CString> setDOW30{
	"AAPL", "AMGN", "AMZN", "AXP", "BA", "CAT", "CRM", "CSCO", "CVX", "DIS",
	"DOW", "IBM", "INTC", "JNJ", "JPM", "KO", "MMM", "MRK", "MSFT", "NKE",
	"PG", "UNH", "V", "VZ", "WMT", "GS", "HD", "HON", "MCD", "TRV"
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
	m_fUpdateDayLine = true;
	m_fUpdateFinancialState = true;
	m_fUpdateIEXTopOFBook = true;

	m_fTiingoDataInquiryFinished = false;
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
	string_view sView;
	int iStringViewLength = 0;
	string s2;
	int statusCode = 0;

	ASSERT(m_pCurrentProduct != nullptr);

	m_eErrorMessageData = ERROR_NO_ERROR__;
	// ��һ��switch�����json���ݸ�ʽ�Ĵ���
	switch (m_dwHTTPStatusCode) {
	case 200:
		if (gl_systemConfiguration.IsPaidTypeTiingoAccount()) return m_eErrorMessageData; // �����˻�ֱ�ӷ���
		if (pWebData->GetBufferLength() == 137) { // ����Ϊ��json��ʽ����
			auto strView = pWebData->GetStringView(0, 75); // ֻʹ��ǰ75���ַ�
			if (strView.compare(_T("You have run over your 500 symbol look up for this month. Please upgrade at")) == 0) { 	// �ﵽ��������
				gl_systemMessage.PushInnerSystemInformationMessage(_T("Tiingo symbol reach 500"));
				return ERROR_TIINGO_REACH_MAX_SYMBOL_LIMIT__;
			}
		}
		break;
	case 400: // bad request
		if (pWebData->GetBufferLength() == 62) {
			auto strView = pWebData->GetStringView(0, 75); // ֻʹ��ǰ75���ַ�
			if (strView.compare(_T("[\"Error: Endpoint only available for US and US - listed Stocks\"]")) == 0) { 	// ��������Ʊ���ṩ��������
				gl_systemMessage.PushInnerSystemInformationMessage(_T("Error: Endpoint only available for US and US - listed Stocks\"]"));
				return ERROR_TIINGO_ENDPOINT_ONLY_FOR_US_LISTED_STOCK__;
			}
		}
		break;
	case 403:
		break;
	case 404:
		break;
	default:
		break;
	}

	json js;
	if (!pWebData->CreateJson(js)) { // ��json��ʽ����Ӧ�ó��֡�
		m_eErrorMessageData = ERROR_NO_ERROR__;
		return m_eErrorMessageData; // ��ʱ������ȷ
	}

	// �ڶ���switch����json��ʽ�Ĵ���
	switch (m_dwHTTPStatusCode) {
	case 400:
		break;
	case 403:
		break;
	case 404:
		break;
	default:
		// δ�����״̬
		break;
	}

	try {
		string error = js.at(_T("detail"));
		int i;
		CString s;
		try {
			m_eErrorMessageData = mapTiingoErrorMap.at(error);
		} catch (exception&) {
			m_eErrorMessageData = ERROR_TIINGO_NOT_HANDLED__;
		}
		if (m_dwHTTPStatusCode == 404) { // 404����Ŀǰ��������������Ȩ��ʱ���ش������404��
			m_eErrorMessageData = ERROR_TIINGO_NO_RIGHT_TO_ACCESS__;
		}
		switch (m_eErrorMessageData) {
		case ERROR_TIINGO_NO_RIGHT_TO_ACCESS__:// ��Ȩ����
			m_pCurrentProduct->SetReceivedDataStatus(NO_ACCESS_RIGHT_);
			if (m_pCurrentProduct->CheckInaccessible()) {
				// ���ϵͳ������Ȩ��ѯ��������, Ŀǰ�������ϵͳ��Ϣ�б���
				gl_systemMessage.PushInnerSystemInformationMessage(_T("No right to access: ") + m_pCurrentProduct->GetInquiry() + _T(",  Exchange = ") + m_pCurrentProduct->GetInquiringExchange());
			}
			break;
		case ERROR_TIINGO_MISSING_API_KEY__: // ȱ��API key
			gl_systemMessage.PushErrorMessage(_T("Tiingo missing API key"));
			m_pCurrentProduct->SetReceivedDataStatus(NO_ACCESS_RIGHT_);
			break;
		case ERROR_TIINGO_ADD_ON_PERMISSION_NEEDED__: // ��Ҫ���򸽼����֤
			m_pCurrentProduct->SetReceivedDataStatus(NO_ACCESS_RIGHT_);
			gl_systemConfiguration.SetTiingoAccountAddOnPaid(false);
			break;
		case ERROR_TIINGO_INQUIRE_RATE_TOO_HIGH__:// ����Ƶ�ʳ���
			// ���Ͳ�ѯƵ��200ms��
			// todo �������ֻ��ϵͳ����Ƶ�ʳ�������ϵͳ�������������
			i = gl_systemConfiguration.GetWorldMarketTiingoInquiryTime();
			gl_systemConfiguration.SetWorldMarketTiingoInquiryTime(i + 200);
			break;
		case ERROR_TIINGO_NOT_HANDLED__: // error not handled
			if (pWebData->GetBufferLength() > 50) iStringViewLength = 50;
			else iStringViewLength = pWebData->GetBufferLength();
			sView = pWebData->GetStringView(0, iStringViewLength);
			s2 = sView;
			statusCode = m_dwHTTPStatusCode;
			gl_warnLogger->warn("TiingoDataSource Error status not handled http status code = {} message = {}", statusCode, s2.c_str());
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

bool CTiingoDataSource::GenerateInquiryMessage(const long lCurrentTime) {
	const long long llTickCount = GetTickCount();

	if (llTickCount > (m_llLastTimeTickCount + gl_systemConfiguration.GetWorldMarketTiingoInquiryTime())) {
		m_llLastTimeTickCount = llTickCount;

		ASSERT(!IsInquiring());
		if (Generate(lCurrentTime)) return true;
	}
	return false;
}

bool CTiingoDataSource::Generate(long lCurrentTime) {
	ASSERT(!IsInquiring());
	ASSERT(lCurrentTime <= GetPrevTime(gl_systemConfiguration.GetWorldMarketResettingTime(), 0, 10, 0)
		|| lCurrentTime >= GetNextTime(gl_systemConfiguration.GetWorldMarketResettingTime(), 0, 5, 0)); // �����г�ʱ���������������Ϣ��
	if (GenerateMarketNews()) return true; // Note �������λ�ڵ�һλ�������ж�tiingo�˻������͡�
	if (GenerateFundamentalDefinition()) return true;
	if (GenerateCompanySymbol()) return true;
	if (GenerateCryptoSymbol()) return true;
	if (GenerateIEXTopOfBook()) return true;
	if (GenerateDayLine()) return true; // ������������Ҫλ�ڰ��������������Ŀ֮�ס�
	if (GenerateFinancialState()) return true;
	if (!IsInquiring()) {
		if (!m_fTiingoDataInquiryFinished) {
			gl_systemMessage.PushInformationMessage(_T("Tiingo data inquiry finished"));
			m_fTiingoDataInquiryFinished = true;
		}
	}
	return true;
}

bool CTiingoDataSource::GenerateMarketNews() {
	ASSERT(!IsInquiring());
	if (IsUpdateMarketNews()) {
		const CVirtualProductWebDataPtr p = m_TiingoFactory.CreateProduct(gl_pWorldMarket, MARKET_NEWS_);
		StoreInquiry(p);
		SetInquiring(true);
		gl_systemMessage.PushInformationMessage(_T("Tiingo Market news�Ѹ���"));
		gl_pWorldMarket->SetCurrentTiingoFunction(_T("Market news"));
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
		gl_systemMessage.PushInformationMessage(_T("Inquiring Tiingo Fundamental Definition..."));
		gl_pWorldMarket->SetCurrentTiingoFunction(_T("Fundamental Definition"));
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
		gl_systemMessage.PushInformationMessage(_T("Inquire Tiingo stock symbol..."));
		gl_pWorldMarket->SetCurrentTiingoFunction(_T("Company symbol"));
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
		gl_pWorldMarket->SetCurrentTiingoFunction(_T("Fundamental Definition"));
		gl_systemMessage.PushInformationMessage(_T("Inquiring Tiingo crypto symbol..."));
		return true;
	}
	return false;
}

bool CTiingoDataSource::GenerateIEXTopOfBook() {
	ASSERT(!IsInquiring());
	if (IsUpdateIEXTopOfBook()) {
		const CVirtualProductWebDataPtr p = m_TiingoFactory.CreateProduct(gl_pWorldMarket, TIINGO_IEX_TOP_OF_BOOK_);
		StoreInquiry(p);
		SetInquiring(true);
		return true;
	}
	return false;
}

bool CTiingoDataSource::GenerateFinancialState() {
	bool fHaveInquiry = false;
	size_t lStockSetSize = gl_dataContainerTiingoStock.Size();
	constexpr int iInquireType = TIINGO_FINANCIAL_STATEMENT_;

	ASSERT(!IsInquiring());
	if (IsUpdateFinancialState()) {
		bool fFound = false;
		CTiingoStockPtr pTiingoStock;
		while (m_lCurrentUpdateFinancialStatementPos < lStockSetSize) {
			pTiingoStock = gl_dataContainerTiingoStock.GetStock(m_lCurrentUpdateFinancialStatementPos);
			if (pTiingoStock->IsUpdateFinancialState()) {
				if (gl_systemConfiguration.IsTiingoAccountAddOnPaid()) {
					if (!gl_tiingoInaccessibleStock.HaveStock(iInquireType, pTiingoStock->GetSymbol())) {
						fFound = true;
						break;
					}
				}
				else {
					if (setDOW30.contains(pTiingoStock->GetSymbol())) {
						if (!gl_tiingoInaccessibleStock.HaveStock(iInquireType, pTiingoStock->GetSymbol())) {
							fFound = true;
							break;
						}
					}
				}
			}
			m_lCurrentUpdateFinancialStatementPos++;
		}
		if (fFound) {
			fHaveInquiry = true;
			const CVirtualProductWebDataPtr p = m_TiingoFactory.CreateProduct(gl_pWorldMarket, iInquireType);
			p->SetIndex(m_lCurrentUpdateFinancialStatementPos);
			StoreInquiry(p);
			gl_pWorldMarket->SetCurrentTiingoFunction(_T("Financial statement: ") + pTiingoStock->GetSymbol());
			SetInquiring(true);
		}
		else {
			if (m_lCurrentUpdateFinancialStatementPos >= lStockSetSize) m_lCurrentUpdateFinancialStatementPos = 0;
			SetUpdateFinancialState(false);
			const CString str = "Tiingo financial statements�������";
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
		CTiingoStockPtr pTiingoStock;
		bool fFound = false;
		while (m_lCurrentUpdateDayLinePos < lStockSetSize) {
			pTiingoStock = gl_dataContainerTiingoStock.GetStock(m_lCurrentUpdateDayLinePos);
			if (pTiingoStock->IsUpdateDayLine()) {
				if (gl_systemConfiguration.IsPaidTypeTiingoAccount()) { // �����˻��������й�Ʊ������
					if (!gl_tiingoInaccessibleStock.HaveStock(iInquireType, pTiingoStock->GetSymbol())) {
						fFound = true;
						break;
					}
				}
				else { // ����˻�ֻ����DOW30����ѡ��Ʊ�����ߡ�
					if (setDOW30.contains(pTiingoStock->GetSymbol()) || gl_dataContainerChosenFinnhubStock.IsSymbol(pTiingoStock->GetSymbol())) {
						if (!gl_tiingoInaccessibleStock.HaveStock(iInquireType, pTiingoStock->GetSymbol())) {
							fFound = true;
							break;
						}
					}
				}
			}
			m_lCurrentUpdateDayLinePos++;
		}
		if (fFound) {
			fHaveInquiry = true;
			const CVirtualProductWebDataPtr p = m_TiingoFactory.CreateProduct(gl_pWorldMarket, iInquireType);
			p->SetIndex(m_lCurrentUpdateDayLinePos);
			StoreInquiry(p);
			SetInquiring(true);
			gl_pWorldMarket->SetCurrentTiingoFunction(_T("Day line: ") + pTiingoStock->GetSymbol());
		}
		else {
			SetUpdateDayLine(false);
			const CString str = "Tiingo��Ʊ������ʷ���ݸ������";
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	if (m_lCurrentUpdateDayLinePos >= lStockSetSize) m_lCurrentUpdateDayLinePos = 0;
	return fHaveInquiry;
}
