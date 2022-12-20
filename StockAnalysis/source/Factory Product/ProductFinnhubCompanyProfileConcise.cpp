#include "pch.h"

#include"jsonParse.h"
#include"worldMarket.h"

#include "ProductFinnhubCompanyProfileConcise.h"

IMPLEMENT_DYNCREATE(CProductFinnhubCompanyProfileConcise, CProductFinnhub)

CProductFinnhubCompanyProfileConcise::CProductFinnhubCompanyProfileConcise() {
	m_strClassName = _T("Finnhub company profile concise");
	m_strInquiry = _T("https://finnhub.io/api/v1/stock/profile2?symbol=");
	m_lIndex = -1;
}

CString CProductFinnhubCompanyProfileConcise::CreateMessage(void) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	CString strMessage;
	const auto pStock = dynamic_cast<CWorldMarket*>(m_pMarket)->GetStock(m_lIndex);

	m_strInquiringExchange = pStock->GetExchangeCode();
	m_strTotalInquiryMessage = m_strInquiry + pStock->GetSymbol();
	return m_strTotalInquiryMessage;
}

bool CProductFinnhubCompanyProfileConcise::ParseAndStoreWebData(CWebDataPtr pWebData) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	const auto pStock = dynamic_cast<CWorldMarket*>(m_pMarket)->GetStock(m_lIndex);
	pStock->SetCompanyProfileUpdated(true);
	if (ParseFinnhubStockProfileConcise(pWebData, pStock)) {
		pStock->SetProfileUpdateDate(((CWorldMarket*)m_pMarket)->GetMarketDate());
		pStock->SetUpdateProfileDB(true);
		return true;
	}
	return false;
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
bool CProductFinnhubCompanyProfileConcise::ParseFinnhubStockProfileConcise(CWebDataPtr pWebData, CWorldStockPtr pStock) {
	string s;
	string sError;

	ASSERT(pWebData->IsJSonContentType());
	if (!pWebData->IsParsed()) return false;
	if (pWebData->IsVoidJson()) return true; // 即使为空，也完成了查询。
	if (pWebData->CheckNoRightToAccess()) return true;

	const auto pjs = pWebData->GetJSon();
	try {
		s = jsonGetString(pjs, _T("ticker"));
		if (!s.empty()) pStock->SetTicker(s.c_str());
		s = jsonGetString(pjs, _T("country"));
		if (!s.empty()) pStock->SetCountry(s.c_str());
		s = jsonGetString(pjs, _T("currency"));
		if (!s.empty()) pStock->SetCurrency(s.c_str());
		s = jsonGetString(pjs, _T("exchange"));
		if (!s.empty()) pStock->SetListedExchange(s.c_str());
		s = jsonGetString(pjs, _T("name"));
		if (!s.empty()) pStock->SetName(s.c_str());
		s = jsonGetString(pjs, _T("finnhubIndustry"));
		if (!s.empty()) pStock->SetFinnhubIndustry(s.c_str());
		s = jsonGetString(pjs, _T("logo"));
		if (!s.empty()) pStock->SetLogo(s.c_str());
		pStock->SetMarketCapitalization(jsonGetDouble(pjs, _T("marketCapitalization")));
		s = jsonGetString(pjs, _T("phone"));
		if (!s.empty()) pStock->SetPhone(s.c_str());
		pStock->SetShareOutstanding(jsonGetDouble(pjs, _T("shareOutstanding")));
		s = jsonGetString(pjs, _T("weburl"));
		if (!s.empty()) pStock->SetWebURL(s.c_str());
		s = jsonGetString(pjs, _T("ipo"));
		if (!s.empty()) pStock->SetIPODate(s.c_str());
	}
	catch (json::exception& e) {
		ReportJSonErrorToSystemMessage(_T("Finnhub Stock Profile Concise "), e.what());
		return false; // 出现错误则返回任务失败
	}
	return true;
}
