#include "pch.h"

#include"CallableFunction.h"
#include"WebInquirer.h"
#include "DataTiingoForexWebSocket.h"

CDataTiingoForexWebSocket::CDataTiingoForexWebSocket() : CVirtualDataWebSocket() {
}

CDataTiingoForexWebSocket::~CDataTiingoForexWebSocket(void) {
}

/// <summary>
/// Tiingo Forex的数据源格式：wss://api.tiingo.com/fx，其密钥是随后发送的。
/// </summary>
/// <param name=""></param>
/// <returns></returns>
bool CDataTiingoForexWebSocket::Connecting(void) {
	std::string url("wss://api.tiingo.com/fx");

	m_webSocket.Connecting(url, FunctionProcessTiingoForexWebSocket);
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//
// 5状态下每秒接收100K左右。
//
// threshlodlevel 5: ALL Top-of-Book updates
// thresholdlevel 7: A top-of-book update that is due to a change in either the bid/ask price or size.
//
//////////////////////////////////////////////////////////////////////////////////////////////////
bool CDataTiingoForexWebSocket::Send(vector<CString> vSymbol)
{
	static bool sm_fSendAuth = true;
	CString str;
	CString strPreffix = _T("{\"eventName\":\"subscribe\",\"authorization\":\"");
	CString strMiddle = _T("\",\"eventData\":{\"thresholdLevel\":5,\"tickers\":["); //7：A top - of - book update that is due to a change in either the bid / ask price or size.
	CString strSuffix = _T("]}}"); //7：A top - of - book update that is due to a change in either the bid / ask price or size.
	CString strAuth = gl_pTiingoWebInquiry->GetInquiringStringSuffix();
	strAuth = strAuth.Right(strAuth.GetLength() - 7);

	vSymbol.push_back(_T("gbpaud")); // 多加一个Tiingo制式的代码。由于目前自选crypto使用的是finnhub制式的代码格式，皆为无效代码。
	vSymbol.push_back(_T("eurnok")); // 多加一个Tiingo制式的代码。由于目前自选crypto使用的是finnhub制式的代码格式，皆为无效代码。
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