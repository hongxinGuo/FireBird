#include "pch.h"

#include"TimeConvert.h"
#include"jsonParse.h"
#include"JsonGetValue.h"

#include"worldMarket.h"

#include "ProductFinnhubCompanyNews.h"

CProductFinnhubCompanyNews::CProductFinnhubCompanyNews() {
	m_strClassName = _T("Finnhub company news");
	m_strInquiryFunction = _T("https://finnhub.io/api/v1/company-news?symbol=");
	m_lIndex = -1;
}

CString CProductFinnhubCompanyNews::CreateMessage() {
	ASSERT(std::strcmp(typeid(*m_pMarket).name(), _T("class CWorldMarket")) == 0);

	const auto pStock = dynamic_cast<CWorldMarket*>(m_pMarket)->GetStock(m_lIndex);
	CString strMessage = m_strInquiryFunction + pStock->GetSymbol();
	CString strTemp = ConvertDateToTimeStamp(pStock->GetCompanyNewsUpdateDate());
	strMessage += _T("&from=");
	strMessage += strTemp;
	strTemp = ConvertDateToTimeStamp(gl_pWorldMarket->GetMarketDate());
	strMessage += _T("&to=");
	strMessage += strTemp;

	m_strInquiry = strMessage;
	m_strInquiringExchange = pStock->GetExchangeCode();
	return m_strInquiry;
}

bool CProductFinnhubCompanyNews::ParseAndStoreWebData(CWebDataPtr pWebData) {
	ASSERT(std::strcmp(typeid(*m_pMarket).name(), _T("class CWorldMarket")) == 0);

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
	pStock->SetCompanyNewsUpdateDate(m_pMarket->GetMarketDate());
	pStock->SetUpdateCompanyNews(false);
	pStock->SetUpdateProfileDB(true);

	return true;
}

/// <summary>
/// 公司新闻，目前只提供北美公司的新闻
/// </summary>
///
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
CCompanyNewsVectorPtr CProductFinnhubCompanyNews::ParseFinnhubCompanyNews(const CWebDataPtr& pWebData) {
	string s;

	auto pvFinnhubCompanyNews = make_shared<vector<CCompanyNewsPtr>>();

	ASSERT(pWebData->IsJSonContentType());
	if (!pWebData->IsParsed()) return pvFinnhubCompanyNews;
	if (pWebData->IsVoidJson()) {
		m_iReceivedDataStatus = VOID_DATA_;
		return pvFinnhubCompanyNews;
	}
	if (pWebData->CheckNoRightToAccess()) {
		m_iReceivedDataStatus = NO_ACCESS_RIGHT_;
		return pvFinnhubCompanyNews;
	}
	const auto pjs = pWebData->GetJSon();
	try {
		for (auto it = pjs->begin(); it != pjs->end(); ++it) {
			auto pCompanyNews = make_shared<CFinnhubCompanyNews>();
			s = jsonGetString(it, _T("category"));
			if (!s.empty()) pCompanyNews->m_strCategory = s.c_str();
			const auto dateTime = jsonGetLongLong(it, _T("datetime"));
			pCompanyNews->m_llDateTime = ConvertToDateTime(dateTime, 0);
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
