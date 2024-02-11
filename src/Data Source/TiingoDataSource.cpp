#include"pch.h"

#include"FinnhubInquiryType.h"

#include "TiingoDataSource.h"

#include"WorldMarket.h"

CTiingoDataSource::CTiingoDataSource() {
	ASSERT(gl_systemConfiguration.IsInitialized());
	m_strInquiryFunction = _T(""); // Tiingo�и������ݣ�����ǰ׺���������뺯��ÿ�����ã���ͬ��ǰ׺���벻ͬ�����ݡ�
	m_strParam = _T("");
	m_strSuffix = _T("");
	m_strSuffix = _T("&token=");
	m_strInquiryToken = _T("");
	m_lInquiringNumber = 1; // Tiingoʵʱ���ݲ�ѯ����Ĭ��ֵ

	CTiingoDataSource::ConfigureSession();
	CTiingoDataSource::Reset();
}

bool CTiingoDataSource::Reset() {
	m_fUpdateStockSymbol = true;
	m_fUpdateCryptoSymbol = true;
	m_fUpdateDayLine = true;

	m_fTiingoDataInquiryFinished = false;
	return true;
}

bool CTiingoDataSource::GenerateInquiryMessage(const long lCurrentTime) {
	const long long llTickCount = GetTickCount();

	if (llTickCount > (m_llLastTimeTickCount + gl_systemConfiguration.GetWorldMarketTiingoInquiryTime())) {
		m_llLastTimeTickCount = llTickCount;

		if (!IsInquiring()) {
			ASSERT(!HaveInquiry());
			InquireTiingo();
			if (IsInquiring()) {
				ASSERT(HaveInquiry());
				return true;
			}
		}
	}
	return false;
}

void CTiingoDataSource::InquireTiingo() {
	ASSERT(!IsInquiring());
	if (gl_pWorldMarket->IsSystemReady()) {
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

bool CTiingoDataSource::InquireCompanySymbol() {
	if (!IsInquiring() && IsUpdateStockSymbol()) {
		const CVirtualProductWebDataPtr p = m_TiingoFactory.CreateProduct(gl_pWorldMarket, STOCK_SYMBOLS_);
		StoreInquiry(p);
		SetInquiring(true);
		gl_systemMessage.PushInformationMessage(_T("Tiingo stock symbol�Ѹ���"));
		return true;
	}
	return false;
}

bool CTiingoDataSource::InquireCryptoSymbol() {
	if (!IsInquiring() && IsUpdateCryptoSymbol()) {
		const CVirtualProductWebDataPtr p = m_TiingoFactory.CreateProduct(gl_pWorldMarket, CRYPTO_SYMBOLS_);
		StoreInquiry(p);
		SetInquiring(true);
		gl_systemMessage.PushInformationMessage(_T("Tiingo crypto symbol�Ѹ���"));

		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// Tiingo����������������Finnhub���������غ�����ֻ����ͬʱ��������֮һ��
// Ŀǰ�˹���ֻ����������ѡ��Ʊ���������ݡ�
//
//
//////////////////////////////////////////////////////////////////////////////////////////
bool CTiingoDataSource::InquireDayLine() {
	const auto lStockSetSize = gl_dataContainerChosenWorldStock.Size();
	bool fHaveInquiry = false;

	ASSERT(gl_pWorldMarket->IsSystemReady());
	if (!IsInquiring() && IsUpdateDayLine()) {
		CWorldStockPtr pStock;
		bool fFound = false;
		for (long lCurrentUpdateDayLinePos = 0; lCurrentUpdateDayLinePos < lStockSetSize; lCurrentUpdateDayLinePos++) {
			if (gl_dataContainerChosenWorldStock.GetStock(lCurrentUpdateDayLinePos)->IsDayLineNeedUpdate()) {
				pStock = gl_dataContainerChosenWorldStock.GetStock(lCurrentUpdateDayLinePos);
				fFound = true;
				break;
			}
		}
		if (fFound) {
			fHaveInquiry = true;
			const CVirtualProductWebDataPtr p = m_TiingoFactory.CreateProduct(gl_pWorldMarket, STOCK_PRICE_CANDLES_);
			p->SetIndex(gl_dataContainerFinnhubStock.GetOffset(pStock->GetSymbol()));
			StoreInquiry(p);
			//gl_pWorldMarket->SetCurrentFunction(_T("Tiingo Stock���ߣ�") + pStock->GetSymbol());
			SetInquiring(true);
			//TRACE("����Tiingo %s��������\n", pStock->GetSymbol().GetBuffer());
		}
		else {
			SetUpdateDayLine(false);
			const CString str = "�����г���ѡ��Ʊ������ʷ���ݸ������";
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}

void CTiingoDataSource::ConfigureSession() {
	ASSERT(m_pSession != nullptr);
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 12000); // �������ӳ�ʱʱ��Ϊ12��
	m_pSession->SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, 12000); // ���ý��ճ�ʱʱ��Ϊ12��
	m_pSession->SetOption(INTERNET_OPTION_DATA_RECEIVE_TIMEOUT, 12000); // ���ý��ճ�ʱʱ��Ϊ12��
	m_pSession->SetOption(INTERNET_OPTION_SEND_TIMEOUT, 1000); // ���÷��ͳ�ʱʱ��Ϊ1��
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1); // 1������
}
