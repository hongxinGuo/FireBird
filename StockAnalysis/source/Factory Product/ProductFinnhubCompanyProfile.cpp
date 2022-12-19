#include "pch.h"

#include"jsonParse.h"
#include"worldMarket.h"

#include "ProductFinnhubCompanyProfile.h"

IMPLEMENT_DYNCREATE(CProductFinnhubCompanyProfile, CProductFinnhub)

CProductFinnhubCompanyProfile::CProductFinnhubCompanyProfile() {
	m_strClassName = _T("Finnhub company profile");
	m_strInquiry = _T("https://finnhub.io/api/v1/stock/profile?symbol=");
	m_lIndex = -1;
}

CString CProductFinnhubCompanyProfile::CreateMessage(void) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	CString strMessage;
	const auto pStock = dynamic_cast<CWorldMarket*>(m_pMarket)->GetStock(m_lIndex);

	m_strInquiringExchange = pStock->GetExchangeCode();
	m_strTotalInquiryMessage = m_strInquiry + pStock->GetSymbol();
	return m_strTotalInquiryMessage;
}

bool CProductFinnhubCompanyProfile::ParseAndStoreWebData(CWebDataPtr pWebData) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	const auto pStock = dynamic_cast<CWorldMarket*>(m_pMarket)->GetStock(m_lIndex);
	pStock->SetCompanyProfileUpdated(true);
	if (ParseFinnhubStockProfile2(pWebData, pStock)) {
		pStock->SetProfileUpdateDate(((CWorldMarket*)m_pMarket)->GetMarketDate());
		pStock->SetUpdateProfileDB(true);
		return true;
	}
	return false;
}

