#include"pch.h"

#include"VirtualWebSocket.h"

#include <ixwebsocket/IXNetSystem.h>

#include<gsl/gsl>
using namespace gsl;

CVirtualWebSocket::CVirtualWebSocket(bool fHaveSubscription) : CObject() {
	m_fHaveSubscriptionId = fHaveSubscription;
	m_iSubscriptionId = 0;
	m_url = _T("");
	m_vSymbol.resize(0);
	m_mapSymbol.clear();

	Reset();
}

CVirtualWebSocket::~CVirtualWebSocket() {
	Disconnect();
}

void CVirtualWebSocket::Reset(void) {
	m_iSubscriptionId = 0;
	m_fReveivingData = false;
}

bool CVirtualWebSocket::ConnectWebSocketAndSendMessage(vector<CString> vSymbol) {
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

bool CVirtualWebSocket::IsSymbol(CString strSymbol) {
	if (m_mapSymbol.find(strSymbol) == m_mapSymbol.end()) { // 新符号？
		return false;
	}
	else return true;
}

void CVirtualWebSocket::AppendSymbol(vector<CString> vSymbol) {
	for (auto& strSymbol : vSymbol) {
		if (m_mapSymbol.find(strSymbol) == m_mapSymbol.end()) { // 新符号？
			AddSymbol(strSymbol);
		}
	}
}

bool CVirtualWebSocket::AddSymbol(CString strSymbol) {
	if (IsSymbol(strSymbol)) return false;
	m_mapSymbol[strSymbol] = m_mapSymbol.size();
	m_vSymbol.push_back(strSymbol);
	return true;
}

bool CVirtualWebSocket::DeleteSymbol(CString strSymbol) {
	if (!IsSymbol(strSymbol)) return false;

	m_vSymbol.erase(m_vSymbol.begin() + m_mapSymbol.at(strSymbol));
	m_mapSymbol.erase(strSymbol);
	return true;
}

void CVirtualWebSocket::ClearSymbol(void) {
	m_vSymbol.resize(0);
	m_mapSymbol.clear();
}

CString CVirtualWebSocket::CreateTiingoWebSocketSymbolString(vector<CString> vSymbol)
{
	CString strSymbols;

	for (auto& s : vSymbol) {
		CString strSymbol = _T("\"") + s + _T("\"") + _T(",");
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
	if (m_webSocket.getReadyState() != ix::ReadyState::Closed) {
		m_webSocket.stop();
	}
	while (m_webSocket.getReadyState() != ix::ReadyState::Closed) Sleep(1);

	m_iSubscriptionId = 0;

	return true;
}

UINT ThreadDisconnectWebSocket(not_null<CVirtualWebSocket*> pWebSocket) {
	static bool s_fConnecting = false;
	if (!s_fConnecting) {
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
	if (m_webSocket.getReadyState() != ix::ReadyState::Closed) {
		m_webSocket.stop();
	}
	m_iSubscriptionId = 0;

	return true;
}

bool CVirtualWebSocket::SendMessage(const string& message)
{
	m_webSocket.send(message);

	return true;
}