#include "pch.h"

#include"jsonParse.h"
#include"nlohmannJsonGetValue.h"

#include"FinnhubStock.h"
#include "ContainerFinnhubStock.h"

#include "ProductFinnhubStockDayLine.h"

#include "SystemMessage.h"
#include "FinnhubDataSource.h"
#include"cpr/cpr.h"
#include"DayLine.h"

using namespace std;

CProductFinnhubStockDayLine::CProductFinnhubStockDayLine() {
	m_strInquiryFunction = "https://finnhub.io/api/v1/stock/candle?symbol=";
}

void CProductFinnhubStockDayLine::InquireData(const std::stop_token& st) {
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

		const auto pStock = gl_dataContainerFinnhubStock.GetItem(m_index);
		const auto pvDayLine = Parse(r.text);
		pStock->SetUpdateDayLine(false);
		long lastClose = 0;
		for (auto& dayLine : *pvDayLine) {
			dayLine.SetExchange(pStock->GetExchange());
			dayLine.SetStockSymbol(pStock->GetSymbol());
			if ((lastClose != 0) && (dayLine.GetLastClose() == 0)) dayLine.SetLastClose(lastClose);
			lastClose = dayLine.GetClose();
		}
		if (!pvDayLine->empty()) {
			pStock->UpdateDayLine(pvDayLine);
			if (pStock->GetDayLineSize() > 0) {// 添加了新数据
				pStock->SetUpdateDayLineDB(true);
				pStock->SetUpdateProfileDB(true);
				const auto lSize = pStock->GetDayLineSize() - 1;
			}
		}
	}
}

void CProductFinnhubStockDayLine::WebStatusCheck(cpr::Response& r) {
	switch (r.status_code) {
	case 0: //
		// do nothing
		break;
	case 401: // no right to access
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

void CProductFinnhubStockDayLine::UpdateSystemStatus() {
}

shared_ptr<vector<string>> CProductFinnhubStockDayLine::CreateMessage() {
	const auto pStock = gl_dataContainerFinnhubStock.GetItem(m_index);
	const auto strParam = pStock->GetFinnhubDayLineInquiryParam(GetUTCTime());

	m_strInquiringExchange = pStock->GetExchange();
	m_inquiryString = m_strInquiryFunction + strParam;
	shared_ptr<vector<string>> pInquiryStrings = make_shared<vector<string>>();
	pInquiryStrings->push_back(m_inquiryString);
	return pInquiryStrings;
}

CDayLinesPtr CProductFinnhubStockDayLine::Parse(const string& text) {
	auto pvDayLine = make_shared<vector<CDayLine>>();
	pvDayLine->reserve(3000); // 预先分配空间，避免频繁扩容。一般来说，日线数据不会超过1000条。

	nlohmannJson js2;
	CDayLine pDayLine;
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
		ReportJSonErrorToSystemMessage("Finnhub Stock Candle missing 's' " + GetInquiryFunction(), e.what());
		return pvDayLine;
	}

	try {
		time_t tTemp;
		js2 = jsonGetChild(js, "t");
		for (auto it = js2.begin(); it != js2.end(); ++it) {
			tTemp = jsonGetLongLong(it);
			chrono::local_seconds localTime{ chrono::seconds{ tTemp } };
			pDayLine.SetDate(localTime);
			pvDayLine->push_back(pDayLine);
		}
	} catch (nlohmannJson::exception& e) {
		ReportJSonErrorToSystemMessage("Finnhub Stock Candle missing 't' " + GetInquiryFunction(), e.what());
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
		js2 = jsonGetChild(js, "o");
		i = 0;
		for (auto it = js2.begin(); it != js2.end(); ++it) {
			dTemp = jsonGetDouble(it);
			pvDayLine->at(i++).SetOpen(dTemp * 1000);
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
		js2 = jsonGetChild(js, "v");
		i = 0;
		for (auto it = js2.begin(); it != js2.end(); ++it) {
			llTemp = jsonGetLongLong(it);
			pvDayLine->at(i++).SetVolume(llTemp);
		}
	} catch (nlohmannJson::exception& e) { ReportJSonErrorToSystemMessage("Finnhub Stock Candle Error#3 " + GetInquiryFunction(), e.what()); }
	std::ranges::sort(pvDayLine->begin(), pvDayLine->end(), CompareDayLineDate); // 以日期早晚顺序排列。

	return pvDayLine;
}
