#include "pch.h"

#include"jsonParse.h"
#include"nlohmannJsonGetValue.h"

#include"FinnhubForex.h"

#include "ProductFinnhubForexDayLine.h"

#include "containerFinnhubForexSymbol.h"
#include "DayLine.h"
#include "SystemMessage.h"
#include "FinnhubDataSource.h"
#include"cpr/cpr.h"

using namespace std;

CProductFinnhubForexDayLine::CProductFinnhubForexDayLine() {
	m_strInquiryFunction = "https://finnhub.io/api/v1/forex/candle?symbol=";
}

void CProductFinnhubForexDayLine::InquireData(const std::stop_token& st) {
	auto inquireStrings = CreateMessage();
	for (const auto& inquiry : *inquireStrings) {
		if (st.stop_requested()) break;
		string inquireString = inquiry + "&token=" + gl_pFinnhubDataSource->GetToken();
		cpr::Response r = cpr::Get(cpr::Url{ inquireString });
		m_statusCode = r.status_code;
		m_elapsed = r.elapsed;

		if (m_statusCode != 200) {
			WebStatusCheck(r);
			return;
		}

		const auto pForexSymbol = gl_dataFinnhubForexSymbol.GetItem(m_index);
		const CDayLinesPtr pvDayLine = Parse(r.text);
		pForexSymbol->SetUpdateDayLine(false);
		if (!pvDayLine->empty()) {
			for (auto& dayLine : *pvDayLine) {
				dayLine.SetExchange(pForexSymbol->GetExchange());
				dayLine.SetStockSymbol(pForexSymbol->GetSymbol());
			}
			pForexSymbol->UpdateDayLine(pvDayLine);
			pForexSymbol->SetUpdateDayLineDB(true);
			pForexSymbol->SetUpdateProfileDB(true);
			//ABSL_DLOG(INFO) << std::format("处理%s日线数据\n", pForexSymbol->GetSymbol().c_str());
			return;
		}
		else {
			pForexSymbol->SetUpdateDayLineDB(false);
			pForexSymbol->SetUpdateProfileDB(false);
			//ABSL_DLOG(INFO) << std::format("处理%s日线数据\n", pForexSymbol->GetSymbol().c_str());
		}
	}
}

void CProductFinnhubForexDayLine::WebStatusCheck(cpr::Response& r) {
	switch (r.status_code) {
	case 0: //
		// do nothing
		break;
	case 401: // no right to access
	case 403:
		m_iReceivedDataStatus = NO_ACCESS_RIGHT_;
		CheckInaccessible();
		break;
	default:
		string sType = typeid(this).name();
		string s = std::format("{} error. http code: {}, error code:{}, message:{}", sType, r.status_code, static_cast<int>(r.error.code), r.error.message);
		gl_systemMessage.PushErrorMessage(s);
		break;
	}
}
void CProductFinnhubForexDayLine::UpdateSystemStatus() {
}

shared_ptr<vector<string>> CProductFinnhubForexDayLine::CreateMessage() {
	const auto pForexSymbol = gl_dataFinnhubForexSymbol.GetItem(m_index);

	m_strInquiringExchange = pForexSymbol->GetExchange();
	m_inquiryString = m_strInquiryFunction + pForexSymbol->GetFinnhubDayLineInquiryParam(GetUTCTime());
	shared_ptr<vector<string>> pInquiry = make_shared<vector<string>>();
	pInquiry->push_back(m_inquiryString);
	return pInquiry;
}

CDayLinesPtr CProductFinnhubForexDayLine::Parse(const string& text) {
	auto pvDayLine = make_shared<vector<CDayLine>>();
	pvDayLine->reserve(1000); // 预先分配空间，避免频繁扩容。一般来说，外汇的日线数据不会超过1000条。

	nlohmannJson js2;
	string sError;
	nlohmannJson js;

	if (text.empty()) return pvDayLine;
	if (!::CreateJsonWithNlohmann(js, text)) return pvDayLine;
	if (::IsVoidJson(text)) return pvDayLine; // 即使为空，也完成了查询。
	if (IsNoRightToAccess()) return pvDayLine;

	try {
		auto s = jsonGetString(js, "s");
		if (s == "no_data") {
			// 没有日线数据，无需检查此股票的日线和实时数据
			return pvDayLine;
		}
		if (s != "ok") {
			gl_systemMessage.PushErrorMessage("日线返回值不为ok");
			return pvDayLine;
		}
	} catch (nlohmannJson::exception& e) {
		// 这种请况是此代码出现问题。如服务器返回"error":"you don't have access this resource."
		ReportJSonErrorToSystemMessage("Finnhub Forex Candle没有s项", e.what());
		return pvDayLine;
	}
	try {
		CDayLine dayLine;
		time_t tTemp;
		js2 = jsonGetChild(js, "t");
		for (auto it = js2.begin(); it != js2.end(); ++it) {
			tTemp = jsonGetLongLong(it);
			chrono::local_seconds tp{ chrono::seconds{ tTemp } };
			dayLine.SetDate(tp);
			pvDayLine->push_back(dayLine);
			dayLine.Reset();
		}
	} catch (nlohmannJson::exception& e) {
		ReportJSonErrorToSystemMessage("Finnhub Forex Candle missing 't' ", e.what());
		return pvDayLine;
	}
	try {
		int i;
		int64_t llTemp;
		double dTemp;
		js2 = jsonGetChild(js, "c");
		i = 0;
		for (auto it = js2.begin(); it != js2.end(); ++it) {
			dTemp = jsonGetDouble(it);
			pvDayLine->at(i++).SetClose(dTemp * 1000);
		}
		js2 = jsonGetChild(js, "h");
		i = 0;
		for (auto it = js2.begin(); it != js2.end(); ++it) {
			dTemp = jsonGetDouble(it);
			pvDayLine->at(i++).SetHigh(dTemp * 1000);
		}
		js2 = jsonGetChild(js, "l");
		i = 0;
		for (auto it = js2.begin(); it != js2.end(); ++it) {
			dTemp = jsonGetDouble(it);
			pvDayLine->at(i++).SetLow(dTemp * 1000);
		}
		js2 = jsonGetChild(js, "o");
		i = 0;
		for (auto it = js2.begin(); it != js2.end(); ++it) {
			dTemp = jsonGetDouble(it);
			pvDayLine->at(i++).SetOpen(dTemp * 1000);
		}
		js2 = jsonGetChild(js, "v");
		i = 0;
		for (auto it = js2.begin(); it != js2.end(); ++it) {
			llTemp = jsonGetLongLong(it);
			pvDayLine->at(i++).SetVolume(llTemp);
		}
	} catch (nlohmannJson::exception& e) {
		ReportJSonErrorToSystemMessage("Finnhub Forex Candle missing 'v' ", e.what());
		// 有些外汇交易不提供成交量，忽略就可以了
	}
	std::ranges::sort(pvDayLine->begin(), pvDayLine->end(), CompareDayLineDate);

	return pvDayLine;
}
