#include "pch.h"

#include"TimeConvert.h"
#include"jsonParse.h"
#include"StockCodeConverter.h"
#include"worldMarket.h"

#include "ProductFinnhubCompanyNews.h"

IMPLEMENT_DYNCREATE(CProductFinnhubCompanyNews, CProductFinnhub)

CProductFinnhubCompanyNews::CProductFinnhubCompanyNews() {
	m_strClassName = _T("Finnhub company news");
	m_strInquiringStr = _T("https://finnhub.io/api/v1/company-news?symbol=");
	m_lIndex = -1;
}

CString CProductFinnhubCompanyNews::CreatMessage(void) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	CString strMessage;
	char buffer[50]{};
	int iUpdateDate = 0, iMarketDate = 0;
	int year = 0, month = 0, day = 0;
	CString strTemp;
	CWorldStockPtr pStock = ((CWorldMarket*)m_pMarket)->GetStock(m_lIndex);
	strMessage = m_strInquiringStr + pStock->GetSymbol();
	iUpdateDate = pStock->GetCompanyNewsUpdateDate();
	XferDateToYearMonthDay(iUpdateDate, year, month, day);
	sprintf_s(buffer, _T("%4d-%02d-%02d"), year, month, day);
	strTemp = buffer;
	strMessage += _T("&from=");
	strMessage += strTemp;
	iMarketDate = gl_pWorldMarket->GetMarketDate();
	XferDateToYearMonthDay(iMarketDate, year, month, day);
	sprintf_s(buffer, _T("%4d-%02d-%02d"), year, month, day);
	strTemp = buffer;
	strMessage += _T("&to=");
	strMessage += strTemp;

	m_strTotalInquiryMessage = strMessage;
	m_strInquiringExchange = GetStockExchange(pStock->GetSymbol());
	return m_strTotalInquiryMessage;
}

bool CProductFinnhubCompanyNews::ParseAndStoreWebData(CWebDataPtr pWebData) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	CCompanyNewsVectorPtr pvFinnhubCompanyNews = ParseFinnhubCompanyNews(pWebData);
	CWorldStockPtr pStock = ((CWorldMarket*)m_pMarket)->GetStock(m_lIndex);

	if (pvFinnhubCompanyNews->size() > 0) {
		// 因为接收到的股票代码是本土代码，可能与pStock中的不同（外国的ADR)，所以需要更新股票代码.
		// 例如申请BVDRF的金融数据，回复的股票代码为MBWS.PA
		for (auto& pFinnhubCompanyNews : *pvFinnhubCompanyNews) {
			pFinnhubCompanyNews->m_strCompanySymbol = pStock->GetSymbol();
		}
		pStock->UpdateCompanyNews(pvFinnhubCompanyNews);
		pStock->SetUpdateCompanyNewsDB(true);
	}
	pStock->SetCompanyNewsUpdateDate(((CWorldMarket*)m_pMarket)->GetMarketDate());
	pStock->SetCompanyNewsUpdated(true);

	return true;
}

/// <summary>
/// 公司新闻，目前只提供北美公司的新闻
/// </summary>
/// <param name="pWebData"></param>
/// <param name="pStock"></param>
/// <returns></returns>
/// {
///"category": "company",
///"datetime" : 1666012311,
///"headline" : "Top 10 Stock Picks of William Von Mueffling’s Cantillon Capital Management",
///"id" : 116688201,
///"image" : "https://s.yimg.com/uu/api/res/1.2/8bic0IrK7TXPlRFGOhnriQ--~B/aD02MDA7dz01OTQ7YXBwaWQ9eXRhY2h5b24-/https://media.zenfs.com/en/insidermonkey.com/8c6d80ff8a0b3edee7be3c636143c877",
///"related" : "A",
///"source" : "Yahoo",
///"summary" : "In this article, we discuss the top 10 stock picks of William Von Mueffling’s Cantillon Capital Management. If you want to skip our detailed analysis of Mueffling’s investment philosophy and performance, go directly to Top 5 Stock Picks of William Von Mueffling’s Cantillon Capital Management. William Von Mueffling worked at Lazard Asset Management before launching […]",
///"url" : "https://finnhub.io/api/news?id=a0fe8819916603e447eb52cad56f2cc3bb148097c65e81bf335d39961f67b502"
///		}
CCompanyNewsVectorPtr CProductFinnhubCompanyNews::ParseFinnhubCompanyNews(CWebDataPtr pWebData) {
	string s;
	wstring ws;
	CStringW strW;

	shared_ptr<ptree> ppt;
	ptree pt1, pt2;
	CCompanyNewsPtr pCompanyNews = nullptr;
	INT64 dateTime = 0;

	CCompanyNewsVectorPtr pvFinnhubCompanyNews = make_shared<vector<CCompanyNewsPtr>>();

	ASSERT(pWebData->IsJSonContentType());
	if (!pWebData->IsParsed()) return pvFinnhubCompanyNews;
	if (pWebData->IsVoidJSon()) { m_iReceivedDataStatus = __VOID_DATA__; return pvFinnhubCompanyNews; }
	if (pWebData->NoRightToAccess()) { m_iReceivedDataStatus = __NO_ACCESS_RIGHT__; return pvFinnhubCompanyNews; }
	ppt = pWebData->GetPTree();
	try {
		for (ptree::iterator it = ppt->begin(); it != ppt->end(); ++it) {
			pt1 = it->second;
			pCompanyNews = make_shared<CFinnhubCompanyNews>();
			s = pt1.get<string>(_T("category"));
			if (s.size() > 0) pCompanyNews->m_strCategory = XferToCString(s);
			dateTime = pt1.get<INT64>(_T("datetime"));
			pCompanyNews->m_llDateTime = TransferToDateTime(dateTime, 0);
			s = pt1.get<string>(_T("headline"));
			if (s.size() > 0) pCompanyNews->m_strHeadLine = XferToCString(s);
			pCompanyNews->m_iNewsID = pt1.get<int>(_T("id"));
			s = pt1.get<string>(_T("image"));
			if (s.size() > 0) pCompanyNews->m_strImage = XferToCString(s);
			//if (s.size() > 0) pCompanyNews->m_strImage = s.c_str();
			s = pt1.get<string>(_T("related"));
			if (s.size() > 0) pCompanyNews->m_strRelatedSymbol = XferToCString(s);
			s = pt1.get<string>(_T("source"));
			if (s.size() > 0) pCompanyNews->m_strSource = XferToCString(s);
			s = pt1.get<string>(_T("summary"));
			if (s.size() > 0) pCompanyNews->m_strSummary = XferToCString(s);
			s = pt1.get<string>(_T("url"));
			if (s.size() > 0) pCompanyNews->m_strURL = XferToCString(s);
			pvFinnhubCompanyNews->push_back(pCompanyNews);
		}
	}
	catch (ptree_error& e) {
		ReportJSonErrorToSystemMessage(_T("Finnhub Stock News "), e);
		return pvFinnhubCompanyNews; // 没有公司简介
	}
	return pvFinnhubCompanyNews;
}