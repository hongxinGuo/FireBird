#include "pch.h"

#include"jsonParse.h"
#include"StockCodeConverter.h"
#include"worldMarket.h"

#include "ProductFinnhubCompanyProfileConcise.h"

IMPLEMENT_DYNCREATE(CProductFinnhubCompanyProfileConcise, CProductFinnhub)

CProductFinnhubCompanyProfileConcise::CProductFinnhubCompanyProfileConcise() {
	m_strClassName = _T("Finnhub company profile concise");
	m_strInquiry = _T("https://finnhub.io/api/v1/stock/profile2?symbol=");
	m_lIndex = -1;
}

CString CProductFinnhubCompanyProfileConcise::CreatMessage(void) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	CString strMessage;
	CWorldStockPtr pStock = ((CWorldMarket*)m_pMarket)->GetStock(m_lIndex);

	m_strInquiringExchange = pStock->GetExchangeCode();
	m_strTotalInquiryMessage = m_strInquiry + pStock->GetSymbol();
	return m_strTotalInquiryMessage;
}

bool CProductFinnhubCompanyProfileConcise::ParseAndStoreWebData(CWebDataPtr pWebData, CVirtualDataSource* pDataSource) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	CWorldStockPtr pStock = ((CWorldMarket*)m_pMarket)->GetStock(m_lIndex);
	pStock->SetCompanyProfileUpdated(true);
	if (ParseFinnhubStockProfileConcise(pWebData, pStock)) {
		pStock->SetProfileUpdateDate(((CWorldMarket*)m_pMarket)->GetMarketDate());
		pStock->SetUpdateProfileDB(true);
		return true;
	}
	return false;
}

/// <summary>
/// ���Ĺ�˾��飬��ʽ���£�
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
	if (!pWebData->IsParsed()) return false;
	if (pWebData->IsVoidJson()) return true; // ��ʹΪ�գ�Ҳ����˲�ѯ��
	if (pWebData->CheckNoRightToAccess()) return true;

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
		return false; // ���ִ����򷵻�����ʧ��
	}
	return true;
}