//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// json数据解析的任务皆位于此文件中。
// json解析是目前最费时的函数，故而在DEBUG模式时也要进行全局优化（/GL），只有这样才能在规定时间内处理完数据。
// Netease实时数据是目前最需要关注的，每300毫秒接收900个数据时，系统基本上独占一个核心处理器了。如果再有耗时的任务，就需要
// 将其分配到其他核心处理器上，这样就会导致系统的响应时间增加，而且系统的负载也会增加。
//
// 在DEBUG模式下，boost PTree速度比Nlohmann json快，但Release模式下nlohmann json的速度比boost ptree快50%以上（许多时候是快一倍以上）。
// debug模式下，解析900个NeteaseRTData，nlohmann json耗时160毫秒，PTree耗时120毫秒。
// release模式下，解析900个NeteaseRTData，nlohmann json耗时16毫秒，PTree耗时35毫秒。
//
// 决定只使用nlohmann json解析json制式数据，不再使用boost PropertyTree。
//
// 在Debug模式下，无论怎样调整编译参数，由于连接的动态系统库是debug模式（/MDd），且该json库使用的std库为调试模式，故而其解析速度无法与release模式相比。
//
// Debug模式下，本文件采用全局优化(/GL)等优化选项，其他文件使用默认值。
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include"ChinaStockCodeConverter.h"
#include"TimeConvert.h"
#include"ConvertToString.h"
#include"SystemMessage.h"
#include"WebData.h"
#include"WebRTData.h"

#include"JsonGetValue.h"
#include"NlohmannJsonDeclaration.h"

#include"simdjsonGetValue.h"
using namespace simdjson;

using std::vector;
using std::shared_ptr;
using std::make_shared;
using std::string;

bool CreateJsonWithNlohmann(json& js, const std::string& s, const long lBeginPos, const long lEndPos) {
	try { js = json::parse(s.begin() + lBeginPos, s.end() - lEndPos); }
	catch (json::parse_error&) {
		js.clear();
		return false;
	}
	return true;
}

