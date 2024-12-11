#include "pch.h"
module;
#include"globedef.h"
#include <ixwebsocket/IXWebSocket.h>
module FireBird.WebSocket.TiingoIEX;

import FireBird.DataSource.Quandl;
import FireBird.DataSource.Tiingo;
import FireBird.Accessory.JsonParse;
import FireBird.System.Message;
import FireBird.System.Configuration;
import FireBird.Accessory.JsonGetValue;

void ProcessTiingoIEXWebSocket(const ix::WebSocketMessagePtr& msg) {
	gl_pTiingoIEXWebSocket->SetError(false);
	switch (msg->type) {
	case ix::WebSocketMessageType::Message:
		// 当系统退出时，停止接收WebSocket的过程需要时间，在此期间此回调函数继续执行，而存储器已经析构了，导致出现内存泄漏。
		// 故而需要判断是否系统正在退出（只有在没有退出系统时方可存储接收到的数据）。
		if (!gl_systemConfiguration.IsExitingSystem()) {
			gl_pTiingoIEXWebSocket->PushData(msg->str);
		}
		break;
	case ix::WebSocketMessageType::Error:
		gl_pTiingoIEXWebSocket->SetError(true);
		gl_systemMessage.PushErrorMessage(msg->errorInfo.reason.c_str());
		break;
	case ix::WebSocketMessageType::Open:
		gl_systemMessage.PushWebSocketInfoMessage(_T("Tiingo IEX WebSocket Open"));
		break;
	case ix::WebSocketMessageType::Close:
		gl_systemMessage.PushWebSocketInfoMessage(_T("Tiingo IEX WebSocket Close"));
		break;
	case ix::WebSocketMessageType::Fragment:
		gl_systemMessage.PushWebSocketInfoMessage(_T("Tiingo IEX WebSocket Fragment"));
		break;
	case ix::WebSocketMessageType::Ping:
		gl_systemMessage.PushWebSocketInfoMessage(_T("Tiingo IEX WebSocket Ping"));
		break;
	case ix::WebSocketMessageType::Pong:
		gl_systemMessage.PushWebSocketInfoMessage(_T("Tiingo IEX WebSocket Pong"));
		break;
	default: // error
		break;
	}
}

CTiingoIEXWebSocket::CTiingoIEXWebSocket() {
	ASSERT(gl_systemConfiguration.IsInitialized());
	m_url = _T("wss://api.tiingo.com/iex");
}

/// <summary>
/// Tiingo IEX的数据源格式：wss://api.tiingo.com/iex，其密钥是随后发送的。
/// </summary>
/// 
/// <returns></returns>
void CTiingoIEXWebSocket::Connect() {
	Connecting(m_url, ProcessTiingoIEXWebSocket);
}

void CTiingoIEXWebSocket::Send(const vectorString& vSymbol) {
	ASSERT(IsOpen());

	const string messageAuth(CreateMessage(vSymbol));
	ix::WebSocketSendInfo info = m_webSocket.send(messageAuth);

	gl_systemMessage.PushInnerSystemInformationMessage(messageAuth.c_str());
}

void CTiingoIEXWebSocket::MonitorWebSocket(const vectorString& vSymbol) {
	if (IsConnecting()) return; // 如果正在连接，则不监控该socket
	CVirtualWebSocket::MonitorWebSocket(gl_pTiingoDataSource->IsWebError(), gl_systemConfiguration.IsUsingTiingoIEXWebSocket(), vSymbol);
}

