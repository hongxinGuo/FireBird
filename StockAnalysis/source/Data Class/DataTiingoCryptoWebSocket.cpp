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
/// Tiingo Crypto������Դ��ʽ��wss://api.tiingo.com/crypto������Կ������͵ġ�
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
// �������е�Crypto����ʱ��ÿ��������������50-100K������
//
// thresholdlevel 2: Top-of-Book AND Last Trade updates.
// thresholdlevel 5: only Last Trade updates.
//
//////////////////////////////////////////////////////////////////////////////////////////////
bool CDataTiingoCryptoWebSocket::Send(vector<CString> vSymbol) {
	static bool sm_fSendAuth = true;
	CString str;
	CString strPreffix = _T("{\"eventName\":\"subscribe\",\"authorization\":\"");
	CString strMiddle = _T("\",\"eventData\":{\"thresholdLevel\":2,\"tickers\":["); // 5��Trade Updates per-exchange.2��Top-of-Book quote updates as well as Trade updates. Both quote and trade updates are per-exchange
	CString strSuffix = _T("]}}"); // 5��Trade Updates per-exchange.2��Top-of-Book quote updates as well as Trade updates. Both quote and trade updates are per-exchange
	CString strAuth = gl_pTiingoWebInquiry->GetInquiringStringSuffix();

	strAuth = strAuth.Right(strAuth.GetLength() - 7);
	vSymbol.push_back(_T("dkaeth")); // ���һ��Tiingo��ʽ�Ĵ��롣����Ŀǰ��ѡcryptoʹ�õ���finnhub��ʽ�Ĵ����ʽ����Ϊ��Ч���롣
	vSymbol.push_back(_T("ksmust")); // ���һ��Tiingo��ʽ�Ĵ��롣����Ŀǰ��ѡcryptoʹ�õ���finnhub��ʽ�Ĵ����ʽ����Ϊ��Ч���롣
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