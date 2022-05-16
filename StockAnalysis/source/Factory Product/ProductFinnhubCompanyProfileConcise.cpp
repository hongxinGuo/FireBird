#include "pch.h"

#include"globedef.h"
#include"accessory.h"
#include"worldMarket.h"

#include "ProductFinnhubCompanyProfileConcise.h"

#include<boost/property_tree/ptree.hpp>
#include<boost/property_tree/json_parser.hpp>
using namespace boost::property_tree;

IMPLEMENT_DYNCREATE(CProductFinnhubCompanyProfileConcise, CProductWebSourceData)

CProductFinnhubCompanyProfileConcise::CProductFinnhubCompanyProfileConcise() {
	m_strClassName = _T("Finnhub company profile concise");
	m_strInquiringStr = _T("https://finnhub.io/api/v1/stock/profile2?symbol=");
	m_lIndex = -1;
}

CString CProductFinnhubCompanyProfileConcise::CreatMessage(void) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	CString strMessage;
	CWorldStockPtr pStock = ((CWorldMarket*)m_pMarket)->GetStock(m_lIndex);
	strMessage = m_strInquiringStr + pStock->GetSymbol();
	pStock->SetProfileUpdated(true);

	return strMessage;
}

bool CProductFinnhubCompanyProfileConcise::ProcessWebData(CWebDataPtr pWebData) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	CWorldStockPtr pStock = ((CWorldMarket*)m_pMarket)->GetStock(m_lIndex);
	if (ParseFinnhubStockProfileConcise(pWebData, pStock)) {
		pStock->SetProfileUpdateDate(((CWorldMarket*)m_pMarket)->GetMarketDate());
		pStock->SetProfileUpdated(true);
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
	shared_ptr<ptree> ppt;

	ASSERT(pWebData->IsJSonContentType());
	if (pWebData->IsSucceedParsed()) {
		if (pWebData->IsVoidJSon()) return true; // 即使为空，也完成了查询。
		ppt = pWebData->GetPTree();
		try {
			s = ppt->get<string>(_T("ticker"));
			if (s.size() > 0) pStock->SetTicker(s.c_str());
			s = ppt->get<string>(_T("country"));
			if (s.size() > 0) pStock->SetCountry(s.c_str());
			s = ppt->get<string>(_T("currency"));
			if (s.size() > 0) pStock->SetCurrency(s.c_str());
			s = ppt->get<string>(_T("exchange"));
			if (s.size() > 0) pStock->SetListedExchange(s.c_str());
			s = ppt->get<string>(_T("name"));
			if (s.size() > 0) pStock->SetName(s.c_str());
			s = ppt->get<string>(_T("finnhubIndustry"));
			if (s.size() > 0) pStock->SetFinnhubIndustry(s.c_str());
			s = ppt->get<string>(_T("logo"));
			if (s.size() > 0) pStock->SetLogo(s.c_str());
			s = ppt->get<string>(_T("marketCapitalization"));
			if (s.size() > 0) pStock->SetMarketCapitalization(atof(s.c_str()));
			s = ppt->get<string>(_T("phone"));
			if (s.size() > 0) pStock->SetPhone(s.c_str());
			if (s.size() > 0) pStock->SetShareOutstanding(ptreeGetDouble(*ppt, _T("shareOutstanding")));
			s = ppt->get<string>(_T("weburl"));
			if (s.size() > 0) pStock->SetWebURL(s.c_str());
			s = ppt->get<string>(_T("ipo"));
			if (s.size() > 0) pStock->SetIPODate(s.c_str());
		}
		catch (ptree_error& e) {
			ReportJSonErrorToSystemMessage(_T("Finnhub Stock Profile Concise "), e);
			return false; // 出现错误则返回任务失败
		}
		return true;
	}
	return false;
}