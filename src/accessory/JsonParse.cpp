//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 数据解析的任务皆位于此文件中。
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include "JsonParse.h"

#include"WebData.h"
#include"WebRTData.h"

#include "ChinaStockCodeConverter.h"
#include "InfoReport.h"
#include "Thread.h"

#include"simdjsonGetValue.h"

#include"SystemMessage.h"

#include"nlohmannJsonGetValue.h"
#include"NlohmannJsonDeclaration.h"
#include "ChinaMarket.h"

// 包含concurrencpp.h之前，需要注销max的定义
#undef max
#include"concurrencpp/concurrencpp.h"
using namespace concurrencpp;

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// 将输入的字符串转变成放大了10^power倍的长整型。要确保精确地转换，不使用浮点数过渡。
// 这是coPilot提供的范本，执行时间要慢20%，但更清晰。
//
///////////////////////////////////////////////////////////////////////////////////////////////////

long long StrToDecimal(const std::string_view& svData, const size_t power) {
	std::string buffer;
	buffer.reserve(svData.size() + power + 2);

	try {
		auto iPointPosition = svData.find('.');
		if (iPointPosition == std::string_view::npos) {
			buffer.append(svData);
			buffer.append(power, '0');
			return std::stoll(buffer);
		}
		buffer.append(svData.substr(0, iPointPosition));
		auto fraction = svData.substr(iPointPosition + 1);
		if (fraction.size() > power) fraction = fraction.substr(0, power);
		buffer.append(fraction);
		buffer.append(power > fraction.size() ? power - fraction.size() : 0, '0');
		return std::stoll(buffer);
	} catch (std::out_of_range&) {
		return 0;
	} catch (std::invalid_argument&) {
		return 0;
	}
}

