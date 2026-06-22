#include "pch.h"

#include "ProductEastmoneyDayLine.h"
#include"ChinaMarket.h"

#include "EastmoneyDayLineDataSource.h"
#include "JsonParse.h"
#include"WebData.h"

// Helper: parse one 东方财富 Kline line like:
// "2024-12-31,6.29,6.18,6.32,6.17,483794,313479921.00,2.38,-1.75,-0.11,0.59"
static bool ParseEastmoneyKlineLine(const std::string& line, CDayLine& outDayLine, long& lLastClose) {
	// split by comma (fast, minimal dependencies)
	std::vector<std::string_view> parts;
	parts.reserve(12);
	size_t start = 0;
	while (start < line.size()) {
		auto pos = line.find(',', start);
		if (pos == std::string::npos) {
			parts.emplace_back(line.data() + start, line.size() - start);
			break;
		}
		parts.emplace_back(line.data() + start, pos - start);
		start = pos + 1;
	}
	if (parts.size() < 7) return false; // minimal required fields

	// 0: date (YYYY-MM-DD)
	{
		std::string tmp(parts[0]);
		std::istringstream ss(tmp);
		chrono::local_days ld;
		ss >> chrono::parse("%F", ld);
		outDayLine.SetDate(ld);
	}

	// Field mapping based on sample:
	// 1: close, 2: open, 3: high, 4: low, 5: volume, 6: turnover/amount
	// Prices in Eastmoney sample use 2 decimal digits -> use Str2Long(..., 2)
	try {
		outDayLine.SetOpen(Str2Long(parts[1], 3));
		const long lClose = Str2Long(parts[2], 3);
		outDayLine.SetClose(lClose);
		lLastClose = lClose;
		outDayLine.SetHigh(Str2Long(parts[3], 3));
		outDayLine.SetLow(Str2Long(parts[4], 3));
		// volume: use string form so existing SetVolume(string) (parsing) can handle formats
		outDayLine.SetVolume(Str2Long(parts[5], 2));
		// amount / turnover: keep as string -> SetAmount(string) will convert
		outDayLine.SetAmount(Str2Long(parts[6], 2));
	} catch (...) {
		return false;
	}
	return true;
}

CProductEastmoneyDayLine::CProductEastmoneyDayLine() {
	m_lCurrentStockPosition = 0;
	m_iInquiryNumber = 0;
}

string CProductEastmoneyDayLine::CreateMessage() {
	return m_strInquiryFunction; // 腾讯日线数据的申请字符串目前由CEastmoneyDayLineDataSource类完成，本Product无需动作。
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// {
///	"rc": 0,
///	"rt": 17,
///	"svr": 183641822,
///	"lt": 1,
///	"full": 0,
///	"dlmkts": "",
///	"dsc": "0",
///	"data": {
///		"code": "601872",
///			"market": 1,
///			"name": "招商轮船",
///			"decimal": 2,
///			"dktotal": 4621,
///			"preKPrice": 6.08,
///			"klines": [
///				"2024-12-18,6.11,6.18,6.28,6.10,842743,542829495.00,2.96,1.64,0.10,1.03",
///				"2024-12-19,6.15,6.09,6.16,6.03,528781,333859220.00,2.10,-1.46,-0.09,0.65",
///				"2024-12-20,6.08,5.97,6.10,5.97,701931,437784952.00,2.13,-1.97,-0.12,0.86",
///				"2024-12-23,5.95,6.06,6.08,5.95,675187,423018527.00,2.18,1.51,0.09,0.83",
///				"2024-12-24,6.04,6.30,6.34,6.03,1129216,727525144.00,5.12,3.96,0.24,1.39",
///				"2024-12-25,6.30,6.26,6.36,6.24,456611,296825401.00,1.90,-0.63,-0.04,0.56",
///				"2024-12-26,6.26,6.19,6.26,6.15,374180,239964964.00,1.76,-1.12,-0.07,0.46",
///				"2024-12-27,6.16,6.24,6.25,6.16,371161,239139229.00,1.45,0.81,0.05,0.46",
///				"2024-12-30,6.24,6.29,6.31,6.23,377719,245904544.00,1.28,0.80,0.05,0.46",
///				"2024-12-31,6.29,6.18,6.32,6.17,483794,313479921.00,2.38,-1.75,-0.11,0.59"
///			]
///	 }
/// }
///
///
///
/// 
///////////////////////////////////////////////////////////////////////////////////////////////////////
void CProductEastmoneyDayLine::ParseAndStoreWebData(CWebDataPtr pWebData) {
	if (gl_pEastmoneyDayLineDataSource->GetHTTPStatusCode() != 200) return; //网络数据不正常时不处理。

	auto pDayLineData = make_shared<CDayLineWebData>();
	const string strSymbol = pWebData->GetStockCode();
	ASSERT(gl_dataContainerChinaStock.IsSymbol(strSymbol));
	const string_view svData = pWebData->GetStringView();

	const shared_ptr<vector<CDayLine>> pvDayLine = ParseEastmoneyDayLine(svData, pWebData->GetStockCode());
	if (pvDayLine->size() == 0) return; // 如果没有日线，则不处理
	std::ranges::sort(*pvDayLine, [](const CDayLine& pData1, const CDayLine& pData2) { return pData1.GetDate() < pData2.GetDate(); });
	for (auto& dayLine : *pvDayLine) {
		dayLine.SetStockSymbol(strSymbol);
		pDayLineData->AppendDayLine(dayLine);
	}
	pDayLineData->SetStockCode(strSymbol);
	gl_qDayLine.enqueue(pDayLineData);
}

CDayLinesPtr CProductEastmoneyDayLine::ParseEastmoneyDayLine(const string_view& svData, const string& strStockCode) {
	auto pvDayLine = make_shared<vector<CDayLine>>();
	pvDayLine->reserve(2000);

	const string strStockSymbol = strStockCode;
	try {
		string_view sv;
		long lLastClose = 0;
		ondemand::parser parser;
		ondemand::document doc;

		//const padded_string_view jsonPaddedView(svData, svData.length()); // Note 此时的svData带有长度为SIMDJSON_PADDING长度的后缀
		//doc = parser.iterate(jsonPaddedView).value();
		const padded_string jsonPadded(svData);
		doc = parser.iterate(jsonPadded).value();
		int rc = doc["rc"].get_int32().value();
		if (rc != 0) return pvDayLine;
		ondemand::array dayArray1 = doc["data"]["klines"].get_array().value();
		auto a1 = doc["data"];
		auto a3 = a1["klines"];
		auto a4 = a3.get_array();
		auto dayArray = a4.value();
		for (auto dayLine : dayArray) {
			CDayLine dayLine2;
			dayLine2.SetStockSymbol(strStockSymbol);
			dayLine2.SetLastClose(lLastClose);
			string s = std::string(dayLine.get_string().value());
			if (ParseEastmoneyKlineLine(s, dayLine2, lLastClose)) {
				pvDayLine->push_back(dayLine2);
			}
		}
	} catch (nlohmannJson::exception&) {
		return pvDayLine;
	}
	return pvDayLine;
}
