#include"pch.h"

#include"VirtualWebSocket.h"

#include <ixwebsocket/IXNetSystem.h>

using namespace std;

#include<gsl/gsl>
using namespace gsl;

CVirtualWebSocket::CVirtualWebSocket(bool fHaveSubscription) {
	m_fHaveSubscriptionId = fHaveSubscription;
	m_iSubscriptionId = 0;
	m_url = _T("");
	m_vSymbol.resize(0);
	m_mapSymbol.clear();

	Reset();
}

CVirtualWebSocket::~CVirtualWebSocket() {
	CVirtualWebSocket::Disconnect();
}

void CVirtualWebSocket::Reset() {
	m_iSubscriptionId = 0;
	m_fReceivingData = false;
}

bool CVirtualWebSocket::ConnectWebSocketAndSendMessage(vectorString vSymbol) {
	try {
		AppendSymbol(vSymbol);
		Disconnect();
		Reset();
		Connect();
		while (!IsOpen()) Sleep(1);
		Send(m_vSymbol);
	}
	catch (exception& e) {
		CString sError = e.what();
		return false;
	}

	return true;
}

bool CVirtualWebSocket::IsSymbol(string sSymbol) {
	if (!m_mapSymbol.contains(sSymbol)) {	// ÐÂ·ûºÅ£¿
		return false;
	}
	else return true;
}

void CVirtualWebSocket::AppendSymbol(vectorString vSymbol) {
	for (auto& sSymbol : vSymbol) {
		if (!m_mapSymbol.contains(sSymbol)) {	// ÐÂ·ûºÅ£¿
			AddSymbol(sSymbol);
		}
	}
}

bool CVirtualWebSocket::AddSymbol(const string& sSymbol) {
	if (IsSymbol(sSymbol)) return false;
	m_mapSymbol[sSymbol] = m_mapSymbol.size();
	m_vSymbol.push_back(sSymbol);
	return true;
}

bool CVirtualWebSocket::DeleteSymbol(const string& sSymbol) {
	if (!IsSymbol(sSymbol)) return false;

	m_vSymbol.erase(m_vSymbol.begin() + m_mapSymbol.at(sSymbol));
	m_mapSymbol.erase(sSymbol);
	return true;
}

void CVirtualWebSocket::ClearSymbol() {
	m_vSymbol.resize(0);
	m_mapSymbol.clear();
}

bool CVirtualWebSocket::Connecting(string url, const ix::OnMessageCallback& callback, int iPingPeriod, bool fDeflate) {
	ix::SocketTLSOptions TLSOption;

	ASSERT(GetState() == ix::ReadyState::Closed);
	TLSOption.tls = true;
	m_webSocket.setTLSOptions(TLSOption);

	m_webSocket.setUrl(url);

	// Optional heart beat, sent every iPingPeriod seconds when there is not any traffic
	// to make sure that load balancers do not kill an idle connection.
	m_webSocket.setPingInterval(iPingPeriod);

	// Per message deflate connection is enabled by default. You can tweak its parameters or disable it
	if (fDeflate) m_webSocket.disablePerMessageDeflate();

	// Setup a callback to be fired when a message or an event (open, close, error) is received
	m_webSocket.setOnMessageCallback(callback);

	// Now that our callback is setup, we can start our background thread and receive messages
	StartWebSocket();

	return true;
}

bool CVirtualWebSocket::Disconnect() {
	if (GetState() != ix::ReadyState::Closed) {
		StopWebSocket();
	}
	while (GetState() != ix::ReadyState::Closed) Sleep(1);
	m_iSubscriptionId = 0;

	return true;
}

UINT ThreadDisconnectWebSocket(not_null<CVirtualWebSocket*> pWebSocket) {
	if (static bool s_fConnecting = false; !s_fConnecting) {
		s_fConnecting = true;
		pWebSocket->Disconnect();
		s_fConnecting = false;
	}
	return 70;
}

bool CVirtualWebSocket::CreateThreadDisconnectWebSocket() {
	thread thread1(ThreadDisconnectWebSocket, this);
	thread1.detach();

	return true;
}

bool CVirtualWebSocket::DisconnectWithoutWaitingSucceed() {
	if (GetState() != ix::ReadyState::Closed) {
		StopWebSocket();
	}
	m_iSubscriptionId = 0;

	return true;
}
