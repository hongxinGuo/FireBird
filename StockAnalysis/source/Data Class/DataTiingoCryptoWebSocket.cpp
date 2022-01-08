#include "pch.h"

#include"WebInquirer.h"
#include"CallableFunction.h"
#include "DataTiingoCryptoWebSocket.h"

UINT ThreadConnectingTiingoCryptoWebSocketAndSendMessage(not_null<CDataTiingoCryptoWebSocket*> pDataTiingoCryptoWebSocket, vector<CString> vSymbol) {
	gl_ThreadStatus.IncreaseSavingThread();
	pDataTiingoCryptoWebSocket->ConnectingWebSocketAndSendMessage(vSymbol);
	gl_ThreadStatus.DecreaseSavingThread();

	return 73;
}

CDataTiingoCryptoWebSocket::CDataTiingoCryptoWebSocket() : CVirtualDataWebSocket() {
}

CDataTiingoCryptoWebSocket::~CDataTiingoCryptoWebSocket(void) {
}

/// <summary>
/// Tiingo Crypto的数据源格式：wss://api.tiingo.com/crypto，其密钥是随后发送的。
/// </summary>
/// <param name=""></param>
/// <returns></returns>
bool CDataTiingoCryptoWebSocket::Connecting(void) {
	std::string url("wss://api.tiingo.com/crypto");

	m_webSocket.Connecting(url, FunctionProcessTiingoCryptoWebSocket);

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
// 接收所有的Crypto数据时，每秒数据量大致在50-100K附近。
//
// thresholdlevel 2: Top-of-Book AND Last Trade updates.
// thresholdlevel 5: only Last Trade updates.
//
//////////////////////////////////////////////////////////////////////////////////////////////
bool CDataTiingoCryptoWebSocket::Send(vector<CString> vSymbol) {
	static bool sm_fSendAuth = true;
	CString str;
	CString strPreffix = _T("{\"eventName\":\"subscribe\",\"authorization\":\"");
	CString strMiddle = _T("\",\"eventData\":{\"thresholdLevel\":2,\"tickers\":["); // 5：Trade Updates per-exchange.2：Top-of-Book quote updates as well as Trade updates. Both quote and trade updates are per-exchange
	CString strSuffix = _T("]}}"); // 5：Trade Updates per-exchange.2：Top-of-Book quote updates as well as Trade updates. Both quote and trade updates are per-exchange
	CString strAuth = gl_pTiingoWebInquiry->GetInquiringStringSuffix();

	strAuth = strAuth.Right(strAuth.GetLength() - 7);
	vSymbol.push_back(_T("dkaeth")); // 多加一个Tiingo制式的代码。由于目前自选crypto使用的是finnhub制式的代码格式，皆为无效代码。
	vSymbol.push_back(_T("ksmust")); // 多加一个Tiingo制式的代码。由于目前自选crypto使用的是finnhub制式的代码格式，皆为无效代码。
	CString strSymbols = CreateTiingoWebSocketSymbolString(vSymbol);

	str = strPreffix + strAuth + strMiddle + strSymbols + strSuffix;

	string messageAuth(str);
	ix::WebSocketSendInfo info;

	ASSERT(m_webSocket.IsOpen());

	if (sm_fSendAuth) {
		info = m_webSocket.Send(messageAuth);
	}

	return true;
}

bool CDataTiingoCryptoWebSocket::CreatingThreadConnectingWebSocketAndSendMessage(vector<CString> vSymbol) {
	thread thread1(ThreadConnectingTiingoCryptoWebSocketAndSendMessage, this, vSymbol);
	thread1.detach();

	return true;
}