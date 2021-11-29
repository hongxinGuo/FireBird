#include"pch.h"
#include"globedef.h"
#include"accessory.h"
#include"WebInquirer.h"

using namespace std;
#include<memory>
#include"string"

#include<boost/property_tree/ptree.hpp>
#include<boost/property_tree/json_parser.hpp>
using namespace boost::property_tree;

/// <summary>
/// 
/// https://finnhub.io/docs/api/websocket-trades
/// 
/// 格式为：{"data":[{"c":null,"p":7296.89,"s":"BINANCE:BTCUSDT","t":1575526691134,"v":0.011467}],"type":"trade"}
///        {"type":"ping"}
///        {"msg":"Subscribing to too many symbols","type":"error"}
/// 目前就这三种。
/// </summary>
/// <param name="pData"></param>
/// <returns></returns>
bool ProcessOneFinnhubWebSocketData(shared_ptr<string> pData) {
	ptree pt, pt2, pt3;
	string sType, sSymbol, sMessage;
	CString strMessage;
	string code;
	CFinnhubSocketPtr pFinnhubDataPtr = nullptr;

	try {
		if (ConvertToJSON(pt, *pData)) {
			sType = pt.get<string>(_T("type"));
			if (sType.compare(_T("trade")) == 0) { // 交易数据
				pt2 = pt.get_child(_T("data"));
				for (ptree::iterator it = pt2.begin(); it != pt2.end(); ++it) {
					pt3 = it->second;
					pFinnhubDataPtr = make_shared<CFinnhubSocket>();
					sSymbol = pt3.get<string>(_T("s"));
					pFinnhubDataPtr->m_strSymbol = sSymbol.c_str();
					code = pt3.get<string>(_T("c"));
					if (code.compare(_T("null")) == 0) code = _T("");
					pFinnhubDataPtr->m_strCode = code.c_str();
					pFinnhubDataPtr->m_dLastPrice = pt3.get<double>(_T("p"));
					pFinnhubDataPtr->m_dLastVolume = pt3.get<double>(_T("v"));
					pFinnhubDataPtr->m_iSeconds = pt3.get<time_t>(_T("t"));
					gl_WebInquirer.PushFinnhubSocket(pFinnhubDataPtr);
				}
			}
			else if (sType.compare(_T("ping")) == 0) { // ping  {\"type\":\"ping\"}
				// do nothing
			}
			else if (sType.compare(_T("error")) == 0) { // ERROR {\"msg\":\"Subscribing to too many symbols\",\"type\":\"error\"}
				sMessage = pt.get<string>(_T("msg"));
				strMessage = _T("Finnhub WebSocket error message: ");
				strMessage += sMessage.c_str();
				gl_systemMessage.PushInnerSystemInformationMessage(strMessage);
				return false;
			}
			else {
				// ERROR
				return false;
			}
		}
		else {
			return false;
		}
	}
	catch (ptree_error& e) {
		ReportJSonErrorToSystemMessage(_T("Process One Finnhub WebSocketData "), e);
		return false;
	}

	return true;
}