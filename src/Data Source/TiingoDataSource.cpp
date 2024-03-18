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

	CTiingoDataSource::ConfigureInternetOption();
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

void CTiingoDataSource::ConfigureInternetOption() {
	m_internetOption.option_connect_timeout = 12000;
	m_internetOption.option_receive_timeout = 12000;
	m_internetOption.option_data_receive_timeout = 12000;
	m_internetOption.option_send_timeout = 1000;
	m_internetOption.option_connect_retries = 1;
}
