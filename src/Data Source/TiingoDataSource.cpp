#include"pch.h"

#include"FinnhubInquiryType.h"

#include "TiingoDataSource.h"

#include"WorldMarket.h"

map<string, enum_ErrorMessageData> mapTiingoErrorMap{
	{ _T("You do not have permission to access the News API"), ERROR_TIINGO_NO_RIGHT_TO_ACCESS__ },
	{ _T("Please use an API key."), ERROR_FINNHUB_MISSING_API_KEY__ },
	{ _T(""), ERROR_FINNHUB_INQUIRE_RATE_TOO_HIGH__ }
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
	if (m_dwHTTPStatusCode == 200) return m_eErrorMessageData; // OK? return no error

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
		switch (m_eErrorMessageData) {
		case ERROR_TIINGO_NO_RIGHT_TO_ACCESS__:// ��Ȩ����
			m_pCurrentProduct->SetReceivedDataStatus(NO_ACCESS_RIGHT_);
		// ���ϵͳ������Ȩ��ѯ��������, Ŀǰ�������ϵͳ��Ϣ�б���
			gl_systemMessage.PushInnerSystemInformationMessage(_T("No right to access: ") + m_pCurrentProduct->GetInquiry() + _T(",  Exchange = ") + m_pCurrentProduct->GetInquiringExchange());
			break;
		case ERROR_TIINGO_MISSING_API_KEY__: // ȱ��API key
			break;
		case ERROR_TIINGO_INQUIRE_RATE_TOO_HIGH__:// ����Ƶ�ʳ���
			// ���Ͳ�ѯƵ��200ms��
			// todo �������ֻ��ϵͳ����Ƶ�ʳ�������ϵͳ�������������
			i = gl_systemConfiguration.GetWorldMarketFinnhubInquiryTime();
			gl_systemConfiguration.SetWorldMarketFinnhubInquiryTime(i + 200);
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
	if (gl_pWorldMarket->IsSystemReady()) {
		ASSERT(lCurrentTime <= gl_systemConfiguration.GetWorldMarketResettingTime() - 300
			|| lCurrentTime >= gl_systemConfiguration.GetWorldMarketResettingTime() + 500); // �����г�ʱ���������������Ϣ��
		InquireMarketNews();
		InquireFundamentalDefinition();
		InquireCompanySymbol();
		InquireCryptoSymbol();
		InquireDayLine();
		if (!IsInquiring()) {
			if (!m_fTiingoDataInquiryFinished) {
				gl_systemMessage.PushInformationMessage(_T("Tiingo data inquiry finished"));
				m_fTiingoDataInquiryFinished = true;
			}
		}
	}
}

bool CTiingoDataSource::InquireMarketNews() {
	if (!IsInquiring() && IsUpdateMarketNews()) {
		const CVirtualProductWebDataPtr p = m_TiingoFactory.CreateProduct(gl_pWorldMarket, MARKET_NEWS_);
		StoreInquiry(p);
		SetInquiring(true);
		gl_systemMessage.PushInformationMessage(_T("Tiingo Market news�Ѹ���"));
		return true;
	}
	return false;
}

bool CTiingoDataSource::InquireFundamentalDefinition() {
	if (!IsInquiring() && IsUpdateFundamentalDefinition()) {
		const CVirtualProductWebDataPtr p = m_TiingoFactory.CreateProduct(gl_pWorldMarket, TIINGO_FUNDAMENTAL_DEFINITION_);
		StoreInquiry(p);
		SetInquiring(true);
		gl_systemMessage.PushInformationMessage(_T("Tiingo Fundamental Definition�Ѹ���"));
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
		return true;
	}
	return false;
}

bool CTiingoDataSource::InquireCryptoSymbol() {
	if (!IsInquiring() && IsUpdateCryptoSymbol()) {
		const CVirtualProductWebDataPtr p = m_TiingoFactory.CreateProduct(gl_pWorldMarket, CRYPTO_SYMBOLS_);
		StoreInquiry(p);
		SetInquiring(true);
		gl_systemMessage.PushInformationMessage(_T("Inquiring Tiingo crypto symbol..."));

		return true;
	}
	return false;
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

	ASSERT(gl_pWorldMarket->IsSystemReady());
	if (!IsInquiring() && IsUpdateDayLine()) {
		CWorldStockPtr pStock;
		bool fFound = false;
		long lCurrentUpdateDayLinePos;
		for (lCurrentUpdateDayLinePos = 0; lCurrentUpdateDayLinePos < lStockSetSize; lCurrentUpdateDayLinePos++) {
			auto pTiingoStock = gl_dataContainerTiingoStock.GetStock(lCurrentUpdateDayLinePos);
			if (pTiingoStock->IsDayLineNeedUpdate()) {
				pTiingoStock->SetDayLineNeedUpdate(false);
				fFound = true;
				break;
			}
		}
		if (fFound) {
			fHaveInquiry = true;
			const CVirtualProductWebDataPtr p = m_TiingoFactory.CreateProduct(gl_pWorldMarket, STOCK_PRICE_CANDLES_);
			p->SetIndex(lCurrentUpdateDayLinePos);
			StoreInquiry(p);
			SetInquiring(true);
		}
		else {
			SetUpdateDayLine(false);
			const CString str = "Tiingo��Ʊ������ʷ���ݸ������";
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}

bool CTiingoDataSource::InquireFinancialState() {
	bool fHaveInquiry = false;
	size_t lStockSetSize = gl_dataContainerTiingoStock.Size();

	ASSERT(gl_pWorldMarket->IsSystemReady());
	if (!IsInquiring() && IsUpdateDayLine()) {
		CWorldStockPtr pStock;
		bool fFound = false;
		for (long lCurrentUpdateDayLinePos = 0; lCurrentUpdateDayLinePos < lStockSetSize; lCurrentUpdateDayLinePos++) {
			auto pTiingoStock = gl_dataContainerTiingoStock.GetStock(lCurrentUpdateDayLinePos);
			if (pTiingoStock->IsDayLineNeedUpdate()) {
				pTiingoStock->SetDayLineNeedUpdate(false);
				pStock = gl_dataContainerFinnhubStock.GetStock(pTiingoStock->m_strTicker);
				ASSERT(pStock != nullptr);
				fFound = true;
				break;
			}
		}
		if (fFound) {
			fHaveInquiry = true;
			const CVirtualProductWebDataPtr p = m_TiingoFactory.CreateProduct(gl_pWorldMarket, STOCK_PRICE_CANDLES_);
			p->SetIndex(gl_dataContainerFinnhubStock.GetOffset(pStock->GetSymbol()));
			StoreInquiry(p);
			SetInquiring(true);
		}
		else {
			SetUpdateDayLine(false);
			const CString str = "Tiingo��Ʊ������ʷ���ݸ������";
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}
