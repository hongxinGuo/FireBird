#include "pch.h"

#include"SystemData.h"

#include"WebInquirer.h"
#include "FinnhubWebSocket.h"

#include <ixwebsocket/IXWebSocket.h>

void FunctionProcessFinnhubWebSocket(const ix::WebSocketMessagePtr& msg) {
	switch (msg->type) {
	case ix::WebSocketMessageType::Message:
		// 当系统退出时，停止接收WebSocket的过程需要时间，在此期间此回调函数继续执行，而存储器已经析构了，导致出现内存泄漏。
		// 故而需要判断是否系统正在退出（只有在没有退出系统时方可存储接收到的数据）。
		if (!gl_fExitingSystem) {
			gl_WebInquirer.pushFinnhubWebSocketData(msg->str);
		}
		break;
	case ix::WebSocketMessageType::Error:
		gl_systemMessage.PushErrorMessage(msg->errorInfo.reason.c_str());
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
	static bool s_fDoing = false;
	if (!s_fDoing) {
		s_fDoing = true;
		gl_ThreadStatus.IncreaseSavingThread();
		pDataFinnhubWebSocket->ConnectWebSocketAndSendMessage(vSymbol);
		gl_ThreadStatus.DecreaseSavingThread();
		s_fDoing = false;
	}
	return 70;
}

CFinnhubWebSocket::CFinnhubWebSocket() : CVirtualWebSocket() {
	m_url = _T("wss://ws.finnhub.io");
	SetSubscriptionStatus(false); // finnhub WebSocket没有注册ID
}

CFinnhubWebSocket::~CFinnhubWebSocket(void) {
}

/// <summary>
/// finnhub数据源的格式：wss://ws.finnhub.io/?token=c1i57rv48v6vit20lrc0。
/// </summary>
/// <param name=""></param>
/// <returns></returns>
bool CFinnhubWebSocket::Connect(void) {
	CString strToken = gl_pFinnhubWebInquiry->GetInquiringStringSuffix();
	strToken = "/?" + strToken.Right(strToken.GetLength() - 1);
	string urlAndAuth = m_url + strToken.GetBuffer();

	return Connecting(urlAndAuth, FunctionProcessFinnhubWebSocket);
}

bool CFinnhubWebSocket::Send(vector<CString> vSymbol) {
	string strMessage;

	ASSERT(IsOpen());
	for (long l = 0; l < vSymbol.size(); l++) {
		strMessage = CreateFinnhubWebSocketString(vSymbol.at(l));
		Sending(strMessage);
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
	string sPreffix = _T("{\"type\":\"subscribe\",\"symbol\":\"");
	string sSuffix = _T("\"}");
	string sSymbol = strSymbol.GetBuffer();

	return sPreffix + sSymbol + sSuffix;
}

bool CFinnhubWebSocket::CreatingThreadConnectWebSocketAndSendMessage(vector<CString> vSymbol) {
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
					gl_SystemData.PushFinnhubSocket(pFinnhubDataPtr);
					m_fReveivingData = true;
				}
			}
			else if (sType.compare(_T("ping")) == 0) { // ping  {\"type\":\"ping\"}
			//
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