////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 使用https://github.com/machinezone/IXWebSocket。
// 尝试将ixWebSocket变量封装于此类中。此类负责ixWebSocket的初始化。
//
// ix::initNetSystem();// 在Windows环境下，IXWebSocket库需要初始化一次，且只能初始化一次，此操作由MainFrame的构造函数完成
// ix::uninitNetSystem();// 退出系统时，析构IXWebSocket库，且只能析构一次。此操作由MainFrame的析构函数完成
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <ixwebsocket/IXWebSocket.h>
#include"TemplateMutexAccessQueue.h"
#include "TimeConvert.h"

using std::string;
using std::map;
using std::vector;

using vectorString = vector<string>;

class CVirtualWebSocket : public std::enable_shared_from_this<CVirtualWebSocket> {
public:
	CVirtualWebSocket();
	virtual ~CVirtualWebSocket();

	std::shared_ptr<CVirtualWebSocket> GetShared() { return shared_from_this(); }

	bool ConnectWebSocketAndSendMessage(const vectorString& vSymbol);
	void Reset();

	virtual void Connect() { ASSERT(false); }
	void Disconnect();
	virtual void Send(const vectorString&) { ASSERT(FALSE); }

	virtual void CreateThreadConnectWebSocketAndSendMessage(vectorString vSymbol) { ASSERT(false); } // 继承类必须实现各自的功能

	bool IsSymbol(const string& sSymbol) const;
	void AppendSymbol(const vectorString& vSymbol);
	bool AddSymbol(const string& sSymbol);
	bool DeleteSymbol(const string& sSymbol);
	void ClearSymbol();
	size_t GetSymbolSize() const noexcept { return m_vSymbol.size(); }

	// 状态
	virtual ix::ReadyState GetState() const { return m_webSocket.getReadyState(); }
	bool IsClosed() const { return GetState() == ix::ReadyState::Closed; }
	bool IsOpen() const { return GetState() == ix::ReadyState::Open; }
	bool IsClosing() const { return GetState() == ix::ReadyState::Closing; }
	bool IsConnecting() const { return GetState() == ix::ReadyState::Connecting; }

	virtual void StartWebSocket() { m_webSocket.start(); }
	virtual void StopWebSocket() { m_webSocket.stop(); }

	int GetStatusCode() const noexcept { return m_iStatusCode; }
	void SetStatusCode(int iCode) noexcept { m_iStatusCode = iCode; }
	void SetError(bool fFlag) { m_fError = fFlag; }
	bool IsError() const { return m_fError; }
	string GetStatusMessage() const noexcept { return m_statusMessage; }
	void SetStatusMessage(const string& sMessage) noexcept { m_statusMessage = sMessage; }

	string GetURL() noexcept { return m_url; }
	void SetURL(const string& url) noexcept { m_url = url; }

	int GetSubscriptionId() const noexcept { return m_iSubscriptionId; }
	void SetSubscriptionId(const int iSubscriptionId) noexcept { m_iSubscriptionId = iSubscriptionId; }

	time_t GetHeartbeatTime() const noexcept { return m_HeartbeatTime; }
	void SetHeartbeatTime(const time_t tt) noexcept { m_HeartbeatTime = tt; }
	bool IsIdle(time_t tPeriod = 300) const; // 默认五分钟

	// 实现
	void Connecting(const string& url, const ix::OnMessageCallback& callback, int iPingPeriod = 60, bool fDeflate = true);
	bool CreateThreadDisconnectWebSocket();	// 用于系统退出时。
	bool DisconnectWithoutWaitingSucceed(); // 用于程序运行中途时切断网络链接，此时无需等待。

	vectorString m_vCurrentSymbol;

	size_t DataSize() { return m_qWebSocketData.Size(); }

	void PushData(string data) {
		const auto pData = make_shared<string>(data);
		m_qWebSocketData.PushData(pData);
	}

	void PushData(const shared_ptr<string>& pData) { m_qWebSocketData.PushData(pData); }
	shared_ptr<string> PopData() { return m_qWebSocketData.PopData(); }

protected:
	ix::WebSocket m_webSocket;
	string m_url;

	int m_iStatusCode; // WebSocket返回的状态码。正确：200， 错误：400等。
	bool m_fError{false};
	string m_statusMessage; // 正确时为状态信息，错误时为错误信息。
	int m_iSubscriptionId;

	vectorString m_vSymbol;
	map<string, size_t> m_mapSymbol;

	time_t m_HeartbeatTime; // 最新心跳时间， UTC制式

	CTemplateMutexAccessQueue<string> m_qWebSocketData; // 接收到的WebSocket数据
};

using CVirtualWebSocketPtr = shared_ptr<CVirtualWebSocket>;
