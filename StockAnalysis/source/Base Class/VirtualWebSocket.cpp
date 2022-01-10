#include"pch.h"
#include"VirtualWebSocket.h"

#include <ixwebsocket/IXNetSystem.h>

CVirtualWebSocket::CVirtualWebSocket(bool fHaveSubscription) : CObject() {
	m_fHaveSubscriptionId = fHaveSubscription;
	Reset();
}

CVirtualWebSocket::~CVirtualWebSocket() {
	Deconnecting();
}

void CVirtualWebSocket::Reset(void) {
	m_iSubscriptionId = 0;
	m_iPingPeriod = 0;
	m_fDeflate = true;
}

bool CVirtualWebSocket::ConnectingWebSocketAndSendMessage(vector<CString> vSymbol) {
	if (!IsClosed()) Deconnecting();
	while (!IsClosed()) Sleep(1);
	Reset();
	Connect();
	while (!IsOpen()) Sleep(1);
	Send(vSymbol);

	return true;
}

CString CVirtualWebSocket::CreateTiingoWebSocketSymbolString(vector<CString> vSymbol)
{
	CString strSymbols;
	CString strSymbol;

	for (long l = 0; l < vSymbol.size(); l++) {
		strSymbol = _T("\"") + vSymbol.at(l) + _T("\"") + _T(",");
		strSymbols += strSymbol;
	}
	strSymbols = strSymbols.Left(strSymbols.GetLength() - 1); // 去除最后多余的字符','

	return strSymbols;
}

bool CVirtualWebSocket::Connecting(string url, const ix::OnMessageCallback& callback, int iPingPeriod, bool fDeflate) {
	ix::SocketTLSOptions TLSOption;

	ASSERT(m_webSocket.getReadyState() == ix::ReadyState::Closed);
	TLSOption.tls = true;
	m_webSocket.setTLSOptions(TLSOption);

	m_url = url;
	m_webSocket.setUrl(url);

	// Optional heart beat, sent every iPingPeriod seconds when there is not any traffic
	// to make sure that load balancers do not kill an idle connection.
	m_webSocket.setPingInterval(iPingPeriod);
	m_iPingPeriod = iPingPeriod;

	// Per message deflate connection is enabled by default. You can tweak its parameters or disable it
	if (fDeflate) m_webSocket.disablePerMessageDeflate();
	m_fDeflate = fDeflate;

	// Setup a callback to be fired when a message or an event (open, close, error) is received
	m_webSocket.setOnMessageCallback(callback);

	// Now that our callback is setup, we can start our background thread and receive messages
	m_webSocket.start();

	return true;
}

bool CVirtualWebSocket::Deconnecting(void) {
	if (m_webSocket.getReadyState() != ix::ReadyState::Closed) {
		m_webSocket.stop();
	}
	while (m_webSocket.getReadyState() != ix::ReadyState::Closed) Sleep(1);

	m_iSubscriptionId = 0;

	return true;
}

bool CVirtualWebSocket::Sending(string message)
{
	m_webSocket.send(message);

	return true;
}