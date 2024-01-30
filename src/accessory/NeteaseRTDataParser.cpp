//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// json���ݽ����������λ�ڴ��ļ��С�
// Neteaseʵʱ������Ŀǰ����Ҫ��ע�ģ�ÿ300�������900������ʱ��ʹ��Nlohmann json�����Ļ���ϵͳ�����϶�ռһ�����Ĵ������ˡ�������к�ʱ�����񣬾���Ҫ
// ������䵽�������Ĵ������ϣ������ͻᵼ��ϵͳ����Ӧʱ�����ӣ�����ϵͳ�ĸ���Ҳ�����ӡ�ʹ��simdjson��ռ��3%���ҵ�CPUʱ�䣬�����ԡ�
//
// todo ���ǲ��д�����յ���ʵʱ���ݡ�
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
	try {
		js = json::parse(s.begin() + lBeginPos, s.end() - lEndPos);
	}
	catch (json::parse_error&) {
		js.clear();
		return false;
	}
	return true;
}

bool CreateJsonWithNlohmann(json& js, CString& str, const long lBeginPos, const long lEndPos) {
	const string s = str.GetBuffer();
	try {
		js = json::parse(s.begin() + lBeginPos, s.end() - lEndPos);
	}
	catch (json::parse_error&) {
		js.clear();
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//
// �������ļ�file�ж�ȡ������ʽʵʱ���ݣ�����ֵ�����������Ƿ���ָ�ʽ����
// ��ʼ��Ϊ��һ��{��������Ϊ�����ڶ���}��������������账���򱻴�����ַ�Ϊ','�����û�������ˣ��򱻴�����ַ�Ϊ' '��
//
// Ҫ��ȡ�������飬�������ݽӿڣ�http://api.money.126.net/data/feed/0601872
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
//  �������Ĺ�Ʊ���Ƶ���ʽ��������ʱ��ʹ�ã�����boost ptree�����ĵ�֧�ֲ��㣬��ֻ֧��utf8��ʽ��������ȡ�����ַ�ʱ�������룩��
// ���ڲ���wstring��CStringW���ι��ɣ��Ϳ���������ʾ�ˡ�
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
		const string sName = jsonGetString(js, "name");
		pWebRTData->SetStockName(XferToCString(sName)); // ��utf-8�ַ���ת��Ϊ���ֽ��ַ���
		strTime = jsonGetString(js, _T("time"));
		string strSymbol2 = jsonGetString(js, _T("code"));
		pWebRTData->SetTransactionTime(ConvertStringToTime(_T("%04d/%02d/%02d %02d:%02d:%02d"), strTime.c_str()));
	}
	catch (json::exception& e) {// �ṹ������
		// do nothing
		CString strError2 = strSymbol4;
		strError2 += _T(" ");
		strError2 += e.what();
		gl_systemMessage.PushErrorMessage(strError2);
	}
	try {
		pWebRTData->SetVolume(jsonGetLongLong(js, _T("volume")));
		pWebRTData->SetAmount(jsonGetDouble(js,_T("turnover")));
		pWebRTData->SetHigh(static_cast<long>(jsonGetDouble(js, _T("high")) * 1000));
		pWebRTData->SetLow(static_cast<long>(jsonGetDouble(js, _T("low")) * 1000));
		pWebRTData->SetNew(static_cast<long>(jsonGetDouble(js, _T("price")) * 1000));
		pWebRTData->SetLastClose(static_cast<long>(jsonGetDouble(js, _T("yestclose")) * 1000));
		pWebRTData->SetOpen(static_cast<long>(jsonGetDouble(js, _T("open")) * 1000));

		pWebRTData->SetVBuy(0, jsonGetLong(js,_T("bidvol1")));
		pWebRTData->SetVBuy(1, jsonGetLong(js,_T("bidvol2")));
		pWebRTData->SetVBuy(2, jsonGetLong(js,_T("bidvol3")));
		pWebRTData->SetVBuy(3, jsonGetLong(js,_T("bidvol4")));
		pWebRTData->SetVBuy(4, jsonGetLong(js,_T("bidvol5")));
		pWebRTData->SetVSell(0, jsonGetLong(js,_T("askvol1")));
		pWebRTData->SetVSell(1, jsonGetLong(js,_T("askvol2")));
		pWebRTData->SetVSell(2, jsonGetLong(js,_T("askvol3")));
		pWebRTData->SetVSell(3, jsonGetLong(js,_T("askvol4")));
		pWebRTData->SetVSell(4, jsonGetLong(js,_T("askvol5")));

		pWebRTData->SetPSell(0, static_cast<long>(jsonGetDouble(js,_T("ask1")) * 1000));
		pWebRTData->SetPSell(1, static_cast<long>(jsonGetDouble(js,_T("ask2")) * 1000));
		pWebRTData->SetPSell(2, static_cast<long>(jsonGetDouble(js,_T("ask3")) * 1000));
		pWebRTData->SetPSell(3, static_cast<long>(jsonGetDouble(js,_T("ask4")) * 1000));
		pWebRTData->SetPSell(4, static_cast<long>(jsonGetDouble(js,_T("ask5")) * 1000));
		pWebRTData->SetPBuy(0, static_cast<long>(jsonGetDouble(js,_T("bid1")) * 1000));
		pWebRTData->SetPBuy(1, static_cast<long>(jsonGetDouble(js,_T("bid2")) * 1000));
		pWebRTData->SetPBuy(2, static_cast<long>(jsonGetDouble(js,_T("bid3")) * 1000));
		pWebRTData->SetPBuy(3, static_cast<long>(jsonGetDouble(js,_T("bid4")) * 1000));
		pWebRTData->SetPBuy(4, static_cast<long>(jsonGetDouble(js,_T("bid5")) * 1000));

		pWebRTData->CheckNeteaseRTDataActive();
	}
	catch (json::exception&) {// �ǻ�Ծ��Ʊ�������еȣ�
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
// Ҫ��ȡ�������飬�������ݽӿڣ�http://api.money.126.net/data/feed/0601872
//
// _ntes_quote_callback({"0601872":{"code": "0601872", "percent": 0.038251, "high": 5.72, "askvol3": 311970, "askvol2": 257996,
//                      "askvol5": 399200, "askvol4": 201000, "price": 5.7, "open": 5.53, "bid5": 5.65, "bid4": 5.66, "bid3": 5.67,
//                       "bid2": 5.68, "bid1": 5.69, "low": 5.51, "updown": 0.21, "type": "SH", "symbol": "601872", "status": 0,
//                       "ask4": 5.73, "bidvol3": 234700, "bidvol2": 166300, "bidvol1": 641291, "update": "2019/11/04 15:59:54",
//                       "bidvol5": 134500, "bidvol4": 96600, "yestclose": 5.49, "askvol1": 396789, "ask5": 5.74, "volume": 78750304,
//                       "ask1": 5.7, "name": "\u62db\u5546\u8f6e\u8239", "ask3": 5.72, "ask2": 5.71, "arrow": "\u2191",
//                        "time": "2019/11/04 15:59:52", "turnover": 443978974} });
//
// ʹ��json�������Ѿ�û�д��������ˡ�(ż�������У�����ÿ���ӳ���һ�Σ���
//
//////////////////////////////////////////////////////////////////////////////////////////////////
shared_ptr<vector<CWebRTDataPtr>> ParseNeteaseRTDataWithNlohmannJSon(const CWebDataPtr& pData) {
	auto pvWebRTData = make_shared<vector<CWebRTDataPtr>>(); // Ҫȷ�����ص�����ָ�벻Ϊ�գ��ϼ�������Ҫ��ָ�룬��ʹ�����ݿ���Ϊ�գ�
	const string_view svHeader = pData->GetStringView(0, 21);
	if (svHeader.compare(_T("_ntes_quote_callback(")) != 0) {
		CString str = _T("Bad netease RT data header : ");
		str.Append(svHeader.data(), svHeader.size());
		gl_systemMessage.PushErrorMessage(str);
		return pvWebRTData;
	}
	json js;

	if (!pData->CreateJson(js, 21, 2)) {	// ��������ǰ21λΪǰ׺������λΪ��׺
		gl_systemMessage.PushErrorMessage(_T("Netease RT data json parse error"));
		return pvWebRTData;
	}
	pvWebRTData = ParseNeteaseRTData(&js);

	return pvWebRTData;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//
// �������ļ�file�ж�ȡ������ʽʵʱ���ݣ�����ֵ�����������Ƿ���ָ�ʽ����
// ��ʼ��Ϊ��һ��{��������Ϊ�����ڶ���}��������������账���򱻴�����ַ�Ϊ','�����û�������ˣ��򱻴�����ַ�Ϊ' '��
//
// Ҫ��ȡ�������飬�������ݽӿڣ�http://api.money.126.net/data/feed/0601872
//
// _ntes_quote_callback({"0601872":{"code": "0601872", "percent": 0.038251, "high": 5.72, "askvol3": 311970, "askvol2": 257996,
//                      "askvol5": 399200, "askvol4": 201000, "price": 5.7, "open": 5.53, "bid5": 5.65, "bid4": 5.66, "bid3": 5.67,
//                       "bid2": 5.68, "bid1": 5.69, "low": 5.51, "updown": 0.21, "type": "SH", "symbol": "601872", "status": 0,
//                       "ask4": 5.73, "bidvol3": 234700, "bidvol2": 166300, "bidvol1": 641291, "update": "2019/11/04 15:59:54",
//                       "bidvol5": 134500, "bidvol4": 96600, "yestclose": 5.49, "askvol1": 396789, "ask5": 5.74, "volume": 78750304,
//                       "ask1": 5.7, "name": "\u62db\u5546\u8f6e\u8239", "ask3": 5.72, "ask2": 5.71, "arrow": "\u2191",
//                        "time": "2019/11/04 15:59:52", "turnover": 443978974} });
//
// Ŀǰ�����±귽���������ݣ����ٶ��ܴﵽNlohmann json���������ϡ�
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
			auto key = item_key.key();
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
			pWebRTData->SetStockName(XferToCString(sName)); // ��utf-8�ַ���ת��Ϊ���ֽ��ַ���
			pWebRTData->SetPSell(2, jsonGetDouble(item, _T("ask3")) * 1000);
			pWebRTData->SetPSell(1, jsonGetDouble(item, _T("ask2")) * 1000);
			strTime = jsonGetStringView(item, _T("time"));
			pWebRTData->SetTransactionTime(ConvertStringToTime(_T("%04d/%02d/%02d %02d:%02d:%02d"), strTime.c_str()));

			pWebRTData->SetLastClose(jsonGetDouble(item, _T("yestclose")) * 1000);
			pWebRTData->SetAmount(jsonGetDouble(item, _T("turnover")));

			pWebRTData->CheckNeteaseRTDataActive();
			pvWebRTData->push_back(pWebRTData);
		}
	}
	catch (simdjson_error& error) {
		const string sError = error.what();
		CString str = "Netease RT Data Error: ";
		str += sError.c_str();
		gl_systemMessage.PushErrorMessage(str);
	}
	return pvWebRTData;
}

