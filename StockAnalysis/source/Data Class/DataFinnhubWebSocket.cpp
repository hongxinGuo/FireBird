#include "pch.h"

#include"CallableFunction.h"
#include"WebInquirer.h"
#include "DataFinnhubWebSocket.h"

UINT ThreadConnectingFinnhubWebSocketAndSendMessage(not_null<CDataFinnhubWebSocket*> pDataFinnhubWebSocket, vector<CString> vSymbol) {
	gl_ThreadStatus.IncreaseSavingThread();
	pDataFinnhubWebSocket->ConnectingWebSocketAndSendMessage(vSymbol);
	gl_ThreadStatus.DecreaseSavingThread();

	return 70;
}

CDataFinnhubWebSocket::CDataFinnhubWebSocket() : CVirtualDataWebSocket() {
	m_webSocket.SetSubscriptionStatus(false); // finnhub WebSocket没有注册ID
}

CDataFinnhubWebSocket::~CDataFinnhubWebSocket(void) {
}

/// <summary>
/// finnhub数据源的格式：wss://ws.finnhub.io/?token=c1i57rv48v6vit20lrc0。
/// </summary>
/// <param name=""></param>
/// <returns></returns>
bool CDataFinnhubWebSocket::Connecting(void) {
	std::string url("wss://ws.finnhub.io");
	CString strToken = gl_pFinnhubWebInquiry->GetInquiringStringSuffix();
	strToken = "/?" + strToken.Right(strToken.GetLength() - 1);
	url += strToken.GetBuffer();

	return m_webSocket.Connecting(url, FunctionProcessFinnhubWebSocket);
}

bool CDataFinnhubWebSocket::Send(vector<CString> vSymbol) {
	string strMessage;

	ASSERT(m_webSocket.IsOpen());
	for (long l = 0; l < vSymbol.size(); l++) {
		strMessage = CreateFinnhubWebSocketString(vSymbol.at(l));
		m_webSocket.Send(strMessage);
	}

	return true;
}

/// <summary>
/// Finnhub web socket格式： {"type":"subscribe","symbol":"符号串"},IEX， Crypto, Forex皆使用此格式
/// 如{"type":"subscribe","symbol":"MSFT"}, {"type":"subscribe","symbol":"BINANCE:LTCBTC"}, {"type":"subscribe","symbol":"OANDA:AUD_SGD"}
/// </summary>
/// <param name="strSymbol"></param>
/// <returns></returns>
string CDataFinnhubWebSocket::CreateFinnhubWebSocketString(CString strSymbol) {
	string sPreffix = _T("{\"type\":\"subscribe\",\"symbol\":\"");
	string sSuffix = _T("\"}");
	string sSymbol = strSymbol.GetBuffer();

	return sPreffix + sSymbol + sSuffix;
}

bool CDataFinnhubWebSocket::CreatingThreadConnectingWebSocketAndSendMessage(vector<CString> vSymbol) {
	thread thread1(ThreadConnectingFinnhubWebSocketAndSendMessage, this, vSymbol);
	thread1.detach();

	return true;
}