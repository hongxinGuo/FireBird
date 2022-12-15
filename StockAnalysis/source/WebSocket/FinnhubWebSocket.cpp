#include "pch.h"

#include"JsonParse.h"

#include "FinnhubWebSocket.h"
#include"FinnhubWebInquiry.h"

#include <ixwebsocket/IXWebSocket.h>

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
		break;
	}
}

UINT ThreadConnectFinnhubWebSocketAndSendMessage(not_null<CFinnhubWebSocket*> pDataFinnhubWebSocket, vector<CString> vSymbol) {
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

CFinnhubWebSocket::CFinnhubWebSocket() : CVirtualWebSocket() {
	m_url = _T("wss://ws.finnhub.io");
	SetSubscriptionStatus(false); // finnhub WebSocket没有注册ID
}

/// <summary>
/// finnhub数据源的格式：wss://ws.finnhub.io/?token=c1i57rv48v6vit20lrc0。
/// </summary>
/// <param name=""></param>
/// <returns></returns>
bool CFinnhubWebSocket::Connect(void) {
	CString strToken = gl_pFinnhubWebInquiry->GetInquiryToken();
	strToken = "/?" + strToken.Right(strToken.GetLength() - 1);
	const string urlAndAuth = m_url + strToken.GetBuffer();

	return Connecting(urlAndAuth, ProcessFinnhubWebSocket);
}

bool CFinnhubWebSocket::Send(vector<CString> vSymbol) {
	string strMessage;

	ASSERT(IsOpen());
	for (long l = 0; l < vSymbol.size(); l++) {
		strMessage = CreateFinnhubWebSocketString(vSymbol.at(l));
		SendMessage(strMessage);
		gl_systemMessage.PushInnerSystemInformationMessage(strMessage.c_str());
	}

	return true;
}

/// <summary>
/// Finnhub web socket格式： {"type":"subscribe","symbol":"符号串"},IEX， Crypto, Forex皆使用此格式
/// 如{"type":"subscribe","symbol":"MSFT"}, {"type":"subscribe","symbol":"BINANCE:LTCBTC"}, {"type":"subscribe","symbol":"OANDA:AUD_SGD"}
/// </summary>
/// <param name="strSymbol"></param>
/// <returns></returns>
string CFinnhubWebSocket::CreateFinnhubWebSocketString(CString strSymbol) {
	const string sPrefix = _T("{\"type\":\"subscribe\",\"symbol\":\"");
	const string sSuffix = _T("\"}");
	const string sSymbol = strSymbol.GetBuffer();

	return sPrefix + sSymbol + sSuffix;
}

bool CFinnhubWebSocket::CreateThreadConnectWebSocketAndSendMessage(vector<CString> vSymbol) {
	thread thread1(ThreadConnectFinnhubWebSocketAndSendMessage, this, vSymbol);
	thread1.detach();

	return true;
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
		if (ptree pt; ParseWithPTree(pt, *pData)) {
			sType = ptreeGetString(pt, _T("type"));
			if (sType == _T("trade")) {
				CFinnhubSocketPtr pFinnhubDataPtr;
				ptree pt3;
				ptree pt2;
				// 交易数据
				pt2 = pt.get_child(_T("data"));
				for (ptree::iterator it = pt2.begin(); it != pt2.end(); ++it) {
					pt3 = it->second;
					pFinnhubDataPtr = make_shared<CFinnhubSocket>();
					sSymbol = ptreeGetString(pt3, _T("s"));
					pFinnhubDataPtr->m_strSymbol = sSymbol.c_str();
					code = ptreeGetString(pt3, _T("c"));
					pFinnhubDataPtr->m_strCode = code.c_str();
					pFinnhubDataPtr->m_dLastPrice = ptreeGetDouble(pt3, _T("p"));
					pFinnhubDataPtr->m_dLastVolume = ptreeGetDouble(pt3, _T("v"));
					pFinnhubDataPtr->m_iSeconds = pt3.get<time_t>(_T("t"));
					gl_SystemData.PushFinnhubSocket(pFinnhubDataPtr);
					m_fReceivingData = true;
				}
			}
			else if (sType == _T("ping")) {
				// ping  {\"type\":\"ping\"}
				//
			}
			else if (sType == _T("error")) {
				CString strMessage;
				// ERROR {\"msg\":\"Subscribing to too many symbols\",\"type\":\"error\"}
				sMessage = ptreeGetString(pt, _T("msg"));
				strMessage = _T("Finnhub WebSocket error message: ");
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
	catch (ptree_error& e) {
		ReportJSonErrorToSystemMessage(_T("Process One Finnhub WebSocketData "), e);
		return false;
	}

	return true;
}