///////////////////////////////////////////////////////////////////////
///
/// 格式：
/// {
///  "eventName" : "subscribe",
///  "authorization" : d897a00b7cc630d235316a4683156, // tiingo token
///  "eventData" :
///   {
///    "thresholdLevel" : 5,
///		 "tickers" : ["AAPL","MSFT", "RIG"]
///   }
/// }
///
///////////////////////////////////////////////////////////////////////
string CTiingoIEXWebSocket::CreateMessage(const vectorString& vSymbol) {
	vectorString vSymbol2;
	nlohmann::ordered_json jsonMessage;
	jsonMessage["eventName"] = _T("subscribe");
	jsonMessage["authorization"] = gl_pTiingoDataSource->GetInquiryToken();
	jsonMessage["eventData"]["thresholdLevel"] = 5; // threshold的有效数字为0或者5
	for (auto str : vSymbol) {
		std::ranges::transform(str, str.begin(), ::tolower); // Tiingo webSocket使用小写字符
		vSymbol2.push_back(str);
	}
	jsonMessage["eventData"]["tickers"] = vSymbol2;
	jsonMessage["eventData"]["tickers"].emplace_back(_T("rig"));// tiingo使用的stock符号与finnhub不同
	jsonMessage["eventData"]["tickers"].emplace_back(_T("eurusd"));// tiingo使用的stock符号与finnhub不同
	jsonMessage["eventData"]["tickers"].emplace_back(_T("spy"));// tiingo使用的stock符号与finnhub不同
	jsonMessage["eventData"]["tickers"].emplace_back(_T("uso"));// tiingo使用的stock符号与finnhub不同

	return jsonMessage.dump();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// https://api.tiingo.com/documentation/websockets/iex
//
// 共五种格式：
// {"messageType":"I","data":{"subscriptionId":2563367},"response":{"code":200,"message":"Success"}}
// {"data":{"tickers":["*","uso","msft","tnk"],"thresholdLevel":"0"},"messageType":"I","response":{"code":200,"message":"Success"}}
// {"messageType":"H","response":{"code":200,"message":"HeartBeat"}}
// {"messageType":"A","service":"iex","data":["Q","2019-01-30T13:33:45.383129126-05:00",1548873225383129126,"vym",100,81.58,81.585,81.59,100,null,null,0,0,null,null,null]}
// {"messageType":"A","service":"iex","data":["T","2019-01-30T13:33:45.594808294-05:00",1548873225594808294,"wes",null,null,null,null,null,50.285,200,null,0,0,0,0]}
// {"messageType":"E","response":{"code":400,"message":"thresholdLevel not valid}}
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CTiingoIEXWebSocket::ParseTiingoIEXWebSocketData(shared_ptr<string> pData) {
	string sSymbol;
	string strSymbol;
	string sTicker, sExchange, sValue;
	CTiingoIEXSocketPtr pIEXData = nullptr;
	chrono::time_point<chrono::system_clock, chrono::nanoseconds> tpTime;
	chrono::sys_seconds tpTime1;
	stringstream ss;
	chrono::sys_seconds tpTime2;
	chrono::minutes Minutes;
	string sString;
	tm tm_;
	time_t tt;
	try {
		if (nlohmann::ordered_json js; CreateJsonWithNlohmann(js, *pData)) {
			int i = 0;
			string sMessageType;
			char chType;
			string sService;
			string sType;
			nlohmann::ordered_json js2, js3, js4;
			nlohmann::ordered_json::iterator it;
			sType = jsonGetString(js, _T("messageType"));
			if (sType.empty()) return false;
			switch (sType.at(0)) {
			case 'A': // 交易数据
				pIEXData = make_shared<CTiingoIEXSocket>();
				sService = jsonGetString(js, _T("service"));
				if (sService != _T("iex")) return false; // 此项必须为"iex"
				js2 = jsonGetChild(js, _T("data"));
				it = js2.begin();
				sMessageType = jsonGetString(it); // message type, 'Q'、'T'或者'B'
				chType = sMessageType.at(0);
				pIEXData->m_chMessageType = chType;
				switch (chType) {
				case 'Q': // top-of-book update message
					pIEXData->m_sDateTime = jsonGetString(++it);
					ss.str(pIEXData->m_sDateTime);
					chrono::from_stream(ss, "%FT%H:%M:%12S%Ez", tpTime, &sString, &Minutes);
					pIEXData->m_tpTime = chrono::time_point_cast<chrono::seconds>(tpTime);
					pIEXData->m_iNanoseconds = jsonGetLongLong(++it);
					pIEXData->m_sSymbol = jsonGetString(++it);
					pIEXData->m_dBidSize = jsonGetDouble(++it);
					pIEXData->m_dBidPrice = jsonGetDouble(++it);
					pIEXData->m_dMidPrice = jsonGetDouble(++it);
					pIEXData->m_dAskPrice = jsonGetDouble(++it);
					pIEXData->m_dAskSize = jsonGetDouble(++it);
					pIEXData->m_dLastPrice = jsonGetDouble(++it);
					pIEXData->m_dLastSize = jsonGetDouble(++it);
					pIEXData->m_iAfterHour = jsonGetInt(++it);
					pIEXData->m_iISO = jsonGetInt(++it);
					pIEXData->m_iOddlot = jsonGetInt(++it);
					pIEXData->m_iNMSRule611 = jsonGetInt(++it);
					break;
				case 'T': // T last trade message
					pIEXData->m_sDateTime = jsonGetString(++it);
					pIEXData->m_iNanoseconds = jsonGetLongLong(++it);
					pIEXData->m_sSymbol = jsonGetString(++it);
					pIEXData->m_dBidSize = jsonGetDouble(++it);
					pIEXData->m_dBidPrice = jsonGetDouble(++it);
					pIEXData->m_dMidPrice = jsonGetDouble(++it);
					pIEXData->m_dAskPrice = jsonGetDouble(++it);
					pIEXData->m_dAskSize = jsonGetDouble(++it);
					pIEXData->m_dLastPrice = jsonGetDouble(++it);
					pIEXData->m_dLastSize = jsonGetInt(++it);
					pIEXData->m_iHalted = jsonGetInt(++it);
					pIEXData->m_iISO = jsonGetInt(++it);
					pIEXData->m_iOddlot = jsonGetInt(++it);
					pIEXData->m_iNMSRule611 = jsonGetInt(++it);
					break;
				case 'B': // 'B' trade break messages
					i++;
					break;
				default: // 错误
					gl_systemMessage.PushInnerSystemInformationMessage(_T("Tiingo IEX WebSocket type error"));
					return false;
				}
				gl_SystemData.PushTiingoIEXSocket(pIEXData);
				m_HeartbeatTime = GetUTCTime();
				break;
			case 'I':  // 共两种。一种是报告当前查询证券代码，另一种是报告注册信息
				js2 = jsonGetChild(js, _T("data"));
				try { // {"data":{"tickers":["*","uso","msft","tnk"],"thresholdLevel":"0"},"messageType":"I","response":{"code":200,"message":"Success"}}
					js3 = js2.at(_T("tickers"));
					for (auto it2 = js3.begin(); it2 != js3.end(); ++it2) {
						strSymbol = jsonGetString(it2);
						m_vCurrentInquireSymbol.emplace_back(strSymbol.c_str());
					}
				} catch (nlohmann::ordered_json::exception&) { // 注册ID {"messageType":"I","data":{"subscriptionId":2563367},"response":{"code":200,"message":"Success"}}
					ASSERT(GetSubscriptionId() == 0);
					SetSubscriptionId(jsonGetInt(&js2, _T("subscriptionId")));
				}
				break;
			case 'H': //Heart beat {\"messageType\":\"H\",\"response\":{\"code\":200,\"message\":\"HeartBeat\"}}
				js3 = jsonGetChild(js, _T("response"));
				m_iStatusCode = js3.at(_T("code"));
				m_statusMessage = js3.at(_T("message"));
				break;
			case 'E':  //error message {"messageType":"E","response":{"code":400,"message":"thresholdLevel not valid"}}
				js4 = jsonGetChild(js, _T("response"));
				m_iStatusCode = js4.at(_T("code"));
				m_statusMessage = js4.at(_T("message"));
				break;
			default:
				gl_systemMessage.PushInnerSystemInformationMessage(_T("Tiingo IEX WebSocket type error"));
				return false;
			}
		}
		else {
			// ERROR
			return false;
		}
	} catch (nlohmann::ordered_json::exception& e) {
		ReportJSonErrorToSystemMessage(_T("Tiingo IEX WebSocket "), e.what());
		return false;
	}

	return true;
}
