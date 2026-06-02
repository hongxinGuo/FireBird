#include "pch.h"

#include"jsonParse.h"
#include"nlohmannJsonGetValue.h"

#include"WorldMarket.h"
#include"FinnhubCrypto.h"

#include "ProductFinnhubCryptoDayLine.h"

#include "WebData.h"

CProductFinnhubCryptoDayLine::CProductFinnhubCryptoDayLine() {
	m_strInquiryFunction = "https://finnhub.io/api/v1/crypto/candle?symbol=";
}

string CProductFinnhubCryptoDayLine::CreateMessage() {
	const auto pCryptoSymbol = gl_dataFinnhubCryptoSymbol.GetItem(m_lIndex);

	m_strInquiringExchange = pCryptoSymbol->GetExchange();
	m_strInquiry = m_strInquiryFunction + pCryptoSymbol->GetFinnhubDayLineInquiryParam(GetUTCTime());
	return m_strInquiry;
}

void CProductFinnhubCryptoDayLine::ParseAndStoreWebData(CWebDataPtr pWebData) {
	const auto pCryptoSymbol = gl_dataFinnhubCryptoSymbol.GetItem(m_lIndex);
	const auto pvDayLine = ParseFinnhubCryptoCandle(pWebData);
	pCryptoSymbol->SetUpdateDayLine(false);
	if (!pvDayLine->empty()) {
		for (auto& dayLine : *pvDayLine) {
			dayLine.SetExchange(pCryptoSymbol->GetExchange());
			dayLine.SetStockSymbol(pCryptoSymbol->GetSymbol());
			const long lTemp = gl_pWorldMarket->ConvertToDate(dayLine.GetMarketTime());
			dayLine.SetDate(lTemp);
		}
		pCryptoSymbol->UpdateDayLine(pvDayLine);
		pCryptoSymbol->UpdateDayLineStartEndDate();
		if (gl_pWorldMarket->GetLastTradeDate() <= pCryptoSymbol->GetDayLineEndDate()) {
			pCryptoSymbol->SetIPOStatus(_STOCK_IPOED_);
		}
		else {
			pCryptoSymbol->SetIPOStatus(_STOCK_DELISTED_);
		}
		pCryptoSymbol->SetUpdateDayLineDB(true);
		pCryptoSymbol->SetUpdateProfileDB(true);
	}
	else {
		pCryptoSymbol->SetUpdateDayLineDB(false);
		if (!pCryptoSymbol->IsDelisted()) {
			pCryptoSymbol->SetIPOStatus(_STOCK_DELISTED_);
			pCryptoSymbol->SetUpdateProfileDB(true);
		}
	}
}

CDayLinesPtr CProductFinnhubCryptoDayLine::ParseFinnhubCryptoCandle(CWebDataPtr pWebData) {
	auto pvDayLine = make_shared<vector<CDayLine>>();
	pvDayLine->reserve(1000);

	auto pvDayLineReturn = make_shared<vector<CDayLine>>();
	nlohmannJson js2;
	string sError;
	nlohmannJson js;

	if (!pWebData->CreateJson(js)) return pvDayLine;
	if (!IsValidData(pWebData)) return pvDayLine;

	try {
		string s;
		s = jsonGetString(js, "s");
		if (s == "no_data") {
			// 没有日线数据，无需检查此股票的日线和实时数据
			return pvDayLine;
		}
		if (s != "ok") {
			gl_systemMessage.PushErrorMessage("日线返回值不为ok");
			return pvDayLine;
		}
	} catch (nlohmannJson::exception&) {
		// 这种请况是此代码出现问题。如服务器返回"error":"you don't have access this resource."
		ReportJSonErrorToSystemMessage("Finnhub Crypto Candle missing 's': ", pWebData->GetDataBuffer());
		return pvDayLine;
	}
	try {
		CDayLine dayLine;
		time_t tTemp = 0;
		js2 = jsonGetChild(js, "t");
		for (auto it = js2.begin(); it != js2.end(); ++it) {
			tTemp = it->get<INT64>();
			dayLine.SetTime(tTemp);
			pvDayLine->push_back(dayLine);
			dayLine.Reset();
		}
	} catch (nlohmannJson::exception& e) {
		ReportJSonErrorToSystemMessage("Finnhub Crypto Candle missing 't' ", e.what());
		return pvDayLine;
	}
	try {
		int i = 0;
		INT64 llTemp;
		double dTemp;
		js2 = jsonGetChild(js, "c");
		i = 0;
		for (auto it = js2.begin(); it != js2.end(); ++it) {
			dTemp = jsonGetDouble(it);
			pvDayLine->at(i++).SetClose(static_cast<long>(dTemp * 1000));
		}
		js2 = jsonGetChild(js, "h");
		i = 0;
		for (auto it = js2.begin(); it != js2.end(); ++it) {
			dTemp = jsonGetDouble(it);
			pvDayLine->at(i++).SetHigh(static_cast<long>(1000 * dTemp));
		}
		js2 = jsonGetChild(js, "l");
		i = 0;
		for (auto it = js2.begin(); it != js2.end(); ++it) {
			dTemp = jsonGetDouble(it);
			pvDayLine->at(i++).SetLow(static_cast<long>(1000 * dTemp));
		}
		js2 = jsonGetChild(js, "o");
		i = 0;
		for (auto it = js2.begin(); it != js2.end(); ++it) {
			dTemp = jsonGetDouble(it);
			pvDayLine->at(i++).SetOpen(static_cast<long>(1000 * dTemp));
		}
		js2 = jsonGetChild(js, "v");
		i = 0;
		for (auto it = js2.begin(); it != js2.end(); ++it) {
			llTemp = jsonGetLongLong(it);
			pvDayLine->at(i++).SetVolume(llTemp);
		}
	} catch (nlohmannJson::exception& e) {
		ReportJSonErrorToSystemMessage("Finnhub Crypto Candle ", e.what());
		// 有些外汇交易不提供成交量，忽略就可以了
	}
	std::ranges::sort(pvDayLine->begin(), pvDayLine->end(), CompareDayLineDate);
	// 清除掉交易日期为零的无效数据
	for (auto& pDayLine2 : *pvDayLine) {
		if (pDayLine2.GetMarketTime() > 0) {
			pvDayLineReturn->push_back(pDayLine2);
		}
	}
	return pvDayLineReturn;
}
