//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// json数据解析的任务皆位于此文件中。
// 解析是目前最费时的函数，故而DEBUG模式时也要进行全局优化（/GL），只有这样才能在规定时间内处理完数据。
// Netease实时数据是目前最需要关注的，每300毫秒接收900个数据时，系统基本上独占一个核心处理器了。如果再有耗时的任务，就需要
// 将其分配到其他核心处理器上，这样就会导致系统的响应时间增加，而且系统的负载也会增加。
//
// 在DEBUG模式下，boost PTree速度比Nlohmann json快，但Release模式下nlohmann json的速度比boost ptree快50%左右。
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include "JsonParse.h"

#include"WebData.h"
#include"WebRTData.h"

#include "ChinaStockCodeConverter.h"
#include "InfoReport.h"
import FireBird.Thread;

import simdjson.GetValue;
#include"simdjson.h"
using namespace simdjson;

#include"ConvertToString.h"
#include"SystemMessage.h"

#include"JsonGetValue.h"
#include"NlohmannJsonDeclaration.h"

#include"spdlog/spdlog.h"


#include "ChinaMarket.h"
#undef max // 包含concurrencpp.h之前，需要注销max的定义
#include"concurrencpp/concurrencpp.h"
using namespace concurrencpp;

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// 将输入的字符串转变成放大了10^power倍的长整型。要确保精确地转换，不使用浮点数过渡。
//
///////////////////////////////////////////////////////////////////////////////////////////////////
long StrToDecimal(const string_view& svData, int power) {
	char buffer[100]{};
	const auto iPointPosition = svData.find_first_of('.');
	if (iPointPosition == std::string_view::npos) {		// 没有小数点？
		long l = 10;
		if (power > 0) {
			for (int i = 1; i < power; i++) l *= 10;
		}
		else l = 1;
		return atol(svData.data()) * l;
	}
	// 有小数点
	int i;
	for (i = 0; i < iPointPosition; i++) {
		buffer[i] = svData.at(i);
	}
	while (++i < svData.length()) {
		buffer[i - 1] = svData.at(i);
		if (--power < 0) break;
	}
	while (power-- > 0) {
		buffer[i - 1] = '0';
		i++;
	}
	buffer[i - 1] = 0x000;
	ASSERT(i < 99);
	return atol(buffer);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 出现exception时，上级调用函数负责处理
//
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
string_view GetNextField(const string_view& svData, size_t& lCurrentPos, char delimiter) {
	const string_view sv(svData.data() + lCurrentPos, svData.length() - lCurrentPos);
	const auto lEnd = sv.find_first_of(delimiter);
	if (lEnd > sv.length()) throw exception("GetNextField() out of range"); // 没找到的话抛出异常
	lCurrentPos += lEnd + 1; // 将当前位置移至本数据之后
	return string_view(sv.data(), lEnd);
}

void ReportJsonError(const json::parse_error& e, const std::string& s) {
	char buffer[180]{}, buffer2[100]{};
	int i;
	CString str = e.what();
	gl_systemMessage.PushErrorMessage(_T("Nlohmann JSon Reading Error ") + str);
	for (i = 0; i < 180; i++) buffer[i] = 0x000;
	for (i = 0; i < 180; i++) {
		if ((e.byte - 90 + i) < s.size()) { buffer[i] = s.at(e.byte - 90 + i); }
		else break;
	}
	sprintf_s(buffer2, _T("%d  "), static_cast<long>(s.size()));
	str = buffer2;
	sprintf_s(buffer2, _T("%d  "), static_cast<long>(e.byte));
	str += buffer2;
	sprintf_s(buffer2, _T("%d  "), i);
	str += buffer2;
	str += buffer;
	gl_systemMessage.PushErrorMessage(str);
}

void ReportJSonErrorToSystemMessage(const CString& strPrefix, const CString& strWhat) {
	gl_systemMessage.PushErrorMessage(strPrefix + strWhat);
}

void ReportJSonErrorToSystemMessage(const CString& strPrefix, const CString& strWhat, const char* jsonData) {
	string s(jsonData);
	s = s.substr(0, 40);
	CString str = strWhat;
	str += " ";
	str += s.c_str();

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
// 使用这种多线程模式与单线程模式相比，速度快1倍以上。
//
// Note 调用此函数的线程不能使用thread_pool_executor或者background_executor生成，只能使用thread_executor生成，原因待查。
// Note 使用max_concurrency_level时，实际效果大致为4个。估计与调度有关，即CPU的占比增至100%，但解析时间并没有减少。故而将gl_concurrency_level设为4.
//
//////////////////////////////////////////////////////////////////////////////////////////////////
result<bool> ParseSinaRTDataUsingCoroutine(shared_ptr<thread_pool_executor> tpe, shared_ptr<vector<string_view>> pvStringView) {
	vector<result<bool>> results;
	const auto DataSize = pvStringView->size();
	const auto chunk_size = 1 + DataSize / gl_concurrency_level;
	for (int i = 0; i < gl_concurrency_level; i++) {
		auto chunk_begin = i * chunk_size;
		auto chunk_end = chunk_begin + chunk_size;
		if (chunk_end > DataSize) chunk_end = DataSize;
		auto result = tpe->submit([pvStringView, chunk_begin, chunk_end] {
			try {
				for (auto j = chunk_begin; j < chunk_end; j++) {
					const auto pRTData = make_shared<CWebRTData>();
					pRTData->ParseSinaData(pvStringView->at(j));
					gl_qChinaMarketRTData.enqueue(pRTData); // Note 多个协程并行往里存时，无法通过size_approx()函数得到队列数量。
				}
			} catch (exception& e) {
				ReportErrorToSystemMessage(_T("ParseSinaData异常 "), e);
			}
			return true;
		});
		results.emplace_back(std::move(result));
	}
	bool succeed = true;
	for (auto& r : results) {
		succeed = succeed & co_await r;
	}
	co_return succeed;
}

void ParseSinaRTData(const CWebDataPtr& pWebData) {
	pWebData->ResetCurrentPos();
	const shared_ptr<vector<string_view>> pvStringView = make_shared<vector<string_view>>();
	try {
		while (!pWebData->IsLastDataParagraph()) {
			pvStringView->emplace_back(pWebData->GetCurrentSinaData());
		}
	} catch (exception& e) {
		ReportErrorToSystemMessage(_T("ParseSinaData异常 "), e);
	}
	auto result = ParseSinaRTDataUsingCoroutine(gl_runtime.thread_pool_executor(), pvStringView); //Note 必须使用thread_pool_executor
	result.get();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 当所有被查询的股票皆为非上市股票时，腾讯实时股票服务器会返回一个21个字符长的字符串：v_pv_none_match=\"1\";\n
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
bool IsTengxunRTDataInvalid(const CWebDataPtr& pWebDataReceived) {
	const string_view sv = pWebDataReceived->GetStringView(0, 21);

	if (sv.compare(_T("v_pv_none_match=\"1\";\n")) == 0) {
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
// 使用这种多线程模式与单线程模式相比，速度快1倍以上。
//
// Note 调用此函数得线程不能使用thread_pool_executor或者background_executor生成，只能使用thread_executor生成，原因待查。
// Note 使用max_concurrency_level时，实际效果大致为4个。估计与调度有关，即CPU的占比增至100%，但解析时间并没有减少。故而将gl_concurrency_level设为4.
//
//////////////////////////////////////////////////////////////////////////////////////////////////
concurrencpp::result<bool> ParseTengxunRTDataUsingCoroutine(shared_ptr<concurrencpp::thread_pool_executor> tpe, shared_ptr<vector<string_view>> pvStringView) {
	bool succeed = true;
	vector<concurrencpp::result<bool>> results;
	const auto DataSize = pvStringView->size();
	const auto chunk_size = 1 + DataSize / gl_concurrency_level;
	for (int i = 0; i < gl_concurrency_level; i++) {
		auto chunk_begin = i * chunk_size;
		auto chunk_end = chunk_begin + chunk_size;
		if (chunk_end > DataSize) chunk_end = DataSize;
		auto result = tpe->submit([pvStringView, chunk_begin, chunk_end] {
			try {
				for (auto j = chunk_begin; j < chunk_end; j++) {
					const auto pRTData = make_shared<CWebRTData>();
					const string_view sv = pvStringView->at(j);
					pRTData->ParseTengxunData(sv);
					gl_qChinaMarketRTData.enqueue(pRTData); // Note 多个协程并行往里存时，无法通过size_approx()函数得到队列数量。
				}
			} catch (exception& e) {
				ReportErrorToSystemMessage(_T("ParseSinaData异常 "), e);
			}
			return true;
		});
		results.emplace_back(std::move(result));
	}
	for (auto& r : results) {
		succeed = succeed & r.get();
	}
	co_return succeed;
}

void ParseTengxunRTData(const CWebDataPtr& pWebData) {
	pWebData->ResetCurrentPos();
	const shared_ptr<vector<string_view>> pvStringView = make_shared<vector<string_view>>();
	try {
		while (!pWebData->IsLastDataParagraph()) {
			pvStringView->emplace_back(pWebData->GetCurrentTengxunData());
		}
	} catch (exception& e) {
		ReportErrorToSystemMessage(_T("ParseTengxunData异常 "), e);
	}

	auto result = ParseTengxunRTDataUsingCoroutine(gl_runtime.thread_pool_executor(), pvStringView); //Note 必须使用thread_pool_executor
	result.get(); // 等待线程执行完后方继续。
}

///////////////////////////////////////////////////////////////////////////////////////////////
//
// 处理从网易日线服务器上读取的股票日线数据。
// 数据制式为： 日期,股票代码,名称,收盘价,最高价,最低价,开盘价,前收盘,涨跌额,换手率,成交量,成交金额,总市值,流通市值\r\n
//
// 日线数据是逆序的，最新日期的在前面。
//
/////////////////////////////////////////////////////////////////////////////////////////////////
CDayLineWebDataPtr ParseNeteaseDayLine(const CWebDataPtr& pWebData) {
	auto pData = make_shared<CDayLineWebData>();

	pData->TransferWebDataToBuffer(pWebData);
	pData->ProcessNeteaseDayLineData(); //pData的日线数据是逆序的，最新日期的在前面。

	return pData;
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
shared_ptr<vector<CDayLinePtr>> ParseTengxunDayLine(const string_view& svData, const CString& strStockCode) {
	auto pvDayLine = make_shared<vector<CDayLinePtr>>();
	const CString strStockSymbol = XferTengxunToStandard(strStockCode);
	try {
		long year, month, day;
		string_view sv;
		long lLastClose = 0;
		ondemand::parser parser;
		ondemand::document doc;

		//const padded_string_view jsonPaddedView(svData, svData.length()); // Note 此时的svData带有长度为SIMDJSON_PADDING长度的后缀
		//doc = parser.iterate(jsonPaddedView).value();
		const padded_string jsonPadded(svData);
		doc = parser.iterate(jsonPadded).value();
		ondemand::array dayArray = doc["data"][strStockCode]["day"].get_array().value(); // 使用索引strStockCode找到日线数组
		// 以下为不使用索引strStockCode找到日线数组的方法
		//ondemand::value data = doc["data"];
		//ondemand::field field = *data.get_object().begin();
		//ondemand::value stock = field.value();
		//ondemand::array dayArray = stock["day"].get_array();
		for (auto dayLine : dayArray) {
			auto pDayLine = make_shared<CDayLine>();
			pDayLine->SetStockSymbol(strStockSymbol);
			pDayLine->SetLastClose(lLastClose);
			ondemand::array_iterator it = dayLine.get_array().begin();
			ondemand::value item = (*it).value();
			sv = jsonGetStringView(item);
			string str1(sv.data(), sv.length()); // 这里需要转换一下，直接使用string_view会导致内存溢出
			sscanf_s(str1.data(), _T("%4d-%02d-%02d"), &year, &month, &day);
			pDayLine->SetDate(year * 10000 + month * 100 + day);
			item = (*++it).value();
			sv = jsonGetStringView(item);
			pDayLine->SetOpen(StrToDecimal(sv, 3));
			item = (*++it).value();
			sv = jsonGetStringView(item);
			const long lClose = StrToDecimal(sv, 3);
			pDayLine->SetClose(lClose);
			lLastClose = lClose;
			item = (*++it).value();
			sv = jsonGetStringView(item);
			pDayLine->SetHigh(StrToDecimal(sv, 3));
			item = (*++it).value();
			sv = jsonGetStringView(item);
			pDayLine->SetLow(StrToDecimal(sv, 3));
			item = (*++it).value();
			sv = jsonGetStringView(item);
			pDayLine->SetVolume(atof(sv.data()) * 100);

			pvDayLine->push_back(pDayLine);
		}
	} catch (json::exception&) {
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
	const CString strSymbol = pWebData->GetStockCode();
	ASSERT(gl_dataContainerChinaStock.IsSymbol(strSymbol));
	const CString strDisplaySymbol = gl_dataContainerChinaStock.GetStock(strSymbol)->GetDisplaySymbol();
	const string_view svData = pWebData->GetStringView(0, pWebData->GetBufferLength());

	const shared_ptr<vector<CDayLinePtr>> pvDayLine = ParseTengxunDayLine(svData, XferStandardToTengxun(pWebData->GetStockCode()));
	std::ranges::sort(*pvDayLine, [](const CDayLinePtr& pData1, const CDayLinePtr& pData2) { return pData1->GetMarketDate() < pData2->GetMarketDate(); });
	for (const auto& pDayLine : *pvDayLine) {
		pDayLine->SetStockSymbol(strSymbol);
		pDayLine->SetDisplaySymbol(strDisplaySymbol);
		pDayLineData->AppendDayLine(pDayLine);
	}
	return pDayLineData;
}

bool CreateJsonWithNlohmann(json& js, const std::string& s, const long lBeginPos, const long lEndPos) {
	try {
		js = json::parse(s.begin() + lBeginPos, s.end() - lEndPos);
	} catch (json::parse_error&) {
		js.clear();
		return false;
	}
	return true;
}

bool CreateJsonWithNlohmann(json& js, CString& str, const long lBeginPos, const long lEndPos) {
	const string s = str.GetBuffer();
	try {
		js = json::parse(s.begin() + lBeginPos, s.end() - lEndPos);
	} catch (json::parse_error&) {
		js.clear();
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//
// 从网络文件file中读取网易制式实时数据，返回值是所读数据是否出现格式错误。
// 开始处为第一个{，结束处为倒数第二个}。如果尚有数据需处理，则被处理的字符为','；如果没有数据了，则被处理的字符为' '。
//
// 要获取最新行情，访问数据接口：http://api.money.126.net/data/feed/0601872
//
// _ntes_quote_callback({"0601872":{"code": "0601872", "percent": 0.038251, "high": 5.72, "askvol3": 311970, "askvol2": 257996,
//                      "askvol5": 399200, "askvol4": 201000, "price": 5.7, "open": 5.53, "bid5": 5.65, "bid4": 5.66, "bid3": 5.67,
//                       "bid2": 5.68, "bid1": 5.69, "low": 5.51, "updown": 0.21, "type": "SH", "symbol": "601872", "status": 0,
//                       "ask4": 5.73, "bidvol3": 234700, "bidvol2": 166300, "bidvol1": 641291, "update": "2019/11/04 15:59:54",
//                       "bidvol5": 134500, "bidvol4": 96600, "yestclose": 5.49, "askvol1": 396789, "ask5": 5.74, "volume": 78750304,
//                       "ask1": 5.7, "name": "\u62db\u5546\u8f6e\u8239", "ask3": 5.72, "ask2": 5.71, "arrow": "\u2191",
//                        "time": "2019/11/04 15:59:52", "turnover": 443978974} });
//
//
//  网易中文股票名称的制式不明，暂时不使用（由于boost ptree对中文的支持不足，其只支持utf8制式，导致提取中文字符时出现乱码）。
// 现在采用wstring和CStringW两次过渡，就可以正常显示了。
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void ParseOneNeteaseRTData(const json::iterator& it, const CWebRTDataPtr& pWebRTData) {
	string strTime;
	CString strSymbol4;
	json js = it.value();
	const string symbolName = it.key();

	try {
		strSymbol4 = XferNeteaseToStandard(symbolName);
		pWebRTData->SetSymbol(strSymbol4);
		const string sName = jsonGetString(js, "name");
		pWebRTData->SetStockName(XferToCString(sName)); // 将utf-8字符集转换为多字节字符集
		strTime = jsonGetString(js, _T("time"));
		string strSymbol2 = jsonGetString(js, _T("code"));
		std::stringstream ss(strTime);
		chrono::sys_seconds tpTime;
		chrono::from_stream(ss, "%Y/%m/%d %T", tpTime);
		tpTime -= gl_pChinaMarket->GetMarketTimeZoneOffset();
		pWebRTData->SetTimePoint(tpTime);
		auto tt = tpTime.time_since_epoch().count();
		pWebRTData->SetTransactionTime(tt);
	} catch (json::exception& e) {// 结构不完整
		// do nothing
		CString strError2 = strSymbol4;
		strError2 += _T(" ");
		strError2 += e.what();
		gl_systemMessage.PushErrorMessage(strError2);
	}
	try {
		pWebRTData->SetVolume(jsonGetLongLong(js, _T("volume")));
		pWebRTData->SetAmount(jsonGetDouble(js, _T("turnover")));
		pWebRTData->SetHigh(static_cast<long>(jsonGetDouble(js, _T("high")) * 1000));
		pWebRTData->SetLow(static_cast<long>(jsonGetDouble(js, _T("low")) * 1000));
		pWebRTData->SetNew(static_cast<long>(jsonGetDouble(js, _T("price")) * 1000));
		pWebRTData->SetLastClose(static_cast<long>(jsonGetDouble(js, _T("yestclose")) * 1000));
		pWebRTData->SetOpen(static_cast<long>(jsonGetDouble(js, _T("open")) * 1000));

		pWebRTData->SetVBuy(0, jsonGetLong(js, _T("bidvol1")));
		pWebRTData->SetVBuy(1, jsonGetLong(js, _T("bidvol2")));
		pWebRTData->SetVBuy(2, jsonGetLong(js, _T("bidvol3")));
		pWebRTData->SetVBuy(3, jsonGetLong(js, _T("bidvol4")));
		pWebRTData->SetVBuy(4, jsonGetLong(js, _T("bidvol5")));
		pWebRTData->SetVSell(0, jsonGetLong(js, _T("askvol1")));
		pWebRTData->SetVSell(1, jsonGetLong(js, _T("askvol2")));
		pWebRTData->SetVSell(2, jsonGetLong(js, _T("askvol3")));
		pWebRTData->SetVSell(3, jsonGetLong(js, _T("askvol4")));
		pWebRTData->SetVSell(4, jsonGetLong(js, _T("askvol5")));

		pWebRTData->SetPSell(0, static_cast<long>(jsonGetDouble(js, _T("ask1")) * 1000));
		pWebRTData->SetPSell(1, static_cast<long>(jsonGetDouble(js, _T("ask2")) * 1000));
		pWebRTData->SetPSell(2, static_cast<long>(jsonGetDouble(js, _T("ask3")) * 1000));
		pWebRTData->SetPSell(3, static_cast<long>(jsonGetDouble(js, _T("ask4")) * 1000));
		pWebRTData->SetPSell(4, static_cast<long>(jsonGetDouble(js, _T("ask5")) * 1000));
		pWebRTData->SetPBuy(0, static_cast<long>(jsonGetDouble(js, _T("bid1")) * 1000));
		pWebRTData->SetPBuy(1, static_cast<long>(jsonGetDouble(js, _T("bid2")) * 1000));
		pWebRTData->SetPBuy(2, static_cast<long>(jsonGetDouble(js, _T("bid3")) * 1000));
		pWebRTData->SetPBuy(3, static_cast<long>(jsonGetDouble(js, _T("bid4")) * 1000));
		pWebRTData->SetPBuy(4, static_cast<long>(jsonGetDouble(js, _T("bid5")) * 1000));

		pWebRTData->CheckNeteaseRTDataActive();
	} catch (json::exception&) {// 非活跃股票（已下市等）
		pWebRTData->SetActive(false);
	}
}

shared_ptr<vector<CWebRTDataPtr>> ParseNeteaseRTData(json* pjs) {
	auto pvWebRTData = make_shared<vector<CWebRTDataPtr>>();

	for (auto it = pjs->begin(); it != pjs->end(); ++it) {
		auto pRTData = make_shared<CWebRTData>();
		pRTData->SetDataSource(NETEASE_RT_WEB_DATA_);
		ParseOneNeteaseRTData(it, pRTData);
		pRTData->CheckNeteaseRTDataActive();
		pvWebRTData->push_back(pRTData);
	}
	return pvWebRTData;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//
// 从网络文件file中读取网易制式实时数据，返回值是所读数据是否出现格式错误。
// 开始处为第一个{，结束处为倒数第二个}。如果尚有数据需处理，则被处理的字符为','；如果没有数据了，则被处理的字符为' '。
//
// 要获取最新行情，访问数据接口：http://api.money.126.net/data/feed/0601872
//
// _ntes_quote_callback({"0601872":{"code": "0601872", "percent": 0.038251, "high": 5.72, "askvol3": 311970, "askvol2": 257996,
//                      "askvol5": 399200, "askvol4": 201000, "price": 5.7, "open": 5.53, "bid5": 5.65, "bid4": 5.66, "bid3": 5.67,
//                       "bid2": 5.68, "bid1": 5.69, "low": 5.51, "updown": 0.21, "type": "SH", "symbol": "601872", "status": 0,
//                       "ask4": 5.73, "bidvol3": 234700, "bidvol2": 166300, "bidvol1": 641291, "update": "2019/11/04 15:59:54",
//                       "bidvol5": 134500, "bidvol4": 96600, "yestclose": 5.49, "askvol1": 396789, "ask5": 5.74, "volume": 78750304,
//                       "ask1": 5.7, "name": "\u62db\u5546\u8f6e\u8239", "ask3": 5.72, "ask2": 5.71, "arrow": "\u2191",
//                        "time": "2019/11/04 15:59:52", "turnover": 443978974} });
//
// 目前采用下标方法解析数据，其速度能达到Nlohmann json的三倍以上。
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////
shared_ptr<vector<CWebRTDataPtr>> ParseNeteaseRTDataWithSimdjson(string_view svJsonData) {
	string symbolCode, strTime;
	auto pvWebRTData = make_shared<vector<CWebRTDataPtr>>();
	try {
		ondemand::parser parser;
		const padded_string jsonPadded(svJsonData);
		ondemand::document doc = parser.iterate(jsonPadded).value();
		for (ondemand::field item_key : doc.get_object()) {
			auto pWebRTData = make_shared<CWebRTData>();
			pWebRTData->SetDataSource(NETEASE_RT_WEB_DATA_);
			//auto key = item_key.key();
			ondemand::object item = item_key.value();
			CString strSymbol4 = XferNeteaseToStandard(jsonGetStringView(item, _T("code")));
			pWebRTData->SetSymbol(strSymbol4);
			pWebRTData->SetVSell(0, jsonGetInt64(item, _T("askvol1")));
			pWebRTData->SetVSell(2, jsonGetInt64(item, _T("askvol3")));
			pWebRTData->SetVSell(1, jsonGetInt64(item, _T("askvol2")));
			pWebRTData->SetVSell(4, jsonGetInt64(item, _T("askvol5")));
			pWebRTData->SetVSell(3, jsonGetInt64(item, _T("askvol4")));
			pWebRTData->SetNew(StrToDecimal(jsonGetRawJsonToken(item, _T("price")), 3));
			pWebRTData->SetOpen(StrToDecimal(jsonGetRawJsonToken(item, _T("open")), 3));
			pWebRTData->SetPBuy(4, StrToDecimal(jsonGetRawJsonToken(item, _T("bid5")), 3));
			pWebRTData->SetPBuy(3, StrToDecimal(jsonGetRawJsonToken(item, _T("bid4")), 3));
			pWebRTData->SetPBuy(2, StrToDecimal(jsonGetRawJsonToken(item, _T("bid3")), 3));
			pWebRTData->SetPBuy(1, StrToDecimal(jsonGetRawJsonToken(item, _T("bid2")), 3));
			pWebRTData->SetPBuy(0, StrToDecimal(jsonGetRawJsonToken(item, _T("bid1")), 3));
			pWebRTData->SetHigh(StrToDecimal(jsonGetRawJsonToken(item, _T("high")), 3));
			pWebRTData->SetLow(StrToDecimal(jsonGetRawJsonToken(item, _T("low")), 3));
			pWebRTData->SetVBuy(2, jsonGetInt64(item, _T("bidvol3")));
			pWebRTData->SetVBuy(0, jsonGetInt64(item, _T("bidvol1")));
			pWebRTData->SetVBuy(1, jsonGetInt64(item, _T("bidvol2")));
			pWebRTData->SetVBuy(4, jsonGetInt64(item, _T("bidvol5")));
			pWebRTData->SetVBuy(3, jsonGetInt64(item, _T("bidvol4")));
			pWebRTData->SetVolume(jsonGetInt64(item, _T("volume")));
			pWebRTData->SetPSell(4, StrToDecimal(jsonGetRawJsonToken(item, _T("ask5")), 3));
			pWebRTData->SetPSell(3, StrToDecimal(jsonGetRawJsonToken(item, _T("ask4")), 3));
			pWebRTData->SetPSell(0, StrToDecimal(jsonGetRawJsonToken(item, _T("ask1")), 3));

			string_view sNameView = jsonGetStringView(item, "name");
			string sName(sNameView);
			pWebRTData->SetStockName(XferToCString(sName)); // 将utf-8字符集转换为多字节字符集
			pWebRTData->SetPSell(2, StrToDecimal(jsonGetRawJsonToken(item, _T("ask3")), 3));
			pWebRTData->SetPSell(1, StrToDecimal(jsonGetRawJsonToken(item, _T("ask2")), 3));
			strTime = jsonGetStringView(item, _T("time"));
			std::stringstream ss(strTime);
			chrono::sys_seconds tpTime;
			chrono::from_stream(ss, "%Y/%m/d %T", tpTime);
			tpTime -= gl_pChinaMarket->GetMarketTimeZoneOffset();
			auto tt = tpTime.time_since_epoch().count();
			pWebRTData->SetTransactionTime(tt);
			pWebRTData->SetLastClose(StrToDecimal(jsonGetRawJsonToken(item, _T("yestclose")), 3));
			pWebRTData->SetAmount(StrToDecimal(jsonGetRawJsonToken(item, _T("turnover")), 0));

			pWebRTData->CheckNeteaseRTDataActive();
			pvWebRTData->push_back(pWebRTData);
		}
	} catch (simdjson_error& error) {
		const string sError = error.what();
		CString str = "Netease RT Data Error: ";
		str += sError.c_str();
		gl_systemMessage.PushErrorMessage(str);
	}
	return pvWebRTData;
}

shared_ptr<vector<CWebRTDataPtr>> ParseNeteaseRTDataWithSimdjson(const CWebDataPtr& pData) {
	return ParseNeteaseRTDataWithSimdjson(pData->GetStringView(21, pData->GetBufferLength() - 21 - 2)); // 网易json数据
}
