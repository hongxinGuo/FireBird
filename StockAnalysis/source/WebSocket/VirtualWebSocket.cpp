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

CVirtualWebSocket::~CVirtualWebSocket() { Disconnect(); }

void CVirtualWebSocket::Reset(void) {
	m_iSubscriptionId = 0;
	m_fReceivingData = false;
}

bool CVirtualWebSocket::ConnectWebSocketAndSendMessage(vectorString vSymbol) {
	try {
		AppendSymbol(vSymbol);
		Disconnect();
		while (!IsClosed()) Sleep(1);
		ASSERT(IsClosed());
		Reset();
		Connect();
		while (!IsOpen()) Sleep(1);
		ASSERT(IsOpen());
		Send(m_vSymbol);
	}
	catch (exception& e) {
		CString sError = e.what();
		return false;
	}

	return true;
}

bool CVirtualWebSocket::IsSymbol(string sSymbol) {
	if (!m_mapSymbol.contains(sSymbol)) {
		// 新符号？
		return false;
	}
	else return true;
}

void CVirtualWebSocket::AppendSymbol(vectorString vSymbol) {
	for (auto& sSymbol : vSymbol) {
		if (!m_mapSymbol.contains(sSymbol)) {
			// 新符号？
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

void CVirtualWebSocket::ClearSymbol(void) {
	m_vSymbol.resize(0);
	m_mapSymbol.clear();
}

string CVirtualWebSocket::CreateTiingoWebSocketSymbolString(vectorString vSymbol) {
	string sSymbols;

	for (auto& s : vSymbol) {
		const string sSymbol = _T("\"") + s + _T("\"") + _T(",");
		sSymbols += sSymbol;
	}
	sSymbols.erase(sSymbols.end() - 1); // 去除最后多余的字符','

	return sSymbols;
}

bool CVirtualWebSocket::Connecting(string url, const ix::OnMessageCallback& callback, int iPingPeriod, bool fDeflate) {
	ix::SocketTLSOptions TLSOption;

	ASSERT(m_webSocket.getReadyState() == ix::ReadyState::Closed);
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
	m_webSocket.start();

	return true;
}

bool CVirtualWebSocket::Disconnect(void) {
	if (m_webSocket.getReadyState() != ix::ReadyState::Closed) { m_webSocket.stop(); }
	while (m_webSocket.getReadyState() != ix::ReadyState::Closed) Sleep(1);

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

bool CVirtualWebSocket::CreateThreadDisconnectWebSocket(void) {
	thread thread1(ThreadDisconnectWebSocket, this);
	thread1.detach();

	return true;
}

bool CVirtualWebSocket::DisconnectWithoutWaitingSucceed(void) {
	if (m_webSocket.getReadyState() != ix::ReadyState::Closed) { m_webSocket.stop(); }
	m_iSubscriptionId = 0;

	return true;
}

bool CVirtualWebSocket::SendMessage(const string& message) {
	m_webSocket.send(message);

	return true;
}
