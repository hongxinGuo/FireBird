#include"pch.h"

#include"FinnhubInquiryType.h"
#include "TiingoDataSource.h"

#include "TiingoInaccessibleStock.h"
#include"WorldMarket.h"

map<string, enum_ErrorMessageData> mapTiingoErrorMap{
	{ _T("You do not have permission to access the News API"), ERROR_TIINGO_NO_RIGHT_TO_ACCESS__ },
	{ _T("Please supply a token"), ERROR_TIINGO_MISSING_API_KEY__ },
	{ _T("Error: Free and Power plans are limited to the DOW 30. If you would like access to all supported tickers, then please E-mail support@tiingo.com to get the Fundamental Data API added as an add-on service."), ERROR_TIINGO_ADD_ON_PERMISSION_NEEDED__ },
	{ _T("Error: resampleFreq must be in 'Min' or 'Hour' only"), ERROR_TIINGO_FREQUENCY__ },
	{ _T("You have run over your 500 symbol look up for this month. Please upgrade at https://api.tiingo.com/pricing to have your limits increased."), ERROR_TIINGO_REACH_MAX_SYMBOL_LIMIT__ },
	{ _T(""), ERROR_TIINGO_INQUIRE_RATE_TOO_HIGH__ }
};

set<CString> setDOW30{
	"AAPL",
	"AMGN",
	"AMZN",
	"AXP",
	"BA",
	"CAT",
	"CRM",
	"CSCO",
	"CVX",
	"DIS",
	"DOW",
	"IBM",
	"INTC",
	"JNJ",
	"JPM",
	"KO",
	"MMM",
	"MRK",
	"MSFT",
	"NKE",
	"PG",
	"UNH",
	"V",
	"VZ",
	"WMT",
	"GS",
	"HD",
	"HON",
	"MCD",
	"TRV"

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
	m_fUpdateMarketNews = true;
	m_fUpdateFundamentalDefinitions = true;
	m_fUpdateStockSymbol = true;
	m_fUpdateCryptoSymbol = true;
	m_fUpdateDayLine = true;

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

	m_eErrorMessageData = ERROR_NO_ERROR__;
	if (m_dwHTTPStatusCode == 200) {
		if (gl_systemConfiguration.IsPaidTypeTiingoAccount()) return m_eErrorMessageData; // �����˻�ֱ�ӷ���
		if (pWebData->GetBufferLength() == 137) {
			auto strView = pWebData->GetStringView(0, 75); // ֻʹ��ǰ75���ַ�
			if (strView.compare(_T("You have run over your 500 symbol look up for this month. Please upgrade at")) == 0) { 	// �ﵽ��������
				gl_systemMessage.PushInnerSystemInformationMessage(_T("Tiingo symbol reach 500"));
				return ERROR_TIINGO_REACH_MAX_SYMBOL_LIMIT__;
			}
		}
	}

	json js;
	pWebData->CreateJson(js);

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

void CTiingoDataSource::Inquire(long lCurrentTime) {
	ASSERT(!IsInquiring());
	//if (gl_pWorldMarket->IsSystemReady()) {
	ASSERT(lCurrentTime <= gl_systemConfiguration.GetWorldMarketResettingTime() - 300
		|| lCurrentTime >= gl_systemConfiguration.GetWorldMarketResettingTime() + 500); // �����г�ʱ���������������Ϣ��
	InquireMarketNews(); // Note �������λ�ڵ�һλ�������ж�tiingo�˻������͡�
	InquireFundamentalDefinition();
	InquireCompanySymbol();
	InquireCryptoSymbol();
	InquireIEXTopOfBook();
	InquireFinancialState();
	InquireDayLine();
	if (!IsInquiring()) {
		if (!m_fTiingoDataInquiryFinished) {
			gl_systemMessage.PushInformationMessage(_T("Tiingo data inquiry finished"));
			m_fTiingoDataInquiryFinished = true;
		}
	}
	//}
}

bool CTiingoDataSource::InquireMarketNews() {
	if (!IsInquiring() && IsUpdateMarketNews()) {
		const CVirtualProductWebDataPtr p = m_TiingoFactory.CreateProduct(gl_pWorldMarket, MARKET_NEWS_);
		StoreInquiry(p);
		SetInquiring(true);
		gl_systemMessage.PushInformationMessage(_T("Tiingo Market news�Ѹ���"));
		gl_pWorldMarket->SetCurrentTiingoFunction(_T("Market news"));
		return true;
	}
	return false;
}

bool CTiingoDataSource::InquireFundamentalDefinition() {
	if (!IsInquiring() && IsUpdateFundamentalDefinition()) {
		const CVirtualProductWebDataPtr p = m_TiingoFactory.CreateProduct(gl_pWorldMarket, TIINGO_FUNDAMENTAL_DEFINITION_);
		StoreInquiry(p);
		SetInquiring(true);
		gl_systemMessage.PushInformationMessage(_T("Inquiring Tiingo Fundamental Definition..."));
		gl_pWorldMarket->SetCurrentTiingoFunction(_T("Fundamental Definition"));
		return true;
	}
	return false;
}

bool CTiingoDataSource::InquireCompanySymbol() {
	if (!IsInquiring() && IsUpdateStockSymbol()) {
		const CVirtualProductWebDataPtr p = m_TiingoFactory.CreateProduct(gl_pWorldMarket, STOCK_SYMBOLS_);
		StoreInquiry(p);
		SetInquiring(true);
		gl_systemMessage.PushInformationMessage(_T("Inquire Tiingo stock symbol..."));
		gl_pWorldMarket->SetCurrentTiingoFunction(_T("Company symbol"));
		return true;
	}
	return false;
}

bool CTiingoDataSource::InquireCryptoSymbol() {
	if (!IsInquiring() && IsUpdateCryptoSymbol()) {
		const CVirtualProductWebDataPtr p = m_TiingoFactory.CreateProduct(gl_pWorldMarket, CRYPTO_SYMBOLS_);
		StoreInquiry(p);
		SetInquiring(true);
		gl_pWorldMarket->SetCurrentTiingoFunction(_T("Fundamental Definition"));
		gl_systemMessage.PushInformationMessage(_T("Inquiring Tiingo crypto symbol..."));
		return true;
	}
	return false;
}

bool CTiingoDataSource::InquireIEXTopOfBook() {
	if (!IsInquiring() && IsUpdateIEXTopOfBook()) {
		const CVirtualProductWebDataPtr p = m_TiingoFactory.CreateProduct(gl_pWorldMarket, TIINGO_IEX_TOP_OF_BOOK_);
		StoreInquiry(p);
		SetInquiring(true);
		return true;
	}
	return false;
}

bool CTiingoDataSource::InquireFinancialState() {
	bool fHaveInquiry = false;
	size_t lStockSetSize = gl_dataContainerTiingoStock.Size();
	constexpr int iInquireType = TIINGO_FINANCIAL_STATEMENT_;

	if (!IsInquiring() && IsUpdateFinancialState()) {
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
bool CTiingoDataSource::InquireDayLine() {
	bool fHaveInquiry = false;
	size_t lStockSetSize = gl_dataContainerTiingoStock.Size();
	constexpr int iInquireType = STOCK_PRICE_CANDLES_;

	if (!IsInquiring() && IsUpdateDayLine()) {
		CTiingoStockPtr pTiingoStock;
		bool fFound = false;
		while (m_lCurrentUpdateDayLinePos < lStockSetSize) {
			pTiingoStock = gl_dataContainerTiingoStock.GetStock(m_lCurrentUpdateDayLinePos);
			if (pTiingoStock->IsUpdateDayLine()) {
				if (gl_systemConfiguration.IsTiingoAccountAddOnPaid()) { // �����˻��������й�Ʊ������
					if (!gl_tiingoInaccessibleStock.HaveStock(iInquireType, pTiingoStock->GetSymbol())) {
						fFound = true;
						break;
					}
				}
				else { // ����˻�ֻ����DOW30����ѡ��Ʊ�����ߡ�
					if (setDOW30.contains(pTiingoStock->GetSymbol()) || gl_dataContainerChosenWorldStock.IsSymbol(pTiingoStock->GetSymbol())) {
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
