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

#include<string>
#include<memory>

#include "ChinaStockCodeConverter.h"
#include "InfoReport.h"
#include "JsonGetValue.h"

#include"simdjsonGetValue.h"
using namespace simdjson;

using namespace std;

void ReportJsonError(const json::parse_error& e, const std::string& s) {
	char buffer[180]{}, buffer2[100];
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
// 从网络文件file中读取新浪制式实时数据，返回值是所读数据是否出现格式错误。
// 新浪实时数据的格式为javascript
// 
// todo 考虑将pRTData->ReadSinaData函数改为线程模式并行执行解析任务。由于数据中不会包含相同股票的实时数据，故而不会出现同时操作同一个股票的问题，
// 所以可以并行解析
//
//////////////////////////////////////////////////////////////////////////////////////////////////
shared_ptr<vector<CWebRTDataPtr>> ParseSinaRTData(const CWebDataPtr& pWebData) {
	auto pvWebRTData = make_shared<vector<CWebRTDataPtr>>();
	try {
		pWebData->ResetCurrentPos();
		while (!pWebData->IsLastDataParagraph()) {
			const string_view svData = pWebData->GetCurrentSinaData();
			auto pRTData = make_shared<CWebRTData>();
			pRTData->ReadSinaData(svData);
			pvWebRTData->push_back(pRTData);
		}
	}
	catch (exception& e) {
		ReportErrorToSystemMessage(_T("ReadSinaData异常 "), e);
		return pvWebRTData;
	}
	return pvWebRTData;
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

//////////////////////////////////////////////////////////////////////////////////////////
//
// 腾讯实时数据，如果遇到被查询股票代码为非上市时，只是简单略过，不返回数据。故而查询900个股票，返回的数据量要小于900.
// 只有当所有的查询股票皆为非上市时，才返回一个21个字符串：v_pv_none_match=\"1\";\n
//
//
// 从网络文件file中读取腾讯制式实时数据，返回值是所读数据是否出现格式错误。
//
// 要获取最新行情，访问数据接口：http://qt.gtimg.cn/q=sz002818
//
// 20200101日结构：
// v_sz000001="51~平安银行~000001~15.59~15.90~15.75~1046363~518391~527971~
//             15.58~2365~15.57~802~15.56~1855~15.55~2316~15.54~320~15.59~661~15.60~15381~15.61~3266~15.62~450~15.63~520~~
//             20190930154003~-0.31~-1.95~15.89~15.57~15.59/1046363/1645828527~1046363~164583~0.54~11.27~~
//             15.89~15.57~2.01~3025.36~3025.38~1.15~17.49~14.31~
//             0.73~-12617~15.73~9.82~12.19~~~1.24~164582.85~0.00~0~~GP-A~68.91~~0.82";\n
//
// 20230627日结构：
// v_sh600026="1~中远海能~600026~12.25~12.40~12.34~366190~150067~216123~
//						 12.24~36~12.23~803~12.22~501~12.21~905~12.20~1899~12.25~1026~12.26~1158~12.27~645~12.28~615~12.29~139~~
//						 20230627155915~-0.15~-1.21~12.44~11.95~12.25/366190/445074382~366190~44507~1.05~23.12~~
//						 12.44~11.95~3.95~425.66~584.42~1.79~13.64~11.16~1.01~561~12.15~13.33~40.10~~~1.53~44507.4382~0.0000~0~~GP-A~1.66~-8.85~0.00~
//						 7.73~4.16~21.04~8.58~1.16~7.64~-11.81~3474776395~4770776395~7.26~- 34.49~3474776395~~~17.22~0.16~~CNY~0~___D__F__N";\n
//
// 0: 市场（上海：1，深圳：51, 港股：100， 美股：200）。
// 1 : 名字
// 2 : 代码
// 3 : 现价
// 4 : 昨收
// 5 : 今开
// 6 : 成交量（手）
// 7 : 外盘
// 8 : 内盘 （第一行结束）
// 9 : 买一
// 10 : 买一量（手）
// 11 - 18 : 买二 - 买五
// 19 : 卖一
// 20 : 卖一量
// 21 - 28 : 卖二 - 卖五
// 29 : 最近逐笔成交 ( 第二行结束）
// 30 : 时间（此时间为当地市场时间，此处为东八区北京标准时间）
// 31 : 涨跌
// 32 : 涨跌 %
// 33 : 最高
// 34 : 最低
// 35 : 价格 / 成交量（手） / 成交额（元）。 可以使用此处的数据代替36、37处的数据，这样就可以使用腾讯实时数据了。
// 36 : 成交量（手）
// 37 : 成交额（万）
// 38 : 换手率
// 39 : 市盈率
// 40 : /       (第三行结束）
// 41 : 最高
// 42 : 最低
// 43 : 振幅
// 44 : 流通市值(单位为：亿）
// 45 : 总市值（单位为：亿）
// 46 : 市净率
// 47 : 涨停价
// 48 : 跌停价   （第四行结束）
// 49 ：量比
// 50 :
// 51 : 均价
// 52 ：动态市盈率
// 53 ：静态市盈率
// 54 ：
// 55 ：
// 56 ：
// 57 ：成交额
// 58 ：
// 59 ：
// 60 ：
// 61 ：GP-A
// 62 ：
// 63 ：
// 64 ：
// 65 ：
// 66 ：
//
//
// 腾讯实时数据中，成交量的单位为手，无法达到计算所需的精度（股），故而只能作为数据补充之用。
//
//////////////////////////////////////////////////////////////////////////////////////////////////
shared_ptr<vector<CWebRTDataPtr>> ParseTengxunRTData(const CWebDataPtr& pWebData) {
	auto pvWebRTData = make_shared<vector<CWebRTDataPtr>>();
	try {
		pWebData->ResetCurrentPos();
		if (IsTengxunRTDataInvalid(pWebData)) return pvWebRTData; // 处理这21个字符串的函数可以放在这里，也可以放在最前面。
		while (!pWebData->IsLastDataParagraph()) {
			auto pRTData = make_shared<CWebRTData>();
			const string_view svData = pWebData->GetCurrentTengxunData();
			pRTData->ReadTengxunData(svData);
			pvWebRTData->push_back(pRTData);
		}
	}
	catch (exception& e) {
		ReportErrorToSystemMessage(_T("ReadTengxunData异常 "), e);
		return pvWebRTData;
	}
	return pvWebRTData;
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
		double dLastClose = 0;
		const simdjson::padded_string jsonPadded(svData);
		ondemand::parser parser;
		ondemand::document doc = parser.iterate(jsonPadded);
		// 不使用索引strStockCode找到日线数组的方法
		//ondemand::value data = doc["data"];
		//ondemand::field field = *data.get_object().begin();
		//ondemand::value stock = field.value();
		//ondemand::array dayArray = stock["day"].get_array();
		ondemand::array dayArray = doc["data"][strStockCode]["day"].get_array(); // 使用strStockCode找到日线数组
		for (ondemand::value dayLine : dayArray) {
			auto pDayLine = make_shared<CDayLine>();
			pDayLine->SetStockSymbol(strStockSymbol);
			pDayLine->SetLastClose(dLastClose * 1000);
			ondemand::array_iterator it = dayLine.get_array().begin();
			ondemand::value item = *it;
			sv = jsonGetStringView(item);
			sscanf_s(sv.data(), _T("%4d-%02d-%02d"), &year, &month, &day);
			pDayLine->SetDate(year * 10000 + month * 100 + day);
			item = *++it;
			sv = jsonGetStringView(item);
			pDayLine->SetOpen(atof(sv.data()) * 1000);
			item = *++it;
			sv = jsonGetStringView(item);
			const double dClose = atof(sv.data());
			pDayLine->SetClose(dClose * 1000);
			dLastClose = dClose;
			item = *++it;
			sv = jsonGetStringView(item);
			pDayLine->SetHigh(atof(sv.data()) * 1000);
			item = *++it;
			sv = jsonGetStringView(item);
			pDayLine->SetLow(atof(sv.data()) * 1000);
			item = *++it;
			sv = jsonGetStringView(item);
			pDayLine->SetVolume(atof(sv.data()) * 100);

			pvDayLine->push_back(pDayLine);
		}
	}
	catch (json::exception&) {
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
	const CString strDisplaySymbol = gl_dataContainerChinaStock.GetStock(strSymbol)->GetDisplaySymbol();
	const string_view svData = pWebData->GetStringView(0, pWebData->GetBufferLength());

	const shared_ptr<vector<CDayLinePtr>> pvDayLine = ParseTengxunDayLine(svData, XferStandardToTengxun(pWebData->GetStockCode()));
	ranges::sort(*pvDayLine, [](const CDayLinePtr& pData1, const CDayLinePtr& pData2) { return pData1->GetMarketDate() < pData2->GetMarketDate(); });
	for (const auto& pDayLine : *pvDayLine) {
		pDayLine->SetStockSymbol(strSymbol);
		pDayLine->SetDisplaySymbol(strDisplaySymbol);
		pDayLineData->AppendDayLine(pDayLine);
	}
	return pDayLineData;
}
