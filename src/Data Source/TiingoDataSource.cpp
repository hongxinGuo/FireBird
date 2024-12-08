#include"pch.h"

#include"globedef.h"

#include"SystemMessage.h"

import FireBird.FinnhubInquiryType;
#include "TiingoDataSource.h"

import FireBird.Tiingo.InaccessibleStock; 
#include "TimeConvert.h"
#include"WorldMarket.h"

map<string, enum_ErrorMessageData> mapTiingoErrorMap{
	{ _T("You do not have permission to access the News API"), ERROR_TIINGO_NO_RIGHT_TO_ACCESS__ }, // http״̬�룺403
	{ _T("Error: resampleFreq must be in 'Min' or 'Hour' only"), ERROR_TIINGO_FREQUENCY__ },
	{ _T("You have run over your 500 symbol look up for this month. Please upgrade at https://api.tiingo.com/pricing to have your limits increased."), ERROR_TIINGO_REACH_MAX_SYMBOL_LIMIT__ }, // http״̬�룺200
	{ _T("Not found."), ERROR_TIINGO_NOT_FOUND__ },
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
	ASSERT(m_pCurrentProduct != nullptr);

	string s2;
	CString str;
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
				gl_systemMessage.PushErrorMessage(_T("Tiingo missing API key"));
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
		gl_warnLogger->warn("{}", str.GetBuffer());
		return ERROR_TIINGO_NOT_HANDLED__;
	case 404:
		if (pWebData->GetBufferLength() == 23) { // {"detail":"Not found."}
			strView = pWebData->GetStringView(0, 23); // 
			if (strView.compare(_T("{\"detail\":\"Not found.\"}")) == 0) { 	//
				str = _T("Warning: Tiingo symbol not exist: ");
				str += m_pCurrentProduct->GetInquiry();
				gl_systemMessage.PushInnerSystemInformationMessage(str);
				gl_warnLogger->warn("{}", str.GetBuffer());
				return ERROR_TIINGO_NOT_FOUND__;
			}
		}
		str = _T("No right to access: ") + m_pCurrentProduct->GetInquiry() + _T(",  Exchange = ") + m_pCurrentProduct->GetInquiringExchange();
		gl_systemMessage.PushInnerSystemInformationMessage(str);
		gl_warnLogger->warn("{}", str.GetBuffer());
		return ERROR_TIINGO_NO_RIGHT_TO_ACCESS__; // Ŀǰ��������������Ȩ��ʱ���ش������404��
	default:
		l = pWebData->GetBufferLength() > 30 ? 30 : pWebData->GetBufferLength();
		strView = pWebData->GetStringView(0, l); //
		s2 = strView;
		str = _T("Warning: Tiingo no handled ");
		str += s2.c_str();
		str += _T("  ") + m_pCurrentProduct->GetInquiry();
		gl_systemMessage.PushInnerSystemInformationMessage(str);
		gl_warnLogger->warn("{}", str.GetBuffer());
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
		CString s;
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
				gl_systemMessage.PushInnerSystemInformationMessage(_T("No right to access: ") + m_pCurrentProduct->GetInquiry() + _T(",  Exchange = ") + m_pCurrentProduct->GetInquiringExchange());
			}
			break;
		case ERROR_TIINGO_MISSING_API_KEY__: // ȱ��API key
			gl_systemMessage.PushErrorMessage(_T("Tiingo missing API key"));
			m_pCurrentProduct->SetReceivedDataStatus(NO_ACCESS_RIGHT_);
			break;
		case ERROR_TIINGO_INQUIRE_RATE_TOO_HIGH__:// ����Ƶ�ʳ���
			// ���Ͳ�ѯƵ��200ms��
			// todo �������ֻ��ϵͳ����Ƶ�ʳ�������ϵͳ�������������
			i = gl_systemConfiguration.GetWorldMarketTiingoInquiryTime();
			gl_systemConfiguration.SetWorldMarketTiingoInquiryTime(i + 200);
			break;
		case ERROR_TIINGO_NOT_FOUND__:
			m_pCurrentProduct->SetReceivedDataStatus(NO_ACCESS_RIGHT_);
			gl_systemMessage.PushInnerSystemInformationMessage(_T("Tiingo symbol not exist"));
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
	const long long llTickCount = GetTickCount();

	if (llTickCount <= (m_llLastTimeTickCount + gl_systemConfiguration.GetWorldMarketTiingoInquiryTime())) return false;

	m_llLastTimeTickCount = llTickCount;
	ASSERT(!IsInquiring());
	ASSERT(lCurrentTime <= GetPrevTime(gl_systemConfiguration.GetWorldMarketResettingTime(), 0, 10, 0)
		|| lCurrentTime >= GetNextTime(gl_systemConfiguration.GetWorldMarketResettingTime(), 0, 5, 0)); // �����г�ʱ���������������Ϣ��
	if (GenerateMarketNews()) return true; // Note �������λ�ڵ�һλ�������ж�tiingo�˻������͡�
	if (GenerateFundamentalDefinition()) return true;
	if (GenerateCompanySymbol()) return true;
	if (GenerateCryptoSymbol()) return true;
