#include"pch.h"
#include"WebSocket.h"

#include <ixwebsocket/IXNetSystem.h>

CWebSocket::CWebSocket(bool fHaveSubscription) : CObject() {
	m_fHaveSubscriptionId = fHaveSubscription;
	m_iSubscriptionId = 0;
	m_iPingPeriod = 0;
	m_fDeflate = true;
	m_Preffix = m_Suffix = _T("");
}

CWebSocket::~CWebSocket() {
	Deconnecting();
}

bool CWebSocket::Connecting(string url, const ix::OnMessageCallback& callback, int iPingPeriod, bool fDeflate) {
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

bool CWebSocket::Deconnecting(void) {
	if (m_webSocket.getReadyState() != ix::ReadyState::Closed) {
		m_webSocket.stop();
	}
	while (m_webSocket.getReadyState() != ix::ReadyState::Closed) Sleep(1);

	return true;
}

bool CWebSocket::Send(string message)
{
	m_webSocket.send(m_Preffix + message + m_Suffix);

	return true;
}