bool CreateJsonWithNlohmann(json& js, CString& str, const long lBeginPos, const long lEndPos) {
	const string s = str.GetBuffer();
	try { js = json::parse(s.begin() + lBeginPos, s.end() - lEndPos); }
	catch (json::parse_error&) {
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
		strSymbol4 = XferNeteaseToStandard(symbolName.c_str());
		pWebRTData->SetSymbol(strSymbol4);
		const string sName = jsonGetString(&js, "name");
		pWebRTData->SetStockName(XferToCString(sName)); // 将utf-8字符集转换为多字节字符集
		strTime = jsonGetString(&js, _T("time"));
		string strSymbol2 = jsonGetString(&js, _T("code"));
		pWebRTData->SetTransactionTime(ConvertStringToTime(_T("%04d/%02d/%02d %02d:%02d:%02d"), strTime.c_str()));
	}
	catch (json::exception& e) {// 结构不完整
		// do nothing
		CString strError2 = strSymbol4;
		strError2 += _T(" ");
		strError2 += e.what();
		gl_systemMessage.PushErrorMessage(strError2);
	}
	try {
		pWebRTData->SetVolume(jsonGetLongLong(&js, _T("volume")));
		pWebRTData->SetAmount(jsonGetLongLong(&js,_T("turnover")));
		pWebRTData->SetHigh(static_cast<long>(jsonGetDouble(&js, _T("high")) * 1000));
		pWebRTData->SetLow(static_cast<long>(jsonGetDouble(&js, _T("low")) * 1000));
		pWebRTData->SetNew(static_cast<long>(jsonGetDouble(&js, _T("price")) * 1000));
		pWebRTData->SetLastClose(static_cast<long>(jsonGetDouble(&js, _T("yestclose")) * 1000));
		pWebRTData->SetOpen(static_cast<long>(jsonGetDouble(&js, _T("open")) * 1000));

		pWebRTData->SetVBuy(0, jsonGetLong(&js,_T("bidvol1")));
		pWebRTData->SetVBuy(1, jsonGetLong(&js,_T("bidvol2")));
		pWebRTData->SetVBuy(2, jsonGetLong(&js,_T("bidvol3")));
		pWebRTData->SetVBuy(3, jsonGetLong(&js,_T("bidvol4")));
		pWebRTData->SetVBuy(4, jsonGetLong(&js,_T("bidvol5")));
		pWebRTData->SetVSell(0, jsonGetLong(&js,_T("askvol1")));
		pWebRTData->SetVSell(1, jsonGetLong(&js,_T("askvol2")));
		pWebRTData->SetVSell(2, jsonGetLong(&js,_T("askvol3")));
		pWebRTData->SetVSell(3, jsonGetLong(&js,_T("askvol4")));
		pWebRTData->SetVSell(4, jsonGetLong(&js,_T("askvol5")));

		pWebRTData->SetPSell(0, static_cast<long>(jsonGetDouble(&js,_T("ask1")) * 1000));
		pWebRTData->SetPSell(1, static_cast<long>(jsonGetDouble(&js,_T("ask2")) * 1000));
		pWebRTData->SetPSell(2, static_cast<long>(jsonGetDouble(&js,_T("ask3")) * 1000));
		pWebRTData->SetPSell(3, static_cast<long>(jsonGetDouble(&js,_T("ask4")) * 1000));
		pWebRTData->SetPSell(4, static_cast<long>(jsonGetDouble(&js,_T("ask5")) * 1000));
		pWebRTData->SetPBuy(0, static_cast<long>(jsonGetDouble(&js,_T("bid1")) * 1000));
		pWebRTData->SetPBuy(1, static_cast<long>(jsonGetDouble(&js,_T("bid2")) * 1000));
		pWebRTData->SetPBuy(2, static_cast<long>(jsonGetDouble(&js,_T("bid3")) * 1000));
		pWebRTData->SetPBuy(3, static_cast<long>(jsonGetDouble(&js,_T("bid4")) * 1000));
		pWebRTData->SetPBuy(4, static_cast<long>(jsonGetDouble(&js,_T("bid5")) * 1000));

		pWebRTData->CheckNeteaseRTDataActive();
	}
	catch (json::exception&) {// 非活跃股票（已下市等）
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
// 使用json解析，已经没有错误数据了。(偶尔还会有，大致每分钟出现一次）。
//
//////////////////////////////////////////////////////////////////////////////////////////////////
shared_ptr<vector<CWebRTDataPtr>> ParseNeteaseRTDataWithNlohmannJSon(const CWebDataPtr& pData) {
	auto pvWebRTData = make_shared<vector<CWebRTDataPtr>>(); // 要确保返回的向量指针不为空（上级函数需要此指针，即使其内容可能为空）
	const string_view svHeader = pData->GetStringView(0, 21);
	if (svHeader.compare(_T("_ntes_quote_callback(")) != 0) {
		CString str = _T("Bad netease RT data header : ");
		str.Append(svHeader.data(), svHeader.size());
		gl_systemMessage.PushErrorMessage(str);
		return pvWebRTData;
	}
	ASSERT(!pData->IsParsed());
	if (!pData->IsParsed()) {
		if (!pData->CreateJson(21, 2)) {	// 网易数据前21位为前缀，后两位为后缀
			gl_systemMessage.PushErrorMessage(_T("Netease RT data json parse error"));
			return pvWebRTData;
		}
	}
	ASSERT(pData->IsParsed());
	json* pjs = pData->GetJSon();
	pvWebRTData = ParseNeteaseRTData(pjs);

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
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////
shared_ptr<vector<CWebRTDataPtr>> ParseNeteaseRTDataWithSimdjson(string_view svJsonData) {
	string symbolCode, strTime;
	auto pvWebRTData = make_shared<vector<CWebRTDataPtr>>();
	try {
		ondemand::parser parser;
		const simdjson::padded_string jsonPadded(svJsonData);
		ondemand::document doc = parser.iterate(jsonPadded);

		for (ondemand::field item_key : doc.get_object()) {
			auto pWebRTData = make_shared<CWebRTData>();
			pWebRTData->SetDataSource(NETEASE_RT_WEB_DATA_);

			ondemand::object item = item_key.value();
			const string_view strSymbolView2 = jsonGetStringView(item, _T("code"));
			symbolCode = strSymbolView2;
			CString strSymbol4 = XferNeteaseToStandard(symbolCode.c_str());
			pWebRTData->SetSymbol(strSymbol4);
			pWebRTData->SetVSell(0, jsonGetInt64(item, _T("askvol1")));
			pWebRTData->SetVSell(2, jsonGetInt64(item, _T("askvol3")));
			pWebRTData->SetVSell(1, jsonGetInt64(item, _T("askvol2")));
			pWebRTData->SetVSell(4, jsonGetInt64(item, _T("askvol5")));
			pWebRTData->SetVSell(3, jsonGetInt64(item, _T("askvol4")));
			pWebRTData->SetNew(jsonGetDouble(item, _T("price")) * 1000);
			pWebRTData->SetOpen(jsonGetDouble(item, _T("open")) * 1000);
			pWebRTData->SetPBuy(4, jsonGetDouble(item, _T("bid5")) * 1000);
			pWebRTData->SetPBuy(3, jsonGetDouble(item, _T("bid4")) * 1000);
			pWebRTData->SetPBuy(2, jsonGetDouble(item, _T("bid3")) * 1000);
			pWebRTData->SetPBuy(1, jsonGetDouble(item, _T("bid2")) * 1000);
			pWebRTData->SetPBuy(0, jsonGetDouble(item, _T("bid1")) * 1000);
			pWebRTData->SetHigh(jsonGetDouble(item, _T("high")) * 1000);
			pWebRTData->SetLow(jsonGetDouble(item, _T("low")) * 1000);
			pWebRTData->SetVBuy(2, jsonGetInt64(item, _T("bidvol3")));
			pWebRTData->SetVBuy(0, jsonGetInt64(item, _T("bidvol1")));
			pWebRTData->SetVBuy(1, jsonGetInt64(item, _T("bidvol2")));
			pWebRTData->SetVBuy(4, jsonGetInt64(item, _T("bidvol5")));
			pWebRTData->SetVBuy(3, jsonGetInt64(item, _T("bidvol4")));
			pWebRTData->SetVolume(jsonGetInt64(item, _T("volume")));
			pWebRTData->SetPSell(4, jsonGetDouble(item, _T("ask5")) * 1000);
			pWebRTData->SetPSell(3, jsonGetDouble(item, _T("ask4")) * 1000);
			pWebRTData->SetPSell(0, jsonGetDouble(item, _T("ask1")) * 1000);

			string_view sNameView = jsonGetStringView(item, "name");
			string sName(sNameView);
			pWebRTData->SetStockName(XferToCString(sName)); // 将utf-8字符集转换为多字节字符集
			pWebRTData->SetPSell(2, jsonGetDouble(item, _T("ask3")) * 1000);
			pWebRTData->SetPSell(1, jsonGetDouble(item, _T("ask2")) * 1000);
			strTime = jsonGetStringView(item, _T("time"));
			pWebRTData->SetTransactionTime(ConvertStringToTime(_T("%04d/%02d/%02d %02d:%02d:%02d"), strTime.c_str()));

			pWebRTData->SetLastClose(jsonGetDouble(item, _T("yestclose")) * 1000);
			pWebRTData->SetAmount(jsonGetInt64(item, _T("turnover")));

			pWebRTData->CheckNeteaseRTDataActive();
			pvWebRTData->push_back(pWebRTData);
		}
	}
	catch (simdjson_error& error) {
		string sError = error.what();
	}
	return pvWebRTData;
}

shared_ptr<vector<CWebRTDataPtr>> ParseNeteaseRTDataWithSimdjson2(string_view svJsonData) {
	string symbolCode, strTime;
	auto pvWebRTData = make_shared<vector<CWebRTDataPtr>>();
	try {
		ondemand::parser parser;
		const simdjson::padded_string jsonPadded(svJsonData);
		ondemand::document doc = parser.iterate(jsonPadded);
		for (ondemand::field item_key : doc.get_object()) {
			auto pWebRTData = make_shared<CWebRTData>();
			pWebRTData->SetDataSource(NETEASE_RT_WEB_DATA_);
			ondemand::object item = item_key.value();
			const string_view strSymbolView2 = item.find_field("code").get_string();
			symbolCode = strSymbolView2;
			CString strSymbol4 = XferNeteaseToStandard(symbolCode.c_str());
			pWebRTData->SetSymbol(strSymbol4);
			pWebRTData->SetVSell(0, item.find_field("askvol1").get_int64());
			pWebRTData->SetVSell(2, item.find_field("askvol3").get_int64());
			pWebRTData->SetVSell(1, item.find_field("askvol2").get_int64());
			pWebRTData->SetVSell(4, item.find_field("askvol5").get_int64());
			pWebRTData->SetVSell(3, item.find_field("askvol4").get_int64());
			pWebRTData->SetNew(item.find_field("price").get_double() * 1000);
			pWebRTData->SetOpen(item.find_field("open").get_double() * 1000);
			pWebRTData->SetPBuy(4, item.find_field("bid5").get_double() * 1000);
			pWebRTData->SetPBuy(3, item.find_field("bid4").get_double() * 1000);
			pWebRTData->SetPBuy(2, item.find_field("bid3").get_double() * 1000);
			pWebRTData->SetPBuy(1, item.find_field("bid2").get_double() * 1000);
			pWebRTData->SetPBuy(0, item.find_field("bid1").get_double() * 1000);
			pWebRTData->SetHigh(item.find_field("high").get_double() * 1000);
			pWebRTData->SetLow(item.find_field("low").get_double() * 1000);
			pWebRTData->SetVBuy(0, item.find_field("bidvol1").get_int64());
			pWebRTData->SetVBuy(2, item.find_field("bidvol3").get_int64());
			pWebRTData->SetVBuy(1, item.find_field("bidvol2").get_int64());
			pWebRTData->SetVBuy(4, item.find_field("bidvol5").get_int64());
			pWebRTData->SetVBuy(3, item.find_field("bidvol4").get_int64());
			pWebRTData->SetVolume(item.find_field("volume").get_int64());
			pWebRTData->SetPSell(4, item.find_field("ask5").get_double() * 1000);
			pWebRTData->SetPSell(3, item.find_field("ask4").get_double() * 1000);
			pWebRTData->SetPSell(0, item.find_field("ask1").get_double() * 1000);

			string_view sNameView = item.find_field("name").get_string();
			string sName(sNameView);
			pWebRTData->SetStockName(XferToCString(sName)); // 将utf-8字符集转换为多字节字符集
			pWebRTData->SetPSell(2, item.find_field("ask3").get_double() * 1000);
			pWebRTData->SetPSell(1, item.find_field("ask2").get_double() * 1000);
			const string_view svTime = item.find_field("time").get_string();
			strTime = svTime;
			pWebRTData->SetTransactionTime(ConvertStringToTime(_T("%04d/%02d/%02d %02d:%02d:%02d"), strTime.c_str()));
			pWebRTData->SetLastClose(item.find_field("yestclose").get_double() * 1000);
			pWebRTData->SetAmount(item.find_field("turnover").get_int64());
			pWebRTData->CheckNeteaseRTDataActive();
			pvWebRTData->push_back(pWebRTData);
		}
	}
	catch (simdjson_error& error) {
		string sError = error.what();
	}
	return pvWebRTData;
}

shared_ptr<vector<CWebRTDataPtr>> ParseNeteaseRTDataWithSimdjson(CWebDataPtr pData) {
	return ParseNeteaseRTDataWithSimdjson(pData->GetStringView(21, pData->GetBufferLength() - 21 - 2)); // 网易json数据
}
