#include "pch.h"

#include"jsonParse.h"
#include"nlohmannJsonGetValue.h"

#include"FinnhubStock.h"

#include "ProductFinnhubStockPriceQuote.h"

#include "ContainerFinnhubStock.h"
#include "FinnhubDataSource.h"
#include "SystemMessage.h"
#include"cpr/cpr.h"

CProductFinnhubStockPriceQuote::CProductFinnhubStockPriceQuote() {
	m_strInquiryFunction = "https://finnhub.io/api/v1/quote?symbol=";
}

void CProductFinnhubStockPriceQuote::InquireData(const std::stop_token& st) {
	auto inquireStrings = CreateMessage();
	for (const auto& inquiry : *inquireStrings) {
		if (st.stop_requested()) break;
		string inquireString = inquiry + "&token=" + gl_pFinnhubDataSource->GetToken();
		m_r = cpr::Get(cpr::Url{ inquireString });

		if (m_r.status_code != 200) {
			WebStatusCheck(m_r);
			return;
		}

		const auto pStock = gl_dataContainerFinnhubStock.GetItem(m_index);
		if (Parse(m_r.text, pStock)) {
			if ((pStock->GetTransactionTime() + 3600 * 12 - GetUTCTime()) > 0) {
				// 交易时间不早于12小时，则设置此股票为活跃股票
				pStock->SetActive(true);
			}
			return;
		}
	}
}

void CProductFinnhubStockPriceQuote::WebStatusCheck(cpr::Response& r) {
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
void CProductFinnhubStockPriceQuote::UpdateSystemStatus() {
}

shared_ptr<vector<string>> CProductFinnhubStockPriceQuote::CreateMessage() {
	const auto pStock = gl_dataContainerFinnhubStock.GetItem(m_index);
	const auto strParam = pStock->GetSymbol();

	m_strInquiringExchange = pStock->GetExchange();
	m_inquiryString = m_strInquiryFunction + strParam;
	shared_ptr<vector<string>> pInquiry = make_shared<vector<string>>();
	pInquiry->push_back(m_inquiryString);
	return pInquiry;
}

bool CProductFinnhubStockPriceQuote::Parse(const string& text, const CFinnhubStockPtr& pStock) {
	nlohmannJson js;

	if (!::CreateJsonWithNlohmann(js, text)) return false;
	if (::IsVoidJson(text)) return false;// 无数据
	if (IsNoRightToAccess()) return false; // 无权访问

	try {
		double dTemp = jsonGetDouble(js, "c");
		pStock->SetNew(dTemp * 1000);
		dTemp = jsonGetDouble(js, "h");
		pStock->SetHigh(dTemp * 1000);
		dTemp = jsonGetDouble(js, "l");
		pStock->SetLow(dTemp * 1000);
		dTemp = jsonGetDouble(js, "o");
		pStock->SetOpen(dTemp * 1000);
		dTemp = jsonGetDouble(js, "pc");
		pStock->SetLastClose(dTemp * 1000);
		const auto tt = jsonGetLongLong(js, "t");
		pStock->SetTransactionTime(tt);
	} catch (nlohmannJson::exception& e) {
		// 数据格式不对，跳过。
		ReportJSonErrorToSystemMessage("Finnhub Stock Quote ", e.what());
		return false;
	}
	return true;
}