long long StrToDecimal2(const std::string_view& svData, const size_t power) {
	try {
		auto iPointPosition = svData.find('.');
		if (iPointPosition == std::string_view::npos) {
			std::string result(svData);
			result.append(power, '0');
			return std::stoll(result);
		}
		std::string result(svData.substr(0, iPointPosition));
		auto fraction = svData.substr(iPointPosition + 1);
		if (fraction.size() > power) {
			fraction = fraction.substr(0, power);
		}
		result.append(fraction);
		if (power > fraction.size()) {
			result.append(power - fraction.size(), '0');
		}
		return std::stoll(result);
	} catch (std::out_of_range&) {
		return 0;
	} catch (std::invalid_argument&) {
		return 0;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// 将输入的字符串转变成放大了10^power倍的长整型。要确保精确地转换，不使用浮点数过渡。
// 这是coPilot提供的范本，执行时间要慢20%，但更清晰。
//
///////////////////////////////////////////////////////////////////////////////////////////////////
long Str2Long(const std::string_view& svData, const size_t power) {
	std::string buffer;
	buffer.reserve(svData.size() + power + 2);

	try {
		auto iPointPosition = svData.find('.');
		if (iPointPosition == std::string_view::npos) {
			buffer.append(svData);
			buffer.append(power, '0');
			return std::stoll(buffer);
		}
		buffer.append(svData.substr(0, iPointPosition));
		auto fraction = svData.substr(iPointPosition + 1);
		if (fraction.size() > power) fraction = fraction.substr(0, power);
		buffer.append(fraction);
		buffer.append(power > fraction.size() ? power - fraction.size() : 0, '0');
		return std::stol(buffer);
	} catch (std::out_of_range&) {
		return 0;
	} catch (std::invalid_argument&) {
		return 0;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 出现exception时，上级调用函数负责处理
//
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
string_view GetNextField(const string_view& svData, size_t& lCurrentPos, const char delimiter) {
	const string_view sv = svData.substr(lCurrentPos);
	const auto lEnd = sv.find_first_of(delimiter);
	if (lEnd > sv.length()) throw exception("GetNextField() out of range"); // 没找到的话抛出异常
	lCurrentPos += lEnd + 1; // 将当前位置移至本数据之后
	return string_view{ sv.data(), lEnd };
}

namespace {
	void ReportJsonError(const nlohmannJson::parse_error& e, const std::string& s) {
		char buffer[180]{};
		size_t i;
		for (i = 0; i < 180; i++) {
			if ((e.byte - 90 + i) < s.size()) {
				buffer[i] = s.at(e.byte - 90 + i);
			}
			else break;
		}
		buffer[i] = 0x000;
		const string s2 = std::format("Nlohmann JSon Reading Error {} {:Ld} {:Ld} {:d} {}", e.what(), s.size(), e.byte, i, buffer);
		gl_systemMessage.PushErrorMessage(s2);
	}
}

void ReportJSonErrorToSystemMessage(const string& strPrefix, const string& strWhat) {
	string s = strPrefix;
	s += strWhat;
	gl_systemMessage.PushErrorMessage(s);
}

void ReportJSonErrorToSystemMessage(const string& strPrefix, const string& strWhat, const char* jsonData) {
	string s(jsonData);
	s = s.substr(0, 40);
	string str = strWhat;
	str += " ";
	str += s;

	ReportJSonErrorToSystemMessage(strPrefix, str);
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//
// 解析新浪实时数据
//
// 使用thread pool + coroutine并行解析，每个工作线程解析1/gl_concurrency_level数的数据，将解析后的数据存入缓存队列。
// 由于数据中不会包含相同股票的实时数据，故而不会出现同时操作同一个股票的问题，所以可以并行解析
// 只有工作线程都执行完后，本函数方可退出。
//
// 使用这种多线程模式与单线程模式相比，4个线程时运行时间减至1/3，更多的线程会减少运行时间，但效率降低。
//
// Note 调用此函数得线程不能使用thread_pool_executor或者background_executor生成，只能使用thread_executor生成，原因待查。
// Note 测试时，发现8个线程时效率反而下降，原因待查（CPU有8个物理核，16个逻辑核）。
//
//////////////////////////////////////////////////////////////////////////////////////////////////
result<bool> ParseSinaRTDataUsingCoroutine(shared_ptr<vector<string_view>> pvStringView) {
	vector<result<bool>> results;
	long DataSize = pvStringView->size();
	const auto chunk_size = 1 + std::div(DataSize, gl_concurrency_level).quot;
	//const auto chunk_size = 1 + DataSize / gl_concurrency_level;
	//for (long i = 0; i < gl_concurrency_level; i++) {
	for (auto i : std::views::iota(0, gl_concurrency_level)) {
		auto chunk_begin = i * chunk_size;
		auto chunk_end = chunk_begin + chunk_size;
		chunk_end = min(chunk_end, DataSize);
		auto result = gl_runtime.thread_pool_executor()->submit([pvStringView, chunk_begin, chunk_end] {
			for (auto j = chunk_begin; j < chunk_end; j++) {
				const auto pRTData = make_shared<CWebRTData>();
				pRTData->ParseSinaData(pvStringView->at(j));
				gl_qChinaMarketRTData.enqueue(pRTData); // Note 多个协程并行往里存时，无法通过size_approx()函数得到队列数量。
			}
			return true;
		});
		results.emplace_back(std::move(result));
	}

	bool succeed = true;
	for (auto& r : results) {
		succeed &= co_await r; // r.get(); wait for each task to complete
	}
	//bool succeed = ranges::all_of(results, [](auto& r) { return  r.get(); });
	co_return succeed;
}

void ParseSinaRTData(const CWebDataPtr& pWebData) {
	pWebData->ResetCurrentPos();
	const shared_ptr<vector<string_view>> pvStringView = make_shared<vector<string_view>>();
	while (!pWebData->IsLastDataParagraph()) {
		pvStringView->emplace_back(pWebData->GetCurrentSinaData());
	}
	if (pvStringView->empty()) return;
	ParseSinaRTDataUsingCoroutine(pvStringView).get();// 在这里堵塞
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 当所有被查询的股票皆为非上市股票时，腾讯实时股票服务器会返回一个21个字符长的字符串：v_pv_none_match=\"1\";\n
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
bool IsTengxunRTDataInvalid(const CWebDataPtr& pWebDataReceived) {
	const string_view sv = pWebDataReceived->GetStringView(0, 21);

	if (sv == "v_pv_none_match=\"1\";\n") {
		ASSERT(pWebDataReceived->GetBufferLength() == 21);
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//
// 解析腾讯实时数据
//
// 使用thread pool + coroutine并行解析，每个工作线程解析1/gl_concurrency_level数的数据，将解析后的数据存入缓存队列。
// 由于数据中不会包含相同股票的实时数据，故而不会出现同时操作同一个股票的问题，所以可以并行解析
//
// 使用这种多线程模式与单线程模式相比，4个线程时运行时间减至1/3，更多的线程会减少运行时间，但效率降低。
//
// Note 调用此函数得线程不能使用thread_pool_executor或者background_executor生成，只能使用thread_executor生成，原因待查。
//
//////////////////////////////////////////////////////////////////////////////////////////////////
concurrencpp::result<bool> ParseTengxunRTDataUsingCoroutine(shared_ptr<concurrencpp::thread_pool_executor> tpe, shared_ptr<vector<string_view>> pvStringView) {
	bool succeed = true;
	vector<concurrencpp::result<bool>> results;
	const long DataSize = pvStringView->size();
	const auto chunk_size = 1 + std::div(DataSize, gl_concurrency_level).quot;
	//const auto chunk_size = 1 + DataSize / gl_concurrency_level;
	//for (long i = 0; i < gl_concurrency_level; i++) {
	for (auto i : std::views::iota(0, gl_concurrency_level)) {
		auto chunk_begin = i * chunk_size;
		auto chunk_end = chunk_begin + chunk_size;
		chunk_end = min(chunk_end, DataSize);
		auto result = tpe->submit([pvStringView, chunk_begin, chunk_end] {
			try {
				for (auto j = chunk_begin; j < chunk_end; j++) {
					const auto pRTData = make_shared<CWebRTData>();
					const string_view sv = pvStringView->at(j);
					pRTData->ParseTengxunData(sv);
					gl_qChinaMarketRTData.enqueue(pRTData); // Note 多个协程并行往里存时，无法通过size_approx()函数得到队列数量。
				}
			} catch (exception& e) {
				ReportErrorToSystemMessage("ParseSinaData异常 ", e);
			}
			return true;
		});
		results.emplace_back(std::move(result));
	}
	for (auto& r : results) {
		succeed = succeed & co_await r;// r.get();
	}
	co_return succeed;
}

void ParseTengxunRTData(const CWebDataPtr& pWebData) {
	pWebData->ResetCurrentPos();
	shared_ptr<vector<string_view>> pvStringView = make_shared<vector<string_view>>();
	while (!pWebData->IsLastDataParagraph()) {
		pvStringView->emplace_back(pWebData->GetCurrentTengxunData());
	}
	if (pvStringView->empty()) return;
	ParseTengxunRTDataUsingCoroutine(gl_runtime.thread_pool_executor(), pvStringView).get(); // 等待线程执行完后方继续。
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
// 腾讯日线数据结构：
// {
// "code":0,
// "msg":"",
// "data": { "sh600601": { "day":	[
//																	["2023-01-19","2.550","2.600","2.610","2.550","86162.000"],
//																	["2023-01-20","2.600","2.620","2.620","2.590","100735.000"]
//																],
//													"qt":{},
//													"mx_price":{"mx":[],"price":[]},
//													"prec":"2.560",
//													"version":"16"
//												}
//					}
// }
//
// 使用simdjson解析速度release模式下比Nholmann json快50%，但debug模式下慢一倍。
//
////////////////////////////////////////////////////////////////////////////////////////////////
CDayLinesPtr ParseTengxunDayLine(const string_view& svData, const string& strStockCode) {
	auto pvDayLine = make_shared<vector<CDayLine>>();
	pvDayLine->reserve(2000);

	const string strStockSymbol = XferTengxunToStandard(strStockCode);
	try {
		string_view sv;
		long lLastClose = 0;
		ondemand::parser parser;
		ondemand::document doc;

		//const padded_string_view jsonPaddedView(svData, svData.length()); // Note 此时的svData带有长度为SIMDJSON_PADDING长度的后缀
		//doc = parser.iterate(jsonPaddedView).value();
		const padded_string jsonPadded(svData);
		doc = parser.iterate(jsonPadded).value();
		auto a1 = doc["data"];
		auto a2 = a1[strStockCode];
		auto a3 = a2["day"];
		auto a4 = a3.get_array();
		auto dayArray = a4.value();
		//ondemand::array dayArray = doc["data"][strStockCode]["day"].get_array().value(); // 使用索引strStockCode找到日线数组
		// 以下为不使用索引strStockCode找到日线数组的方法
		//ondemand::value data = doc["data"];
		//ondemand::field field = *data.get_object().begin();
		//ondemand::value stock = field.value();
		//ondemand::array dayArray = stock["day"].get_array();
		for (auto dayLine : dayArray) {
			CDayLine dayLine2;
			dayLine2.SetStockSymbol(strStockSymbol);
			dayLine2.SetLastClose(lLastClose);
			ondemand::array_iterator it = dayLine.get_array().begin();
			ondemand::value item = (*it).value();
			sv = simdjsonGetStringView(item);
			string str1(sv.data(), sv.length()); // 这里需要转换一下，直接使用string_view会导致内存溢出
			istringstream ss(str1);
			chrono::local_days ld;
			ss >> chrono::parse("%F", ld);
			dayLine2.SetDate(ld);
			item = (*++it).value();
			sv = simdjsonGetStringView(item);
			dayLine2.SetOpen(Str2Long(sv, 3));
			item = (*++it).value();
			sv = simdjsonGetStringView(item);
			const long lClose = Str2Long(sv, 3);
			dayLine2.SetClose(lClose);
			lLastClose = lClose;
			item = (*++it).value();
			sv = simdjsonGetStringView(item);
			dayLine2.SetHigh(Str2Long(sv, 3));
			item = (*++it).value();
			sv = simdjsonGetStringView(item);
			dayLine2.SetLow(Str2Long(sv, 3));
			item = (*++it).value();
			sv = simdjsonGetStringView(item);
			dayLine2.SetVolume(atof(sv.data()) * 100);

			pvDayLine->push_back(dayLine2);
		}
	} catch (nlohmannJson::exception&) {
		return pvDayLine;
	}
	return pvDayLine;
}

//
// 腾讯日线数据结构：
// {
// "code":0,
// "msg":"",
// "data":
//   {
//   "sh600601":
//      { "day":
//				[ ["2023-01-19","2.550","2.600","2.610","2.550","86162.000"],
//					["2023-01-20","2.600","2.620","2.620","2.590","100735.000"]],
//				"qt":{},
//				"mx_price":{"mx":[],"price":[]},
//				"prec":"2.560",
//				"version":"16"
//		  }
//	 }
// }
//
CDayLineWebDataPtr ParseTengxunDayLine(const CWebDataPtr& pWebData) {
	auto pDayLineData = make_shared<CDayLineWebData>();
	const string strSymbol = pWebData->GetStockCode();
	ASSERT(gl_dataContainerChinaStock.IsSymbol(strSymbol));
	const string_view svData = pWebData->GetStringView();

	const shared_ptr<vector<CDayLine>> pvDayLine = ParseTengxunDayLine(svData, XferStandardToTengxun(pWebData->GetStockCode()));
	std::ranges::sort(*pvDayLine, [](const CDayLine& pData1, const CDayLine& pData2) { return pData1.GetDate() < pData2.GetDate(); });
	for (auto& dayLine : *pvDayLine) {
		dayLine.SetStockSymbol(strSymbol);
		pDayLineData->AppendDayLine(dayLine);
	}
	pDayLineData->SetStockCode(strSymbol);
	return pDayLineData;
}

bool CreateJsonWithNlohmann(nlohmannJson& js, const std::string& s, const long lBeginPos, const long lEndPos) {
	try {
		js = nlohmannJson::parse(s.begin() + lBeginPos, s.end() - lEndPos);
	} catch (nlohmannJson::parse_error&) {
		js.clear();
		return false;
	}
	return true;
}
