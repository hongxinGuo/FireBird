module;
#include <afx.h>

module FireBirdLib.Product.AlphaVantage.StockSplits;
import FireBirdLib.Accessory.JsonParse;
import FireBirdLib.Accessory.NlohmannJson.GetValue;
import FireBirdLib.Accessory.NlohmannJson.Declaration;

import FireBirdLib.Container.FireBirdLib.Stock.TiingoStock;
import FireBirdLib.Stock.TiingoStock;
import StockSplit;
import SystemMessage;
import FireBirdLib.WebData;

import std;
using std::make_shared;
using std::istringstream;
using std::chrono::local_days;

CProductAlphaVantageStockSplits::CProductAlphaVantageStockSplits() {
	m_strInquiryFunction = "https://www.alphavantage.co/query?function=SPLITS&symbol=";
}

///////////////////////////////////////////////////////////////////////////////////////////
///
/// 申请全部数据。
///
///////////////////////////////////////////////////////////////////////////////////////////
string CProductAlphaVantageStockSplits::CreateMessage() {
	const auto pStock = gl_dataContainerTiingoStock.GetStock(GetIndex());
	ASSERT(pStock->IsActive()); // 活跃股票
	m_strInquiringSymbol = pStock->GetSymbol();

	m_strInquiry = m_strInquiryFunction + m_strInquiringSymbol;
	return m_strInquiry;
}

void CProductAlphaVantageStockSplits::ParseAndStoreWebData(CWebDataPtr pWebData) {
	const auto pTiingoStock = gl_dataContainerTiingoStock.GetStock(m_index);

	auto pvSplits = ParseAlphaVantageStockSplits(pWebData);
	if (!pvSplits->empty()) {
		for (auto& pSplits2 : *pvSplits) {
			pSplits2->SetSymbol(pTiingoStock->GetSymbol());
		}
		pTiingoStock->AddStockSplits(pvSplits);
		pTiingoStock->SetUpdateSplitDB(true);
	}
	// 清除stock split的更新标识
	pTiingoStock->SetUpdateSplit(false);
	pTiingoStock->SetUpdateProfileDB(true);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// AlphaVantage股票分割格式：
// {
//	"symbol": "IBM",
//	"data" : [
//		{
//			"effective_date": "2021-11-04",
//				"split_factor" : "1.0460"
//		},
//		{
//			"effective_date": "1999-05-27",
//			"split_factor" : "2.0000"
//		}
//	]
// }
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CStockSplitsPtr CProductAlphaVantageStockSplits::ParseAlphaVantageStockSplits(const CWebDataPtr& pWebData) {
	auto pvSplits = make_shared<vector<shared_ptr<CStockSplit>>>();
	pvSplits->reserve(100);

	string s;
	nlohmannJson js;

	if (!pWebData->CreateJson(js)) return pvSplits;
	if (!IsValidData(pWebData)) return pvSplits;

	try {
		s = js.at("detail"); // 是否有报错信息
		string strMessage = "AlphaVantage stock split ";
		strMessage += s;
		gl_systemMessage.PushErrorMessage(strMessage); // 报告错误信息
		return pvSplits;
	} catch (nlohmannJson::exception&) {
		// 正确， do nothing，继续执行
	}
	try {
		string strSymbol = js.at("symbol"); // 股票代码
		auto jsData = js.at("data"); // 股票分割数据
		for (auto it = jsData.begin(); it != jsData.end(); ++it) {
			CStockSplitPtr pSplits = make_shared<CStockSplit>();
			s = jsonGetString(it, "effective_date");
			istringstream ss(s);
			local_days ld;
			ss >> parse("%F", ld);
			pSplits->SetDate(ld);
			double dTemp = jsonGetDouble(it, "close");
			pSplits->SetRatio(dTemp);

			pvSplits->push_back(pSplits);
		}
	} catch (nlohmannJson::exception& e) {
		string str3 = pWebData->GetDataBuffer();
		str3 = str3.substr(0, 120);
		ReportJSonErrorToSystemMessage("AlphaVantage FireBirdLib.Stock Splits " + str3, e.what());
		return pvSplits; // 数据解析出错的话，则放弃。
	}
	std::ranges::sort(*pvSplits, [](const CStockSplitPtr& pSplits1, const CStockSplitPtr& pSplits2) { return pSplits1->GetDate() < pSplits2->GetDate(); }); // 以日期早晚顺序排列。

	return pvSplits;
}
