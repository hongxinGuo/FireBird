//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Tiingo市场新闻。
///	Note 此信息为premium，使用此信息来判断Tiingo账户类型（免费还是付费）
///
///
///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "pch.h"

#include"jsonParse.h"

#include"TiingoStock.h"
#include "ProductTiingoMarketNews.h"

#include "JsonGetValue.h"
#include "TiingoDataSource.h"

#include"simdjsonGetValue.h"

CProductTiingoMarketNews::CProductTiingoMarketNews() {
	m_strInquiryFunction = _T("https://api.tiingo.com/tiingo/news?");
}

CString CProductTiingoMarketNews::CreateMessage() {
	m_strInquiry = m_strInquiryFunction;
	return m_strInquiry;
}

void CProductTiingoMarketNews::ParseAndStoreWebData(CWebDataPtr pWebData) {
	const auto pvTiingoMarketNews = ParseTiingoMarketNews(pWebData);
	if (!pvTiingoMarketNews->empty()) {
		char buffer[100];
		long lTemp = 0;
		for (const auto& pTiingoStock : *pvTiingoMarketNews) {
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// [{
//	"source":"cnbc.com",
//		"crawlDate" : "2019-01-29T22:20:01.696871Z",
//		"description" : "Apple CEO Tim Cook told CNBC that trade tensions between the U.S. and China have improved since late
//		December.",
//		"url" : "https://www.cnbc.com/2019/01/29/apples-ceo-sees-optimism-as-trade-tension-between-us-and-china-lessens.html",
//		"publishedDate" : "2019-01-29T22:17:00Z",
//		"tags" : [
//			"China",
//				"Economic Measures",
//				"Economics",
//				"Markets",
//				"Stock",
//				"Technology",
//				"Tiingo Top",
//				"Trade"
//		] ,
//		"tickers": [
//			"aapl"
//		] ,
//		"id" : 15063835,
//		"title" : "Apple CEO Tim Cook on US-China trade negotiations: 'There is a bit more optimism in the air'"
// }...]
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CVectorTiingoMarketNewsPtr CProductTiingoMarketNews::ParseTiingoMarketNews(const CWebDataPtr& pWebData) {
	auto pvTiingoMarketNews = make_shared<vector<CTiingoMarketNewsPtr>>();
	CString strNULL = _T(" ");
	CTiingoMarketNewsPtr pMarketNews = nullptr;
	string s1;
	CString strNumber;
	int year, month, day, hour, minute, second, extra;
	double f;
	if (!IsValidData(pWebData)) return pvTiingoMarketNews;

	try {
		string_view sv;
		string_view svJson = pWebData->GetStringView(0, pWebData->GetBufferLength());
		ondemand::parser parser;
		const simdjson::padded_string jsonPadded(svJson);
		ondemand::document doc = parser.iterate(jsonPadded).value();

		CString str;
		int iCount = 0;
		for (auto item : doc) {
			auto itemValue = item.value();
			pMarketNews = make_shared<CTiingoMarketNews>();
			s1 = jsonGetStringView(itemValue, _T("source"));
			pMarketNews->m_strSource = s1.c_str();
			s1 = jsonGetStringView(itemValue, _T("crawlDate"));
			sscanf_s(s1.c_str(), _T("%04i-%02i-%02iT%02i:%02i:%02i.%dZ"), &year, &month, &day, &hour, &minute, &second, &f);
			pMarketNews->m_llCrawlDate = static_cast<INT64>(year) * 10000000000 + month * 100000000 + day * 1000000 + hour * 10000 + minute * 100 + second;
			s1 = jsonGetStringView(itemValue, _T("description"));
			pMarketNews->m_strDescription = s1.c_str();
			s1 = jsonGetStringView(itemValue, _T("url"));
			pMarketNews->m_strUrl = s1.c_str();
			long l = jsonGetInt64(itemValue, _T("id"));
			pMarketNews->m_lId = l;
			s1 = jsonGetStringView(itemValue, _T("title"));
			pMarketNews->m_strTitle = s1.c_str();
			s1 = jsonGetStringView(itemValue, _T("publishedDate"));
			sscanf_s(s1.c_str(), _T("%04i-%02i-%02iT%02i:%02i:%02iZ"), &year, &month, &day, &hour, &minute, &second);
			pMarketNews->m_LLPublishDate = static_cast<INT64>(year) * 10000000000 + month * 100000000 + day * 1000000 + hour * 10000 + minute * 100 + second;

			auto jArray = jsonGetArray(itemValue, _T("tickers"));
			for (auto value : itemValue[_T("tickers")]) {
				auto s2 = value.get_string().value();
				string s4(s2.data(), s2.length());
				pMarketNews->m_strTickers = s4.c_str();
				break; // 只存储第一个证券代码
			}
			auto array = jsonGetArray(itemValue, _T("tags"));
			auto s = array.raw_json().value();
			string sTemp(s.data(), s.length());
			pMarketNews->m_strTags = sTemp.c_str();

			pvTiingoMarketNews->push_back(pMarketNews);
			iCount++;
		}
	} catch (simdjson_error& error) {
		ReportJSonErrorToSystemMessage(_T("Tiingo market news "), error.what());
	}

	return pvTiingoMarketNews;
}

void CProductTiingoMarketNews::UpdateDataSourceStatus(CVirtualDataSourcePtr pDataSource) {
	ASSERT(strcmp(typeid(*pDataSource).name(), _T("class CTiingoDataSource")) == 0);
	dynamic_pointer_cast<CTiingoDataSource>(pDataSource)->SetUpdateMarketNews(false);
	if (IsNoRightToAccess()) {
		gl_systemConfiguration.ChangeTiingoAccountTypeToFree(); // Note 在此确定Tiingo账户类型
	}
}
