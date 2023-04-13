#include "pch.h"

#include"JsonParse.h"
#include"JsonGetValue.h"

#include "FinnhubWebSocket.h"

#include <ixwebsocket/IXWebSocket.h>

#include "FinnhubDataSource.h"
#include "TimeConvert.h"

using std::thread;
using std::make_shared;

void ProcessFinnhubWebSocket(const ix::WebSocketMessagePtr& msg) {
	CString str;
	switch (msg->type) {
	case ix::WebSocketMessageType::Message:
		// 当系统退出时，停止接收WebSocket的过程需要时间，在此期间此回调函数继续执行，而存储器已经析构了，导致出现内存泄漏。
		// 故而需要判断是否系统正在退出（只有在没有退出系统时方可存储接收到的数据）。
		if (!gl_systemStatus.IsExitingSystem()) {
			gl_finnhubWebSocket.PushData(msg->str);
		}
		break;
	case ix::WebSocketMessageType::Error:
		str = _T("Finnhub WebSocket Error: ");
		str += msg->errorInfo.reason.c_str();
		gl_systemMessage.PushErrorMessage(str);
		break;
	case ix::WebSocketMessageType::Open:
		gl_systemMessage.PushWebSocketInfoMessage(_T("Finnhub WebSocket Open"));
		break;
	case ix::WebSocketMessageType::Close:
		gl_systemMessage.PushWebSocketInfoMessage(_T("Finnhub WebSocket Close"));
		break;
	case ix::WebSocketMessageType::Fragment:
		gl_systemMessage.PushWebSocketInfoMessage(_T("Finnhub WebSocket Fragment"));
		break;
	case ix::WebSocketMessageType::Ping:
		gl_systemMessage.PushWebSocketInfoMessage(_T("Finnhub WebSocket Ping"));
		break;
	case ix::WebSocketMessageType::Pong:
		gl_systemMessage.PushWebSocketInfoMessage(_T("Finnhub WebSocket Pong"));
		break;
	default: // error
		ASSERT(0);
		break;
	}
}

UINT ThreadConnectFinnhubWebSocketAndSendMessage(not_null<CFinnhubWebSocket*> pDataFinnhubWebSocket, const vectorString& vSymbol) {
	static bool s_fConnecting = false;
	if (!s_fConnecting) {
		s_fConnecting = true;
		if (pDataFinnhubWebSocket->ConnectWebSocketAndSendMessage(vSymbol)) {
			gl_systemMessage.PushInnerSystemInformationMessage(_T("开启Finnhub web socket服务"));
		}
		s_fConnecting = false;
	}
	return 70;
}

CFinnhubWebSocket::CFinnhubWebSocket() {
	m_url = _T("wss://ws.finnhub.io");
	SetSubscriptionStatus(false); // finnhub WebSocket没有注册ID
}

/// <summary>
/// finnhub数据源的格式：wss://ws.finnhub.io/?token=c1i57rv48v6vit20lrc0。
/// </summary>
void CFinnhubWebSocket::Connect() {
	CString strToken = gl_pFinnhubDataSource->GetInquiryToken();
	strToken = "/?token=" + strToken;
	const string urlAndAuth = m_url + strToken.GetBuffer();

	Connecting(urlAndAuth, ProcessFinnhubWebSocket);
}

void CFinnhubWebSocket::Send(const vectorString& vSymbol) {
	string strMessage;

	ASSERT(IsOpen());
	for (long l = 0; l < vSymbol.size(); l++) {
		strMessage = CreateFinnhubWebSocketString(vSymbol.at(l));
		SendString(strMessage);
		gl_systemMessage.PushInnerSystemInformationMessage(strMessage.c_str());
	}
}

/// <summary>
/// Finnhub web socket格式： {"type":"subscribe","symbol":"符号串"},IEX， Crypto, Forex皆使用此格式
/// 如{"type":"subscribe","symbol":"MSFT"}, {"type":"subscribe","symbol":"BINANCE:LTCBTC"}, {"type":"subscribe","symbol":"OANDA:AUD_SGD"}
/// </summary>
string CFinnhubWebSocket::CreateFinnhubWebSocketString(string sSymbol) {
	json symbol;
	symbol["type"] = _T("subscribe");
	symbol["symbol"] = sSymbol;
	return symbol.dump();
}

void CFinnhubWebSocket::CreateThreadConnectWebSocketAndSendMessage(vectorString vSymbol) {
	thread thread1(ThreadConnectFinnhubWebSocketAndSendMessage, this, vSymbol);
	thread1.detach();
}

/// <summary>
///
/// https://finnhub.io/docs/api/websocket-trades
///
/// 目前三种。
/// 格式为：{"data":[{"c":null,"p":7296.89,"s":"BINANCE:BTCUSDT","t":1575526691134,"v":0.011467}],"type":"trade"}
///        {"type":"ping"}
///        {"msg":"Subscribing to too many symbols","type":"error"}
/// </summary>
/// <param name="pData"></param>
/// <returns></returns>
bool CFinnhubWebSocket::ParseFinnhubWebSocketData(shared_ptr<string> pData) {
	string sType, sSymbol, sMessage;
	string code;

	try {
		if (json pt; CreateJsonWithNlohmann(pt, *pData)) {
			sType = jsonGetString(&pt, _T("type"));
			if (sType == _T("trade")) { // {"data":[{"c":null,"p":7296.89,"s":"BINANCE:BTCUSDT","t":1575526691134,"v":0.011467}],"type":"trade"}
				json js2;
				json pt3;
				// 交易数据
				js2 = jsonGetChild(&pt, _T("data"));
				for (auto it = js2.begin(); it != js2.end(); ++it) {
					const auto pFinnhubDataPtr = make_shared<CFinnhubSocket>();
					pFinnhubDataPtr->m_sSymbol = jsonGetString(it, _T("s"));
					pt3 = jsonGetChild(it, _T("c"));
					for (auto it2 = pt3.begin(); it2 != pt3.end(); ++it2) { pFinnhubDataPtr->m_vCode.push_back(jsonGetString(it2)); }
					pFinnhubDataPtr->m_dLastPrice = jsonGetDouble(it, _T("p"));
					pFinnhubDataPtr->m_dLastVolume = jsonGetDouble(it, _T("v"));
					pFinnhubDataPtr->m_iSeconds = jsonGetLongLong(it, _T("t"));
					gl_SystemData.PushFinnhubSocket(pFinnhubDataPtr);
					m_fReceivingData = true;
				}
			}
			else if (sType == _T("ping")) {
				// ping  {\"type\":\"ping\"}
				m_HeartbeatTime = GetUTCTime();
			}
			else if (sType == _T("error")) {
				// ERROR {\"msg\":\"Subscribing to too many symbols\",\"type\":\"error\"}
				sMessage = jsonGetString(&pt, _T("msg"));
				CString strMessage = "Finnhub WebSocket error message: ";
				strMessage += sMessage.c_str();
				gl_systemMessage.PushInnerSystemInformationMessage(strMessage);
				return false;
			}
			else {
				// ERROR
				CString strMsg = _T("Finnhub Web Socket type error: ");
				strMsg += sType.c_str();
				gl_systemMessage.PushInnerSystemInformationMessage(strMsg);
				return false;
			}
		}
		else {
			gl_systemMessage.PushInnerSystemInformationMessage(_T("Finnhub Web Socket json error"));
			return false;
		}
	}
	catch (json::exception& e) {
		ReportJSonErrorToSystemMessage(_T("Process One Finnhub WebSocketData "), e.what());
		return false;
	}

	return true;
}
