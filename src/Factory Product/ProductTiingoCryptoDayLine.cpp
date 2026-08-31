#include "pch.h"

#include"jsonParse.h"
#include"nlohmannJsonGetValue.h"
#include "ProductTiingoCryptoDayLine.h"

#include "ContainerFinnhubCrypto.h"
#include "ContainerTiingoCryptoSymbol.h"
#include "SystemMessage.h"
#include "FinnhubCrypto.h"
#include "TiingoCrypto.h"
#include"DayLine.h"
#include "log.h"
#include "TiingoDataSource.h"

#include"cpr/cpr.h"

using namespace std;

CProductTiingoCryptoDayLine::CProductTiingoCryptoDayLine() {
	m_strInquiryFunction = "https://api.tiingo.com/tiingo/crypto/price?";
}

void CProductTiingoCryptoDayLine::InquireData(const std::stop_token& st) {
	auto inquireStrings = CreateMessage();
	for (const auto& inquiry : *inquireStrings) {
		if (st.stop_requested()) break;
		string s = inquiry + "&token=" + gl_pTiingoDataSource->GetToken();
		m_r = cpr::Get(cpr::Url{ s });

		if (m_r.status_code != 200) {
			WebStatusCheck(m_r);
			return;
		}
		const auto pCrypto = gl_dataFinnhubCryptoSymbol.GetItem(m_index);
		const CDayLinesPtr pvDayLine = Parse(m_r.text);
		pCrypto->SetUpdateDayLine(false);
	}
}

void CProductTiingoCryptoDayLine::WebStatusCheck(cpr::Response& r) {
	switch (r.status_code) {
	case 0:
		break;
	case 302: //redirected, not an error
		break;
	case 401:
	case 403: // forbidden
		m_iReceivedDataStatus = NO_ACCESS_RIGHT_;
		CheckInaccessible();
		break;
	default:
		WebErrorReport();
		break;
	}
}

void CProductTiingoCryptoDayLine::UpdateSystemStatus() {
}

///////////////////////////////////////////////////////////////////////////////////////////
///
/// 即使是免费账户，tiingo日线也能够提供30年以上的数据，故而至少申请一次全部数据（自19800101开始）。
///	此后为了减少数据流量，可以只申请未下载的数据。
///	Finnhub的免费日线只提供一年的。本系统最初的执行时间为2019年，即finnhub没有2018年以前的日线。
///
///////////////////////////////////////////////////////////////////////////////////////////
shared_ptr<vector<string>> CProductTiingoCryptoDayLine::CreateMessage() {
	const auto pCrypto = gl_dataContainerTiingoCryptoSymbol.GetCrypto(GetIndex());
	string strParam = pCrypto->m_strName + "&startDate=2010-01-02&resampleFreq=1day"; // 永远申请完整日线
	pCrypto->SetUpdateDayLine(false);
	m_strInquiringSymbol = pCrypto->GetSymbol();

	m_inquiryString = m_strInquiryFunction + strParam;
	shared_ptr<vector<string>> pInquiry = make_shared<vector<string>>();
	pInquiry->push_back(m_inquiryString);
	return pInquiry;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Tiingo日线格式：
// [{
//	"date":"2019-01-02T00:00:00.000Z",
//		"close" : 157.92,
//		"high" : 158.85,
//		"low" : 154.23,
//		"open" : 154.89,
//		"volume" : 37039737,
//		"adjClose" : 157.92,
//		"adjHigh" : 158.85,
//		"adjLow" : 154.23,
//		"adjOpen" : 154.89,
//		"adjVolume" : 37039737,
//		"divCash" : 0.0,
//		"splitFactor" : 1.0
// },
//	{
//		"date":"2019-01-03T00:00:00.000Z",
//		"close" : 142.19,
//		"high" : 145.72,
//		"low" : 142.0,
//		"open" : 143.98,
//		"volume" : 91312195,
//		"adjClose" : 142.19,
//		"adjHigh" : 145.72,
//		"adjLow" : 142.0,
//		"adjOpen" : 143.98,
//		"adjVolume" : 91312195,
//		"divCash" : 0.0,
//		"splitFactor" : 1.0
//	}
// ]
//
// 如果没有股票600600.SS日线数据，则返回：{"detail":"Error:Ticker '600600.SS' not found"}
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CDayLinesPtr CProductTiingoCryptoDayLine::Parse(const string& text) {
	auto pvDayLine = make_shared<vector<CDayLine>>();
	pvDayLine->reserve(3000); // 预留空间，避免频繁扩容。

	string s;
	nlohmannJson js;

	if (text.empty()) return pvDayLine;
	if (!::CreateJsonWithNlohmann(js, text)) return pvDayLine;
	if (::IsVoidJson(text)) return pvDayLine;

	try {
		s = js.at("detail"); // 是否有报错信息
		string strMessage = "Tiingo crypto dayLine ";
		strMessage += s;
		gl_systemMessage.PushErrorMessage(strMessage); // 报告错误信息
		return pvDayLine;
	} catch (nlohmannJson::exception&) {
		// 正确， do nothing，继续执行
	}
	try {
		for (auto it = js.begin(); it != js.end(); ++it) {
			CDayLine dayLine;
			s = jsonGetString(it, "date");
			istringstream ss1(s);
			chrono::local_time<chrono::milliseconds> lms;
			ss1 >> chrono::parse("%FT%T%Z", lms);
			dayLine.SetDate(floor<chrono::days>(lms));
			double dTemp = jsonGetDouble(it, "close");
			dayLine.SetClose(dTemp * 1000);
			dTemp = jsonGetDouble(it, "high");
			dayLine.SetHigh(dTemp * 1000);
			dTemp = jsonGetDouble(it, "low");
			dayLine.SetLow(dTemp * 1000);
			dTemp = jsonGetDouble(it, "open");
			dayLine.SetOpen(dTemp * 1000);
			long lTemp = jsonGetLong(it, "volume");
			dayLine.SetVolume(lTemp);
			pvDayLine->push_back(dayLine);
		}
	} catch (nlohmannJson::exception& e) {
		string str3 = text.substr(0, 120);
		ReportJSonErrorToSystemMessage("Tiingo Crypto DayLine " + str3, e.what());
		return pvDayLine; // 数据解析出错的话，则放弃。
	}
	std::ranges::sort(pvDayLine->begin(), pvDayLine->end(), CompareDayLineDate); // 以日期早晚顺序排列。

	return pvDayLine;
}
