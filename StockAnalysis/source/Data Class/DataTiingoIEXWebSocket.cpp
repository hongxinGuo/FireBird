#include "pch.h"

#include"WebInquirer.h"
#include"CallableFunction.h"
#include "DataTiingoIEXWebSocket.h"

CDataTiingoIEXWebSocket::CDataTiingoIEXWebSocket() : CVirtualDataWebSocket() {
}

CDataTiingoIEXWebSocket::~CDataTiingoIEXWebSocket(void) {
}

/// <summary>
/// Tiingo IEX������Դ��ʽ��wss://api.tiingo.com/iex������Կ������͵ġ�
/// </summary>
/// <param name=""></param>
/// <returns></returns>
bool CDataTiingoIEXWebSocket::Connecting(void) {
	std::string url("wss://api.tiingo.com/iex");

	m_webSocket.Connecting(url, FunctionProcessTiingoIEXWebSocket);

	return true;
}

bool CDataTiingoIEXWebSocket::Send(vector<CString> vSymbol) {
	static bool sm_fSendAuth = true;
	CString str;
	CString strSymbols;
	CString strPreffix = _T("{\"eventName\":\"subscribe\",\"authorization\":\"");
	CString strMiddle = _T("\",\"eventData\":{\"thresholdLevel\":5,\"tickers\":[");
	CString strSuffix = _T("]}}");
	CString strAuth = gl_pTiingoWebInquiry->GetInquiringStringSuffix();
	strAuth = strAuth.Right(strAuth.GetLength() - 7);

	strSymbols = CreateTiingoWebSocketSymbolString(vSymbol); // ȥ����������ַ�','

	str = strPreffix + strAuth + strMiddle + strSymbols + strSuffix;

	string messageAuth(str);
	ix::WebSocketSendInfo info;

	ASSERT(m_webSocket.IsOpen());

	if (sm_fSendAuth) {
		info = m_webSocket.Send(messageAuth);
	}

	return true;
}