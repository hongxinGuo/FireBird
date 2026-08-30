#include "pch.h"

#include"TimeConvert.h"
#include"jsonParse.h"
#include"nlohmannJsonGetValue.h"

#include"worldMarket.h"
#include "ContainerFinnhubStock.h"
#include"FinnhubStock.h"
#include"FinnhubCompanyNews.h"

#include "ProductFinnhubCompanyNews.h"

#include "FinnhubDataSource.h"
#include "SystemMessage.h"
#include "cpr/cpr.h"

using std::make_shared;

CProductFinnhubCompanyNews::CProductFinnhubCompanyNews() {
	m_strInquiryFunction = "https://finnhub.io/api/v1/company-news?symbol=";
}

void CProductFinnhubCompanyNews::InquireData(const std::stop_token& st) {
	auto inquireStrings = CreateMessage();
	for (const auto& inquiry : *inquireStrings) {
		if (st.stop_requested()) break;
		string inquireString = inquiry + "&token=" + gl_pFinnhubDataSource->GetToken();
		m_r = cpr::Get(cpr::Url{ inquireString });

		if (m_r.status_code != 200) {
			WebStatusCheck(m_r);
			return;
		}

		auto pvFinnhubCompanyNews = Parse(m_r.text);
		const auto pStock = gl_dataContainerFinnhubStock.GetItem(m_index);

		if (!pvFinnhubCompanyNews->empty()) {
			// 因为接收到的股票代码是本土代码，可能与pStock中的不同（外国的ADR)，所以需要更新股票代码.
			// 例如申请BVDRF的金融数据，回复的股票代码为MBWS.PA
			for (auto& finnhubCompanyNews : *pvFinnhubCompanyNews) {
				finnhubCompanyNews.m_strCompanySymbol = pStock->GetSymbol();
			}
			pStock->UpdateCompanyNews(pvFinnhubCompanyNews);
			pStock->SetUpdateCompanyNewsDB(true);
			pvFinnhubCompanyNews = nullptr;
		}
		pStock->SetCompanyNewsUpdateDate(gl_pWorldMarket->GetMarketDate());
		pStock->SetUpdateCompanyNews(false);
		pStock->SetUpdateProfileDB(true);
	}
}

void CProductFinnhubCompanyNews::WebStatusCheck(cpr::Response& r) {
	string s;
	switch (r.status_code) {
	case 0:
		break;
	case 302: //redirected, not an error
		break;
	case 401:
	case 403: // forbidden
		m_iReceivedDataStatus = NO_ACCESS_RIGHT_;
		CheckInaccessible();
		break;
	default:
		s = std::format("Finnhub company profile concise http error {}. code:{} message: {}", r.status_code, static_cast<int>(r.error.code), r.error.message);
		gl_systemMessage.PushInnerSystemInformationMessage(s);
		break;
	}
}
void CProductFinnhubCompanyNews::UpdateSystemStatus() {
	CProductFinnhub::UpdateSystemStatus();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 提供最新一年内的免费公司新闻，付费版提供最近20年内的公司新闻
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
shared_ptr<vector<string>> CProductFinnhubCompanyNews::CreateMessage() {
	const auto pStock = gl_dataContainerFinnhubStock.GetItem(m_index);
	string strMessage = m_strInquiryFunction + pStock->GetSymbol();
	auto limitTime = GetPrevDay(gl_pWorldMarket->GetMarketDate(), 360); // 最近一年内
	auto limitTime2 = limitTime > pStock->GetCompanyNewsUpdateDate() ? limitTime : pStock->GetCompanyNewsUpdateDate();
	string sTemp = ConvertDateToTimeStamp(limitTime2);
	strMessage += "&from=";
	strMessage += sTemp;
	sTemp = ConvertDateToTimeStamp(gl_pWorldMarket->GetMarketDate());
	strMessage += "&to=";
	strMessage += sTemp;

	m_inquiryString = strMessage;
	m_strInquiringExchange = pStock->GetExchange();
	shared_ptr<vector<string>> pInquiry = make_shared<vector<string>>();
	pInquiry->push_back(m_inquiryString);
	return pInquiry;
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
CCompanyNewssPtr CProductFinnhubCompanyNews::Parse(const string& text) {
	nlohmannJson js;
	auto pvFinnhubCompanyNews = make_shared<vector<CFinnhubCompanyNews>>();
	pvFinnhubCompanyNews->reserve(100);

	if (text.empty()) return pvFinnhubCompanyNews;
	if (!::CreateJsonWithNlohmann(js, text)) return pvFinnhubCompanyNews;
	if (::IsVoidJson(text)) return pvFinnhubCompanyNews; // 即使为空，也完成了查询。
	if (IsNoRightToAccess()) return pvFinnhubCompanyNews;

	try {
		string s;
		for (auto it = js.begin(); it != js.end(); ++it) {
			CFinnhubCompanyNews companyNews;
			s = jsonGetString(it, "category");
			if (!s.empty()) companyNews.m_strCategory = s;
			const auto dateTime = jsonGetLongLong(it, "datetime");
			companyNews.m_DateTime = toSysTime(dateTime);
			s = jsonGetString(it, "headline");
			if (!s.empty()) companyNews.m_strHeadLine = s;
			companyNews.m_iNewsID = jsonGetInt(it, "id");
			s = jsonGetString(it, "image");
			if (!s.empty()) companyNews.m_strImage = s;
			//if (s.size() > 0) pCompanyNews->m_strImage = s;
			s = jsonGetString(it, "related");
			if (!s.empty()) companyNews.m_strRelatedSymbol = s;
			s = jsonGetString(it, "source");
			if (!s.empty()) companyNews.m_strSource = s;
			s = jsonGetString(it, "summary");
			if (!s.empty()) companyNews.m_strSummary = s;
			s = jsonGetString(it, "url");
			if (!s.empty()) companyNews.m_strURL = s;
			pvFinnhubCompanyNews->push_back(companyNews);
			companyNews.Reset();
		}
	} catch (nlohmannJson::exception& e) {
		ReportJSonErrorToSystemMessage("Finnhub Stock News ", e.what());
		return pvFinnhubCompanyNews; // 没有公司简介
	}
	return pvFinnhubCompanyNews;
}
