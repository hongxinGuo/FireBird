#include"pch.h"

#include"globedef.h"

#include"SystemMessage.h"

#include"VirtualWebSocket.h"

#include "Thread.h"
#include "WorldMarket.h"

CVirtualWebSocket::CVirtualWebSocket() {
	m_iSubscriptionId = 0;
	m_HeartbeatTime = 0;
	m_url = _T("");
	m_vSymbol.resize(0);
	m_mapSymbol.clear();

	Reset();
}

CVirtualWebSocket::~CVirtualWebSocket() {
	Disconnect();
}

void CVirtualWebSocket::Reset() {
	m_iSubscriptionId = 0;
}

void CVirtualWebSocket::TaskConnectAndSendMessage(vectorString vSymbol) {
	if (IsConnecting()) { // ����������ӣ��������ɵڶ�������
		TRACE("WebSocket���������У��������ɵڶ�������\n");
		gl_dailyWebSocketLogger->info("{} WebSocket���������У��������ɵڶ�������", m_url);
		return;
	}
	TRACE("TaskConnectAndSendMessage\n");
	gl_dailyWebSocketLogger->info("{} TaskConnectAndSendMessage", m_url);
	gl_runtime.thread_executor()->post([this, vSymbol] {
		this->GetShared()->ConnectAndSendMessage(vSymbol);
	});
}

/////////////////////////////////////////////////////////////////////////////////////////////
//
// ���Ӳ���������
//
// 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
bool CVirtualWebSocket::ConnectAndSendMessage(const vectorString& vSymbol) {
	//ASSERT(IsClosed());
	try {
		AppendSymbol(vSymbol);
		Connect();
		//ASSERT(!IsOpen()); // Connect����Connecting,���첽�ġ�
		while (!IsOpen()) {
			if (gl_systemConfiguration.IsExitingSystem()) return false;
			Sleep(1);
		}
		Send(m_vSymbol);
	} catch (exception& e) {
		const CString sError = e.what();
		gl_systemMessage.PushInnerSystemInformationMessage(sError);
		return false;
	}
	return true;
}

void CVirtualWebSocket::AppendSymbol(const vectorString& vSymbol) {
	for (auto& sSymbol : vSymbol) {
		if (!m_mapSymbol.contains(sSymbol)) {	// �·��ţ�
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

bool CVirtualWebSocket::IsIdle(time_t tPeriod) const {
	return GetUTCTime() > m_HeartbeatTime + tPeriod;
}

void CVirtualWebSocket::Connecting(const string& url, const ix::OnMessageCallback& callback, int iPingPeriod, bool fDeflate) {
	ix::SocketTLSOptions TLSOption;

	ASSERT(IsClosed());
	TLSOption.tls = true;
	m_webSocket.setTLSOptions(TLSOption);

	m_webSocket.setUrl(url);

	// Optional heart beat, sent every iPingPeriod seconds when there is not any traffic
	// to make sure that load balancers do not kill an idle connection.
	m_webSocket.setPingInterval(iPingPeriod);

	// Per message deflate connection is enabled by default. You can tweak its parameters or disable it
	//if (fDeflate) m_webSocket.disablePerMessageDeflate();

	// enable auto reconnecting
	const bool bEnabled = m_webSocket.isAutomaticReconnectionEnabled();
	if (!bEnabled) m_webSocket.enableAutomaticReconnection();

	// Setup a callback to be fired when a message or an event (open, close, error) is received
	m_webSocket.setOnMessageCallback(callback);

	// Now that our callback is setup, we can start our background thread and receive messages
	StartWebSocket();
	ASSERT(!IsOpen()); // StartWebSocket()���첽��
}

void CVirtualWebSocket::MonitorWebSocket(bool fDataSourceError, bool fWebSocketOpened, const vectorString& vSymbol) {
	if (fDataSourceError) { // ��ص�DataSource���ִ���
		if (IsOpen()) {
			TaskDisconnect();
		}
		return;
	}

	if (fWebSocketOpened) { // ����web socket���ݣ�
		if (IsError() || IsIdle()) { // �������⣿
			if (IsOpen()) {
				gl_dailyWebSocketLogger->info("Error or Idle, close");
				TaskDisconnect(); // �����������ʱ���ڴ�״̬����ر�֮��Ϊ�������ٴ򿪣�
			}
		}
		if (IsClosed()) {
			TaskConnectAndSendMessage(vSymbol);
			SetHeartbeatTime(GetUTCTime());
		}
	}
	else { // �ر�WebSocket?
		if (IsOpen()) {
			TaskDisconnect(); // �����������ʱ���ڴ�״̬����ر�֮��Ϊ�������ٴ򿪣�
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
//
// Disconnect()��ͬ����
//
/////////////////////////////////////////////////////////////////////////////
void CVirtualWebSocket::Disconnect() {
	m_iSubscriptionId = 0;
	StopWebSocket();	// stop()��ͬ���ģ�ִ�����socket�ѹرա�
}

void CVirtualWebSocket::TaskDisconnect() {
	gl_runtime.thread_executor()->post([this] {
		this->GetShared()->Disconnect();
	});
}
