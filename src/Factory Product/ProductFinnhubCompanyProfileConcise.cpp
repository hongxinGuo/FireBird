#include "pch.h"

#include"jsonParse.h"
#include"nlohmannJsonGetValue.h"

#include"worldMarket.h"
#include "ContainerFinnhubStock.h"

#include "ProductFinnhubCompanyProfileConcise.h"

#include "ContainerTiingoStock.h"
#include "FinnhubDataSource.h"
#include "TiingoStock.h"
#include"FinnhubStock.h"
#include "SystemMessage.h"

#include"cpr/cpr.h"

CProductFinnhubCompanyProfileConcise::CProductFinnhubCompanyProfileConcise() {
	m_strInquiryFunction = "https://finnhub.io/api/v1/stock/profile2?symbol=";
}
void CProductFinnhubCompanyProfileConcise::InquireData(const std::stop_token& st, const string& strHeaders, const string& strParams, const string& strSuffix, const string& strInquiryToken) {
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
		pStock->SetUpdateCompanyProfile(false);
		const bool fSucceed = Parse(r.text, pStock);
		if (fSucceed) {
			pStock->SetShareOutstanding(pStock->GetShareOutstanding());
			if (gl_dataContainerTiingoStock.IsSymbol(pStock->GetSymbol())) { // 同时更新tiingo的股本数据
				CTiingoStockPtr pTiingoStock = gl_dataContainerTiingoStock.GetStock(pStock->GetSymbol());
				pTiingoStock->SetShareOutstanding(pStock->GetShareOutstanding()); // finnhub的单位是百万股
				pTiingoStock->SetMarketCapitalization(pStock->GetMarketCapitalization()); // 单位为百万元。
				pTiingoStock->SetUpdateProfileDB(true);
			}
		}
		if (fSucceed) {
			pStock->SetProfileUpdateDate(gl_pWorldMarket->GetMarketDate());
			pStock->SetUpdateProfileDB(true);
		}
	}
}
void CProductFinnhubCompanyProfileConcise::WebStatusCheck(cpr::Response& r) {
	string s;
	switch (r.status_code) {
	case 0:
		break;
	case 302: //redirected, not an error
	case 403: // forbidden
		s = std::format("Finnhub company profile concise http error {}. code:{} message:{}", r.status_code, static_cast<int>(r.error.code), r.error.message);
		gl_systemMessage.PushInnerSystemInformationMessage(s);
		break;
	default:
		s = std::format("Finnhub company profile concise http error {}. code:{} message: {}", r.status_code, static_cast<int>(r.error.code), r.error.message);
		gl_systemMessage.PushInnerSystemInformationMessage(s);
		break;
	}
}

shared_ptr<vector<string>> CProductFinnhubCompanyProfileConcise::CreateMessage() {
	const auto pStock = gl_dataContainerFinnhubStock.GetItem(m_index);

	m_strInquiringExchange = pStock->GetExchange();
	m_inquiryString = m_strInquiryFunction + pStock->GetSymbol();
	shared_ptr<vector<string>> pInquiry = make_shared<vector<string>>();
	pInquiry->push_back(m_inquiryString);
	return pInquiry;
}

/// <summary>
/// 简版的公司简介，格式如下：
/// "country": "US",
/// "currency": "USD",
/// "exchange" : "NASDAQ/NMS (GLOBAL MARKET)",
/// "ipo" : "1980-12-12",
/// "marketCapitalization" : 1415993,
/// "name" : "Apple Inc",
/// "phone" : "14089961010",
/// "shareOutstanding" : 4375.47998046875,
/// "ticker" : "AAPL",
/// "weburl" : "https://www.apple.com/",
/// "logo" : "https://static.finnhub.io/logo/87cb30d8-80df-11ea-8951-00000000092a.png",
/// "finnhubIndustry" : "Technology"
///
/// </summary>
/// <param name="pWebData"></param>
/// <param name="text"></param>
/// <param name="pStock"></param>
/// <returns></returns>
/// 
/// 
bool CProductFinnhubCompanyProfileConcise::Parse(const string& text, const CFinnhubStockPtr& pStock) const {
	string sError;
	nlohmannJson js;

	if (text.empty()) return true;
	if (!::CreateJsonWithNlohmann(js, text)) return false;
	if (::IsVoidJson(text)) return true; // 即使为空，也完成了查询。
	if (IsNoRightToAccess()) return true;

	try {
		string s = jsonGetString(js, "ticker");
		if (!s.empty()) pStock->SetTicker(s);
		s = jsonGetString(js, "country");
		if (!s.empty()) pStock->SetCountry(s);
		s = jsonGetString(js, "currency");
		if (!s.empty()) pStock->SetCurrency(s);
		s = jsonGetString(js, "exchange");
		//if (!s.empty()) pStock->SetExchangeCode(s); // 这里是交易所的全称，不使用此项数据。
		s = jsonGetString(js, "name");
		if (!s.empty()) pStock->SetName(s);
		s = jsonGetString(js, "finnhubIndustry");
		if (!s.empty()) pStock->SetFinnhubIndustry(s);
		s = jsonGetString(js, "logo");
		if (!s.empty()) pStock->SetLogo(s);
		pStock->SetMarketCapitalization(jsonGetDouble(js, "marketCapitalization"));
		s = jsonGetString(js, "phone");
		if (!s.empty()) pStock->SetPhone(s);
		pStock->SetShareOutstanding(jsonGetDouble(js, "shareOutstanding"));
		s = jsonGetString(js, "weburl");
		if (!s.empty()) pStock->SetWebURL(s);
		s = jsonGetString(js, "ipo");
		if (!s.empty()) pStock->SetIPODate(s);
	} catch (nlohmannJson::exception& e) {
		ReportJSonErrorToSystemMessage("Finnhub Stock Profile Concise ", e.what());
		return false; // 出现错误则返回任务失败
	}
	return true;
}

void CProductFinnhubCompanyProfileConcise::UpdateSystemStatus() {
}
