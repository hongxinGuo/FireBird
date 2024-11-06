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

#include "ProductTiingoStockDailyMeta.h"

#include "TiingoDataSource.h"

#include"simdjsonGetValue.h"
#include "WorldMarket.h"

CProductTiingoStockDailyMeta::CProductTiingoStockDailyMeta() {
	m_strInquiryFunction = _T("https://api.tiingo.com/tiingo/news?");
}

CString CProductTiingoStockDailyMeta::CreateMessage() {
	m_strInquiringSymbol = _T("All");
	m_strInquiry = m_strInquiryFunction;
	return m_strInquiry;
}

void CProductTiingoStockDailyMeta::ParseAndStoreWebData(CWebDataPtr pWebData) {
	const auto pvTiingoStockDailyMeta = ParseTiingoStockDailyMeta(pWebData);
	if (!pvTiingoStockDailyMeta->empty()) {
		for (const auto& pStockDailyMeta : *pvTiingoStockDailyMeta) {
		}
	}
	gl_systemConfiguration.DecreaseTiingoBandWidth(pWebData->GetBufferLength());
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
CVectorTiingoStockDailyMetaPtr CProductTiingoStockDailyMeta::ParseTiingoStockDailyMeta(const CWebDataPtr& pWebData) {
	auto pvTiingoStockDailyMeta = make_shared<vector<CTiingoStockDailyMetaPtr>>();
	CString strNULL = _T(" ");
	CTiingoStockDailyMetaPtr pStockDailyMeta = nullptr;
	string s1;
	CString strNumber;
	int year, month, day, hour, minute, second;
	float f;
	if (!IsValidData(pWebData)) return pvTiingoStockDailyMeta;

	try {
		string_view svJson = pWebData->GetStringView(0, pWebData->GetBufferLength());
		ondemand::parser parser;
		const simdjson::padded_string jsonPadded(svJson);
		ondemand::document doc = parser.iterate(jsonPadded).value();

		CString str;
		int iCount = 0;
	} catch (simdjson_error& error) {
		ReportJSonErrorToSystemMessage(_T("Tiingo market news "), error.what());
	}

	return pvTiingoStockDailyMeta;
}

void CProductTiingoStockDailyMeta::UpdateDataSourceStatus(CVirtualDataSourcePtr pDataSource) {
	ASSERT(strcmp(typeid(*pDataSource).name(), _T("class CTiingoDataSource")) == 0);
	dynamic_pointer_cast<CTiingoDataSource>(pDataSource)->SetUpdateStockDailyMeta(false);
	gl_systemMessage.PushInformationMessage(_T("Tiingo market news已更新"));

	if (IsNoRightToAccess()) { // Note 在此确定Tiingo账户类型
		gl_systemConfiguration.ChangeTiingoAccountTypeToFree();
		gl_systemMessage.PushInnerSystemInformationMessage(_T("free Tiingo account"));
	}
	else {
		gl_systemConfiguration.ChangeTiingoAccountTypeToPaid();
		gl_systemMessage.PushInnerSystemInformationMessage(_T("Paid Tiingo account"));
	}
}