/// <summary>
/// 高级版的公司简介，需要申请付费账号
/// </summary>
/// {
///  "address": "1 Apple Park Way",
///  "city": "CUPERTINO",
///  "country": "US",
///  "currency": "USD",
///  "cusip": "",
///  "sedol":"2046251",
///  "description": "Apple Inc. is an American multinational technology company headquartered in Cupertino, California, that designs, develops, and sells consumer electronics, computer software, and online services. It is considered one of the Big Four technology companies, alongside Amazon, Google, and Microsoft. The company's hardware products include the iPhone smartphone, the iPad tablet computer, the Mac personal computer, the iPod portable media player, the Apple Watch smartwatch, the Apple TV digital media player, the AirPods wireless earbuds and the HomePod smart speaker. Apple's software includes the macOS, iOS, iPadOS, watchOS, and tvOS operating systems, the iTunes media player, the Safari web browser, the Shazam acoustic fingerprint utility, and the iLife and iWork creativity and productivity suites, as well as professional applications like Final Cut Pro, Logic Pro, and Xcode. Its online services include the iTunes Store, the iOS App Store, Mac App Store, Apple Music, Apple TV+, iMessage, and iCloud. Other services include Apple Store, Genius Bar, AppleCare, Apple Pay, Apple Pay Cash, and Apple Card.",
///  "employeeTotal": "137000",
///  "exchange": "NASDAQ/NMS (GLOBAL MARKET)",
///  "ggroup": "Technology Hardware & Equipment",
///  "gind": "Technology Hardware, Storage & Peripherals",
///  "gsector": "Information Technology",
///  "gsubind": "Technology Hardware, Storage & Peripherals",
///  "ipo": "1980-12-12",
///  "isin": "",
///  "marketCapitalization": 1415993,
///  "naics": "Communications Equipment Manufacturing",
///  "naicsNationalIndustry": "Radio and Television Broadcasting and Wireless Communications Equipment Manufacturing",
///  "naicsSector": "Manufacturing",
///  "naicsSubsector": "Computer and Electronic Product Manufacturing",
///  "name": "Apple Inc",
///  "phone": "14089961010",
///  "shareOutstanding": 4375.47998046875,
///  "state": "CALIFORNIA",
///  "ticker": "AAPL",
///  "weburl": "https://www.apple.com/",
///  "logo": "https://static.finnhub.io/logo/87cb30d8-80df-11ea-8951-00000000092a.png",
///  "finnhubIndustry":"Technology"
///  }
/// <returns></returns>
bool CProductFinnhubCompanyProfile::ParseFinnhubStockProfile(CWebDataPtr pWebData, CWorldStockPtr pStock) {
	string s;

	ASSERT(pWebData->IsJSonContentType());
	if (pWebData->IsParsed()) {
		if (pWebData->IsVoidJson()) return true; // 无数据
		if (pWebData->CheckNoRightToAccess()) return true;
		const auto ppt = pWebData->GetPTree();
		try {
			s = ppt->get<string>(_T("address"));
			pStock->SetAddress(s.c_str());
			s = ppt->get<string>(_T("city"));
			pStock->SetCity(s.c_str());
			s = ppt->get<string>(_T("country"));
			if (!s.empty()) pStock->SetCountry(s.c_str());
			s = ppt->get<string>(_T("currency"));
			if (!s.empty()) pStock->SetCurrency(s.c_str());
			s = ppt->get<string>(_T("cusip"));
			if (!s.empty()) pStock->SetCusip(s.c_str());
			s = ppt->get<string>(_T("sedol"));
			if (!s.empty()) pStock->SetSedol(s.c_str());
			s = ppt->get<string>(_T("description"));
			if (!s.empty()) pStock->SetDescription(s.c_str());
			s = ppt->get<string>(_T("exchange"));
			if (!s.empty()) pStock->SetListedExchange(s.c_str());
			s = ppt->get<string>(_T("ggroup"));
			if (!s.empty()) pStock->SetGgroup(s.c_str());
			s = ppt->get<string>(_T("gind"));
			if (!s.empty()) pStock->SetGind(s.c_str());
			s = ppt->get<string>(_T("gsector"));
			if (!s.empty()) pStock->SetGsector(s.c_str());
			s = ppt->get<string>(_T("gsubind"));
			if (!s.empty()) pStock->SetGsubind(s.c_str());
			s = ppt->get<string>(_T("ipo"));
			if (!s.empty()) pStock->SetIPODate(s.c_str());
			s = ppt->get<string>(_T("isin"));
			if (!s.empty()) pStock->SetIsin(s.c_str());
			s = ppt->get<string>(_T("marketCapitalization"));
			if (!s.empty()) pStock->SetMarketCapitalization(atof(s.c_str()));

			s = ppt->get<string>(_T("naics"));
			if (!s.empty()) pStock->SetNaics(s.c_str());
			s = ppt->get<string>(_T("naicsNationalIndustry"));
			if (!s.empty()) pStock->SetNaicsNationalIndustry(s.c_str());
			s = ppt->get<string>(_T("naicsSector"));
			if (!s.empty()) pStock->SetNaicsSector(s.c_str());
			s = ppt->get<string>(_T("naicsSubsector"));
			if (!s.empty()) pStock->SetNaicsSubsector(s.c_str());
			s = ppt->get<string>(_T("name"));
			if (!s.empty()) pStock->SetName(s.c_str());
			s = ppt->get<string>(_T("phone"));
			if (!s.empty()) pStock->SetPhone(s.c_str());

			s = ppt->get<string>(_T("shareOutstanding"));
			if (!s.empty()) pStock->SetShareOutstanding(atof(s.c_str()));
			s = ppt->get<string>(_T("state"));
			if (!s.empty()) pStock->SetState(s.c_str());
			s = ppt->get<string>(_T("ticker"));
			if (!s.empty()) pStock->SetTicker(s.c_str());
			s = ppt->get<string>(_T("weburl"));
			if (!s.empty()) pStock->SetWebURL(s.c_str());

			s = ppt->get<string>(_T("logo"));
			if (!s.empty()) pStock->SetLogo(s.c_str());
			s = ppt->get<string>(_T("finnhubIndustry"));
			if (!s.empty()) pStock->SetFinnhubIndustry(s.c_str());
		}
		catch (ptree_error& e) {
			ReportJSonErrorToSystemMessage(_T("Finnhub Stock Profile "), e);
			return false; // 没有公司简介
		}
		return true;
	}
	return false;
}

