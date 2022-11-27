//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// json���ݽ����������λ�ڴ��ļ��С�
// ������Ŀǰ���ʱ�ĺ������ʶ�DEBUGģʽʱҲҪ����ȫ���Ż���/GL����ֻ�����������ڹ涨ʱ���ڴ��������ݡ�
// Neteaseʵʱ������Ŀǰ����Ҫ��ע�ģ�ÿ300�������900������ʱ��ϵͳ�����϶�ռһ�����Ĵ������ˡ�������к�ʱ�����񣬾���Ҫ
// ������䵽�������Ĵ������ϣ������ͻᵼ��ϵͳ����Ӧʱ�����ӣ�����ϵͳ�ĸ���Ҳ�����ӡ�
//
// Ϊ�˱�֤���ļ��ܹ������Ż������ļ���ʹ��Ԥ�ȱ����pch.h�����ǰ�ʵ��ͷ�ļ����ڱ��ļ��С������ܹ���֤�����Ż�����ɡ�
//
// ��DEBUGģʽ�£�boost PTree�ٶȱ�Nlohmann json�죬��Releaseģʽ��nlohmann json���ٶȱ�boost ptree��50%���ҡ�
// ����900��NeteaseRTData��nlohmann json��ʱ16���룬PTree��ʱ35���롣
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include"stdafx.h"
#include"StockCodeConverter.h"
#include"TimeConvert.h"
#include"SystemMessage.h"
#include"WebRTData.h"

#include<boost/property_tree/ptree.hpp>
#include<boost/property_tree/json_parser.hpp>
using namespace boost::property_tree;

#include"NlohmannJsonDeclaration.h"

void ReportJSonErrorToSystemMessage(CString strPrefix, ptree_error& e);

using namespace std;
#include<string>

bool CreateNlohmannJson(json* pjs, std::string& s, long lBeginPos, long lEndPos) {
	try {
		*pjs = json::parse(s.begin() + lBeginPos, s.end() - lEndPos);
	}
	catch (json::parse_error& e) {
		gl_systemMessage.PushErrorMessage("nlohmann json parse error");
		//ReportJsonError(e, s);
		pjs = nullptr;
		return false;
	}
	return true;
}

CString XferToCString(string s);

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
bool ParseOneNeteaseRTDataWithNlohmannJSon(json::iterator& it, CWebRTDataPtr pWebRTData) {
	bool fSucceed = true;
	string symbolName, strSymbol2, strTime, strUpdateTime, strName;
	double dHigh, dLow, dNew, dOpen, dLastClose;
	array<double, 5> aAsk{ 0,0,0,0,0 }, aBid{ 0,0,0,0,0 };
	CString strSymbol4, str1, strName3;
	string sName;
	json js;

	try {
		symbolName = it.key();
		js = it.value();
		strSymbol4 = XferNeteaseToStandred(symbolName.c_str());
		pWebRTData->SetSymbol(strSymbol4);
		sName = js.at("name");
		pWebRTData->SetStockName(XferToCString(sName)); // ��utf-8�ַ���ת��Ϊ���ֽ��ַ���
		strTime = js.at(_T("time"));
		strSymbol2 = js.at(_T("code"));
		pWebRTData->SetTransactionTime(ConvertStringToTime(_T("%04d/%02d/%02d %02d:%02d:%02d"), strTime.c_str()));
	}
	catch (json::out_of_range& e) { // �ṹ������
		// do nothing
		CString strError2 = strSymbol4;
		strError2 += _T(" ");
		strError2 += e.what();
		gl_systemMessage.PushErrorMessage(strError2);
		fSucceed = false;
	}
	try {
		pWebRTData->SetVolume(js.at(_T("volume")));
		pWebRTData->SetAmount(js.at(_T("turnover")));
		dHigh = js.at(_T("high"));
		pWebRTData->SetHigh(dHigh * 1000);
		dLow = js.at(_T("low"));
		pWebRTData->SetLow(dLow * 1000);
		dNew = js.at(_T("price"));
		pWebRTData->SetNew(dNew * 1000);
		dLastClose = js.at(_T("yestclose"));
		pWebRTData->SetLastClose(dLastClose * 1000);
		dOpen = js.at(_T("open"));
		pWebRTData->SetOpen(dOpen * 1000);

		pWebRTData->SetVBuy(0, js.at(_T("bidvol1")));
		pWebRTData->SetVBuy(1, js.at(_T("bidvol2")));
		pWebRTData->SetVBuy(2, js.at(_T("bidvol3")));
		pWebRTData->SetVBuy(3, js.at(_T("bidvol4")));
		pWebRTData->SetVBuy(4, js.at(_T("bidvol5")));
		pWebRTData->SetVSell(0, js.at(_T("askvol1")));
		pWebRTData->SetVSell(1, js.at(_T("askvol2")));
		pWebRTData->SetVSell(2, js.at(_T("askvol3")));
		pWebRTData->SetVSell(3, js.at(_T("askvol4")));
		pWebRTData->SetVSell(4, js.at(_T("askvol5")));
		aAsk[0] = js.at(_T("ask1"));
		pWebRTData->SetPSell(0, aAsk[0] * 1000);
		aAsk[1] = js.at(_T("ask2"));
		pWebRTData->SetPSell(1, aAsk[1] * 1000);
		aAsk[2] = js.at(_T("ask3"));
		pWebRTData->SetPSell(2, aAsk[2] * 1000);
		aAsk[3] = js.at(_T("ask4"));
		pWebRTData->SetPSell(3, aAsk[3] * 1000);
		aAsk[4] = js.at(_T("ask5"));
		pWebRTData->SetPSell(4, aAsk[4] * 1000);
		aBid[0] = js.at(_T("bid1"));
		pWebRTData->SetPBuy(0, aBid[0] * 1000);
		aBid[1] = js.at(_T("bid2"));
		pWebRTData->SetPBuy(1, aBid[1] * 1000);
		aBid[2] = js.at(_T("bid3"));
		pWebRTData->SetPBuy(2, aBid[2] * 1000);
		aBid[3] = js.at(_T("bid4"));
		pWebRTData->SetPBuy(3, aBid[3] * 1000);
		aBid[4] = js.at(_T("bid5"));
		pWebRTData->SetPBuy(4, aBid[4] * 1000);

		pWebRTData->CheckNeteaseRTDataActive();
		fSucceed = true;
	}
	catch (json::out_of_range&) { // �ǻ�Ծ��Ʊ�������еȣ�
		pWebRTData->SetActive(false);
		fSucceed = true;
	}
	pWebRTData->SetActive(true);

	return fSucceed;
}

bool ParseWithPTree(ptree& pt, string& s) {
	stringstream ss(s);
	try {
		read_json(ss, pt);
	}
	catch (ptree_error& e) {
#ifndef _DEBUG
		char buffer[180];
		for (int i = 0; i < 80; i++) {
			buffer[i] = s.at(i);
		}
		buffer[80] = 0x000;
		CString str = buffer;
		ReportJSonErrorToSystemMessage(_T("PTree JSon Reading Error ") + str + _T(" "), e);
#endif
		return false;
	}
	return true;
}

bool ParseWithPTree(shared_ptr<ptree>& ppt, string& s) {
	ASSERT(ppt != nullptr);
	stringstream ss(s);
	try {
		read_json(ss, *ppt);
	}
	catch (ptree_error& e) {
#ifndef _DEBUG
		CString str = s.c_str();
		str = str.Left(160);
		ReportJSonErrorToSystemMessage(_T("PTree JSon Reading Error ") + str + _T(" "), e);
#endif
		return false;
	}
	return true;
}