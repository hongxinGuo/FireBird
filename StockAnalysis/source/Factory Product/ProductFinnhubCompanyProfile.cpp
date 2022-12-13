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
	if (ParseFinnhubStockProfile(pWebData, pStock)) {
		pStock->SetProfileUpdateDate(((CWorldMarket*)m_pMarket)->GetMarketDate());
		pStock->SetUpdateProfileDB(true);
		return true;
	}
	return false;
}

/// <summary>
/// 高级版的公司简介，需要申请付费账号
/// </summary>
/// <param name="pWebData"></param>
/// <param name="pStock"></param>
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
