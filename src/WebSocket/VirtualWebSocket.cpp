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
	if (IsConnecting()) { // 如果正在连接，则不再生成第二个连接
		TRACE("WebSocket正在连接中，不再生成第二个连接\n");
		gl_dailyWebSocketLogger->info("{} WebSocket正在连接中，不再生成第二个连接", m_url);
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
// 联接并发送命令
//
// 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
bool CVirtualWebSocket::ConnectAndSendMessage(const vectorString& vSymbol) {
	//ASSERT(IsClosed());
	try {
		AppendSymbol(vSymbol);
		Connect();
		//ASSERT(!IsOpen()); // Connect调用Connecting,是异步的。
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
		if (!m_mapSymbol.contains(sSymbol)) {	// 新符号？
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
	ASSERT(!IsOpen()); // StartWebSocket()是异步的
}

void CVirtualWebSocket::MonitorWebSocket(bool fDataSourceError, bool fWebSocketOpened, const vectorString& vSymbol) {
	if (fDataSourceError) { // 相关的DataSource出现错误
		if (IsOpen()) {
			TaskDisconnect();
		}
		return;
	}

	if (fWebSocketOpened) { // 接收web socket数据？
		if (IsError() || IsIdle()) { // 出现问题？
			if (IsOpen()) {
				gl_dailyWebSocketLogger->info("Error or Idle, close");
				TaskDisconnect(); // 如果出现问题时处于打开状态，则关闭之（为了随后的再打开）
			}
		}
		if (IsClosed()) {
			TaskConnectAndSendMessage(vSymbol);
			SetHeartbeatTime(GetUTCTime());
		}
	}
	else { // 关闭WebSocket?
		if (IsOpen()) {
			TaskDisconnect(); // 如果出现问题时处于打开状态，则关闭之（为了随后的再打开）
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
//
// Disconnect()是同步的
//
/////////////////////////////////////////////////////////////////////////////
void CVirtualWebSocket::Disconnect() {
	m_iSubscriptionId = 0;
	StopWebSocket();	// stop()是同步的，执行完后socket已关闭。
}

void CVirtualWebSocket::TaskDisconnect() {
	gl_runtime.thread_executor()->post([this] {
		this->GetShared()->Disconnect();
	});
}