#ifndef _DEBUG
	if (lCurrentTime < 170100) {// ���к󷽲�����dailyMeta�����ߵ����ݡ�
	gl_systemMessage.SetCurrentTiingoFunction(_T("waiting market close"));
		return true; 
	}
#endif
	if (GenerateStockDailyMeta()) return true;
	if (GenerateDayLine()) return true; // ������������Ҫλ�ڰ��������������Ŀ֮�ס�
	ASSERT(gl_dataContainerTiingoNewSymbol.IsEmpty()); // ���߸������������ᱻ���
	if (GenerateFinancialState()) return true;
	if (GenerateIEXTopOfBook(lCurrentTime)) return true;

	ASSERT(!IsInquiring());
	if (!m_fTiingoDataInquiryFinished) {
		gl_systemMessage.PushInformationMessage(_T("Tiingo data inquiry finished"));
		gl_systemMessage.SetCurrentTiingoFunction(_T("finished"));
		m_fTiingoDataInquiryFinished = true;
	}
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
		if (lCurrentTime < 180000) { // 18��󷽿ɽ���IEX����
			gl_pWorldMarket->AddTask(WORLD_MARKET_TIINGO_INQUIRE_IEX_TOP_OF_BOOK__, GetNextTime(180000, 0, 1, 0));
			SetUpdateIEXTopOfBook(false);
			return false;
		}
		if (gl_systemConfiguration.GetTiingoIEXTopOfBookUpdateDate() < gl_pWorldMarket->GetCurrentTradeDate()) {
			const CVirtualProductWebDataPtr p = m_TiingoFactory.CreateProduct(gl_pWorldMarket, TIINGO_IEX_TOP_OF_BOOK_);
			StoreInquiry(p);
			SetInquiring(true);
			gl_systemMessage.SetCurrentTiingoFunction(_T("IEX top of book"));
			gl_pWorldMarket->AddTask(WORLD_MARKET_TIINGO_BUILD_TODAY_STOCK_DAYLINE__, GetNextTime(gl_pWorldMarket->GetMarketTime(), 0, 1, 0));
			return true;
		}
		else {
			SetUpdateIEXTopOfBook(false); // �������������յ�������
		}
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
				if (gl_systemConfiguration.IsPaidTypeTiingoAccount()) { // ����Ǹ����˻��Ļ�
					fFound = true;
					break;
				}
				if (gl_dataContainerTiingoNewSymbol.IsSymbol(pTiingoStock->GetSymbol())) { // ����˻�ֻ�����¹�Ʊ��Daily meta
					fFound = true;
					break;
				}
				pTiingoStock->SetUpdateStockDailyMeta(false); // ����˻�ʱ�����¾ɹ�Ʊ��DailyMeta
			}
		}
		if (fFound) {
			fHaveInquiry = true;
			const CVirtualProductWebDataPtr p = m_TiingoFactory.CreateProduct(gl_pWorldMarket, iInquireType);
			p->SetIndex(lCurrentUpdatePos);
			StoreInquiry(p);
			gl_systemMessage.SetCurrentTiingoFunction(_T("daily meta: ") + pTiingoStock->GetSymbol());
			SetInquiring(true);
		}
		else {
			gl_systemMessage.SetCurrentTiingoFunction(_T(""));
			SetUpdateStockDailyMeta(false);
			const CString str = "Tiingo stock daily meta updated";
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
			gl_systemMessage.SetCurrentTiingoFunction(_T("Day line: ") + pTiingoStock->GetSymbol());
		}
		else {
			gl_systemMessage.SetCurrentTiingoFunction(_T(""));
			SetUpdateDayLine(false);
			const CString str = "Tiingo��Ʊ������ʷ���ݸ������";
			gl_systemMessage.PushInformationMessage(str);
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
			gl_systemMessage.SetCurrentTiingoFunction(_T("Financial statement: ") + pTiingoStock->GetSymbol());
			SetInquiring(true);
		}
		else {
			gl_systemMessage.SetCurrentTiingoFunction(_T(""));
			SetUpdateFinancialState(false);
			const CString str = "Tiingo financial statements�������";
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}