bool CProductFinnhubCompanyProfile::ParseFinnhubStockProfile2(CWebDataPtr pWebData, CWorldStockPtr pStock) {
	string s;
	double d = 0.0;

	ASSERT(pWebData->IsJSonContentType());
	if (pWebData->IsParsed()) {
		if (pWebData->IsVoidJson()) return true; // 无数据
		if (pWebData->CheckNoRightToAccess()) return true;
		const auto pjs = pWebData->GetJSon();
		try {
			s = jsonGetString(pjs, _T("address"));
			pStock->SetAddress(s.c_str());
			s = jsonGetString(pjs, _T("city"));
			pStock->SetCity(s.c_str());
			s = jsonGetString(pjs, _T("country"));
			if (!s.empty()) pStock->SetCountry(s.c_str());
			s = jsonGetString(pjs, _T("currency"));
			if (!s.empty()) pStock->SetCurrency(s.c_str());
			s = jsonGetString(pjs, _T("cusip"));
			if (!s.empty()) pStock->SetCusip(s.c_str());
			s = jsonGetString(pjs, _T("sedol"));
			if (!s.empty()) pStock->SetSedol(s.c_str());
			s = jsonGetString(pjs, _T("description"));
			if (!s.empty()) pStock->SetDescription(s.c_str());
			s = jsonGetString(pjs, _T("exchange"));
			if (!s.empty()) pStock->SetListedExchange(s.c_str());
			s = jsonGetString(pjs, _T("ggroup"));
			if (!s.empty()) pStock->SetGgroup(s.c_str());
			s = jsonGetString(pjs, _T("gind"));
			if (!s.empty()) pStock->SetGind(s.c_str());
			s = jsonGetString(pjs, _T("gsector"));
			if (!s.empty()) pStock->SetGsector(s.c_str());
			s = jsonGetString(pjs, _T("gsubind"));
			if (!s.empty()) pStock->SetGsubind(s.c_str());
			s = jsonGetString(pjs, _T("ipo"));
			if (!s.empty()) pStock->SetIPODate(s.c_str());
			s = jsonGetString(pjs, _T("isin"));
			if (!s.empty()) pStock->SetIsin(s.c_str());
			d = jsonGetDouble(pjs, _T("marketCapitalization"));
			pStock->SetMarketCapitalization(d);

			s = jsonGetString(pjs, _T("naics"));
			if (!s.empty()) pStock->SetNaics(s.c_str());
			s = jsonGetString(pjs, _T("naicsNationalIndustry"));
			if (!s.empty()) pStock->SetNaicsNationalIndustry(s.c_str());
			s = jsonGetString(pjs, _T("naicsSector"));
			if (!s.empty()) pStock->SetNaicsSector(s.c_str());
			s = jsonGetString(pjs, _T("naicsSubsector"));
			if (!s.empty()) pStock->SetNaicsSubsector(s.c_str());
			s = jsonGetString(pjs, _T("name"));
			if (!s.empty()) pStock->SetName(s.c_str());
			s = jsonGetString(pjs, _T("phone"));
			if (!s.empty()) pStock->SetPhone(s.c_str());

			d = jsonGetDouble(pjs, _T("shareOutstanding"));
			pStock->SetShareOutstanding(d);
			s = jsonGetString(pjs, _T("state"));
			if (!s.empty()) pStock->SetState(s.c_str());
			s = jsonGetString(pjs, _T("ticker"));
			if (!s.empty()) pStock->SetTicker(s.c_str());
			s = jsonGetString(pjs, _T("weburl"));
			if (!s.empty()) pStock->SetWebURL(s.c_str());

			s = jsonGetString(pjs, _T("logo"));
			if (!s.empty()) pStock->SetLogo(s.c_str());
			s = jsonGetString(pjs, _T("finnhubIndustry"));
			if (!s.empty()) pStock->SetFinnhubIndustry(s.c_str());
		}
		catch (json::exception& e) {
			ReportJSonErrorToSystemMessage(_T("Finnhub Stock Profile "), e.what());
			return false; // 没有公司简介
		}
		return true;
	}
	return false;
}
