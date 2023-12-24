#include"pch.h"

#include"FinnhubInquiryType.h"

#include "TiingoDataSource.h"

#include"WorldMarket.h"

CTiingoDataSource::CTiingoDataSource() {
	ASSERT(gl_systemConfiguration.IsInitialized());
	m_strInquiryFunction = _T(""); // Tiingo有各种数据，故其前缀由数据申请函数每次设置，不同的前缀申请不同的数据。
	m_strParam = _T("");
	m_strSuffix = _T("");
	m_strSuffix = _T("&token=");
	m_strInquiryToken = _T("");
	m_lInquiringNumber = 1; // Tiingo实时数据查询数量默认值

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
		m_qProduct.push(p);
		SetInquiring(true);
		gl_systemMessage.PushInformationMessage(_T("Tiingo stock symbol已更新"));
		return true;
	}
	return false;
}

bool CTiingoDataSource::InquireCryptoSymbol() {
	if (!IsInquiring() && IsUpdateCryptoSymbol()) {
		const CVirtualProductWebDataPtr p = m_TiingoFactory.CreateProduct(gl_pWorldMarket, CRYPTO_SYMBOLS_);
		m_qProduct.push(p);
		SetInquiring(true);
		gl_systemMessage.PushInformationMessage(_T("Tiingo crypto symbol已更新"));

		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// Tiingo的下载日线数据与Finnhub的日线下载函数，只允许同时运行其中之一。
// 目前此功能只用于下载自选股票的日线数据。
//
//
//////////////////////////////////////////////////////////////////////////////////////////
bool CTiingoDataSource::InquireDayLine() {
	const auto lStockSetSize = gl_containerChosenStock.Size();
	bool fHaveInquiry = false;

	ASSERT(gl_pWorldMarket->IsSystemReady());
	if (!IsInquiring() && IsUpdateDayLine()) {
		CWorldStockPtr pStock;
		bool fFound = false;
		for (long lCurrentUpdateDayLinePos = 0; lCurrentUpdateDayLinePos < lStockSetSize; lCurrentUpdateDayLinePos++) {
			if (gl_containerChosenStock.GetStock(lCurrentUpdateDayLinePos)->IsDayLineNeedUpdate()) {
				pStock = gl_containerChosenStock.GetStock(lCurrentUpdateDayLinePos);
				fFound = true;
				break;
			}
		}
		if (fFound) {
			fHaveInquiry = true;
			const CVirtualProductWebDataPtr p = m_TiingoFactory.CreateProduct(gl_pWorldMarket, STOCK_PRICE_CANDLES_);
			p->SetIndex(gl_containerStock.GetOffset(pStock->GetSymbol()));
			m_qProduct.push(p);
			//gl_pWorldMarket->SetCurrentFunction(_T("Tiingo Stock日线：") + pStock->GetSymbol());
			SetInquiring(true);
			TRACE("申请Tiingo %s日线数据\n", pStock->GetSymbol().GetBuffer());
		}
		else {
			SetUpdateDayLine(false);
			const CString str = "美国市场自选股票日线历史数据更新完毕";
			gl_systemMessage.PushInformationMessage(str);
		}
	}
	return fHaveInquiry;
}

void CTiingoDataSource::ConfigureSession() {
	ASSERT(m_pSession != nullptr);
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 120000); // 设置连接超时时间为120秒
	m_pSession->SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, 120000); // 设置接收超时时间为120秒
	m_pSession->SetOption(INTERNET_OPTION_SEND_TIMEOUT, 1000); // 设置发送超时时间为1秒
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1); // 1次重试
}

/////////////////////////////////////////////////////////////////////////////////////
//
//
// 使用nlohmann json解析。
//
////////////////////////////////////////////////////////////////////////////////////
bool CTiingoDataSource::ParseData(CWebDataPtr pWebData) {
	return pWebData->CreateJson(0, 0);
}