// bug ������������ᵼ�±���ʱ���ӳ�4�������ң������Ǳ����������⡣����΢�����˸����⣬�ȴ���Ӧ��
// neteaseʵʱ���ݵ�˳���г��仯������ʹ�ô���˳�����������
/*
shared_ptr<vector<CWebRTDataPtr>> ParseNeteaseRTDataWithSimdjson3(string_view svJsonData) {
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
			pWebRTData->SetStockName(XferToCString(sName)); // ��utf-8�ַ���ת��Ϊ���ֽ��ַ���
			pWebRTData->SetPSell(2, item.find_field("ask3").get_double() * 1000);
			pWebRTData->SetPSell(1, item.find_field("ask2").get_double() * 1000);
			const string_view svTime = item.find_field("time").get_string();
			strTime = svTime;
			pWebRTData->SetTransactionTime(ConvertStringToTime(_T("%04d/%02d/%02d %02d:%02d:%02d"), strTime.c_str()));
			pWebRTData->SetLastClose(item.find_field("yestclose").get_double() * 1000);
			pWebRTData->SetAmount(item.find_field("turnover").get_double());
			pWebRTData->CheckNeteaseRTDataActive();
			pvWebRTData->push_back(pWebRTData);
		}
	}
	catch (simdjson_error& error) {
		string sError = error.what();
	}
	return pvWebRTData;
}
*/

shared_ptr<vector<CWebRTDataPtr>> ParseNeteaseRTDataWithSimdjson(const CWebDataPtr& pData) {
	return ParseNeteaseRTDataWithSimdjson(pData->GetStringView(21, pData->GetBufferLength() - 21 - 2)); // ����json����
}
