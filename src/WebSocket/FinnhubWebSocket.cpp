#include "pch.h"

#include"JsonParse.h"
#include"JsonGetValue.h"

#include "FinnhubWebSocket.h"
#include"FinnhubWebInquiry.h"

#include <ixwebsocket/IXWebSocket.h>

#include<thread>
#include<memory>
using std::thread;
using std::make_shared;

void ProcessFinnhubWebSocket(const ix::WebSocketMessagePtr& msg) {
	CString str;
	switch (msg->type) {
	case ix::WebSocketMessageType::Message:
		// ��ϵͳ�˳�ʱ��ֹͣ����WebSocket�Ĺ�����Ҫʱ�䣬�ڴ��ڼ�˻ص���������ִ�У����洢���Ѿ������ˣ����³����ڴ�й©��
		// �ʶ���Ҫ�ж��Ƿ�ϵͳ�����˳���ֻ����û���˳�ϵͳʱ���ɴ洢���յ������ݣ���
		if (!gl_systemStatus.IsExitingSystem()) { gl_finnhubWebSocket.PushData(msg->str); }
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

UINT ThreadConnectFinnhubWebSocketAndSendMessage(not_null<CFinnhubWebSocket*> pDataFinnhubWebSocket, vectorString vSymbol) {
	static bool s_fConnecting = false;
	if (!s_fConnecting) {
		s_fConnecting = true;
		if (pDataFinnhubWebSocket->ConnectWebSocketAndSendMessage(vSymbol)) {
			gl_systemMessage.PushInnerSystemInformationMessage(_T("����Finnhub web socket����"));
		}
		s_fConnecting = false;
	}
	return 70;
}

CFinnhubWebSocket::CFinnhubWebSocket() : CVirtualWebSocket() {
	m_url = _T("wss://ws.finnhub.io");
	SetSubscriptionStatus(false); // finnhub WebSocketû��ע��ID
}

/// <summary>
/// finnhub����Դ�ĸ�ʽ��wss://ws.finnhub.io/?token=c1i57rv48v6vit20lrc0��
/// </summary>
/// <param name=""></param>
/// <returns></returns>
bool CFinnhubWebSocket::Connect(void) {
	CString strToken = gl_pFinnhubWebInquiry->GetInquiryToken();
	strToken = "/?" + strToken.Right(strToken.GetLength() - 1);
	const string urlAndAuth = m_url + strToken.GetBuffer();

	return Connecting(urlAndAuth, ProcessFinnhubWebSocket);
}

bool CFinnhubWebSocket::Send(vectorString vSymbol) {
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
/// Finnhub web socket��ʽ�� {"type":"subscribe","symbol":"���Ŵ�"},IEX�� Crypto, Forex��ʹ�ô˸�ʽ
/// ��{"type":"subscribe","symbol":"MSFT"}, {"type":"subscribe","symbol":"BINANCE:LTCBTC"}, {"type":"subscribe","symbol":"OANDA:AUD_SGD"}
/// </summary>
/// <param name="strSymbol"></param>
/// <returns></returns>
string CFinnhubWebSocket::CreateFinnhubWebSocketString(string sSymbol) {
	const string sPrefix = _T("{\"type\":\"subscribe\",\"symbol\":\"");
	const string sSuffix = _T("\"}");

	return sPrefix + sSymbol + sSuffix;
}

bool CFinnhubWebSocket::CreateThreadConnectWebSocketAndSendMessage(vectorString vSymbol) {
	thread thread1(ThreadConnectFinnhubWebSocketAndSendMessage, this, vSymbol);
	thread1.detach();

	return true;
}

/// <summary>
///
/// https://finnhub.io/docs/api/websocket-trades
///
/// Ŀǰ���֡�
/// ��ʽΪ��{"data":[{"c":null,"p":7296.89,"s":"BINANCE:BTCUSDT","t":1575526691134,"v":0.011467}],"type":"trade"}
///        {"type":"ping"}
///        {"msg":"Subscribing to too many symbols","type":"error"}
/// </summary>
/// <param name="pData"></param>
/// <returns></returns>
bool CFinnhubWebSocket::ParseFinnhubWebSocketData(shared_ptr<string> pData) {
	string sType, sSymbol, sMessage;
	string code;

	try {
		if (json pt; NlohmannCreateJson(&pt, *pData)) {
			sType = jsonGetString(&pt, _T("type"));
			if (sType == _T("trade")) { // {"data":[{"c":null,"p":7296.89,"s":"BINANCE:BTCUSDT","t":1575526691134,"v":0.011467}],"type":"trade"}
				json js2;
				json pt3;
				// ��������
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
				//
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