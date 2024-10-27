#include "pch.h"

#include"jsonParse.h"
#include"JsonGetValue.h"

#include"worldMarket.h"

#include "ProductFinnhubCompanyProfileConcise.h"

CProductFinnhubCompanyProfileConcise::CProductFinnhubCompanyProfileConcise() {
	m_strInquiryFunction = _T("https://finnhub.io/api/v1/stock/profile2?symbol=");
}

CString CProductFinnhubCompanyProfileConcise::CreateMessage() {
	CString strMessage;
	const auto pStock = gl_dataContainerFinnhubStock.GetStock(m_lIndex);

	m_strInquiringExchange = pStock->GetExchangeCode();
	m_strInquiry = m_strInquiryFunction + pStock->GetSymbol();
	return m_strInquiry;
}

void CProductFinnhubCompanyProfileConcise::ParseAndStoreWebData(CWebDataPtr pWebData) {
	ASSERT(std::strcmp(typeid(*GetMarket()).name(), _T("class CWorldMarket")) == 0);
	const auto pStock = gl_dataContainerFinnhubStock.GetStock(m_lIndex);
	pStock->SetUpdateCompanyProfile(false);
	const bool fSucceed = ParseFinnhubStockProfileConcise(pWebData, pStock);
	if (fSucceed || pWebData->IsVoidJson() || IsNoRightToAccess()) {
		pStock->SetProfileUpdateDate(GetMarket()->GetMarketDate());
		pStock->SetUpdateProfileDB(true);
	}
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
/// <param name="pStock"></param>
/// <returns></returns>
bool CProductFinnhubCompanyProfileConcise::ParseFinnhubStockProfileConcise(const CWebDataPtr& pWebData, const CFinnhubStockPtr& pStock) const {
	string s;
	string sError;
	json js;

	if (!pWebData->CreateJson(js)) return false;
	if (pWebData->IsVoidJson()) return true; // 即使为空，也完成了查询。
	if (IsNoRightToAccess()) return true;

	try {
		s = jsonGetString(js, _T("ticker"));
		if (!s.empty()) pStock->SetTicker(s.c_str());
		s = jsonGetString(js, _T("country"));
		if (!s.empty()) pStock->SetCountry(s.c_str());
		s = jsonGetString(js, _T("currency"));
		if (!s.empty()) pStock->SetCurrency(s.c_str());
		s = jsonGetString(js, _T("exchange"));
		if (!s.empty()) pStock->SetExchangeCode(s.c_str());
		s = jsonGetString(js, _T("name"));
		if (!s.empty()) pStock->SetName(s.c_str());
		s = jsonGetString(js, _T("finnhubIndustry"));
		if (!s.empty()) pStock->SetFinnhubIndustry(s.c_str());
		s = jsonGetString(js, _T("logo"));
		if (!s.empty()) pStock->SetLogo(s.c_str());
		pStock->SetMarketCapitalization(jsonGetDouble(js, _T("marketCapitalization")));
		s = jsonGetString(js, _T("phone"));
		if (!s.empty()) pStock->SetPhone(s.c_str());
		pStock->SetShareOutstanding(jsonGetDouble(js, _T("shareOutstanding")));
		s = jsonGetString(js, _T("weburl"));
		if (!s.empty()) pStock->SetWebURL(s.c_str());
		s = jsonGetString(js, _T("ipo"));
		if (!s.empty()) pStock->SetIPODate(s.c_str());
	} catch (json::exception& e) {
		ReportJSonErrorToSystemMessage(_T("Finnhub Stock Profile Concise "), e.what());
		return false; // 出现错误则返回任务失败
	}
	return true;
}
