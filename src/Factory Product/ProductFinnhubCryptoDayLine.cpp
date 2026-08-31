#include "pch.h"

#include"jsonParse.h"
#include"nlohmannJsonGetValue.h"

#include"FinnhubCrypto.h"

#include "ProductFinnhubCryptoDayLine.h"

#include "ContainerFinnhubCrypto.h"
#include "SystemMessage.h"
#include"DayLine.h"
#include "FinnhubDataSource.h"
#include"cpr/cpr.h"

using namespace std;

CProductFinnhubCryptoDayLine::CProductFinnhubCryptoDayLine() {
	m_strInquiryFunction = "https://finnhub.io/api/v1/crypto/candle?symbol=";
}

void CProductFinnhubCryptoDayLine::InquireData(const std::stop_token& st) {
	auto inquireStrings = CreateMessage();
	for (const auto& inquiry : *inquireStrings) {
		if (st.stop_requested()) break;
		string inquireString = inquiry + "&token=" + gl_pFinnhubDataSource->GetToken();
		m_r = cpr::Get(cpr::Url{ inquireString });

		if (m_r.status_code != 200) {
			WebStatusCheck(m_r);
			return;
		}

		const auto pCryptoSymbol = gl_dataFinnhubCryptoSymbol.GetItem(m_index);
		const auto pvDayLine = Parse(m_r.text);
		pCryptoSymbol->SetUpdateDayLine(false);
		if (!pvDayLine->empty()) {
			for (auto& dayLine : *pvDayLine) {
				dayLine.SetExchange(pCryptoSymbol->GetExchange());
				dayLine.SetStockSymbol(pCryptoSymbol->GetSymbol());
			}
			pCryptoSymbol->UpdateDayLine(pvDayLine);
			pCryptoSymbol->UpdateDayLineStartEndDate();
			pCryptoSymbol->SetUpdateDayLineDB(true);
			pCryptoSymbol->SetUpdateProfileDB(true);
		}
		else {
			pCryptoSymbol->SetUpdateDayLineDB(false);
		}
	}
}

void CProductFinnhubCryptoDayLine::WebStatusCheck(cpr::Response& r) {
	switch (r.status_code) {
	case 0: //
		// do nothing
		break;
	case 401: // no right to access
	case 403: // forbidden
		m_iReceivedDataStatus = NO_ACCESS_RIGHT_;
		CheckInaccessible();
		break;
	default:
		WebErrorReport();
		break;
	}
}
void CProductFinnhubCryptoDayLine::UpdateSystemStatus() {
}

shared_ptr<vector<string>> CProductFinnhubCryptoDayLine::CreateMessage() {
	const auto pCryptoSymbol = gl_dataFinnhubCryptoSymbol.GetItem(m_index);
	shared_ptr<vector<string>> pInquiry = make_shared<vector<string>>();

	m_strInquiringExchange = pCryptoSymbol->GetExchange();
	m_inquiryString = m_strInquiryFunction + pCryptoSymbol->GetFinnhubDayLineInquiryParam(GetUTCTime());
	pInquiry->push_back(m_inquiryString);
	return pInquiry;
}

CDayLinesPtr CProductFinnhubCryptoDayLine::Parse(const string& text) {
	auto pvDayLine = make_shared<vector<CDayLine>>();
	pvDayLine->reserve(1000);

	auto pvDayLineReturn = make_shared<vector<CDayLine>>();
	nlohmannJson js2;
	string sError;
	nlohmannJson js;

	if (text.empty()) return pvDayLine;
	if (!::CreateJsonWithNlohmann(js, text)) return pvDayLine;
	if (::IsVoidJson(text)) return pvDayLine; // 即使为空，也完成了查询。
	if (IsNoRightToAccess()) return pvDayLine;

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
		ReportJSonErrorToSystemMessage("Finnhub Crypto Candle missing 's': ", text);
		return pvDayLine;
	}
	try {
		CDayLine dayLine;
		time_t tTemp = 0;
		js2 = jsonGetChild(js, "t");
		for (auto it = js2.begin(); it != js2.end(); ++it) {
			tTemp = it->get<int64_t>();
			chrono::local_seconds tp{ chrono::seconds{ tTemp } };
			dayLine.SetDate(tp);
			pvDayLine->push_back(dayLine);
			dayLine.Reset();
		}
	} catch (nlohmannJson::exception& e) {
		ReportJSonErrorToSystemMessage("Finnhub Crypto Candle missing 't' ", e.what());
		return pvDayLine;
	}
	try {
		int i = 0;
		int64_t llTemp;
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
		if (pDayLine2.GetDate().time_since_epoch().count() > 0) {
			pvDayLineReturn->push_back(pDayLine2);
		}
	}
	return pvDayLineReturn;
}
