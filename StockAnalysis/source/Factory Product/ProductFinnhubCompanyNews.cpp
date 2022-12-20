#include "pch.h"

#include"TimeConvert.h"
#include"jsonParse.h"
#include"worldMarket.h"

#include "ProductFinnhubCompanyNews.h"

IMPLEMENT_DYNCREATE(CProductFinnhubCompanyNews, CProductFinnhub)

CProductFinnhubCompanyNews::CProductFinnhubCompanyNews() {
	m_strClassName = _T("Finnhub company news");
	m_strInquiry = _T("https://finnhub.io/api/v1/company-news?symbol=");
	m_lIndex = -1;
}

CString CProductFinnhubCompanyNews::CreateMessage(void) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	char buffer[50]{};
	int iUpdateDate = 0, iMarketDate = 0;
	int year = 0, month = 0, day = 0;
	const auto pStock = dynamic_cast<CWorldMarket*>(m_pMarket)->GetStock(m_lIndex);
	CString strMessage = m_strInquiry + pStock->GetSymbol();
	iUpdateDate = pStock->GetCompanyNewsUpdateDate();
	XferDateToYearMonthDay(iUpdateDate, year, month, day);
	sprintf_s(buffer, _T("%4d-%02d-%02d"), year, month, day);
	CString strTemp = buffer;
	strMessage += _T("&from=");
	strMessage += strTemp;
	iMarketDate = gl_pWorldMarket->GetMarketDate();
	XferDateToYearMonthDay(iMarketDate, year, month, day);
	sprintf_s(buffer, _T("%4d-%02d-%02d"), year, month, day);
	strTemp = buffer;
	strMessage += _T("&to=");
	strMessage += strTemp;

	m_strTotalInquiryMessage = strMessage;
	m_strInquiringExchange = pStock->GetExchangeCode();
	return m_strTotalInquiryMessage;
}

bool CProductFinnhubCompanyNews::ParseAndStoreWebData(CWebDataPtr pWebData) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	const auto pvFinnhubCompanyNews = ParseFinnhubCompanyNews(pWebData);
	const auto pStock = dynamic_cast<CWorldMarket*>(m_pMarket)->GetStock(m_lIndex);

	if (!pvFinnhubCompanyNews->empty()) {
		// 因为接收到的股票代码是本土代码，可能与pStock中的不同（外国的ADR)，所以需要更新股票代码.
		// 例如申请BVDRF的金融数据，回复的股票代码为MBWS.PA
		for (const auto& pFinnhubCompanyNews : *pvFinnhubCompanyNews) {
			pFinnhubCompanyNews->m_strCompanySymbol = pStock->GetSymbol();
		}
		pStock->UpdateCompanyNews(pvFinnhubCompanyNews);
		pStock->SetUpdateCompanyNewsDB(true);
	}
	pStock->SetCompanyNewsUpdateDate(((CWorldMarket*)m_pMarket)->GetMarketDate());
	pStock->SetCompanyNewsUpdated(true);
	pStock->SetUpdateProfileDB(true);

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

	auto pvFinnhubCompanyNews = make_shared<vector<CCompanyNewsPtr>>();

	ASSERT(pWebData->IsJSonContentType());
	if (!pWebData->IsParsed()) return pvFinnhubCompanyNews;
	if (pWebData->IsVoidJson()) {
		m_iReceivedDataStatus = _VOID_DATA_;
		return pvFinnhubCompanyNews;
	}
	if (pWebData->CheckNoRightToAccess()) {
		m_iReceivedDataStatus = _NO_ACCESS_RIGHT_;
		return pvFinnhubCompanyNews;
	}
	const auto pjs = pWebData->GetJSon();
	try {
		for (auto it = pjs->begin(); it != pjs->end(); ++it) {
			auto pCompanyNews = make_shared<CFinnhubCompanyNews>();
			s = jsonGetString(it, _T("category"));
			if (!s.empty()) pCompanyNews->m_strCategory = s.c_str();
			const auto dateTime = jsonGetLongLong(it, _T("datetime"));
			pCompanyNews->m_llDateTime = TransferToDateTime(dateTime, 0);
			s = jsonGetString(it, _T("headline"));
			if (!s.empty()) pCompanyNews->m_strHeadLine = s.c_str();
			pCompanyNews->m_iNewsID = jsonGetInt(it,_T("id"));
			s = jsonGetString(it,_T("image"));
			if (!s.empty()) pCompanyNews->m_strImage = s.c_str();
			//if (s.size() > 0) pCompanyNews->m_strImage = s.c_str();
			s = jsonGetString(it,_T("related"));
			if (!s.empty()) pCompanyNews->m_strRelatedSymbol = s.c_str();
			s = jsonGetString(it,_T("source"));
			if (!s.empty()) pCompanyNews->m_strSource = s.c_str();
			s = jsonGetString(it,_T("summary"));
			if (!s.empty()) pCompanyNews->m_strSummary = s.c_str();
			s = jsonGetString(it,_T("url"));
			if (!s.empty()) pCompanyNews->m_strURL = s.c_str();
			pvFinnhubCompanyNews->push_back(pCompanyNews);
		}
	}
	catch (json::exception& e) {
		ReportJSonErrorToSystemMessage(_T("Finnhub Stock News "), e.what());
		return pvFinnhubCompanyNews; // 没有公司简介
	}
	return pvFinnhubCompanyNews;
}
