#include "pch.h"

#include"globedef.h"
#include"accessory.h"
#include"worldMarket.h"

#include "ProductFinnhubCompanyProfile.h"

#include<boost/property_tree/ptree.hpp>
#include<boost/property_tree/json_parser.hpp>
using namespace boost::property_tree;

IMPLEMENT_DYNCREATE(CProductFinnhubCompanyProfile, CProductWebSourceData)

CProductFinnhubCompanyProfile::CProductFinnhubCompanyProfile() {
	m_strClassName = _T("Finnhub company profile");
	m_strInquiringStr = _T("https://finnhub.io/api/v1/stock/profile?symbol=");
	m_lIndex = -1;
}

CString CProductFinnhubCompanyProfile::CreatMessage(void) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	CString strMessage;
	CWorldStockPtr pStock = ((CWorldMarket*)m_pMarket)->GetStock(m_lIndex);
	strMessage = m_strInquiringStr + pStock->GetSymbol();
	pStock->SetProfileUpdated(true);

	return strMessage;
}

bool CProductFinnhubCompanyProfile::ProcessWebData(CWebDataPtr pWebData) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	CWorldStockPtr pStock = ((CWorldMarket*)m_pMarket)->GetStock(m_lIndex);
	if (ParseFinnhubStockProfile(pWebData, pStock)) {
		pStock->SetProfileUpdateDate(((CWorldMarket*)m_pMarket)->GetMarketDate());
		pStock->SetProfileUpdated(true);
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
	ptree pt;
	string s;

	if (pWebData->GetBufferLength() < 20) {
		return true; // 没有公司简介
	}
	if (!pWebData->CreatePTree(pt)) return false;
	try {
		s = pt.get<string>(_T("address"));
		pStock->SetAddress(s.c_str());
		s = pt.get<string>(_T("city"));
		pStock->SetCity(s.c_str());
		s = pt.get<string>(_T("country"));
		if (s.size() > 0) pStock->SetCountry(s.c_str());
		s = pt.get<string>(_T("currency"));
		if (s.size() > 0) pStock->SetCurrency(s.c_str());
		s = pt.get<string>(_T("cusip"));
		if (s.size() > 0) pStock->SetCusip(s.c_str());
		s = pt.get<string>(_T("sedol"));
		if (s.size() > 0) pStock->SetSedol(s.c_str());
		s = pt.get<string>(_T("description"));
		if (s.size() > 0) pStock->SetDescription(s.c_str());
		s = pt.get<string>(_T("exchange"));
		if (s.size() > 0) pStock->SetListedExchange(s.c_str());
		s = pt.get<string>(_T("ggroup"));
		if (s.size() > 0) pStock->SetGgroup(s.c_str());
		s = pt.get<string>(_T("gind"));
		if (s.size() > 0) pStock->SetGind(s.c_str());
		s = pt.get<string>(_T("gsector"));
		if (s.size() > 0) pStock->SetGsector(s.c_str());
		s = pt.get<string>(_T("gsubind"));
		if (s.size() > 0) pStock->SetGsubind(s.c_str());
		s = pt.get<string>(_T("ipo"));
		if (s.size() > 0) pStock->SetIPODate(s.c_str());
		s = pt.get<string>(_T("isin"));
		if (s.size() > 0) pStock->SetIsin(s.c_str());
		s = pt.get<string>(_T("marketCapitalization"));
		if (s.size() > 0) pStock->SetMarketCapitalization(atof(s.c_str()));

		s = pt.get<string>(_T("naics"));
		if (s.size() > 0) pStock->SetNaics(s.c_str());
		s = pt.get<string>(_T("naicsNationalIndustry"));
		if (s.size() > 0) pStock->SetNaicsNationalIndustry(s.c_str());
		s = pt.get<string>(_T("naicsSector"));
		if (s.size() > 0) pStock->SetNaicsSector(s.c_str());
		s = pt.get<string>(_T("naicsSubsector"));
		if (s.size() > 0) pStock->SetNaicsSubsector(s.c_str());
		s = pt.get<string>(_T("name"));
		if (s.size() > 0) pStock->SetName(s.c_str());
		s = pt.get<string>(_T("phone"));
		if (s.size() > 0) pStock->SetPhone(s.c_str());

		s = pt.get<string>(_T("shareOutstanding"));
		if (s.size() > 0) pStock->SetShareOutstanding(atof(s.c_str()));
		s = pt.get<string>(_T("state"));
		if (s.size() > 0) pStock->SetState(s.c_str());
		s = pt.get<string>(_T("ticker"));
		if (s.size() > 0) pStock->SetTicker(s.c_str());
		s = pt.get<string>(_T("weburl"));
		if (s.size() > 0) pStock->SetWebURL(s.c_str());

		s = pt.get<string>(_T("logo"));
		if (s.size() > 0) pStock->SetLogo(s.c_str());
		s = pt.get<string>(_T("finnhubIndustry"));
		if (s.size() > 0) pStock->SetFinnhubIndustry(s.c_str());
		return true;
	}
	catch (ptree_error& e) {
		ReportJSonErrorToSystemMessage(_T("Finnhub Stock Profile "), e);
		return false; // 没有公司简介
	}
}