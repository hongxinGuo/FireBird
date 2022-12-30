//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// json���ݽ����������λ�ڴ��ļ��С�
// json������Ŀǰ���ʱ�ĺ������ʶ���DEBUGģʽʱҲҪ����ȫ���Ż���/GL����ֻ�����������ڹ涨ʱ���ڴ��������ݡ�
// Neteaseʵʱ������Ŀǰ����Ҫ��ע�ģ�ÿ300�������900������ʱ��ϵͳ�����϶�ռһ�����Ĵ������ˡ�������к�ʱ�����񣬾���Ҫ
// ������䵽�������Ĵ������ϣ������ͻᵼ��ϵͳ����Ӧʱ�����ӣ�����ϵͳ�ĸ���Ҳ�����ӡ�
//
// Ϊ�˱�֤���ļ��ܹ������Ż������ļ���ʹ��Ԥ�ȱ����pch.h�����ǰ�ʵ��ͷ�ļ����ڱ��ļ��С������ܹ���֤�����Ż�����ɡ�
//
// ��DEBUGģʽ�£�boost PTree�ٶȱ�Nlohmann json�죬��Releaseģʽ��nlohmann json���ٶȱ�boost ptree��50%���ϣ����ʱ���ǿ�һ�����ϣ���
// debugģʽ�£�����900��NeteaseRTData��nlohmann json��ʱ160���룬PTree��ʱ120���롣
// releaseģʽ�£�����900��NeteaseRTData��nlohmann json��ʱ16���룬PTree��ʱ35���롣
//
// ��Debugģʽ�£�����������������������������ӵĶ�̬ϵͳ����debugģʽ��/MDd�����Ҹ�json��ʹ�õ�std��Ϊ����ģʽ���ʶ�������ٶ��޷���releaseģʽ��ȡ�
//
// Debugģʽ�£����ļ�����ȫ���Ż�(/GL)���Ż�ѡ������ļ�ʹ��Ĭ��ֵ��
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// #include"pch.h"
// ��ʹ��pch.hԤ����ͷ�����ǽ�����ͷ�ļ���������Ա��ڷֱ����
#include"stdafx.h"
#include"ChinaStockCodeConverter.h"
#include"TimeConvert.h"
#include"SystemMessage.h"
#include"WebData.h"
#include"WebRTData.h"

#include"JsonGetValue.h"
#include"NlohmannJsonDeclaration.h"

#include<string>
#include<memory>
#include<vector>
using std::vector;
using std::shared_ptr;
using std::make_shared;
using std::string;

//using namespace std;

bool NlohmannCreateJson(json* pjs, const std::string& s, const long lBeginPos, const long lEndPos) {
	try {
		*pjs = json::parse(s.begin() + lBeginPos, s.end() - lEndPos);
	}
	catch (json::parse_error&) {
		//gl_systemMessage.PushErrorMessage("nlohmann json parse error");
		pjs = nullptr;
		return false;
	}
	return true;
}

CString XferToCString(const std::string& s);

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
bool ParseOneNeteaseRTData(const json::iterator& it, const CWebRTDataPtr pWebRTData) {
	bool fSucceed = true;
	string strTime, strUpdateTime, strName;
	CString strSymbol4, str1, strName3;
	json js = it.value();;
	const string symbolName = it.key();

	try {
		strSymbol4 = XferNeteaseToStandard(symbolName.c_str());
		pWebRTData->SetSymbol(strSymbol4);
		const string sName = jsonGetString(&js, "name");
		pWebRTData->SetStockName(XferToCString(sName)); // ��utf-8�ַ���ת��Ϊ���ֽ��ַ���
		strTime = jsonGetString(&js, _T("time"));
		string strSymbol2 = jsonGetString(&js, _T("code"));
		pWebRTData->SetTransactionTime(ConvertStringToTime(_T("%04d/%02d/%02d %02d:%02d:%02d"), strTime.c_str()));
	}
	catch (json::exception& e) {
		// �ṹ������
		// do nothing
		CString strError2 = strSymbol4;
		strError2 += _T(" ");
		strError2 += e.what();
		gl_systemMessage.PushErrorMessage(strError2);
		fSucceed = false;
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
		fSucceed = true;
	}
	catch (json::exception&) {
		// �ǻ�Ծ��Ʊ�������еȣ�
		pWebRTData->SetActive(false);
		fSucceed = true;
	}

	return fSucceed;
}

shared_ptr<vector<CWebRTDataPtr>> ParseNeteaseRTData(json* pjs) {
	auto pvWebRTData = make_shared<vector<CWebRTDataPtr>>();

	for (auto it = pjs->begin(); it != pjs->end(); ++it) {
		auto pRTData = make_shared<CWebRTData>();
		pRTData->SetDataSource(NETEASE_RT_WEB_DATA_);
		if (ParseOneNeteaseRTData(it, pRTData)) {
			pRTData->CheckNeteaseRTDataActive();
			pvWebRTData->push_back(pRTData);
		}
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
shared_ptr<vector<CWebRTDataPtr>> ParseNeteaseRTDataWithNlohmannJSon(CWebDataPtr pData) {
	auto pvWebRTData = make_shared<vector<CWebRTDataPtr>>();

	// ��ȡʵʱ����ʱ�á�Ϊ�˲��Խ����ٶ�
	if (static int i = 0; i < pData->GetBufferLength()) {
		//SaveToFile(_T("C:\\StockAnalysis\\NeteaseRTData.json"), pData->GetDataBuffer());
		i = pData->GetBufferLength();
	}
	bool fProcess = true;
	if (!pData->IsParsed()) {
		if (!pData->CreateNlohmannJson(21, 2)) {	// ��������ǰ21λΪǰ׺������λΪ��׺
			gl_systemMessage.PushErrorMessage(_T("Netease RT data json parse error"));
			fProcess = false;
		}
	}
	if (fProcess && pData->IsParsed()) {
		json* pjs = pData->GetJSon();
		pvWebRTData = ParseNeteaseRTData(pjs);
	}
	return pvWebRTData;
}
