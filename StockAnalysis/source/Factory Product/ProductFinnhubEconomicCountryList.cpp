#include "pch.h"

#include"jsonParse.h"
#include"WorldMarket.h"
#include"Country.h"
#include"CallableFunction.h"

#include "ProductFinnhubEconomicCountryList.h"

IMPLEMENT_DYNCREATE(CProductFinnhubEconomicCountryList, CProductFinnhub)

CProductFinnhubEconomicCountryList::CProductFinnhubEconomicCountryList() {
	m_strClassName = _T("Finnhub economic country list");
	m_strInquiry = _T("https://finnhub.io/api/v1/country?");
	m_lIndex = -1;
}

CString CProductFinnhubEconomicCountryList::CreateMessage(void) {
	m_strTotalInquiryMessage = m_strInquiry;
	return m_strTotalInquiryMessage;
}

bool CProductFinnhubEconomicCountryList::ParseAndStoreWebData(CWebDataPtr pWebData) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	const auto pvCountry = ParseFinnhubCountryList2(pWebData);
	for (const auto& pCountry : *pvCountry) {
		if (!dynamic_cast<CWorldMarket*>(m_pMarket)->IsCountry(pCountry)) {
			dynamic_cast<CWorldMarket*>(m_pMarket)->AddCountry(pCountry);
		}
	}

	return true;
}

CCountryVectorPtr CProductFinnhubEconomicCountryList::ParseFinnhubCountryList(CWebDataPtr pWebData) {
	auto pvCountry = make_shared<vector<CCountryPtr>>();
	CCountryPtr pCountry = nullptr;
	string s;

	ASSERT(pWebData->IsJSonContentType());
	if (!pWebData->IsParsed()) return pvCountry;
	if (pWebData->IsVoidJson()) {
		m_iReceivedDataStatus = _VOID_DATA_;
		return pvCountry;
	}
	if (pWebData->CheckNoRightToAccess()) {
		m_iReceivedDataStatus = _NO_ACCESS_RIGHT_;
		return pvCountry;
	}
	const auto ppt = pWebData->GetPTree();
	try {
		for (ptree::iterator it = ppt->begin(); it != ppt->end(); ++it) {
			pCountry = make_shared<CCountry>();
			ptree pt2 = it->second;
			s = pt2.get<string>(_T("code2"));
			if (!s.empty()) pCountry->m_strCode2 = s.c_str();
			s = pt2.get<string>(_T("code3"));
			pCountry->m_strCode3 = s.c_str();
			s = pt2.get<string>(_T("codeNo"));
			pCountry->m_strCodeNo = s.c_str();
			s = pt2.get<string>(_T("country"));
			pCountry->m_strCountry = s.c_str();
			s = pt2.get<string>(_T("currency"));
			pCountry->m_strCurrency = s.c_str();
			s = pt2.get<string>(_T("currencyCode"));
			pCountry->m_strCurrencyCode = s.c_str();
			pvCountry->push_back(pCountry);
		}
	}
	catch (ptree_error& e) {
		ReportJSonErrorToSystemMessage(_T("Finnhub Country List "), e);
		return pvCountry;
	}
	ranges::sort(pvCountry->begin(), pvCountry->end(), CompareCountryList);
	return pvCountry;
}

CCountryVectorPtr CProductFinnhubEconomicCountryList::ParseFinnhubCountryList2(CWebDataPtr pWebData) {
	auto pvCountry = make_shared<vector<CCountryPtr>>();
	CCountryPtr pCountry = nullptr;
	string s;

	ASSERT(pWebData->IsJSonContentType());
	if (!pWebData->IsParsed()) return pvCountry;
	if (pWebData->IsVoidJson()) {
		m_iReceivedDataStatus = _VOID_DATA_;
		return pvCountry;
	}
	if (pWebData->CheckNoRightToAccess()) {
		m_iReceivedDataStatus = _NO_ACCESS_RIGHT_;
		return pvCountry;
	}
	const auto pjs = pWebData->GetJSon();
	try {
		for (auto it = pjs->begin(); it != pjs->end(); ++it) {
			pCountry = make_shared<CCountry>();
			s = jsonGetString(it, _T("code2"));
			if (!s.empty()) pCountry->m_strCode2 = s.c_str();
			s = jsonGetString(it,_T("code3"));
			pCountry->m_strCode3 = s.c_str();
			s = jsonGetString(it,_T("codeNo"));
			pCountry->m_strCodeNo = s.c_str();
			s = jsonGetString(it,_T("country"));
			pCountry->m_strCountry = s.c_str();
			s = jsonGetString(it,_T("currency"));
			pCountry->m_strCurrency = s.c_str();
			s = jsonGetString(it,_T("currencyCode"));
			pCountry->m_strCurrencyCode = s.c_str();
			pvCountry->push_back(pCountry);
		}
	}
	catch (json::exception& e) {
		ReportJSonErrorToSystemMessage(_T("Finnhub Country List "), e.what());
		return pvCountry;
	}
	ranges::sort(pvCountry->begin(), pvCountry->end(), CompareCountryList);
	return pvCountry;
}
